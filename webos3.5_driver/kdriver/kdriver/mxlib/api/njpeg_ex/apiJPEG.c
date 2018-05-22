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
/// file    apiJPEG.c
/// @brief  JPEG API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "njpeg_def.h"
#include "drvNJPD.h"
//#include "Utl.h"
////#include "drvUART.h"

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <setjmp.h>
#endif
#include "jpeg_memory.h"
#include "apiJPEG.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#if SUPPORT_OJPD
#include "apiJPEG_OJPD.h"
#endif

#include "utopia.h"
#include "apiJPEG_priv.h"
#include "apiJPEG_v2.h"
void* pInstantNJPD = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define SUPPORT_PROGRESSIVE_MODE    1

#define ENABLE_JPEG_NO_SIZE_LOWER_BOUND 0

#define SUPPORT_PROGRESSIVE_SCLAEDOWN_MODE    1 //CL82399

#define SW_OPTIMIZE         0
#ifndef MSOS_TYPE_LINUX_KERNEL
#define SW_NJPD_RGB_CMYK      0
#else
#define SW_NJPD_RGB_CMYK      0
#endif

/* Need to check whether OS support jump API or not */
#ifdef CMODEL
#define NJPD_LONGJUMP_SUPPORT
#endif
#if 0
#if !defined(MSOS_TYPE_ECOS)
#define USE_LIBC
#endif
#ifndef USE_LIBC
#define jmp_buf             MS_U32
#define setjmp(jmp_state)   FALSE
#define longjmp(jmp_state, status)
#endif
#endif

#define MJPEG_SW_PARSING_IN_MIU0      FALSE // TRUE
#define PRINT_NJPD_DECODE_TIME   0
#define LOG_DATA_TO_USB         0

#ifdef NJPEG_UTOPIA_2K
    #define VDEC_DRIVER_BASE KERNEL_MODE
#else
    #define VDEC_DRIVER_BASE 0
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#undef printf
#define printf(format, args...)          printk(format, ##args);
#endif

#if PRINT_NJPD_DECODE_TIME
#include "../drv/wdt/drvWDT.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// Max. allocated blocks
#define JPEG_MAXBLOCKS    50

#define DEFAULT_DECODE_TIMEOUT  100

//fractional bits in scale factors
#define IFAST_SCALE_BITS    2

#define JPEG_TIFF_SOI_OFFSET                    0x0201
#define JPEG_TIFF_JPEG_IMG_BYTES                0x0202

#define JPEG_TIFF_BIG_ENDIAN                    0x4D4D
#define JPEG_TIFF_LITTLE_ENDIAN                 0x4949

//EXIF Tag
#define JPEG_EXIF_TAG_MANUFACTURER              0x010F
#define JPEG_EXIF_TAG_MODEL                     0x0110
#define JPEG_EXIF_TAG_ORIENTATION               0x0112
#define JPEG_EXIF_TAG_DATETIME_MOD              0x0132
#define JPEG_EXIF_TAG_EXPOSURE_TIME             0x829A
#define JPEG_EXIF_TAG_F_NUMBER                  0x829D
#define JPEG_EXIF_TAG_IFD_POINTER               0x8769
#define JPEG_EXIF_TAG_EXPOSURE_PROGRAM          0x8822
#define JPEG_EXIF_TAG_ISO_SPEED_RATING          0x8827
#define JPEG_EXIF_TAG_DATETIME_ORI              0x9003
#define JPEG_EXIF_TAG_SHUTTER_SPEED             0x9201
#define JPEG_EXIF_TAG_APERTURE                  0x9202
#define JPEG_EXIF_TAG_EXPOSURE_BIAS             0x9204
#define JPEG_EXIF_TAG_FLASH                     0x9209
#define JPEG_EXIF_TAG_FOCAL_LENGTH              0x920A
#define JPEG_EXIF_TAG_IMAGE_WIDTH               0xA002
#define JPEG_EXIF_TAG_IMAGE_HEIGHT              0xA003

#define JPEG_MANUFACTURER_SIZE                  32
#define JPEG_MODEL_SIZE                         128

#define MIN_READBUFFER_SIZE                     128
#define MRC_CHECK_COUNT                         5000
#define MRC_DECODE_TIMEOUT                      3000
#define WRITE_RLE_TIMEOUT_MS                    10000
#define WRITE_RLE_TIMEOUT_COUNT                 1000000

#define MRC_BUFFER_ADDR         _u32ReadBufferAddr[eNJPDNum]
#define MRC_BUFFER_SIZE         _u32ReadBufferSize[eNJPDNum]
#define MWC_BUFFER_ADDR         _u32WriteBufferAddr[eNJPDNum]
#define MWC_BUFFER_SIZE         _u32WriteBufferSize[eNJPDNum]
#define INTERNAL_BUFFER_ADDR    _u32InternalBufferAddr[eNJPDNum]
#define INTERNAL_BUFFER_SIZE    _u32InternalBufferSize[eNJPDNum]

#define HUFF_EXTEND_TBL(x,s) ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

#define HUFF_EXTEND_P(x,s)  HUFF_EXTEND_TBL(x,s)

/* EXIF parsing section */
#define EndianChangeL(_x)   \
    ((((_x) & 0xff) << 24) | (((_x) & 0xff00) << 8) | (((_x) & 0xff0000) >> 8) | (((_x) & 0xff000000) >> 24))

#define EndianChangeS(_x)   \
    ((((_x) & 0xff) << 8) | (((_x) & 0xff00) >> 8))

#define JPEG_TAG_EXIF   EndianChangeL(0x45786966)

#define JPEG_ABS(x)     (((x)>=0)?(x):(-(x)))


static APP0_Unit App0_Unit_Data[E_NJPD_NJPD_TOTAL];

// No need to do ZAG order in NJPD mode
#if SW_NJPD_RGB_CMYK
JPEG_STATIC MS_U16 _u16PaddingMcuNumber;

MS_U32 u32_Decode_Line;
MS_U8 *u8Out_buf;
//#define DCTSIZE 8
//#define DCTELEM int
#define PASS1_BITS  2
#define FIX_3_0727  ((MS_S32)25172)  /* FIX(3.072711026) */
#define FIX_2_5629  ((MS_S32)20995)  /* FIX(2.562915447) */
#define FIX_2_0531  ((MS_S32)16819)  /* FIX(2.053119869) */
#define FIX_1_9615  ((MS_S32)16069)  /* FIX(1.961570560) */
#define FIX_1_8477  ((MS_S32)15137)  /* FIX(1.847759065) */
#define FIX_1_5013  ((MS_S32)12299)  /* FIX(1.501321110) */
#define FIX_1_1758  ((MS_S32)9633)   /* FIX(1.175875602) */
#define FIX_0_8999  ((MS_S32)7373)   /* FIX(0.899976223) */
#define FIX_0_7653  ((MS_S32)6270)   /* FIX(0.765366865) */
#define FIX_0_5411  ((MS_S32)4433)   /* FIX(0.541196100) */
#define FIX_0_3901  ((MS_S32)3196)   /* FIX(0.390180644) */
#define FIX_0_2986  ((MS_S32)2446)   /* FIX(0.298631336) */
#define SCALE_DONE ((MS_S32) 1)
#define DESCALE(a,m)  (((a) + (SCALE_DONE << ((m)-1))) >> (m))
#define jpeg_clamp(i) if (i & 0xFF00) i = (((~i) >> 15) & 0xFF);
#define SCALEBITS 16
#define ONE_HALF ((MS_S32) 1 << (SCALEBITS-1))
#define FIX(x) ((MS_S32) ((x) * (1L<<SCALEBITS) + 0.5))


#define HUFF_EXTEND(x,s)    ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

#endif

#if LOG_DATA_TO_USB
FILE *logBinfp;

#define JPEG_DEBUG_API_MSG(format, args...)  do{if(logBinfp) fprintf(logBinfp, format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(logBinfp)  fprintf(logBinfp, format, ##args);}while(0)
#else

#ifndef ANDROID
#define JPEG_DEBUG_API_MSG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) printf(format, ##args);}while(0)
#define UTOPIA_20_DBG(format, args...)        do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#else
#define LOG_TAG "API_JPEG"
#include <cutils/log.h>
#ifndef LOGD
#define JPEG_DEBUG_API_MSG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) ALOGI(format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) ALOGE(format, ##args);}while(0)
#define UTOPIA_20_DBG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) ALOGE(format, ##args);}while(0)
#else
#define JPEG_DEBUG_API_MSG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) LOGI(format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) LOGE(format, ##args);}while(0)
#define UTOPIA_20_DBG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) LOGE(format, ##args);}while(0)
#endif

#endif


#endif
//#define printf_red(args...)           do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)
//#define UTOPIA_20_DBG(args...)        //do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)

#define JPEG_OVER_BUFFER_RET(addr, buf_addr, buf_size) \
do \
{ \
    if(((MS_U32)(addr) < (MS_U32)(buf_addr)) \
    || ((MS_U32)(addr) >= ((MS_U32)(buf_addr) + (MS_U32)(buf_size)))) \
    { \
        JPEG_DEBUG_API_ERR("%s [%d] invalid address 0x%lx\n", __FUNCTION__, __LINE__, (MS_U32)(addr)); \
        return FALSE; \
    } \
} \
while(0)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//JPEG header marker id
typedef enum
{
    E_JPEG_SOF0     = 0xC0
  , E_JPEG_SOF1     = 0xC1
  , E_JPEG_SOF2     = 0xC2
  , E_JPEG_SOF3     = 0xC3
  , E_JPEG_SOF5     = 0xC5
  , E_JPEG_SOF6     = 0xC6
  , E_JPEG_SOF7     = 0xC7
  , E_JPEG_JPG      = 0xC8
  , E_JPEG_SOF9     = 0xC9
  , E_JPEG_SOF10    = 0xCA
  , E_JPEG_SOF11    = 0xCB
  , E_JPEG_SOF13    = 0xCD
  , E_JPEG_SOF14    = 0xCE
  , E_JPEG_SOF15    = 0xCF
  , E_JPEG_DHT      = 0xC4
  , E_JPEG_DAC      = 0xCC
  , E_JPEG_RST0     = 0xD0
  , E_JPEG_RST1     = 0xD1
  , E_JPEG_RST2     = 0xD2
  , E_JPEG_RST3     = 0xD3
  , E_JPEG_RST4     = 0xD4
  , E_JPEG_RST5     = 0xD5
  , E_JPEG_RST6     = 0xD6
  , E_JPEG_RST7     = 0xD7
  , E_JPEG_SOI      = 0xD8
  , E_JPEG_EOI      = 0xD9
  , E_JPEG_SOS      = 0xDA
  , E_JPEG_DQT      = 0xDB
  , E_JPEG_DNL      = 0xDC
  , E_JPEG_DRI      = 0xDD
  , E_JPEG_DHP      = 0xDE
  , E_JPEG_EXP      = 0xDF
  , E_JPEG_APP0     = 0xE0
  , E_JPEG_APP1     = 0xE1
  , E_JPEG_APP2     = 0xE2
  , E_JPEG_APP3     = 0xE3
  , E_JPEG_APP4     = 0xE4
  , E_JPEG_APP5     = 0xE5
  , E_JPEG_APP6     = 0xE6
  , E_JPEG_APP7     = 0xE7
  , E_JPEG_APP8     = 0xE8
  , E_JPEG_APP9     = 0xE9
  , E_JPEG_APP10     = 0xEA
  , E_JPEG_APP11     = 0xEB
  , E_JPEG_APP12     = 0xEC
  , E_JPEG_APP13     = 0xED
  , E_JPEG_APP14     = 0xEE
  , E_JPEG_APP15    = 0xEF
  , E_JPEG_JPG0     = 0xF0
  , E_JPEG_JPG1     = 0xF1
  , E_JPEG_JPG2     = 0xF2
  , E_JPEG_JPG3     = 0xF3
  , E_JPEG_JPG4     = 0xF4
  , E_JPEG_JPG5     = 0xF5
  , E_JPEG_JPG6     = 0xF6
  , E_JPEG_JPG7     = 0xF7
  , E_JPEG_JPG8     = 0xF8
  , E_JPEG_JPG9     = 0xF9
  , E_JPEG_JPG10     = 0xFA
  , E_JPEG_JPG11     = 0xFB
  , E_JPEG_JPG12     = 0xFC
  , E_JPEG_JPG13    = 0xFD
  , E_JPEG_COM      = 0xFE
  , E_JPEG_TEM      = 0x01
  , E_JPEG_ERROR    = 0x100
} JPEG_HdrMarker;

//------------------------------------------------------------------------------
typedef enum
{
    E_RLE_DC = 0,
    E_RLE_AC,
    E_RLE_ZRL,
    E_RLE_EOB,
} JPEG_RLESymbol;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_CoeffBuf
/// @brief \b Struct \b Description: The info of coefficient for JPEG decode
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U8 *pu8Data;         ///<data of coefficient of DC, AC
    MS_U16 u16Block_num_x;  ///<the number of block for width
    MS_U16 u16Block_num_y;  ///<the number of block for height
    MS_U16 u16Block_size;   ///<block size
    MS_U8 u8Block_len_x;    ///<The width of block
    MS_U8 u8Block_len_y;    ///<The height of block
} JPEG_CoeffBuf, *PJPEG_CoeffBuf;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_SVLD
/// @brief \b Struct \b Description: The info of SVLD for JPEG decode
//-----------------------------------------------------------------------------
#if 0
typedef struct
{
    union
    {
        struct
        {
            MS_U32 amp          :11;    ///<The amplitude of value of VLI
            MS_U32 sign         :1;     ///<The sign of value of VLI
            MS_U32 run          :4;     ///<run value
            MS_U32 sym_type     :2;     ///<symbol type
            MS_U32 blk_type     :2;     ///<YUV type
            MS_U32 EOP          :1;     ///<End of picture
            MS_U32 trash        :11;    ///<reserved
        };

        struct
        {
            MS_U8 byte0;    ///<byte0 of SVLD
            MS_U8 byte1;    ///<byte1 of SVLD
            MS_U8 byte2;    ///<byte2 of SVLD
            MS_U8 byte3;    ///<byte3 of SVLD
        };
    };
} JPEG_SVLD;
#else //nJPD
typedef struct
{
    union
    {
        struct
        {
            MS_U32 data         :12;  ///<The value of VLI(2's compl & sign-ext)
            MS_U32 run          :4;   ///<run value
            MS_U32 EOB          :1;   ///<end of block
            MS_U32 altzz        :1;   ///<alternative zig-zag
            MS_U32 zzorder      :1;   ///<zig-zag scan order
            MS_U32 trash        :13;  ///<reserved
        };

        struct
        {
            MS_U8 byte0;    ///<byte0 of SVLD
            MS_U8 byte1;    ///<byte1 of SVLD
            MS_U8 byte2;    ///<byte2 of SVLD
            MS_U8 byte3;    ///<byte3 of SVLD
        };
    };
} JPEG_SVLD;
#endif
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HdrChk
/// @brief \b Struct \b Description: The info of header checking for JPEG decode
//-----------------------------------------------------------------------------
typedef union
{
    struct
    {
        MS_U8 DQT:1;///<has Quant Table?
        MS_U8 DHT:1;///<has Huffman Table?
//      MS_U8 SOF:1;
//      MS_U8 SOS:1;
    };
    MS_U8 result;   ///<wildcard for header check
} JPEG_HdrChk;
//------------------------------------------------------------------------------
typedef MS_BOOL ( *Pdecode_block_func )( MS_U8, MS_U16, MS_U16 );


#ifndef UTOPIA_STRIP
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static APP0_Unit App0_Unit_Data[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U16 JPEG_MAX_HEIGHT[E_NJPD_NJPD_TOTAL] = {1080, 1080};
JPEG_STATIC MS_U16 JPEG_MAX_WIDTH[E_NJPD_NJPD_TOTAL] = {1920, 1920};

// max progressive resolution setting
JPEG_STATIC MS_U16 JPEG_PRO_MAX_HEIGHT[E_NJPD_NJPD_TOTAL] = {768, 768};
JPEG_STATIC MS_U16 JPEG_PRO_MAX_WIDTH[E_NJPD_NJPD_TOTAL] = {1024, 1024};

JPEG_STATIC MS_U16 MAX_JPEG_WIDTH_HD[E_NJPD_NJPD_TOTAL] ={0, 0};
JPEG_STATIC MS_U16 MAX_JPEG_HEIGHT_HD[E_NJPD_NJPD_TOTAL] ={0, 0};

#if SUPPORT_MPO_FORMAT
JPEG_STATIC MS_U16 JPEG_MPO_MAX_HEIGHT[E_NJPD_NJPD_TOTAL] = {1080, 1080};
JPEG_STATIC MS_U16 JPEG_MPO_MAX_WIDTH[E_NJPD_NJPD_TOTAL] = {1920, 1920};
JPEG_STATIC MS_U16 JPEG_MPO_PRO_MAX_HEIGHT[E_NJPD_NJPD_TOTAL] ={768, 768};
JPEG_STATIC MS_U16 JPEG_MPO_PRO_MAX_WIDTH[E_NJPD_NJPD_TOTAL] = {1024, 1024};
#endif

/* entry n is (-1 << n) + 1 */
static const MS_S32 extend_offset[16] =
{
    0, (( -1)<<1)+1, ((-1)<<2)+1, ((-1)<<3) + 1, ((-1)<<4)+1, ((-1)<<5)+1,
    ((-1)<<6)+1, ((-1)<<7)+1, ((-1)<<8)+1, ((-1)<<9)+1, ((-1) <<10)+1,
    ((-1)<<11)+1, ((-1)<<12)+1, ((-1)<<13)+1, ((-1)<<14)+1, ((-1)<<15)+1
};

/* entry n is 2**(n-1) */
static const MS_S32 extend_test[16] =
{
    0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
};

static const MS_S32 extend_mask[] =
{
    0, (1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7), (1<<8),
    (1<<9), (1<<10), (1<<11), (1<<12), (1<<13), (1<<14), (1<<15), (1<<16),
};


JPEG_STATIC MS_U32 _u32ReadBufferAddr[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32ReadBufferAddr_PA[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32ReadBufferSize[E_NJPD_NJPD_TOTAL];
#ifndef MSOS_TYPE_LINUX_KERNEL
JPEG_STATIC MS_U32 _u32WriteBufferAddr[E_NJPD_NJPD_TOTAL];
#endif
JPEG_STATIC MS_U32 _u32WriteBufferAddr_PA[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32WriteBufferSize[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32InternalBufferAddr[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32InternalBufferSize[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32InternalBufferAddr_PA[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U32 _u32RLEOffset[E_NJPD_NJPD_TOTAL];   // offset to record the current RLE access address, progressive use only

JPEG_STATIC JPEG_HdrChk _HeadCheck[E_NJPD_NJPD_TOTAL];

JPEG_STATIC JPEG_BLOCK_TYPE _s16dc_pred[E_NJPD_NJPD_TOTAL][3];

// The width/height may be the thumbnail or original image size, it based on decoding mode
JPEG_STATIC MS_U16 _u16Image_x_size[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Image_y_size[E_NJPD_NJPD_TOTAL];

// The original size of this JPEG file after alignment
JPEG_STATIC MS_U16 _u16OriginalImage_x_size[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16OriginalImage_y_size[E_NJPD_NJPD_TOTAL];

// The original size before alignment
JPEG_STATIC MS_U16 _u16NonAlignmentImage_x_size[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16NonAlignmentImage_y_size[E_NJPD_NJPD_TOTAL];

// The width/height/pitch of image for displaying.
JPEG_STATIC MS_U16 _u16AlignedImageWidth[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16AlignedImagePitch[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16AlignedImageHeight[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16AlignedImagePitch_H[E_NJPD_NJPD_TOTAL];

// Scale Down Factor
JPEG_STATIC MS_U8 _u8ScaleDownFactor[E_NJPD_NJPD_TOTAL];

////JPEG_STATIC PJPEG_FILE_FileSystem_t _pStream;

JPEG_STATIC MS_U8 _u8DecodeType[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_BOOL _bProgressive_flag[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U8 _u8DownScaleRatio[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_BOOL _bFirstRLE[E_NJPD_NJPD_TOTAL];

/******* Thumbnail related *******/
JPEG_STATIC MS_BOOL _bThumbnailFound[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_BOOL _bThumbnailAccessMode[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32ThumbnailOffset[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16ThumbnailSize[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_BOOL _bTiffBigEndian[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U32 _u32ThumbnailBufferOffset[E_NJPD_NJPD_TOTAL];   // keep track of thumb buffer access address
JPEG_STATIC MS_U32 _u16ThumbnailBufferSize[E_NJPD_NJPD_TOTAL];   // keep track of thumb buffer size
/*****************************/

JPEG_STATIC JPEG_HuffInfo _Huff_info[E_NJPD_NJPD_TOTAL][JPEG_MAXHUFFTABLES];

JPEG_STATIC JPEG_QuantTbl _QuantTables[E_NJPD_NJPD_TOTAL][JPEG_MAXQUANTTABLES]; /* pointer to quantization tables */

JPEG_STATIC MS_U8 _u8Comps_in_frame[E_NJPD_NJPD_TOTAL];                             /* # of components in frame */
JPEG_STATIC MS_U8 _u8Comp_h_samp[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];     /* component's horizontal sampling factor */
JPEG_STATIC MS_U8 _u8Comp_v_samp[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];     /* component's vertical sampling factor */
JPEG_STATIC MS_U8 _u8Comp_quant[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];      /* component's quantization table selector */
JPEG_STATIC MS_U8 _u8Comp_ident[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];      /* component's ID */

// The Luma and Chroma (YU) component ID, default is 1 & 2
JPEG_STATIC MS_U8 _u8LumaCi[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U8 _u8ChromaCi[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U8 _u8Chroma2Ci[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U16 _u16Comp_h_blocks[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U16 _u16Comp_v_blocks[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];

JPEG_STATIC MS_U8 _u8Comps_in_scan[E_NJPD_NJPD_TOTAL];                  /* # of components in scan */
JPEG_STATIC MS_U8 _u8Comp_list[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPSINSCAN];      /* components in this scan */
JPEG_STATIC MS_U8 _u8Comp_dc_tab[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];     /* component's DC Huffman coding table selector */
JPEG_STATIC MS_U8 _u8Comp_ac_tab[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];     /* component's AC Huffman coding table selector */

JPEG_STATIC MS_U8 _u8Spectral_start[E_NJPD_NJPD_TOTAL];                 /* spectral selection start */
JPEG_STATIC MS_U8 _u8Spectral_end[E_NJPD_NJPD_TOTAL];                   /* spectral selection end   */
JPEG_STATIC MS_U8 _u8Successive_low[E_NJPD_NJPD_TOTAL];                 /* successive approximation low */
JPEG_STATIC MS_U8 _u8Successive_high[E_NJPD_NJPD_TOTAL];                /* successive approximation high */

JPEG_STATIC MS_U8 _u8Blocks_per_mcu[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32Max_blocks_per_row[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Mcus_per_row[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Mcus_per_col[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U8 _u8Mcu_org[E_NJPD_NJPD_TOTAL][JPEG_MAXBLOCKSPERMCU];

JPEG_STATIC MS_U8 gu8Max_mcu_x_size[E_NJPD_NJPD_TOTAL];          /* MCU's max. X size in pixels */
JPEG_STATIC MS_U8 gu8Max_mcu_y_size[E_NJPD_NJPD_TOTAL];                 /* MCU's max. Y size in pixels */
JPEG_STATIC MS_U16 gu16Max_mcus_per_row[E_NJPD_NJPD_TOTAL];

#if SUPPORT_PROGRESSIVE_MODE
JPEG_STATIC MS_U16 _u16Total_lines_left[E_NJPD_NJPD_TOTAL];               /* total # lines left in image */
JPEG_STATIC MS_U32 _u32Block_y_mcu[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
JPEG_STATIC JPEG_HuffTbl _Huff_tbls[E_NJPD_NJPD_TOTAL][JPEG_MAXHUFFTABLES];
JPEG_STATIC PJPEG_CoeffBuf _DC_Coeffs[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
JPEG_STATIC PJPEG_CoeffBuf _AC_Coeffs[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U32 _u32Last_dc_val[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U32 _u32EOB_run[E_NJPD_NJPD_TOTAL];
#endif

JPEG_STATIC MS_U16 _u16Max_blocks_per_mcu[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Max_mcus_per_col[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U8 gu8Scan_type[E_NJPD_NJPD_TOTAL];                 /* Grey, Yh1v1, Yh1v2, Yh2v1, Yh2v2,
                                       CMYK111, CMYK4114 */
JPEG_STATIC MS_U8 *_pu8In_buf_ofs[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32In_buf_left[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U8 _u8Tem_flag[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_BOOL _bEOF_flag[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U8 *_pu8In_buf[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_S16 _s16Bits_left[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32Bit_buf[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U16 _u16Restart_interval[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Restarts_left[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16Next_restart_num[E_NJPD_NJPD_TOTAL];

JPEG_STATIC void *_pBlocks[E_NJPD_NJPD_TOTAL][JPEG_MAXBLOCKS];         /* list of all dynamically allocated blocks */

JPEG_STATIC JPEG_ErrCode _Error_code[E_NJPD_NJPD_TOTAL];

//#ifdef USE_LIBC
#ifdef NJPD_LONGJUMP_SUPPORT
JPEG_STATIC jmp_buf _jmp_state;
#endif
//#endif

////JPEG_STATIC MS_S32 _Total_Decoded_Size = 0;
#if 0 // not implement for new MDDI - harold
JPEG_STATIC MS_U32  NumPics;
#endif
JPEG_STATIC MS_U32 _u32Total_bytes_read[E_NJPD_NJPD_TOTAL];

//JPEG_STATIC MS_U8 *_pu32ExifHeaderAddr;

JPEG_STATIC JPEG_FillHdrFunc _pFillHdrFunc[E_NJPD_NJPD_TOTAL];

#if SW_NJPD_RGB_CMYK
static const MS_U8 _u8ZAG[64+1] =
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
    ,0
};
#endif

JPEG_STATIC MSIF_Version _api_jpeg_version =
{
    .DDI = { JPEG_API_VERSION },
};

JPEG_STATIC MS_U8 _u8JPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
JPEG_STATIC MS_BOOL bMHEG5[E_NJPD_NJPD_TOTAL] = {FALSE, FALSE};

JPEG_STATIC MS_BOOL _Progressive_ROI_flag[E_NJPD_NJPD_TOTAL]; //CL82399
JPEG_STATIC MS_U16 ROI_width[E_NJPD_NJPD_TOTAL]; //CL82399

JPEG_STATIC JPEG_BuffLoadType u8PreLHFlag[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_U32 u32MRCheckCount[E_NJPD_NJPD_TOTAL];

JPEG_STATIC MS_BOOL _bIsInit[E_NJPD_NJPD_TOTAL];

///JPEG_STATIC MS_U32 jpeg_input = 0;
///#define JPEG_GO     do{scanf("%d", &jpeg_input);}while(0)

#if SW_NJPD_RGB_CMYK
JPEG_BLOCK_TYPE *_ps16Block_seg[JPEG_MAXBLOCKSPERROW];
MS_U8 _u8Block_max_zag_set[JPEG_MAXBLOCKSPERROW];
MS_U8 *gpu8Sample_buf;
MS_S32 gs32Crr[256];
MS_S32 gs32Cbb[256];
MS_S32 gs32Crg[256];
MS_S32 gs32Cbg[256];

MS_U8 *pgu8Scan_line_0;
MS_U8 *pgu8scan_line_1;

//JPEG_STATIC MS_U16 _u16Real_dest_bytes_per_scan_line;
JPEG_STATIC MS_U16 _u16Dest_bytes_per_scan_line;        /* rounded up */
JPEG_STATIC MS_U8 _u8Dest_bytes_per_pixel;            /* currently, 4 (RGB) or 1 (Y) */

#if ( ! SUPPORT_PROGRESSIVE_MODE)
JPEG_STATIC MS_U16 _u16Total_lines_left[E_NJPD_NJPD_TOTAL];               /* total # lines left in image */
JPEG_STATIC MS_U32 _u32Block_y_mcu[E_NJPD_NJPD_TOTAL][JPEG_MAXCOMPONENTS];
//JPEG_STATIC MS_U16 gu16Mcu_lines_left;                 /* total # lines left in this MCU */
#endif
#endif

/*===========================================================================*/
JPEG_STATIC MS_BOOL __bIsMjpeg[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 __u32RealMjpegBase[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 __u32TrickyMjpegBase[E_NJPD_NJPD_TOTAL];
/*===========================================================================*/

//The info of JPEG EXIF
JPEG_STATIC JPEG_EXIF_DateTime _stEXIF_DateTime[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_EXIF_Orientation _eEXIF_Orientation[E_NJPD_NJPD_TOTAL];

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_STATIC MS_U8 _u8EXIF_Manufacturer[E_NJPD_NJPD_TOTAL][JPEG_MANUFACTURER_SIZE];
JPEG_STATIC MS_U8 _u8EXIF_Model[E_NJPD_NJPD_TOTAL][JPEG_MODEL_SIZE];
JPEG_STATIC MS_U16 _u16EXIF_Exposureprogram[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U16 _u16EXIF_Flash[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32EXIF_ISOSpeedRatings[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_ExposureTime[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_FNumber[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_ShutterSpeedValue[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_ApertureValue[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_ExposureBiasValue[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPEG_RATIONAL _stEXIF_FocalLength[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32EXIF_ImageWidth[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 _u32EXIF_ImageHeight[E_NJPD_NJPD_TOTAL];
#endif

//record data offset
JPEG_STATIC MS_U32 u32DataOffset[E_NJPD_NJPD_TOTAL];
JPEG_STATIC MS_U32 u32SOFOffset[E_NJPD_NJPD_TOTAL]; //for SEC request, they need to know the offset of SOF marker

JPEG_STATIC MS_BOOL bIs3HuffTbl[E_NJPD_NJPD_TOTAL];
#if SUPPORT_HIGH_LOW_REVERSE
JPEG_STATIC MS_BOOL bHighLowReverse[E_NJPD_NJPD_TOTAL];
#else
JPEG_STATIC MS_BOOL bDifferentHL[E_NJPD_NJPD_TOTAL];
#endif

JPEG_STATIC JPD_Number eNJPDNum = E_NJPD_NJPD1;

#if SW_NJPD_RGB_CMYK
JPEG_STATIC MS_BOOL bEnableCMYK = TRUE;
JPEG_STATIC MS_BOOL bEnableRGB = TRUE;
#endif


#if SUPPORT_MPO_FORMAT
//MPO buffer
static MS_U8 *_pu8In_buf_MPO_ofs[E_NJPD_NJPD_TOTAL];
static MS_U32 _u32In_buf_MPO_left[E_NJPD_NJPD_TOTAL] = {0xFFFFFFFFUL, 0xFFFFFFFFUL};

static MS_BOOL mpo_load_data[E_NJPD_NJPD_TOTAL];  //check whether load data when start decode or fill buffer.
static MS_BOOL bIsMPOFormat[E_NJPD_NJPD_TOTAL];
static MS_U32 u32MPFOffset[E_NJPD_NJPD_TOTAL];
#endif

#if PRINT_NJPD_DECODE_TIME
static MS_U32 u32MeasureDecodeTime[E_NJPD_NJPD_TOTAL];
static MS_U32 u32MeasureDecodeTimeSW[E_NJPD_NJPD_TOTAL];
static MS_U32 u32MeasureDecodeTimeHW[E_NJPD_NJPD_TOTAL];
static MS_U32 u32MeasureDecodeTimeTable[E_NJPD_NJPD_TOTAL];
#endif

//-------------------------------------------------------------------------------------------------
//  API Layer Mutex function
//-------------------------------------------------------------------------------------------------

#ifdef  MSOS_TYPE_LINUX
MS_S32 s32NJPEGMutexID[E_NJPD_NJPD_TOTAL] = {-1, -1};
static char _u8NJPEG_Mutex[E_NJPD_NJPD_TOTAL][20]={"NJPD0_API_Mutex", "NJPD1_API_Mutex"};

#define  _NJPEG_EX_API_MutexLock(u8Idx)\
{\
    JPEG_DEBUG_API_MSG("%s, Mutex Lock\n", __FUNCTION__);\
    if(s32NJPEGMutexID[u8Idx]!=-1)\
    {\
        MsOS_ObtainMutex(s32NJPEGMutexID[u8Idx], MSOS_WAIT_FOREVER);\
    }\
}

#define _NJPEG_EX_API_MutexUnlock(u8Idx)\
{\
    JPEG_DEBUG_API_MSG("%s, Mutex Unlock\n", __FUNCTION__);\
    if(s32NJPEGMutexID[u8Idx]!=-1)\
    {\
        MsOS_ReleaseMutex(s32NJPEGMutexID[u8Idx]);\
    }\
}

static MS_BOOL _NJPEG_EX_API_MutexCreate(MS_U8 u8Idx)
{
    JPEG_DEBUG_API_MSG("Mutex Create\n");
    s32NJPEGMutexID[u8Idx] = MsOS_CreateMutex(E_MSOS_FIFO,
                                      (char *) _u8NJPEG_Mutex[u8Idx],
                                      MSOS_PROCESS_SHARED);

    if (s32NJPEGMutexID[u8Idx] < 0)
    {
        return FALSE;
    }
    return TRUE;
}
#if 0
static MS_BOOL _NJPEG_EX_API_MutexDelete(MS_U8 u8Idx)
{
    if (s32NJPEGMutexID[u8Idx] != -1)
    {
        JPEG_DEBUG_API_MSG("Mutex Delete\n");
        s32NJPEGMutexID[u8Idx] = -1;
        return MsOS_DeleteMutex(s32NJPEGMutexID[u8Idx]);
    }
    else
    {
        return FALSE;
    }
}
#endif

#else
MS_S32 s32NJPEGMutexID[E_NJPD_NJPD_TOTAL] = {-1, -1};
static MS_BOOL _NJPEG_EX_API_MutexCreate(MS_U8 u8Idx)
{
    s32NJPEGMutexID[u8Idx] = 1;
    return TRUE;
}
#define  _NJPEG_EX_API_MutexLock(u8Idx)
#define _NJPEG_EX_API_MutexUnlock(u8Idx)
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
static unsigned long  u32CRC;

static void verJPD_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verJPD_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verJPD_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    int i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7 : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verJPD_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;

    if (pu8Data == NULL || u32Size == 0) return;

    verJPD_Init_CRC_Table(u32CRCTable);

    #if 0
        int i;
        for(i = 0;i<256;i++)
        {
            printf("0x%08x,\n", u32CRCTable[i]);
        }
    #endif

    for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
    {
        u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (unsigned char)(u32CRC & 0xFF) ];
    }
}
#endif

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
#if 0 // not implement for new MDDI - harold
JPEG_STATIC void MSAPI_MJPEG_Get_Pics(void);
#endif // not implement for new MDDI - harold

JPEG_STATIC void JPEG_terminate(JPEG_ErrCode status);

JPEG_STATIC MS_BOOL JPEG_decode_init(void);

JPEG_STATIC MS_BOOL JPEG_StartDecode(void);

JPEG_STATIC void JPEG_GetAlignedResolution(MS_U16 *width, MS_U16 *height);

#if SW_NJPD_RGB_CMYK
JPEG_STATIC MS_BOOL msAPI_JPEG_transform_row( void );
JPEG_STATIC JPEG_Result msAPI_JPEG_Baseline_Decode(void);
#endif //SW_NJPD_RGB_CMYK

JPEG_STATIC MS_U32 JPEG_GetECS(void);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#ifdef VDEC_UTOPIA_2K
#ifndef JPEG_STACK_SIZE
#define JPEG_STACK_SIZE 4096
#endif
MS_S32 _s32JPEG_Callback_TaskId = -1;
MS_U8   u8JpegCallbackStack[JPEG_STACK_SIZE];
MS_BOOL bJpegCallbackTaskRunning = FALSE;

JPEG_IsrFuncCb gIsrCB;

MS_BOOL _MApi_JPEG_Create_Callback_task(void);
MS_BOOL _MApi_JPEG_Delete_Callback_task(void);
void _MApi_JPEG_Callback_Proc(void);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_S32 s32FillHdrIdx = -1;
MS_S32 s32FillHdrRes = 0;
MS_PHYADDR FillHdrPhyAddr[E_NJPD_NJPD_TOTAL];
MS_U32 u32FillHdrLength[E_NJPD_NJPD_TOTAL];
MS_BOOL JPEG_Get_ISR(void);
void JPEG_Set_ISR(MS_BOOL val);
MS_S32 JPEG_FillHdrFunc_KernlDummy(MS_PHYADDR BufAddr, MS_U32 BufLength)
{
    return 0;
}
#endif

MS_U32 JPEG_FillHeaderFunction(MS_PHYADDR mFillHdrPhyAddr,MS_U32 mu32FillHdrLength)//kernel
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    if(eNJPDNum < 0)
    {
        JPEG_DEBUG_API_ERR("%s [%d] [FATAL][ERROR] invalid eNJPDNum for array\n", __FUNCTION__, __LINE__);
		eNJPDNum = 0;
    }
    FillHdrPhyAddr[eNJPDNum] = mFillHdrPhyAddr;
    u32FillHdrLength[eNJPDNum] = mu32FillHdrLength;
    s32FillHdrIdx = eNJPDNum;
    int timeout = 1000;
    while(s32FillHdrIdx != -1)
    {
        timeout--;
        MsOS_DelayTask(1);
        if(timeout<=0)
        {
            s32FillHdrIdx = -1;
            s32FillHdrRes = -1;
        }
    }
    return s32FillHdrRes;
#else
    return _pFillHdrFunc[eNJPDNum](mFillHdrPhyAddr,mu32FillHdrLength);
#endif
}

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL JPEG_Get_ISR()//kernel
{
    return MDrv_NJPD_Get_ISR();
}

void JPEG_Set_ISR(MS_BOOL val)
{
    MDrv_NJPD_Set_ISR(val);
}
#endif

JPEG_STATIC MS_BOOL JPEG_OVER_BUFFER(MS_U32 addr, MS_U32 buf_addr, MS_U32 buf_size)
{
    if((addr < buf_addr)
    || (addr >= (buf_addr+ buf_size)))
    {
        JPEG_DEBUG_API_ERR("%s [%d] invalid address 0x%lx\n", __FUNCTION__, __LINE__, addr);
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------------------
// Refill the input buffer.
// This method will sit in a loop until (A) the buffer is full or (B)
// the stream's read() method reports an end of file condition.
JPEG_STATIC MS_BOOL JPEG_fill_read_buffer(void)
{
    JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer:\n");
    _u32In_buf_left[eNJPDNum] = 0;
    _pu8In_buf_ofs[eNJPDNum] = _pu8In_buf[eNJPDNum];

    if(_bEOF_flag[eNJPDNum]
    && (FALSE == _bThumbnailAccessMode[eNJPDNum]))
    {
        JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer : EOF!!!\n");
        return TRUE;
    }

    if(_bThumbnailAccessMode[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("Fill Thumbnail, buffer left = %ld, buffer offset = %ld\n", _u16ThumbnailBufferSize[eNJPDNum], _u32ThumbnailBufferOffset[eNJPDNum]);
        if(_u16ThumbnailBufferSize[eNJPDNum]>=MRC_BUFFER_SIZE)
        {
            JPEG_memcpy((void *)(_pu8In_buf[eNJPDNum] + _u32In_buf_left[eNJPDNum]), (void *)(INTERNAL_BUFFER_ADDR + _u32ThumbnailBufferOffset[eNJPDNum]), MRC_BUFFER_SIZE);
            _u32In_buf_left[eNJPDNum] = MRC_BUFFER_SIZE;
            _u32ThumbnailBufferOffset[eNJPDNum] += MRC_BUFFER_SIZE;
            _u16ThumbnailBufferSize[eNJPDNum] -= MRC_BUFFER_SIZE;
        }
        else
        {
            JPEG_memcpy((void *)(_pu8In_buf[eNJPDNum] + _u32In_buf_left[eNJPDNum]), (void *)(INTERNAL_BUFFER_ADDR + _u32ThumbnailBufferOffset[eNJPDNum]), _u16ThumbnailBufferSize[eNJPDNum]);
            _bEOF_flag[eNJPDNum] = TRUE;
            _u32In_buf_left[eNJPDNum] = _u16ThumbnailBufferSize[eNJPDNum];
            _u32ThumbnailBufferOffset[eNJPDNum] += _u16ThumbnailBufferSize[eNJPDNum];
            _u16ThumbnailBufferSize[eNJPDNum] = 0;
        }
    }
    else
    {
        if(_pFillHdrFunc[eNJPDNum])
        {
            MS_S32 byte_read = JPEG_FillHeaderFunction((MS_PHYADDR)MS_VA2PA((MS_U32)_pu8In_buf[eNJPDNum]), MRC_BUFFER_SIZE);
            if(byte_read < 0)
            {
                JPEG_terminate(E_JPEG_STREAM_READ);
                return FALSE;
            }
            _u32In_buf_left[eNJPDNum] = byte_read;
        }
        else
        {
            JPEG_DEBUG_API_MSG("_pFillHdrFunc[eNJPDNum] is not registered!!\n");
            if (bMHEG5[eNJPDNum])
            {
                _u32In_buf_left[eNJPDNum] = MRC_BUFFER_SIZE;
            }
            else
            {
                JPEG_terminate(E_JPEG_NOT_ENOUGH_HEADER_INFO);
                return FALSE;
            }
        }
    }
#if 0
    else
    {
        do
        {
            MS_S32 bytes_read = _pStream->read( _pu8In_buf[eNJPDNum] + _u32In_buf_left[eNJPDNum], MRC_BUFFER_SIZE - _u32In_buf_left[eNJPDNum], &_bEOF_flag[eNJPDNum], _pStream);//, 0 );
            if ( bytes_read == -1 )
            {
                JPEG_terminate( E_JPEG_STREAM_READ );
            }
            _u32In_buf_left[eNJPDNum] += bytes_read;
////            _Total_Decoded_Size += bytes_read;
        } while ( ( _u32In_buf_left[eNJPDNum] < MRC_BUFFER_SIZE ) && ( !_bEOF_flag[eNJPDNum] ) );
    }

    _u32Total_bytes_read[eNJPDNum] += _u32In_buf_left[eNJPDNum];
#endif

    u32DataOffset[eNJPDNum] += MRC_BUFFER_SIZE;
    return TRUE;
}


JPEG_STATIC MS_BOOL JPEG_force_fill_read_buffer(MS_U32 u32Size)
{
    JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer:\n");
    _u32In_buf_left[eNJPDNum] = u32Size;
    _pu8In_buf_ofs[eNJPDNum] = _pu8In_buf[eNJPDNum];

    if(_bEOF_flag[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer : EOF!!!\n");
        return TRUE;
    }

    if(_pFillHdrFunc[eNJPDNum])
    {
        MS_S32 byte_read = 0;
        if((MRC_BUFFER_SIZE-u32Size) > 0)
        {
            JPEG_DEBUG_API_MSG("[VA] _pu8In_buf[eNJPDNum]=0x%lx\n", (MS_U32)_pu8In_buf[eNJPDNum]);
            JPEG_DEBUG_API_MSG("[PA] _pu8In_buf[eNJPDNum]=0x%lx\n", MS_VA2PA((MS_U32)_pu8In_buf[eNJPDNum]));
            JPEG_DEBUG_API_MSG("[PA] callback addr=0x%lx\n", (MS_VA2PA((MS_U32)_pu8In_buf[eNJPDNum])+u32Size));
            byte_read  = JPEG_FillHeaderFunction((MS_PHYADDR)(MS_VA2PA((MS_U32)_pu8In_buf[eNJPDNum])+u32Size), MRC_BUFFER_SIZE-u32Size);
            JPEG_DEBUG_API_MSG("@@@fill byte_read=0x%lx\n", byte_read);
        }

        if(byte_read < 0)
        {
            JPEG_terminate(E_JPEG_STREAM_READ);
            return FALSE;
        }
        _u32In_buf_left[eNJPDNum] += byte_read;
    }
    else
    {
        JPEG_DEBUG_API_MSG("_pFillHdrFunc[eNJPDNum] is not registered!!\n");
        return FALSE;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
// Logical rotate left operation.
JPEG_STATIC MS_U32 JPEG_rol(MS_U32 i, MS_U8 j)
{
    return ( ( i << j ) | ( i >> ( 32 - j ) ) );
}
//------------------------------------------------------------------------------
// Retrieve one character from the input stream.
JPEG_STATIC MS_U8 JPEG_get_char(void)
{
    MS_U8 c;

    // Any bytes remaining in buffer?
    if ( !_u32In_buf_left[eNJPDNum] )
    {
        // Try to get more bytes.
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_get_char : JPEG_fill_read_buffer failed!!!\n");
            _u32In_buf_left[eNJPDNum] = 0;
        }
        // Still nothing to get?
        if ( !_u32In_buf_left[eNJPDNum] )
        {
            // Padd the end of the stream with 0xFF 0xD9 (EOI marker)
            // FIXME: Is there a better padding pattern to use?
            MS_U8 t = _u8Tem_flag[eNJPDNum];
            _u8Tem_flag[eNJPDNum] ^= 1;
            if ( t )
            {
                return ( 0xD9 );
            }
            else
            {
                return ( 0xFF );
            }
        }
    }

    c = *_pu8In_buf_ofs[eNJPDNum]++;
    _u32In_buf_left[eNJPDNum]--;

    return ( c );
}
//------------------------------------------------------------------------------
// Same as previus method, except can indicate if the character is
// a "padd" character or not.
JPEG_STATIC MS_U8 JPEG_get_charP(MS_BOOL *Ppadding_flag)
{
    MS_U8 c;

    if ( !_u32In_buf_left[eNJPDNum] )
    {
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_get_charP : JPEG_fill_read_buffer failed!!!\n");
            _u32In_buf_left[eNJPDNum] = 0;
        }

        if ( !_u32In_buf_left[eNJPDNum] )
        {
            *Ppadding_flag = TRUE;
#if 1
            _u8Tem_flag[eNJPDNum] ^= 1;
            // This function is called during when progressive huffman decoding
            // Should not padding 0xFFD9 at this state
            return 0;
#else
            MS_U8 t;
            t = _u8Tem_flag[eNJPDNum];
            _u8Tem_flag[eNJPDNum] ^= 1;
            if ( t )
            {
                return ( 0xD9 );
            }
            else
            {
                return ( 0xFF );
            }
#endif
        }
    }

    *Ppadding_flag = FALSE;

    c = *_pu8In_buf_ofs[eNJPDNum]++;
    _u32In_buf_left[eNJPDNum]--;

    return ( c );
}
//------------------------------------------------------------------------------
// Inserts a previously retrieved character back into the input buffer.
JPEG_STATIC void JPEG_stuff_char(MS_U8 q)
{
    *( --_pu8In_buf_ofs[eNJPDNum] ) = q;
    _u32In_buf_left[eNJPDNum]++;
}
//------------------------------------------------------------------------------
// Retrieves one character from the input stream, but does
// not read past markers. Will continue to return 0xFF when a
// marker is encountered.
// FIXME: Bad name?
JPEG_STATIC MS_U8 JPEG_get_octet(void)
{
    MS_BOOL padding_flag;
    MS_U8 c = JPEG_get_charP( &padding_flag );

    if ( c == 0xFF )
    {
        if ( padding_flag )
        {
            return ( 0xFF );
        }

        c = JPEG_get_charP( &padding_flag );
        if ( padding_flag )
        {
            JPEG_stuff_char( 0xFF );
            return ( 0xFF );
        }

        if ( c == 0x00 )
        {
            return ( 0xFF );
        }
        else
        {
            JPEG_stuff_char( c );
            JPEG_stuff_char( 0xFF );
            return ( 0xFF );
        }
    }

    return ( c );
}
//------------------------------------------------------------------------------
// Retrieves a variable number of bits from the input stream.
// Markers will not be read into the input bit buffer. Instead,
// an infinite number of all 1's will be returned when a marker
// is encountered.
// FIXME: Is it better to return all 0's instead, like the older implementation?
JPEG_STATIC MS_U32 JPEG_get_bits_2(MS_U8 numbits)
{
    MS_U32 i, c1, c2;

    i = ( _u32Bit_buf[eNJPDNum] >> ( 16 - numbits ) ) & ( ( 1 << numbits ) - 1 );

    _s16Bits_left[eNJPDNum] -= numbits;
    if ( _s16Bits_left[eNJPDNum] <= 0 )
    {
        _u32Bit_buf[eNJPDNum] = JPEG_rol( _u32Bit_buf[eNJPDNum], numbits += _s16Bits_left[eNJPDNum] );

        c1 = JPEG_get_octet();
        c2 = JPEG_get_octet();

        _u32Bit_buf[eNJPDNum] = ( _u32Bit_buf[eNJPDNum] & 0xFFFF ) | ( ( ( MS_U32 )c1 ) << 24 ) | ( ( ( MS_U32 )c2 ) << 16 );

        _u32Bit_buf[eNJPDNum] = JPEG_rol( _u32Bit_buf[eNJPDNum], -_s16Bits_left[eNJPDNum] );

        _s16Bits_left[eNJPDNum] += 16;
    }
    else
    {
        _u32Bit_buf[eNJPDNum] = JPEG_rol( _u32Bit_buf[eNJPDNum], numbits );
    }

    return i;
}
//------------------------------------------------------------------------------
// Decodes a Huffman encoded symbol.
JPEG_STATIC MS_S32 JPEG_huff_decode(JPEG_HuffTbl *Ph)
{
    MS_S32 symbol;

    // Check first 8-bits: do we have a complete symbol?
    symbol = Ph->s16Look_up[( _u32Bit_buf[eNJPDNum] >> 8 ) & 0xFF];
    if ( symbol < 0 )
    {
        // Decode more bits, use a tree traversal to find symbol.
        JPEG_get_bits_2( 8 );

        do
        {
            symbol = Ph->s16Tree[~symbol + ( 1 - JPEG_get_bits_2( 1 ) )];
        }
        while ( symbol < 0 );
    }
    else
    {
        JPEG_get_bits_2( Ph->u8Code_size[symbol] );
    }

    return symbol;
}
//------------------------------------------------------------------------------
// Unconditionally frees all allocated blocks.
JPEG_STATIC void JPEG_free_all_blocks(void)
{
    MS_U8 i;
#if 0
    if ( _pStream )
    {
        _pStream = NULL;
    }
#endif
    for ( i = 0; i < JPEG_MAXBLOCKS; i++ )
    {
        if(_pBlocks[eNJPDNum][i]!=NULL)
            JPEG_MEMORY_free( _pBlocks[eNJPDNum][i] );

        _pBlocks[eNJPDNum][i] = NULL;
    }
}
//------------------------------------------------------------------------------
// This method handles all errors.
// It could easily be changed to use C++ exceptions.
JPEG_STATIC void JPEG_terminate(JPEG_ErrCode status)
{
    _Error_code[eNJPDNum] = status;

    JPEG_free_all_blocks();

    JPEG_DEBUG_API_ERR("%s:ERROR= %d\n", __FUNCTION__, _Error_code[eNJPDNum]);
#ifdef NJPD_LONGJUMP_SUPPORT
    longjmp( _jmp_state, status );
#endif
}
//------------------------------------------------------------------------------
// Allocate a block of memory-- store block's address in list for
// later deallocation by JPEG_free_all_blocks().
JPEG_STATIC void * JPEG_alloc(MS_U32 n)
{
    MS_U8 i;
    void *q;

    // Find a free slot. The number of allocated slots will
    // always be very low, so a linear search is good enough.
    for ( i = 0; i < JPEG_MAXBLOCKS; i++ )
    {
        if ( _pBlocks[eNJPDNum][i] == NULL )
        {
            break;
        }
    }

    if ( i >= JPEG_MAXBLOCKS )
    {
        JPEG_terminate( E_JPEG_TOO_MANY_BLOCKS );
        return NULL;
    }

    //FixMe: eCos does not support aligned allocate ???
    q = JPEG_MEMORY_malloc( n + 8 );
    //q = MsOS_AllocateAlignedMemory(n+8, 8, gs32CachedPoolID);
    //q = MsOS_AllocateMemory(n+8, gs32CachedPoolID);

    if ( q == NULL )
    {
        JPEG_terminate( E_JPEG_NOTENOUGHMEM );
        return NULL;
    }

    JPEG_memset((void *)q, 0, n + 8 );

    _pBlocks[eNJPDNum][i] = q;

    JPEG_DEBUG_API_MSG("JPEG %ld bytes allocated\n", n);

    return ( ( void* )q );
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U32 JPEG_Tiff_EndianChangeL(MS_U32 u32Val)
{
    if (_bTiffBigEndian[eNJPDNum])
        return EndianChangeL(u32Val);
    else
        return u32Val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U16 JPEG_Tiff_EndianChangeS(MS_U16 u16Val)
{
    if (_bTiffBigEndian[eNJPDNum])
        return EndianChangeS(u16Val);
    else
        return u16Val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U16 JPEG_GetU16(MS_U8 *data)
{
    MS_S8 i;
    MS_U16 val = 0;

    for(i = 1; i>=0; i--)
        val = (val << 8) + (MS_U8) *(data + i);


    return val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U32 JPEG_GetU32(MS_U8 *data)
{
    MS_S8 i;
    MS_U32 val = 0;

    for(i = 3; i>=0; i--)
        val = (val << 8) + (MS_U8) *(data + i);

    return val;
}
//------------------------------------------------------------------------------
//***************************************************
//Parse EXIF header
//***************************************************
JPEG_STATIC MS_BOOL JPEG_DecodeExifInfo(MS_U8 *data, MS_U32 data_length)
{
    MS_U8 *pJpegBuff = data;
//    MS_U32 u32tmp, u32Len;
    MS_U32 u32tmp;
    MS_U16 u16Marker, u16Len, u16NumOfEntry, i;
    MS_U8  *pTiffHdrPtr, *pNextIfd;
    MS_U32 u32DataValue;
    MS_U8 *pExifHdr = NULL, *pExifHdrOffet = NULL;
    MS_BOOL bIFD_pt_found = FALSE;

    u32tmp = JPEG_GetU32(pJpegBuff);
    if (JPEG_TAG_EXIF != u32tmp)
    {
        return FALSE;
    }

    // Exif starts here
    pJpegBuff += 6;
    pExifHdr = pJpegBuff;
    u16Marker = JPEG_GetU16(pJpegBuff);
    if (JPEG_TIFF_BIG_ENDIAN == u16Marker)
        _bTiffBigEndian[eNJPDNum] = TRUE;
    else if (JPEG_TIFF_LITTLE_ENDIAN == u16Marker)
        _bTiffBigEndian[eNJPDNum] = FALSE;
    else
        return FALSE;

    _u32ThumbnailOffset[eNJPDNum] += 6;
    pTiffHdrPtr = pJpegBuff;

    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if (u16Marker != 0x002A)
        return FALSE;

    pJpegBuff += 2;
    u16Len = (MS_U16)JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
    pJpegBuff += (u16Len - 4); //ENDIAN(2byte) + 0x002A(2byte)

    // 0th IFD start here
    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    pJpegBuff += 2;

    // Boundary check, prevent from buffer over-run
    if((((MS_U32) pJpegBuff) - INTERNAL_BUFFER_ADDR + u16NumOfEntry*12)>=data_length)
    {
        return FALSE;
    }

//    pJpegBuff += 12*u16NumOfEntry;

    JPEG_DEBUG_API_MSG("IFD0 : u16NumOfEntry = %d\n", u16NumOfEntry);

    //Retrieve orientation
    for(i = 0; i < u16NumOfEntry; i++)
    {
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
        pJpegBuff += 8;

        if((JPEG_EXIF_TAG_ORIENTATION == u16Marker)
        && (E_JPEG_EXIF_ORIENT_NOT_FOUND == _eEXIF_Orientation[eNJPDNum]))
        {
            //The datatype of orientation is short
            u32DataValue = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
            if((E_JPEG_EXIF_ORIENT_ROTATE_0 > u32DataValue)
            || (E_JPEG_EXIF_ORIENT_ROTATE_270 < u32DataValue))
            {
                _eEXIF_Orientation[eNJPDNum] = E_JPEG_EXIF_ORIENT_RESERVED;
            }
            else
            {
                _eEXIF_Orientation[eNJPDNum] = (JPEG_EXIF_Orientation)u32DataValue;
            }
            JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation[eNJPDNum]);
        }
        else if(JPEG_EXIF_TAG_IFD_POINTER == u16Marker)
        {
            bIFD_pt_found = TRUE;
            //datavalue is an offset from exif marker for datetime
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            pExifHdrOffet = pExifHdr + u32DataValue;
            JPEG_OVER_BUFFER_RET(pExifHdrOffet, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
            JPEG_DEBUG_API_MSG("Exif IFD pointer = 0x%lx\n", (MS_U32)pExifHdrOffet);
        }
#if SUPPORT_EXIF_EXTRA_INFO
        else if(JPEG_EXIF_TAG_MANUFACTURER == u16Marker)
        {
            MS_U8 *pIFDPtrOffset;
            pIFDPtrOffset = pJpegBuff;
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pIFDPtrOffset));
            JPEG_DEBUG_API_MSG("MANUFACTURER Offset = 0x%lx\n", u32DataValue);
            pIFDPtrOffset = pExifHdr + u32DataValue;
            if(JPEG_OVER_BUFFER((MS_U32)pIFDPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE))
            {
                pJpegBuff += 4;
                continue;
            }
            //JPEG_DEBUG_API_MSG("MANUFACTURER addr = 0x%lx\n", (MS_U32)pIFDPtrOffset);
            MS_U8 u8i=0;
            while(*(pIFDPtrOffset+u8i) && u8i<JPEG_MANUFACTURER_SIZE)
            {
                _u8EXIF_Manufacturer[eNJPDNum][u8i] = *(pIFDPtrOffset+u8i);
                u8i++;
            }
        }
        else if(JPEG_EXIF_TAG_MODEL== u16Marker)
        {
            MS_U8 *pIFDPtrOffset;
            pIFDPtrOffset = pJpegBuff;
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pIFDPtrOffset));
            JPEG_DEBUG_API_MSG("MODEL Offset = 0x%lx\n", u32DataValue);
            pIFDPtrOffset = pExifHdr + u32DataValue;
            if(JPEG_OVER_BUFFER((MS_U32)pIFDPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE))
            {
                pJpegBuff += 4;
                continue;
            }
            //JPEG_DEBUG_API_MSG("MODEL addr = 0x%lx\n", (MS_U32)pIFDPtrOffset);
            MS_U8 u8i=0;
            while(*(pIFDPtrOffset+u8i) && u8i<JPEG_MODEL_SIZE)
            {
                _u8EXIF_Model[eNJPDNum][u8i] = *(pIFDPtrOffset+u8i);
                u8i++;
            }
        }
#endif
#if 0
        else if((JPEG_EXIF_TAG_DATETIME_MOD == u16Marker)
        && (FALSE == _stEXIF_DateTime[eNJPDNum].bHasDataTime))
        {
            _stEXIF_DateTime[eNJPDNum].bHasDataTime = TRUE;
            //datavalue is an offset from exif marker for datetime
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            pExifOffset = pExifHdr + u32DataValue;
            printf("pExifOffset1 = 0x%lx\n", (MS_U32)pExifOffset);
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Year);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Month);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Day);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Hour);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Minute);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime[eNJPDNum].u32Second);
            JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation[eNJPDNum]);
            JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime======= Exist = %d, Original = %d \n", _stEXIF_DateTime[eNJPDNum].bHasDataTime, bOriDateTimeFound);
            JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Year, _stEXIF_DateTime[eNJPDNum].u32Month, _stEXIF_DateTime[eNJPDNum].u32Day);
            JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Hour, _stEXIF_DateTime[eNJPDNum].u32Minute, _stEXIF_DateTime[eNJPDNum].u32Second);
        }
#endif
        pJpegBuff += 4;
    }

    if(TRUE == bIFD_pt_found)
    {
        u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifHdrOffet));
        pExifHdrOffet += 2;
        JPEG_DEBUG_API_MSG("EXIF IFD : u16NumOfEntry = %d\n", u16NumOfEntry);

        for(i = 0; i < u16NumOfEntry; i++)
        {
            u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifHdrOffet));
#if (SUPPORT_EXIF_EXTRA_INFO==FALSE)
            if(JPEG_EXIF_TAG_DATETIME_ORI == u16Marker)
            {
                pExifHdrOffet += 8;
                _stEXIF_DateTime[eNJPDNum].bHasDataTime = TRUE;
                //datavalue is an offset from exif marker for datetime
                u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifHdrOffet));;
                pExifHdrOffet = pExifHdr + u32DataValue;
                JPEG_OVER_BUFFER_RET(pExifHdrOffet, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                JPEG_DEBUG_API_MSG("pExifOriDateTime Offset = 0x%lx\n", (MS_U32)pExifHdrOffet);
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Year);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Month);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Day);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Hour);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Minute);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime[eNJPDNum].u32Second);
                JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime=======\n");
                JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Year, _stEXIF_DateTime[eNJPDNum].u32Month, _stEXIF_DateTime[eNJPDNum].u32Day);
                JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Hour, _stEXIF_DateTime[eNJPDNum].u32Minute, _stEXIF_DateTime[eNJPDNum].u32Second);
                break;
            }
#else
            MS_U8 *pExifPtrOffset;
            MS_U16 u16Type;
            MS_U32 u32Count;
            switch(u16Marker)
            {
                case JPEG_EXIF_TAG_DATETIME_ORI:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _stEXIF_DateTime[eNJPDNum].bHasDataTime = TRUE;
                    //datavalue is an offset from exif marker for datetime
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    JPEG_DEBUG_API_MSG("pExifOriDateTime Offset = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Year);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Month);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Day);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Hour);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Minute);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime[eNJPDNum].u32Second);
                    JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime=======\n");
                    JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Year, _stEXIF_DateTime[eNJPDNum].u32Month, _stEXIF_DateTime[eNJPDNum].u32Day);
                    JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Hour, _stEXIF_DateTime[eNJPDNum].u32Minute, _stEXIF_DateTime[eNJPDNum].u32Second);
                    break;
                case JPEG_EXIF_TAG_ISO_SPEED_RATING:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 4;
                    u32Count = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        if(u32Count==2)
                        {
                            _u32EXIF_ISOSpeedRatings[eNJPDNum]=JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                        }
                        else if(u32Count==1)
                        {
                            _u32EXIF_ISOSpeedRatings[eNJPDNum]=JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                        }
                    }
                    else if(u16Type==4)
                    {
                        if(u32Count==1)
                        {
                            _u32EXIF_ISOSpeedRatings[eNJPDNum]=JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                        }
                    }
                    JPEG_DEBUG_API_MSG("ISO Speed Rating=%ld\n", _u32EXIF_ISOSpeedRatings[eNJPDNum]);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_TIME:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureTime Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("ExposureTime addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_ExposureTime[eNJPDNum].numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ExposureTime[eNJPDNum].denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureTime numerator=%ld, denominator=%ld\n", _stEXIF_ExposureTime[eNJPDNum].numerator, _stEXIF_ExposureTime[eNJPDNum].denominator);
                    break;
                case JPEG_EXIF_TAG_F_NUMBER:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FNumber Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("FNumber addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_FNumber[eNJPDNum].numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_FNumber[eNJPDNum].denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FNumber numerator=%ld, denominator=%ld\n", _stEXIF_FNumber[eNJPDNum].numerator, _stEXIF_FNumber[eNJPDNum].denominator);
                    break;

                case JPEG_EXIF_TAG_SHUTTER_SPEED:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ShutterSpeedValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("ShutterSpeedValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_ShutterSpeedValue[eNJPDNum].s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ShutterSpeedValue[eNJPDNum].s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ShutterSpeedValue numerator=%ld, denominator=%ld\n", _stEXIF_ShutterSpeedValue[eNJPDNum].s_numerator, _stEXIF_ShutterSpeedValue[eNJPDNum].s_denominator);
                    break;
                case JPEG_EXIF_TAG_APERTURE:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ApertureValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("ApertureValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_ApertureValue[eNJPDNum].numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ApertureValue[eNJPDNum].denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ApertureValue numerator=%ld, denominator=%ld\n", _stEXIF_ApertureValue[eNJPDNum].numerator, _stEXIF_ApertureValue[eNJPDNum].denominator);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_BIAS:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureBiasValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("ExposureBiasValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_ExposureBiasValue[eNJPDNum].s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ExposureBiasValue[eNJPDNum].s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureBiasValue numerator=%ld, denominator=%ld\n", _stEXIF_ExposureBiasValue[eNJPDNum].s_numerator, _stEXIF_ExposureBiasValue[eNJPDNum].s_denominator);
                    break;
                case JPEG_EXIF_TAG_FLASH:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _u16EXIF_Flash[eNJPDNum] = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("EXIF_Flash=0x%x\n", _u16EXIF_Flash[eNJPDNum]);
                    break;
                case JPEG_EXIF_TAG_FOCAL_LENGTH:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FocalLength Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("FocalLength addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_FocalLength[eNJPDNum].numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_FocalLength[eNJPDNum].denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FocalLength numerator=%ld, denominator=%ld\n", _stEXIF_FocalLength[eNJPDNum].numerator, _stEXIF_FocalLength[eNJPDNum].denominator);
                    break;
                case JPEG_EXIF_TAG_IMAGE_WIDTH:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        _u32EXIF_ImageWidth[eNJPDNum] = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    }
                    else
                    {
                        _u32EXIF_ImageWidth[eNJPDNum] = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    }
                    JPEG_DEBUG_API_MSG("EXIF_ImageWidth=%ld\n", _u32EXIF_ImageWidth[eNJPDNum]);
                    break;
                case JPEG_EXIF_TAG_IMAGE_HEIGHT:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        _u32EXIF_ImageHeight[eNJPDNum] = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    }
                    else
                    {
                        _u32EXIF_ImageHeight[eNJPDNum] = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    }
                    JPEG_DEBUG_API_MSG("EXIF_ImageHeight=%ld\n", _u32EXIF_ImageHeight[eNJPDNum]);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_PROGRAM:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _u16EXIF_Exposureprogram[eNJPDNum] = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("EXIF_Exposureprogram=0x%x\n", _u16EXIF_Exposureprogram[eNJPDNum]);
                    break;
                default:
                    break;
            }
#endif
            pExifHdrOffet += 12;
        }
    }

    if(E_JPEG_TYPE_THUMBNAIL != _u8DecodeType[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("decode type is not thumbnail...\n");
        return FALSE;
    }

    // 1th IFD
    u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
    if (u32tmp == 0)
        return FALSE;
    else
    {
        if(u32tmp >= data_length)
        {
            JPEG_DEBUG_API_ERR("u32tmp = %ld is invalid > %ld\n", u32tmp,data_length);
            return FALSE;
        }
        pNextIfd = &pTiffHdrPtr[u32tmp];
        JPEG_DEBUG_API_MSG("1st IFD pNextIfd = 0x%lx, u32tmp = %ld\n", (MS_U32)pNextIfd, u32tmp);
    }

    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pNextIfd));
    pNextIfd += 2;

    // Boundary check, prevent from buffer over-run
    if((((MS_U32) pNextIfd) - INTERNAL_BUFFER_ADDR + u16NumOfEntry*12)>=data_length)
    {
        return FALSE;
    }

    for (i = 0; i < u16NumOfEntry; i++)
    {
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pNextIfd));
//        u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pNextIfd + 4));
        u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pNextIfd + 8));

        switch (u16Marker)
        {
            case JPEG_TIFF_JPEG_IMG_BYTES:
                _u16ThumbnailSize[eNJPDNum] = u32tmp;
                break;
            case JPEG_TIFF_SOI_OFFSET:
                JPEG_DEBUG_API_MSG("Thumbnail marker found in 0x%lx!!\n", (MS_U32)pNextIfd);
                _bThumbnailFound[eNJPDNum] = TRUE;
                _u32ThumbnailOffset[eNJPDNum] += u32tmp;
                break;
            default:
                break;
        }

        pNextIfd += 12;
    }

    // Boundary check, prevent from buffer over-run
    if(_bThumbnailFound[eNJPDNum])
    {
        if((_u32ThumbnailOffset[eNJPDNum] + _u16ThumbnailSize[eNJPDNum]) > data_length)
            _bThumbnailFound[eNJPDNum] = FALSE;

        // means it only contains SOI header..
        if(_u16ThumbnailSize[eNJPDNum]<=2)
            _bThumbnailFound[eNJPDNum] = FALSE;
    }

    return _bThumbnailFound[eNJPDNum];
}

// Used to skip unrecognized markers.
JPEG_STATIC void JPEG_skip_bytes(MS_U32 count)
{
    while(count!=0)
    {
        // Any bytes remaining in buffer?
        if ( !_u32In_buf_left[eNJPDNum] )
        {
            // Try to get more bytes.
            if(!JPEG_fill_read_buffer())
            {
                JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer failed!!!\n");
                _u32In_buf_left[eNJPDNum] = 0;
            }
            // Still nothing to get?
            if ( !_u32In_buf_left[eNJPDNum] )
            {
                // should not happen
                break;
            }
        }

        if(count<_u32In_buf_left[eNJPDNum])
        {
            _u32In_buf_left[eNJPDNum] -= count;
            _pu8In_buf_ofs[eNJPDNum] += count;
            count = 0;
        }
        else
        {
            count -= _u32In_buf_left[eNJPDNum];
            _u32In_buf_left[eNJPDNum] = 0;
        }
    }
}
//------------------------------------------------------------------------------
#if SUPPORT_MPO_FORMAT
#define JPEG_TAG_MPO                EndianChangeL(0x4D504600)
#define JPEG_MPO_TAG_MPF_VERSION        0xB000
#define JPEG_MPO_TAG_NUM_OF_IMAGES      0xB001
#define JPEG_MPO_TAG_MP_ENTRY           0xB002

static JPEG_MPO_INDEX_INFO _stMPOIndex[E_NJPD_NJPD_TOTAL];
static JPEG_MPO_ATTRIBUTE_INFO _stMPOAttri[E_NJPD_NJPD_TOTAL][JPEG_MPO_MAX_SUPPORT_IMAGE];
//For start of offset of MPO(only for first MPO picture)
static MS_U32 _u32App2MarkerOffset[E_NJPD_NJPD_TOTAL];

typedef enum
{
    E_JPEG_MPO_ATTR_IMAGE_NO            = 0xB101
  , E_JPEG_MPO_ATTR_PAN_ORIENTATION     = 0xB201
  , E_JPEG_MPO_ATTR_PAN_OVERLAP_H       = 0xB202
  , E_JPEG_MPO_ATTR_PAN_OVERLAP_V       = 0xB203
  , E_JPEG_MPO_ATTR_BASE_VIEWPOINT_NO   = 0xB204
  , E_JPEG_MPO_ATTR_CONVERGENCE_ANGLE   = 0xB205
  , E_JPEG_MPO_ATTR_BASELINE_LENGTH     = 0xB206
  , E_JPEG_MPO_ATTR_DIVERGENCE_ANGLE    = 0xB207
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_X     = 0xB208
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_Y     = 0xB209
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_Z     = 0xB20A
  , E_JPEG_MPO_ATTR_YAW_ANGLE           = 0xB20B
  , E_JPEG_MPO_ATTR_PITCH_ANGLE         = 0xB20C
  , E_JPEG_MPO_ATTR_ROLL_ANGLE          = 0xB20D
} JPEG_MPO_ATTR_TAG;

#define MAX_MPO_ATTR_TAG_NUM        14          // from E_JPEG_MPO_ATTR_IMAGE_NO to E_JPEG_MPO_ATTR_ROLL_ANGLE

//***************************************************
//Initialize EXIF MPO database
//***************************************************
static void JPEG_InitMPO(void)
{
    MS_U32 i = 0;

    //init MPO Index structure
    _stMPOIndex[eNJPDNum].num_of_image = 1;
    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        _stMPOIndex[eNJPDNum].mp_entry[i].attribute = 0;
        _stMPOIndex[eNJPDNum].mp_entry[i].size = 0;
        _stMPOIndex[eNJPDNum].mp_entry[i].offset = 0;
        _stMPOIndex[eNJPDNum].mp_entry[i].image1_no = 0;
        _stMPOIndex[eNJPDNum].mp_entry[i].image2_no = 0;
    }

    // init MPO Attribute structure
    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        _stMPOAttri[eNJPDNum][i].MPIndividualNum = 0;
        _stMPOAttri[eNJPDNum][i].PanOrientation= 0;
        _stMPOAttri[eNJPDNum][i].PanOverlap_H.numerator = 0;
        _stMPOAttri[eNJPDNum][i].PanOverlap_H.denominator = 0;
        _stMPOAttri[eNJPDNum][i].PanOverlap_V.numerator = 0;
        _stMPOAttri[eNJPDNum][i].PanOverlap_V.denominator = 0;
        _stMPOAttri[eNJPDNum][i].BaseViewpointNum = 0;
        _stMPOAttri[eNJPDNum][i].ConvergenceAngle.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].ConvergenceAngle.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].BaselineLength.numerator = 0;
        _stMPOAttri[eNJPDNum][i].BaselineLength.denominator = 0;
        _stMPOAttri[eNJPDNum][i].VerticalDivergence.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].VerticalDivergence.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_X.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_X.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_Y.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_Y.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_Z.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].AxisDistance_Z.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].YawAngle.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].YawAngle.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].PitchAngle.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].PitchAngle.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].RollAngle.s_numerator = 0;
        _stMPOAttri[eNJPDNum][i].RollAngle.s_denominator = 0;
        _stMPOAttri[eNJPDNum][i].used = FALSE;
    }
}
//***************************************************
//Parse EXIF MPO Attribute header
//***************************************************
static MS_BOOL JPEG_DecodeMPAttrIFD(MS_U8 *data, MS_U8 *start_of_offset, MS_U32 count)
{
    MS_U8 *pMPOAttrPtr = data;
    MS_U8 *pMPO_offset;
    MS_U16 u16Marker, i = 0, image_idx = 0;

    JPEG_DEBUG_API_MSG("JPEG_DecodeMPAttrIFD, count = %ld\n", count);

    if(count >MAX_MPO_ATTR_TAG_NUM)
    {
        JPEG_DEBUG_API_ERR("Warning!!!unreasonable attribute IFD number !!!\n");
        return TRUE;
    }

    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        if(FALSE == _stMPOAttri[eNJPDNum][i].used)
        {
            image_idx = i;
            break;
        }
    }

    if(JPEG_MPO_MAX_SUPPORT_IMAGE <= i)
    {
        JPEG_DEBUG_API_MSG("Not support too much images in MPO!!\n");
        return FALSE;
    }

    _stMPOAttri[eNJPDNum][image_idx].used = TRUE;

    for(i = 0; i < count; i++)
    {
        u16Marker = (JPEG_MPO_ATTR_TAG)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pMPOAttrPtr));
        pMPOAttrPtr += 8;
        switch(u16Marker)
        {
            case E_JPEG_MPO_ATTR_IMAGE_NO:
                _stMPOAttri[eNJPDNum][image_idx].MPIndividualNum = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_PAN_ORIENTATION:
                _stMPOAttri[eNJPDNum][image_idx].PanOrientation = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_PAN_OVERLAP_H:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].PanOverlap_H.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].PanOverlap_H.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_PAN_OVERLAP_V:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].PanOverlap_V.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].PanOverlap_V.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_BASE_VIEWPOINT_NO:
                _stMPOAttri[eNJPDNum][image_idx].BaseViewpointNum = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_CONVERGENCE_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].ConvergenceAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].ConvergenceAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_BASELINE_LENGTH:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].BaselineLength.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].BaselineLength.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_DIVERGENCE_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].VerticalDivergence.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].VerticalDivergence.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_X:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_X.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_X.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_Y:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_Y.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_Y.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_Z:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_Z.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].AxisDistance_Z.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_YAW_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].YawAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].YawAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_PITCH_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].PitchAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].PitchAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_ROLL_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[eNJPDNum][image_idx].RollAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[eNJPDNum][image_idx].RollAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            default:
                JPEG_DEBUG_API_ERR("Unknown MPO attribute marker =0x%x!!!!!!\n", u16Marker);
                break;
        }
        pMPOAttrPtr += 4;
    }
    return TRUE;
}
//***************************************************
//Parse EXIF MPO header
//***************************************************
static MS_BOOL JPEG_DecodeMPOInfo(MS_U8 *data)
{
    MS_U8 *pJpegBuff = data;
    MS_U8 *pMPOHdrPtr;
    MS_U32 u32tmp, u32Len;
    MS_U16 u16Marker, u16NumOfEntry, i;

    u32tmp = JPEG_GetU32(pJpegBuff);
    if (u32tmp != JPEG_TAG_MPO)
    {
        return FALSE;
    }

    // mpo header start from here
    pJpegBuff += 4;
    u16Marker = JPEG_GetU16(pJpegBuff);
    if (u16Marker == JPEG_TIFF_BIG_ENDIAN)
        _bTiffBigEndian[eNJPDNum] = TRUE;
    else if (u16Marker == JPEG_TIFF_LITTLE_ENDIAN)
        _bTiffBigEndian[eNJPDNum] = FALSE;
    else
        return FALSE;

//    _u32ThumbnailOffset[eNJPDNum] += 4;
    pMPOHdrPtr = pJpegBuff;

    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if (u16Marker != 0x002A)
        return FALSE;

    //offset to 1st MPO IFD
    pJpegBuff += 2;
    u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));

    //1st MPO IFD start from here(either MP Index IFD or MP Attribute IFD)
    //Count Info
    pJpegBuff = pMPOHdrPtr + u32Len;
    JPEG_OVER_BUFFER_RET(pJpegBuff, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);

    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));

    JPEG_DEBUG_API_MSG("num of entry = %d\n", u16NumOfEntry);

    //MPFVersion
    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if(u16Marker == JPEG_MPO_TAG_MPF_VERSION)
    {
        pJpegBuff += 12;
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
        if(u16Marker == JPEG_MPO_TAG_NUM_OF_IMAGES)
        {
            //It is a MP Index IFD
            //Check number of images
            pJpegBuff += 8;
            u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));

            if(JPEG_MPO_MAX_SUPPORT_IMAGE < u32tmp)
            {
                JPEG_DEBUG_API_MSG("Not support %ld images in MPO!!\n", u32tmp);
                _stMPOIndex[eNJPDNum].num_of_image = 0xFFFFFFFF;
                return FALSE;
            }

            //Init MPO database
            JPEG_InitMPO();

            //start of offset for every individual picture
            _stMPOIndex[eNJPDNum].start_of_offset = _u32App2MarkerOffset[eNJPDNum] + 8 - MRC_BUFFER_ADDR;

            //the numbers of image
            _stMPOIndex[eNJPDNum].num_of_image = u32tmp;

            JPEG_DEBUG_API_MSG("detected _stMPOIndex[eNJPDNum].num_of_image=0x%lx\n", _stMPOIndex[eNJPDNum].num_of_image);
            //MP Entry
            pJpegBuff += 4;
            u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
            if(u16Marker != JPEG_MPO_TAG_MP_ENTRY)
            {
                JPEG_DEBUG_API_MSG("No MP Entry in MP index IFD!!\n");
                return FALSE;
            }

            //offset to next IFD(MP Attribute IFD)
            pJpegBuff = pMPOHdrPtr + 10 + u16NumOfEntry*12;
            u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            JPEG_DEBUG_API_MSG("pJpegBuff = %lx, u32Len = %ld\n", (MS_U32)pJpegBuff, u32Len);

            //MP Entry Internal Fields
            pJpegBuff += 4;
            for(i = 0; i < _stMPOIndex[eNJPDNum].num_of_image; i++)
            {
                //Image Attribute
                _stMPOIndex[eNJPDNum].mp_entry[i].attribute = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex[eNJPDNum].mp_entry[i].size = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex[eNJPDNum].mp_entry[i].offset = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex[eNJPDNum].mp_entry[i].image1_no = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
                //Image size
                pJpegBuff += 2;
                _stMPOIndex[eNJPDNum].mp_entry[i].image2_no = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
                pJpegBuff += 2;
                JPEG_DEBUG_API_MSG("%d th : attribute = %lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].attribute);
                JPEG_DEBUG_API_MSG("%d th : size = %lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].size);
                JPEG_DEBUG_API_MSG("%d th : offset = %lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].offset);
                JPEG_DEBUG_API_MSG("%d th : image1_no = %x\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].image1_no);
                JPEG_DEBUG_API_MSG("%d th : image2_no = %x\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].image2_no);
            }

            //get MPO Attribute count
            pJpegBuff = pMPOHdrPtr + u32Len;
            JPEG_OVER_BUFFER_RET(pJpegBuff, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);

            u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));

            pJpegBuff += 2;
        }
    }

    //Decode MP Attr header
    return JPEG_DecodeMPAttrIFD(pJpegBuff, pMPOHdrPtr, u16NumOfEntry);
}


#endif

// Parse APP0 to get X&Y density.
static MS_BOOL JPEG_read_app0_marker( void )
{

    MS_U32 left;
    MS_U8 u8Xthumbnail, u8Ythumbnail;
    JPEG_DEBUG_API_MSG("APP0\n");

    //Get APP0 length
    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 2;

    if(left < 8)
    {
        JPEG_terminate( E_JPEG_BAD_APP0_MARKER );
        return FALSE;
    }

    //Skip Block ID "JFIF"
    JPEG_skip_bytes(5);

    left-= 5;

    //Skip Version Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Get units
    App0_Unit_Data[eNJPDNum].u8Unit = JPEG_get_char();
    App0_Unit_Data[eNJPDNum].u16Xdensity= (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    App0_Unit_Data[eNJPDNum].u16Ydensity= (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 5;

    //Get thumbnail x,y
    u8Xthumbnail = JPEG_get_char();
    u8Ythumbnail = JPEG_get_char();

    left-= 2;

    //Skip Version Number
    JPEG_skip_bytes(3*u8Xthumbnail*u8Ythumbnail);

    left-= 3*u8Xthumbnail*u8Ythumbnail;

    JPEG_DEBUG_API_MSG("APP0 : unit : %d\n",(MS_U16)App0_Unit_Data[eNJPDNum].u8Unit);
    JPEG_DEBUG_API_MSG("APP0 : Xdensity : %d\n",App0_Unit_Data[eNJPDNum].u16Xdensity);
    JPEG_DEBUG_API_MSG("APP0 : Ydensity : %d\n",App0_Unit_Data[eNJPDNum].u16Ydensity);

    if(left > 0)
        JPEG_skip_bytes(left);

    return TRUE;
}

void _msAPI_JPEG_get_APP0_info( MS_U8 *unit, MS_U16 *x, MS_U16 *y )
{
    *unit = App0_Unit_Data[eNJPDNum].u8Unit;
    *x = App0_Unit_Data[eNJPDNum].u16Xdensity;
    *y = App0_Unit_Data[eNJPDNum].u16Ydensity;
}
//------------------------------------------------------------------------------
// Read exif info
JPEG_STATIC MS_BOOL JPEG_read_app1_marker(void)
{
    MS_U16 length;
    MS_U8 *exif_buffer = (MS_U8 *) INTERNAL_BUFFER_ADDR;
    MS_U16 i = 0;

    JPEG_DEBUG_API_MSG("APP1\n");

    length = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( length < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
        return FALSE;
    }

    length -= 2;

#if SUPPORT_MPO_FORMAT
    u32MPFOffset[eNJPDNum] = length;
    JPEG_DEBUG_API_MSG("u32MPFOffset[eNJPDNum]=0x%lx\n", u32MPFOffset[eNJPDNum]);
#endif
    while((length - i)!=0)
    {
        exif_buffer[i] = (MS_U8) JPEG_get_char();
        i++;
    }

    if(JPEG_DecodeExifInfo(exif_buffer, length) == TRUE)
    {
        JPEG_DEBUG_API_MSG("FOUND THUMBNAIL!\n");
        _u32ThumbnailBufferOffset[eNJPDNum] = _u32ThumbnailOffset[eNJPDNum];
        _u16ThumbnailBufferSize[eNJPDNum] = _u16ThumbnailSize[eNJPDNum];
    }
    else
    {
        JPEG_DEBUG_API_MSG("NO THUMBNAIL!\n");
    }
    return TRUE;
}

#if SUPPORT_MPO_FORMAT

// Read exif app2 info
static MS_BOOL JPEG_read_app2_marker( void )
{
    MS_U16 length;
    MS_U8 *exif_buffer = (MS_U8 *) (INTERNAL_BUFFER_ADDR + u32MPFOffset[eNJPDNum]);
    MS_U16 i = 0;

    JPEG_DEBUG_API_MSG("APP2\n");

    //For start of offset of MPO(only for first MPO picture)
    JPEG_DEBUG_API_MSG("_pu8In_buf_ofs[eNJPDNum] = %lx\n", (MS_U32)_pu8In_buf_ofs[eNJPDNum]);
    _u32App2MarkerOffset[eNJPDNum] = (MS_U32)_pu8In_buf_ofs[eNJPDNum] - 2;

    length = (MS_U16)((JPEG_get_char()<<8)+JPEG_get_char());

    if ( length < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_APP2_MARKER );
        return FALSE;
    }

    length -= 2;

    if(length > INTERNAL_BUFFER_SIZE - u32MPFOffset[eNJPDNum])
    {
        JPEG_terminate( E_JPEG_NOTENOUGHMEM );
        return FALSE;
    }

    while((length - i)!=0)
    {
        exif_buffer[i] = (MS_U8) JPEG_get_char();
        i++;
    }

    if(JPEG_DecodeMPOInfo(exif_buffer)==TRUE)
    {
        JPEG_DEBUG_API_MSG("FOUND MPO!\n");
        bIsMPOFormat[eNJPDNum] = TRUE;
        u32MPFOffset[eNJPDNum] = 0;           // Reset to zero after FOUND MPO

        //// TODO: MPO format not support thumbnail now....
        //_u8DecodeType[eNJPDNum] = E_JPEG_TYPE_MAIN;
        //JPEG_DEBUG_API_MSG("NOTE: MPO not support thumbnail now!!!\n");
    }
    else
    {
        JPEG_DEBUG_API_MSG("NO MPO!\n");
        // Not clear num_of_image for the image with 2 app2 marker
        //_stMPOIndex[eNJPDNum].num_of_image = 0;
    }
    return TRUE;
}


#endif

// Read app14 info
static MS_BOOL JPEG_read_app14_marker( void )
{

    MS_U32 left;
    MS_U8 u8Transform;

    JPEG_DEBUG_API_MSG("APP14\n");

    //Get APP14 length
    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 2;

    if(left < 8)
    {
        JPEG_terminate( E_JPEG_BAD_APP0_MARKER );
        return FALSE;
    }

    //Skip Block ID "Adobe"
    JPEG_skip_bytes(5);

    left-= 5;

    //Skip Version Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Skip F0 Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Skip F1 Number
    JPEG_skip_bytes(2);

    left-= 2;

    u8Transform=  JPEG_get_char();

    left-= 1;

    if(u8Transform!=1)
    {
        // 0: RGB or CMYK, depends on SOF
        // 1: YCbCr
        // 2: YCCK
        JPEG_DEBUG_API_MSG("Unsupported Adobe Format with Transform=%d\n", u8Transform);
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    if(left > 0)
        JPEG_skip_bytes(left);

    return TRUE;
}

static const MS_U8 default_dht[] = {
    0x01, 0xa2, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x10,
    0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
    0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d,
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
    0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
    0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa, 0x01, 0x00, 0x03, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x11,
    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04,
    0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
    0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
    0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
    0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

JPEG_STATIC MS_BOOL JPEG_read_default_dht(void)
{
    MS_U16 i, index, count;
    MS_U32 left;
    MS_U8 u8Huff_num[17];
    MS_U8 u8Huff_val[256];
    MS_U8 u8Valid[17];
    MS_U32 u32DefaultDHTIndex=0;

    JPEG_DEBUG_API_MSG("Default DHT\n");

    left = (MS_U16) ((default_dht[u32DefaultDHTIndex]<<8) + default_dht[u32DefaultDHTIndex+1]);
    u32DefaultDHTIndex+=2;

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
        return FALSE;
    }

    left -= 2;

    while ( left )
    {
        // set it to zero, initialize
        JPEG_memset((void *)u8Huff_num, 0, 17);
        JPEG_memset((void *)u8Valid, 0, 17);
        JPEG_memset((void *)u8Huff_val, 0, 256);

        index = default_dht[u32DefaultDHTIndex];
        u32DefaultDHTIndex++;

        u8Huff_num[0] = 0;

        count = 0;

        for ( i = 1; i <= 16; i++ )
        {
            u8Huff_num[i] = default_dht[u32DefaultDHTIndex];
            u32DefaultDHTIndex++;
            count += u8Huff_num[i];
        }

        if ( count > 255 )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_COUNTS );
            return FALSE;
        }

        for ( i = 0; i < count; i++ )
        {
            u8Huff_val[i] = default_dht[u32DefaultDHTIndex];
            u32DefaultDHTIndex++;
        }

        i = 1 + 16 + count;

        if ( left < ( MS_U32 )i )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
            return FALSE;
        }

        left -= i;

        index = ( index & 0x0F ) + ( ( index & 0x10 ) >> 4 ) * ( JPEG_MAXHUFFTABLES >> 1 ); //???

        if ( index >= JPEG_MAXHUFFTABLES )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_INDEX );
            return FALSE;
        }

        _Huff_info[eNJPDNum][index].u8SymbolCnt = count;

        if(_Huff_info[eNJPDNum][index].bValid==FALSE)
            _Huff_info[eNJPDNum][index].bValid = TRUE;

        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][index].u8Huff_num, (void *)u8Huff_num, 17 );
        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][index].u8Huff_val, (void *)u8Huff_val, 256 );

        // Compute the inverse order of HuffNum. this step is only needed in NJPD mode (baseline)
        for(i = 1; i<=16; i++)
        {
            if(u8Huff_num[17 - i]!=0)
            {
                count = count - u8Huff_num[17 - i];
                u8Huff_num[17 - i] = count;
                u8Valid[17 - i]=1;
            }
            else
            {
                count = count - u8Huff_num[17 - i];
                u8Huff_num[17 - i] = count;
                u8Valid[17 - i]=0;
            }
        }

        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][index].u8Symbol, (void *)u8Huff_num, 17 );
        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][index].u8Valid, (void *)u8Valid, 17 );
    }
    return TRUE;
}


/* END OF EXIF PARSING SECTION */
//------------------------------------------------------------------------------
// Read a Huffman code table.
JPEG_STATIC MS_BOOL JPEG_read_dht_marker(void)
{
    MS_U16 i, indextmp, count;
    MS_U32 left;
    MS_U8 u8Huff_num[17];
    MS_U8 u8Huff_val[256];
    MS_U8 u8Valid[17];

    JPEG_DEBUG_API_MSG("DHT\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
        return FALSE;
    }

    left -= 2;

    while ( left )
    {
        // set it to zero, initialize
        JPEG_memset((void *)u8Huff_num, 0, 17);
        JPEG_memset((void *)u8Valid, 0, 17);
        JPEG_memset((void *)u8Huff_val, 0, 256);

        indextmp = JPEG_get_char();

        u8Huff_num[0] = 0;

        count = 0;

        for ( i = 1; i <= 16; i++ )
        {
            u8Huff_num[i] = JPEG_get_char();
            count += u8Huff_num[i];
        }

        if ( count > 255 )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_COUNTS );
            return FALSE;
        }

        for ( i = 0; i < count; i++ )
        {
            u8Huff_val[i] = JPEG_get_char();
        }

        i = 1 + 16 + count;

        if ( left < ( MS_U32 )i )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
            return FALSE;
        }

        left -= i;

        indextmp = ( indextmp & 0x0F ) + ( ( indextmp & 0x10 ) >> 4 ) * ( JPEG_MAXHUFFTABLES >> 1 ); //???

        if ( indextmp >= JPEG_MAXHUFFTABLES )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_INDEX );
            return FALSE;
        }

        _Huff_info[eNJPDNum][indextmp].u8SymbolCnt = count;

        if(_Huff_info[eNJPDNum][indextmp].bValid==FALSE)
            _Huff_info[eNJPDNum][indextmp].bValid = TRUE;

        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][indextmp].u8Huff_num, (void *)u8Huff_num, 17 );
        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][indextmp].u8Huff_val, (void *)u8Huff_val, 256 );

        // Compute the inverse order of HuffNum. this step is only needed in NJPD mode (baseline)
        for(i = 1; i<=16; i++)
        {
            if(u8Huff_num[17 - i]!=0)
            {
                count = count - u8Huff_num[17 - i];
                u8Huff_num[17 - i] = count;
                u8Valid[17 - i]=1;
            }
            else
            {
                count = count - u8Huff_num[17 - i];
                u8Huff_num[17 - i] = count;
                u8Valid[17 - i]=0;
            }
        }

        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][indextmp].u8Symbol, (void *)u8Huff_num, 17 );
        JPEG_memcpy( (void *)_Huff_info[eNJPDNum][indextmp].u8Valid, (void *)u8Valid, 17 );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a quantization table.
JPEG_STATIC MS_BOOL JPEG_read_dqt_marker(void)
{
    MS_U16 n, i, prec;
    MS_U32 left;
    MS_U32 temp;

    JPEG_DEBUG_API_MSG("DQT\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_DQT_MARKER );
        return FALSE;
    }

    left -= 2;

    while ( left )
    {
        n = JPEG_get_char();
        prec = n >> 4;
        n &= 0x0F;

        if ( n >= JPEG_MAXQUANTTABLES )
        {
            JPEG_terminate( E_JPEG_BAD_DQT_TABLE );
            return FALSE;
        }

        if(_QuantTables[eNJPDNum][n].bValid == FALSE)
            _QuantTables[eNJPDNum][n].bValid = TRUE;

        // read quantization entries, in zag order
        for ( i = 0; i < 64; i++ )
        {
            temp = JPEG_get_char();

            if ( prec )
            {
                temp = ( temp << 8 ) + JPEG_get_char();
            }

            _QuantTables[eNJPDNum][n].s16Value[i] = temp;
        }

        i = 64 + 1;

        if ( prec )
        {
            i += 64;
        }

        if ( left < ( MS_U32 )i )
        {
            JPEG_terminate( E_JPEG_BAD_DQT_LENGTH );
            return FALSE;
        }

        left -= i;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Read the start of frame (SOF) marker.
JPEG_STATIC MS_BOOL JPEG_read_sof_marker(void)
{
    MS_U8 i;
    MS_U32 left;
    MS_U16 JpegMaxWidth, JpegMaxHeight;
    MS_U8 c1;

    JPEG_DEBUG_API_MSG("SOF\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( JPEG_get_char() != 8 )   /* precision: sorry, only 8-bit precision is supported right now */
    {
        JPEG_terminate( E_JPEG_BAD_PRECISION );
        return FALSE;
    }

    _u16Image_y_size[eNJPDNum] = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    _u16Image_x_size[eNJPDNum] = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    // save the original image size
    _u16OriginalImage_x_size[eNJPDNum] = _u16Image_x_size[eNJPDNum];
    _u16OriginalImage_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum];

    if((_bProgressive_flag[eNJPDNum])
    && (E_JPEG_TYPE_MAIN == _u8DecodeType[eNJPDNum]))
    {
       //Define MAX picture size for progressive, 8 is the max factor of scale down
        JpegMaxWidth  = JPEG_PRO_MAX_WIDTH[eNJPDNum] * 8;
        JpegMaxHeight = JPEG_PRO_MAX_HEIGHT[eNJPDNum] * 8;

        //Calculate Internal Buffer size
        //check needed buffer for progressive jpeg decoding, for worst case:
        MS_U32 buff_size = _u16Image_x_size[eNJPDNum]*_u16Image_y_size[eNJPDNum]*3*2
                         + _u16Image_x_size[eNJPDNum]*_u16Image_y_size[eNJPDNum]*3*2/64
                         + JPEG_DEFAULT_EXIF_SIZE
                         + 8*1024;
//        if( (MS_U32)(_u16Image_x_size[eNJPDNum]*_u16Image_y_size[eNJPDNum]*3*2) > (MS_U32)(INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE) )
        if( buff_size > INTERNAL_BUFFER_SIZE )
        {
            JPEG_DEBUG_API_ERR("Progressive image size too big... do not handle it\n");
            JPEG_terminate( E_JPEG_NOTENOUGHMEM );
            return FALSE;
        }
    }
    else
    {
        //Define MAX picture size for Baseline, 8 is the max factor of scale down
        JpegMaxWidth  = JPEG_MAX_WIDTH[eNJPDNum]*8;
        JpegMaxHeight = JPEG_MAX_HEIGHT[eNJPDNum]*8;
    }

    if ((_u16Image_y_size[eNJPDNum] < 1) || (_u16Image_y_size[eNJPDNum] > JpegMaxHeight))
    {
        JPEG_DEBUG_API_MSG("_u16Image_y_size[eNJPDNum]=%d, JpegMaxHeight=%d\n", _u16Image_y_size[eNJPDNum], JpegMaxHeight);
        JPEG_terminate(E_JPEG_BAD_HEIGHT);
        return FALSE;
    }

    if ((_u16Image_x_size[eNJPDNum] < 1) || (_u16Image_x_size[eNJPDNum] > JpegMaxWidth))
    {
        JPEG_DEBUG_API_MSG("_u16Image_x_size[eNJPDNum]=%d, JpegMaxWidth=%d\n", _u16Image_x_size[eNJPDNum], JpegMaxWidth);
        JPEG_terminate(E_JPEG_BAD_WIDTH);
        return FALSE;
    }

    _u8Comps_in_frame[eNJPDNum] = JPEG_get_char();

    if ( _u8Comps_in_frame[eNJPDNum] > JPEG_MAXCOMPONENTS )
    {
        JPEG_terminate( E_JPEG_TOO_MANY_COMPONENTS );
        return FALSE;
    }

    if ( left != ( MS_U32 )( _u8Comps_in_frame[eNJPDNum] * 3 + 8 ) )
    {
        JPEG_terminate( E_JPEG_BAD_SOF_LENGTH );
        return FALSE;
    }

    for ( i = 0; i < _u8Comps_in_frame[eNJPDNum]; i++ )
    {
        _u8Comp_ident[eNJPDNum][i] = JPEG_get_char();
        if(_u8Comp_ident[eNJPDNum][i]==0)     // The component ID is start from 0 (0 1 2). The normal case is start from 1 (1 2 3) for YUV
        {
            _u8LumaCi[eNJPDNum] = 0;
            _u8ChromaCi[eNJPDNum] = 1;
            _u8Chroma2Ci[eNJPDNum] = 2;
        }

        c1 = JPEG_get_char();

        _u8Comp_h_samp[eNJPDNum][i] = (c1 & 0xf0)>>4;
        _u8Comp_v_samp[eNJPDNum][i] = (c1 & 0x0f);
        _u8Comp_quant[eNJPDNum][i] = JPEG_get_char();

        // patch from LG driver for the file with the following problem:
        // only has one component, but its sampling factor is 1x2
        // Per the JPEG spec A.2.2 (see the attached file, "regardless of the values of H1 and V1"),
        // please always set H=1 & V=1 to hw, when mono image.
        if(_u8Comps_in_frame[eNJPDNum]==1)    //allen.chang 2011/6/16 patch
        {
            _u8Comp_h_samp[eNJPDNum][0] =1;
            _u8Comp_v_samp[eNJPDNum][0] =1;
        }

    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Used to skip unrecognized markers.
JPEG_STATIC MS_BOOL JPEG_skip_variable_marker(void)
{
    MS_U32 left;

    JPEG_DEBUG_API_MSG("SKIP markers\n");

    left = (MS_U32)((JPEG_get_char()<<8)|JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_VARIABLE_MARKER );
        return FALSE;
    }

    left -= 2;

    JPEG_skip_bytes(left);
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a define restart interval (DRI) marker.
JPEG_STATIC MS_BOOL JPEG_read_dri_marker(void)
{
    JPEG_DEBUG_API_MSG("DRI\n");

    if ( (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char()) != 4 )
    {
        JPEG_terminate( E_JPEG_BAD_DRI_LENGTH );
        return FALSE;
    }

    _u16Restart_interval[eNJPDNum] = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a start of scan (SOS) marker.
JPEG_STATIC MS_BOOL JPEG_read_sos_marker(void)
{
    MS_U32 left;
    MS_U16 i, ci, n, c, cc;
    MS_U8 c1;

    JPEG_DEBUG_API_MSG("SOS\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    n = JPEG_get_char();

    _u8Comps_in_scan[eNJPDNum] = n;

    left -= 3;

    if ( ( left != ( MS_U32 )( n * 2 + 3 ) ) || ( n < 1 ) || ( n > JPEG_MAXCOMPSINSCAN ) )
    {
        JPEG_terminate( E_JPEG_BAD_SOS_LENGTH );
        return FALSE;
    }

    for ( i = 0; i < n; i++ )
    {
        cc = JPEG_get_char();
        c = JPEG_get_char();
        left -= 2;

        for ( ci = 0; ci < _u8Comps_in_frame[eNJPDNum]; ci++ )
        {
            if ( cc == _u8Comp_ident[eNJPDNum][ci] )
            {
                break;
            }
        }

        if ( ci >= _u8Comps_in_frame[eNJPDNum] )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_COMP_ID );
            return FALSE;
        }

        _u8Comp_list[eNJPDNum][i] = ci;
        _u8Comp_dc_tab[eNJPDNum][ci] = ( c >> 4 ) & 15;
        _u8Comp_ac_tab[eNJPDNum][ci] = ( c & 15 ) + ( JPEG_MAXHUFFTABLES >> 1 );
    }

    //HW limitation, for baseline JPEG, U.V need to refer to the same DC and AC huffman table.
    if(!_bProgressive_flag[eNJPDNum] && (_u8Comps_in_frame[eNJPDNum] == 3)) //Y.U.V.
    {
        if((_u8Comp_dc_tab[eNJPDNum][1] != _u8Comp_dc_tab[eNJPDNum][2])
        || (_u8Comp_ac_tab[eNJPDNum][1] != _u8Comp_ac_tab[eNJPDNum][2]))
        {
            JPEG_DEBUG_API_MSG("U, V use different Huffman table~~\n");
            bIs3HuffTbl[eNJPDNum] = TRUE;
        }
    }

    _u8Spectral_start[eNJPDNum] = JPEG_get_char();
    _u8Spectral_end[eNJPDNum] = JPEG_get_char();
    c1 = JPEG_get_char();
    _u8Successive_high[eNJPDNum] = (c1 & 0xf0)>>4;
    _u8Successive_low[eNJPDNum] = (c1 & 0x0f);

    if ( !_bProgressive_flag[eNJPDNum] )
    {
        _u8Spectral_start[eNJPDNum] = 0;
        _u8Spectral_end[eNJPDNum] = 63;
    }

    left -= 3;

    JPEG_skip_bytes(left);    /* read past whatever is left */
    return TRUE;
}
//------------------------------------------------------------------------------
// Finds the next marker.
JPEG_STATIC MS_U32 JPEG_next_marker(void) //ok
{
    MS_U32 c, bytes;

    bytes = 0;

    do
    {
        do
        {
            bytes++;

            c = JPEG_get_char();
            //JPEG_DEBUG_API_MSG("c = %X\n",c);
        }
        while ( c != 0xFF );

        do
        {
            c = JPEG_get_char();
            //JPEG_DEBUG_API_MSG("c = %X\n",c);
        }
        while ( c == 0xFF );
    }
    while ( c == 0 );

    // If bytes > 0 here, there where extra bytes before the marker (not good).

    return c;
}
//------------------------------------------------------------------------------
// Process markers. Returns when an SOFx, SOI, EOI, or SOS marker is
// encountered.
JPEG_STATIC MS_U32 JPEG_process_markers(void)
{
    MS_U32 c;

    JPEG_DEBUG_API_MSG("JPEG_process_markers:\n");
    for ( ; ; )
    {
        c = JPEG_next_marker();

        switch ( c )
        {
            case E_JPEG_APP1:
                // Prevent from there's thumbnail in thumbnail... & multiple APP1
                // Although it's impossible.. =_=
//                if((E_JPEG_TYPE_THUMBNAIL == _u8DecodeType[eNJPDNum])
//                && (FALSE == _bThumbnailFound[eNJPDNum]))
                // We need to get EXIF info for decoding main picture or thumbnail.
                if((__bIsMjpeg[eNJPDNum] == FALSE)
                && (FALSE == _bThumbnailFound[eNJPDNum]))
                {
                    if( !JPEG_read_app1_marker() )
                        return FALSE;
                }
                else
                {
                    if( !JPEG_skip_variable_marker() )
                        return FALSE;
                }
                break;
#if SUPPORT_MPO_FORMAT
            case E_JPEG_APP2:
                if( !JPEG_read_app2_marker() )
                    return FALSE;
                break;

#endif
            case E_JPEG_SOF0:
            case E_JPEG_SOF1:
            case E_JPEG_SOF2:
            case E_JPEG_SOF3:
            case E_JPEG_SOF5:
            case E_JPEG_SOF6:
            case E_JPEG_SOF7:
                //      case E_JPEG_JPG:
            case E_JPEG_SOF9:
            case E_JPEG_SOF10:
            case E_JPEG_SOF11:
            case E_JPEG_SOF13:
            case E_JPEG_SOF14:
            case E_JPEG_SOF15:
            case E_JPEG_SOI:
            case E_JPEG_EOI:
            case E_JPEG_SOS:
                {
                    return c;
                }
            case E_JPEG_DHT:
                {
                    if( !JPEG_read_dht_marker() )
                        return FALSE;
                    _HeadCheck[eNJPDNum].DHT = TRUE;
                    break;
                }
                // Sorry, no arithmitic support at this time. Dumb patents!
            case E_JPEG_DAC:
                {
                    JPEG_terminate( E_JPEG_NO_ARITHMETIC_SUPPORT );
                    return FALSE;
                    break;
                }
            case E_JPEG_DQT:
                {
                    if( !JPEG_read_dqt_marker() )
                        return FALSE;
                    _HeadCheck[eNJPDNum].DQT = TRUE;
                    break;
                }
            case E_JPEG_DRI:
                {
                    if( !JPEG_read_dri_marker() )
                        return FALSE;
                    break;
                }
                //case E_JPEG_APP0:  /* no need to read the JFIF marker */

            case E_JPEG_JPG:
            case E_JPEG_RST0:
                /* no parameters */
            case E_JPEG_RST1:
            case E_JPEG_RST2:
            case E_JPEG_RST3:
            case E_JPEG_RST4:
            case E_JPEG_RST5:
            case E_JPEG_RST6:
            case E_JPEG_RST7:
            case E_JPEG_TEM:
                {
                    JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
                    return FALSE;
                    break;
                }
            case E_JPEG_APP0:
                if (!bMHEG5[eNJPDNum])
                {
                    if(!JPEG_skip_variable_marker())
                    {
                        return FALSE;
                    }
                    break;
                }
                if(!JPEG_read_app0_marker())
                {
                    return FALSE;
                }
                break;
            case E_JPEG_APP14:
                if(__bIsMjpeg[eNJPDNum] == FALSE)
                {
                    if( !JPEG_read_app14_marker() )
                        return FALSE;
                }
                else
                {
                    if( !JPEG_skip_variable_marker() )
                        return FALSE;
                }
                break;
            case E_JPEG_DNL:
            case E_JPEG_DHP:
            case E_JPEG_EXP:
#if (SUPPORT_MPO_FORMAT == 0)
            case E_JPEG_APP2:
#endif
            case E_JPEG_APP3:
            case E_JPEG_APP4:
            case E_JPEG_APP5:
            case E_JPEG_APP6:
            case E_JPEG_APP7:
            case E_JPEG_APP8:
            case E_JPEG_APP9:
            case E_JPEG_APP10:
            case E_JPEG_APP11:
            case E_JPEG_APP12:
            case E_JPEG_APP13:
            case E_JPEG_APP15:
            case E_JPEG_JPG0:
            case E_JPEG_JPG1:
            case E_JPEG_JPG2:
            case E_JPEG_JPG3:
            case E_JPEG_JPG4:
            case E_JPEG_JPG5:
            case E_JPEG_JPG6:
            case E_JPEG_JPG7:
            case E_JPEG_JPG8:
            case E_JPEG_JPG9:
            case E_JPEG_JPG10:
            case E_JPEG_JPG11:
            case E_JPEG_JPG12:
            case E_JPEG_JPG13:
            case E_JPEG_COM:
                /* must be DNL, DHP, EXP, APPn, JPGn, COM, or RESn or APP0 */
                {
                    if(!JPEG_skip_variable_marker())
                    {
                        return FALSE;
                    }
                    break;
                }
            default:
                break;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Finds the start of image (SOI) marker.
// This code is rather defensive: it only checks the first 512 bytes to avoid
// FALSE positives.
JPEG_STATIC MS_BOOL JPEG_locate_soi_marker(void)
{
    MS_U32 lastchar, thischar;
    MS_U32 bytesleft;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    lastchar = JPEG_get_char();

    //JPEG_DEBUG_API_MSG("0x%lx\n", lastchar);
    thischar = JPEG_get_char();

    //JPEG_DEBUG_API_MSG("0x%lx\n", thischar);
    /* ok if it's a normal JPEG file without a special header */

    if ((0xFF == lastchar)
    && (E_JPEG_SOI == thischar))
    {
        //JPEG_DEBUG_API_MSG("SOI\n");
        return TRUE;
    }

    // Set this value to 0x1000 for 4k alignment MPO case when parse 2nd/3rd/... JPEG file
    bytesleft = 0x1000; //Fix this number from 512 -> 640 for some cases

    for ( ; ; )
    {
        if ( --bytesleft == 0 )
        {
            JPEG_terminate( E_JPEG_NOT_JPEG );
            return FALSE;
        }

        lastchar = thischar;

        thischar = JPEG_get_char();

        //JPEG_DEBUG_API_MSG("%x ", (MS_U8)thischar);
        //if(bytesleft%8==0)
        //    JPEG_DEBUG_API_MSG("\n");

        if ((0xFF == lastchar)
        && (E_JPEG_SOI == thischar))
        {
            //JPEG_DEBUG_API_MSG("SOI\n");
            break;
        }
    }

    /* Check the next character after marker: if it's not 0xFF, it can't
       be the start of the next marker, so it probably isn't a JPEG */
/*  need to check for more detail, currently _u32Bit_buf[eNJPDNum] is not updated during JPEG_get_char()
    thischar = ( _u32Bit_buf[eNJPDNum] >> 8 ) & 0xFF;

    if ( thischar != 0xFF )
    {
        JPEG_terminate( E_JPEG_NOT_JPEG );
        return FALSE;
    }
*/
    return TRUE;
}
//------------------------------------------------------------------------------
// Find a start of frame (SOF) marker.
JPEG_STATIC MS_BOOL JPEG_locate_sof_marker(void)
{
    MS_U32 c;
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    JPEG_DEBUG_API_MSG("_pu8In_buf_ofs[eNJPDNum]=0x%lx(VA), 0x%lx(PA)=========\n",
        (MS_U32)_pu8In_buf_ofs[eNJPDNum], MS_VA2PA((MS_U32)_pu8In_buf_ofs[eNJPDNum]));
    JPEG_DEBUG_API_MSG("%02x %02x %02x %02x %02x %02x %02x %02x \n", *_pu8In_buf_ofs[eNJPDNum], *(_pu8In_buf_ofs[eNJPDNum]+1),
        *(_pu8In_buf_ofs[eNJPDNum]+2),*(_pu8In_buf_ofs[eNJPDNum]+3),
        *(_pu8In_buf_ofs[eNJPDNum]+4),*(_pu8In_buf_ofs[eNJPDNum]+5),
        *(_pu8In_buf_ofs[eNJPDNum]+6),*(_pu8In_buf_ofs[eNJPDNum]+7)
        );

    if(!JPEG_locate_soi_marker())
        return FALSE;

    c = JPEG_process_markers();

    if( c == FALSE )
        return FALSE;

    switch ( c )
    {
        case E_JPEG_SOF2:
            {
                JPEG_DEBUG_API_MSG("Progressive\n");
                _bProgressive_flag[eNJPDNum] = TRUE;
                u32SOFOffset[eNJPDNum] = u32DataOffset[eNJPDNum] + JPEG_GetECS() - 2;
                if(!JPEG_read_sof_marker())
                    return FALSE;
                break;
            }
        case E_JPEG_SOF0:
            /* baseline DCT */
        case E_JPEG_SOF1:
            /* extended sequential DCT */
            {
                JPEG_DEBUG_API_MSG("Baseline\n");
                u32SOFOffset[eNJPDNum] = u32DataOffset[eNJPDNum] + JPEG_GetECS() - 2;
                if(!JPEG_read_sof_marker())
                    return FALSE;
                break;
            }
        case E_JPEG_SOF9:
            /* Arithmitic coding */
            {
                JPEG_terminate( E_JPEG_NO_ARITHMETIC_SUPPORT );
                return FALSE;
                break;
            }

        default:
            {
                JPEG_DEBUG_API_MSG("parsed unsupported marker = 0x%04lX\n", c);
                JPEG_terminate( E_JPEG_UNSUPPORTED_MARKER );
                return FALSE;
                break;
            }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Find a start of scan (SOS) marker.
JPEG_STATIC MS_BOOL JPEG_locate_sos_marker(void)
{
    MS_U32 c;

    c = JPEG_process_markers();

    if ( c == E_JPEG_EOI )
    {
        return FALSE;
    }
    else if ( c != E_JPEG_SOS )
    {
        JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
        return FALSE;
    }

    if(!JPEG_read_sos_marker())
        return FALSE;

    JPEG_DEBUG_API_MSG(" _pu8In_buf_MPO_ofs[eNJPDNum]=0x%lx, _u32In_buf_MPO_left[eNJPDNum]=0x%lx===========================\n", (MS_U32)_pu8In_buf_ofs[eNJPDNum], (MS_U32)_u32In_buf_left[eNJPDNum]);
    JPEG_DEBUG_API_MSG("%02x %02x %02x %02x %02x %02x %02x %02x \n",
    *_pu8In_buf_ofs[eNJPDNum], *(_pu8In_buf_ofs[eNJPDNum]+1), *(_pu8In_buf_ofs[eNJPDNum]+2), *(_pu8In_buf_ofs[eNJPDNum]+3),
    *(_pu8In_buf_ofs[eNJPDNum]+4), *(_pu8In_buf_ofs[eNJPDNum]+5), *(_pu8In_buf_ofs[eNJPDNum]+6), *(_pu8In_buf_ofs[eNJPDNum]+7)
    );

    return TRUE;
}
//------------------------------------------------------------------------------
// Reset thumbnail parameters
JPEG_STATIC void JPEG_init_thumbnail(void)
{
    _bThumbnailFound[eNJPDNum] = FALSE;
    _u32ThumbnailOffset[eNJPDNum] = 0;
    _u16ThumbnailSize[eNJPDNum] = 0;
    _bTiffBigEndian[eNJPDNum] = FALSE;

    _u32ThumbnailBufferOffset[eNJPDNum] = 0;
    _u16ThumbnailSize[eNJPDNum] = 0;
    _bThumbnailAccessMode[eNJPDNum] = FALSE;

    _stEXIF_DateTime[eNJPDNum].bHasDataTime = FALSE;
    _stEXIF_DateTime[eNJPDNum].u32Year = 0;
    _stEXIF_DateTime[eNJPDNum].u32Month = 0;
    _stEXIF_DateTime[eNJPDNum].u32Day = 0;
    _stEXIF_DateTime[eNJPDNum].u32Hour = 0;
    _stEXIF_DateTime[eNJPDNum].u32Minute = 0;
    _stEXIF_DateTime[eNJPDNum].u32Second = 0;
    _eEXIF_Orientation[eNJPDNum] = E_JPEG_EXIF_ORIENT_NOT_FOUND;


#if SUPPORT_EXIF_EXTRA_INFO
    JPEG_memset((void *)_u8EXIF_Manufacturer[eNJPDNum], 0, JPEG_MANUFACTURER_SIZE);
    JPEG_memset((void *)_u8EXIF_Model[eNJPDNum], 0, JPEG_MODEL_SIZE);
    _u16EXIF_Flash[eNJPDNum] = 0;
    _u16EXIF_Exposureprogram[eNJPDNum] = 0;
    _u32EXIF_ISOSpeedRatings[eNJPDNum] = 0;
    _stEXIF_ShutterSpeedValue[eNJPDNum].numerator = 0;
    _stEXIF_ShutterSpeedValue[eNJPDNum].denominator= 0;
    _stEXIF_ApertureValue[eNJPDNum].s_numerator= 0;
    _stEXIF_ApertureValue[eNJPDNum].s_denominator= 0;
    _stEXIF_ExposureBiasValue[eNJPDNum].numerator= 0;
    _stEXIF_ExposureBiasValue[eNJPDNum].denominator= 0;
    _stEXIF_FocalLength[eNJPDNum].s_numerator= 0;
    _stEXIF_FocalLength[eNJPDNum].s_denominator= 0;
    _u32EXIF_ImageWidth[eNJPDNum] = 0;
    _u32EXIF_ImageHeight[eNJPDNum] = 0;
    _stEXIF_ExposureTime[eNJPDNum].numerator = 0;
    _stEXIF_ExposureTime[eNJPDNum].denominator =0;
    _stEXIF_FNumber[eNJPDNum].numerator = 0;
    _stEXIF_FNumber[eNJPDNum].denominator =0;
#endif
}
//------------------------------------------------------------------------------
// Reset everything to default/uninitialized state.
JPEG_STATIC MS_BOOL JPEG_init(void)
{
    MS_U16 i;
    JPEG_DEBUG_API_MSG("%s!!\n", __FUNCTION__);

    _u8LumaCi[eNJPDNum] = 1;
    _u8ChromaCi[eNJPDNum] = 2;
    _u8Chroma2Ci[eNJPDNum] = 3;
    bIs3HuffTbl[eNJPDNum] = FALSE;
#if SUPPORT_HIGH_LOW_REVERSE
    bHighLowReverse[eNJPDNum] = FALSE;
#else
    bDifferentHL[eNJPDNum] = FALSE;
#endif

    _Error_code[eNJPDNum] = E_JPEG_NO_ERROR;

    _u16Image_x_size[eNJPDNum] = _u16Image_y_size[eNJPDNum] = 0;
    _u16OriginalImage_x_size[eNJPDNum] = _u16OriginalImage_y_size[eNJPDNum] = 0;
    _u16AlignedImageWidth[eNJPDNum] = _u16AlignedImagePitch[eNJPDNum] = _u16AlignedImageHeight[eNJPDNum] = 0;
    _u16AlignedImagePitch_H[eNJPDNum] = 0;

////    _pStream = _Pstream;

    _bProgressive_flag[eNJPDNum] = FALSE;
    _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_ORG;
    _u32RLEOffset[eNJPDNum] = 0;
    _bFirstRLE[eNJPDNum] = TRUE;

    _s16dc_pred[eNJPDNum][0] = _s16dc_pred[eNJPDNum][1] = _s16dc_pred[eNJPDNum][2] = 0;

    _Progressive_ROI_flag[eNJPDNum] = FALSE; //CL82399
    ROI_width[eNJPDNum] = 0; //CL82399
    u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_NONE;

    u32MRCheckCount[eNJPDNum] = 0;

    for(i = 0; i<JPEG_MAXHUFFTABLES; i++)
    {
        _Huff_info[eNJPDNum][i].bValid = FALSE;
        JPEG_memset((void *)_Huff_info[eNJPDNum][i].u8Huff_num, 0, 17);
        JPEG_memset((void *)_Huff_info[eNJPDNum][i].u8Huff_val, 0, 256);
        JPEG_memset((void *)_Huff_info[eNJPDNum][i].u8Symbol, 0, 17);
        JPEG_memset((void *)_Huff_info[eNJPDNum][i].u16Code, 0, 17);
        JPEG_memset((void *)_Huff_info[eNJPDNum][i].u8Valid, 0, 17);
    }

    for(i = 0; i<JPEG_MAXQUANTTABLES; i++)
    {
        _QuantTables[eNJPDNum][i].bValid = FALSE;
        JPEG_memset(_QuantTables[eNJPDNum][i].s16Value, 0, 64);
    }

    gu8Scan_type[eNJPDNum] = E_JPEG_GRAYSCALE;

    _u8Comps_in_frame[eNJPDNum] = 0;

    JPEG_memset((void *)_u8Comp_h_samp[eNJPDNum], 0, sizeof( _u8Comp_h_samp[eNJPDNum] ) );
    JPEG_memset((void *)_u8Comp_v_samp[eNJPDNum], 0, sizeof( _u8Comp_v_samp[eNJPDNum] ) );
    JPEG_memset((void *)_u8Comp_quant[eNJPDNum], 0, sizeof( _u8Comp_quant[eNJPDNum] ) );
    JPEG_memset((void *)_u8Comp_ident[eNJPDNum], 0, sizeof( _u8Comp_ident[eNJPDNum] ) );
    JPEG_memset((void *)_u16Comp_h_blocks[eNJPDNum], 0, sizeof( _u16Comp_h_blocks[eNJPDNum] ) );
    JPEG_memset((void *)_u16Comp_v_blocks[eNJPDNum], 0, sizeof( _u16Comp_v_blocks[eNJPDNum] ) );

    _u8Comps_in_scan[eNJPDNum] = 0;
    JPEG_memset((void *)_u8Comp_list[eNJPDNum], 0, sizeof( _u8Comp_list[eNJPDNum] ) );
    JPEG_memset((void *)_u8Comp_dc_tab[eNJPDNum], 0, sizeof( _u8Comp_dc_tab[eNJPDNum] ) );
    JPEG_memset((void *)_u8Comp_ac_tab[eNJPDNum], 0, sizeof( _u8Comp_ac_tab[eNJPDNum] ) );

    _u8Spectral_start[eNJPDNum] = 0;
    _u8Spectral_end[eNJPDNum] = 0;
    _u8Successive_low[eNJPDNum] = 0;
    _u8Successive_high[eNJPDNum] = 0;

    gu8Max_mcu_x_size[eNJPDNum] = 0;
    gu8Max_mcu_y_size[eNJPDNum] = 0;

    _u8Blocks_per_mcu[eNJPDNum] = 0;
    _u32Max_blocks_per_row[eNJPDNum] = 0;
    _u16Mcus_per_row[eNJPDNum] = 0;
    _u16Mcus_per_col[eNJPDNum] = 0;

    JPEG_memset((void *)_u8Mcu_org[eNJPDNum], 0, sizeof( _u8Mcu_org[eNJPDNum] ) );

    JPEG_memset((void *)_pBlocks[eNJPDNum], 0, sizeof( _pBlocks[eNJPDNum] ) );

#if SUPPORT_PROGRESSIVE_MODE
    _u16Total_lines_left[eNJPDNum] = 0;
    JPEG_memset((void *)_u32Block_y_mcu[eNJPDNum], 0, sizeof( _u32Block_y_mcu[eNJPDNum] ) );
    JPEG_memset((void *)_Huff_tbls[eNJPDNum], 0, sizeof( _Huff_tbls[eNJPDNum] ) );
    JPEG_memset((void *)_DC_Coeffs[eNJPDNum], 0, sizeof( _DC_Coeffs[eNJPDNum] ) );
    JPEG_memset((void *)_AC_Coeffs[eNJPDNum], 0, sizeof( _AC_Coeffs[eNJPDNum] ) );
    JPEG_memset((void *)_u32Last_dc_val[eNJPDNum], 0, sizeof( _u32Last_dc_val[eNJPDNum] ) );

    _u32EOB_run[eNJPDNum] = 0;
#endif

    _pu8In_buf_ofs[eNJPDNum] = _pu8In_buf[eNJPDNum];
////    _u32In_buf_left[eNJPDNum] = 0;
////    _bEOF_flag[eNJPDNum] = FALSE;
    _u8Tem_flag[eNJPDNum] = 0;

    //sharon JPEG_memset((void *)_pu8In_buf[eNJPDNum], 0, sizeof(MS_U8)*(MRC_BUFFER_SIZE + 128) );

    _u16Restart_interval[eNJPDNum] = 0;
    _u16Restarts_left[eNJPDNum] = 0;
    _u16Next_restart_num[eNJPDNum] = 0;

    gu16Max_mcus_per_row[eNJPDNum] = 0;
    _u16Max_blocks_per_mcu[eNJPDNum] = 0;
    _u16Max_mcus_per_col[eNJPDNum] = 0;

#if SW_NJPD_RGB_CMYK
    JPEG_memset( _ps16Block_seg, 0, sizeof( _ps16Block_seg ) );
    gpu8Sample_buf = NULL;
#endif

    // Tell the stream we're going to use it.
    //_pStream->attach();

    // Ready the input buffer.
    if(_bThumbnailAccessMode[eNJPDNum])
    {
        // It means that thumbnail is found and re-call JPEG_init()
        // to re-fill thumbnail data to internal buffer.
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_init : JPEG_fill_read_buffer failed!!!\n");
            return FALSE;
        }
    }
    else
    {
////        _u32In_buf_left[eNJPDNum] = MRC_BUFFER_SIZE;  //sharon
////        _Total_Decoded_Size = MRC_BUFFER_SIZE;  //sharon
////        _u32Total_bytes_read[eNJPDNum] = MRC_BUFFER_SIZE;  //sharon
        _u32Total_bytes_read[eNJPDNum] = _u32In_buf_left[eNJPDNum];
    }

    // Prime the bit buffer.
    _s16Bits_left[eNJPDNum] = 0;
    _u32Bit_buf[eNJPDNum] = 0;

//    _pu32ExifHeaderAddr = 0;

#if SW_NJPD_RGB_CMYK
    for ( i = 0; i < JPEG_MAXBLOCKSPERROW; i++ )
    {
        _u8Block_max_zag_set[i] = 64;
    }
#endif
    return TRUE;
}
//------------------------------------------------------------------------------
// The coeff_buf series of methods originally stored the coefficients
// into a "virtual" file which was located in EMS, XMS, or a disk file. A cache
// was used to make this process more efficient. Now, we can store the entire
// thing in RAM.
JPEG_STATIC PJPEG_CoeffBuf JPEG_coeff_buf_open(MS_U16 block_num_x, MS_U16 block_num_y, MS_U8 block_len_x, MS_U8 block_len_y)
{
    PJPEG_CoeffBuf cb = ( PJPEG_CoeffBuf )JPEG_alloc( sizeof( JPEG_CoeffBuf ) );

    if(cb == NULL)
        return NULL;

    cb->u16Block_num_x = block_num_x;
    cb->u16Block_num_y = block_num_y;

    cb->u8Block_len_x = block_len_x;
    cb->u8Block_len_y = block_len_y;

    cb->u16Block_size = ( block_len_x * block_len_y ) * sizeof( JPEG_BLOCK_TYPE );

    cb->pu8Data = ( MS_U8 * )JPEG_alloc( cb->u16Block_size * block_num_x * block_num_y );

    if(cb->pu8Data == NULL)
        return NULL;

    return cb;
}
//------------------------------------------------------------------------------
JPEG_STATIC JPEG_BLOCK_TYPE * JPEG_coeff_buf_getp( PJPEG_CoeffBuf cb, MS_U16 block_x, MS_U16 block_y )
{
    if ( block_x >= cb->u16Block_num_x )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return NULL;
    }

    if ( block_y >= cb->u16Block_num_y )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return NULL;
    }

    return ( JPEG_BLOCK_TYPE * )((MS_U32)( cb->pu8Data + block_x * cb->u16Block_size
                                            + block_y * (cb->u16Block_size * cb->u16Block_num_x)));
}
//------------------------------------------------------------------------------
// Creates the tables needed for efficient Huffman decoding.
JPEG_STATIC MS_BOOL JPEG_make_huff_table(MS_U8 indextmp)
{
    MS_U16 p, i, l, si;
    MS_U8 huffsize[257];
    MS_U16 huffcode[257];
    MS_U16 _code;
    MS_U16 subtree;
    MS_U16 code_size;
    MS_U16 lastp;
#if SUPPORT_PROGRESSIVE_MODE
    MS_S16 nextfreeentry;
#endif
    MS_S16 currententry;

#if SUPPORT_PROGRESSIVE_MODE
    JPEG_HuffTbl *hs = &_Huff_tbls[eNJPDNum][indextmp];
#endif

    JPEG_memset((void *)huffsize, 0, sizeof(huffsize));
    JPEG_memset((void *)huffcode, 0, sizeof(huffcode));

    p = 0;

    for ( l = 1; l <= 16; l++ )
    {
        for ( i = 1; i <= _Huff_info[eNJPDNum][indextmp].u8Huff_num[l]; i++ )
        {
            huffsize[p++] = l;

            //kevinhuang, add protection
            if ( p >= 257 )
            {
                JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
                return FALSE;
            }
        }
    }

    huffsize[p] = 0;

    lastp = p;

    _code = 0;
    si = huffsize[0];
    p = 0;

    while ( huffsize[p] )
    {
        while ( huffsize[p] == si )
        {
            huffcode[p++] = _code;
            _code++;

            //kevinhuang, add protection
            if ( p >= 257 )
            {
                JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
                return FALSE;
            }
        }

        _code <<= 1;
        si++;
    }

    // Calculate the min code
    for(i = 1; i<=16; i++)
    {
        _Huff_info[eNJPDNum][indextmp].u16Code[i] = huffcode[_Huff_info[eNJPDNum][indextmp].u8Symbol[i]];
    }



#if SUPPORT_PROGRESSIVE_MODE
    // In NJPD mode, SW doesn't need huff table when baseline decoding
#if SW_NJPD_RGB_CMYK
    if((E_JPEG_CMYK != gu8Scan_type[eNJPDNum])
    && (E_JPEG_RGB != gu8Scan_type[eNJPDNum]))
#endif
    {
        if(_bProgressive_flag[eNJPDNum]==FALSE)
            return TRUE;
    }

    JPEG_DEBUG_API_MSG("Make HUFF TABLE\n");

    JPEG_memset((void *)(hs->s16Look_up), 0, sizeof( hs->s16Look_up ) );
    JPEG_memset((void *)(hs->s16Tree), 0, sizeof( hs->s16Tree ) );
    JPEG_memset((void *)(hs->u8Code_size), 0, sizeof( hs->u8Code_size ) );

    nextfreeentry = -1;

    p = 0;

    while ( p < lastp )
    {
        i = _Huff_info[eNJPDNum][indextmp].u8Huff_val[p];
        _code = huffcode[p];
        code_size = huffsize[p];

        hs->u8Code_size[i] = code_size;

        if ( code_size <= 8 )
        {
            _code <<= ( 8 - code_size );

            for ( l = 1 << ( 8 - code_size ); l > 0; l-- )
            {
                hs->s16Look_up[_code] = i;
                _code++;
            }
        }
        else
        {
            subtree = ( _code >> ( code_size - 8 ) ) & 0xFF;

            currententry = hs->s16Look_up[subtree];

            if ( currententry == 0 )
            {
                hs->s16Look_up[subtree] = currententry = nextfreeentry;

                nextfreeentry -= 2;
            }

            _code <<= ( 16 - ( code_size - 8 ) );

            for ( l = code_size; l > 9; l-- )
            {
                if ( ( _code & 0x8000 ) == 0 )
                {
                    currententry--;
                }

                if ( hs->s16Tree[-currententry - 1] == 0 )
                {
                    hs->s16Tree[-currententry - 1] = nextfreeentry;

                    currententry = nextfreeentry;

                    nextfreeentry -= 2;
                }
                else
                {
                    currententry = hs->s16Tree[-currententry - 1];
                }

                _code <<= 1;
            }

            if ( ( _code & 0x8000 ) == 0 )
            {
                currententry--;
            }

            hs->s16Tree[-currententry - 1] = i;
        }

        p++;
    }
#endif  //SUPPORT_PROGRESSIVE_MODE
    return TRUE;
}
//------------------------------------------------------------------------------
// Verifies the quantization tables needed for this scan are available.
JPEG_STATIC MS_BOOL JPEG_check_quant_tables( void ) //ok
{
    MS_U8 i;

    for ( i = 0; i < _u8Comps_in_scan[eNJPDNum]; i++ )
    {
        if ( _QuantTables[eNJPDNum][_u8Comp_quant[eNJPDNum][_u8Comp_list[eNJPDNum][i]]].bValid==FALSE )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_QUANT_TABLE );
            return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Verifies that all the Huffman tables needed for this scan are available.
JPEG_STATIC MS_BOOL JPEG_check_huff_tables( void )
{
    MS_U8 i;

    for ( i = 0; i < _u8Comps_in_scan[eNJPDNum]; i++ )
    {
        if ( ( _u8Spectral_start[eNJPDNum] == 0 ) && ( _Huff_info[eNJPDNum][_u8Comp_dc_tab[eNJPDNum][_u8Comp_list[eNJPDNum][i]]].bValid== FALSE ) )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
            return FALSE;
        }

        if ( ( _u8Spectral_end[eNJPDNum] > 0 ) && ( _Huff_info[eNJPDNum][_u8Comp_ac_tab[eNJPDNum][_u8Comp_list[eNJPDNum][i]]].bValid== FALSE ) )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
            return FALSE;
        }
    }

    for ( i = 0; i < JPEG_MAXHUFFTABLES; i++ )
    {
        if ( _Huff_info[eNJPDNum][i].bValid )
        {
            if(!JPEG_make_huff_table(i))
                return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Determines the component order inside each MCU.
// Also calcs how many MCU's are on each row, etc.
JPEG_STATIC void JPEG_calc_mcu_block_order( void ) //ok
{
    MS_U8 component_num, component_id;
    MS_U8 max_h_samp = 0, max_v_samp = 0;

    for ( component_id = 0; component_id < _u8Comps_in_frame[eNJPDNum]; component_id++ )
    {
        if ( _u8Comp_h_samp[eNJPDNum][component_id] > max_h_samp )
        {
            max_h_samp = _u8Comp_h_samp[eNJPDNum][component_id];
        }

        if ( _u8Comp_v_samp[eNJPDNum][component_id] > max_v_samp )
        {
            max_v_samp = _u8Comp_v_samp[eNJPDNum][component_id];
        }
    }

    if((max_h_samp == 0) || (max_v_samp == 0))
    {
        JPEG_terminate( E_JPEG_NOT_ENOUGH_HEADER_INFO );
        return;
    }

    for ( component_id = 0; component_id < _u8Comps_in_frame[eNJPDNum]; component_id++ )
    {
        _u16Comp_h_blocks[eNJPDNum][component_id] = ( ( ( ( _u16Image_x_size[eNJPDNum] * _u8Comp_h_samp[eNJPDNum][component_id] ) + ( max_h_samp - 1 ) ) / max_h_samp ) + 7 ) / 8;
        _u16Comp_v_blocks[eNJPDNum][component_id] = ( ( ( ( _u16Image_y_size[eNJPDNum] * _u8Comp_v_samp[eNJPDNum][component_id] ) + ( max_v_samp - 1 ) ) / max_v_samp ) + 7 ) / 8;
    }

    if ( _u8Comps_in_scan[eNJPDNum] == 1 )
    {
        _u16Mcus_per_row[eNJPDNum] = _u16Comp_h_blocks[eNJPDNum][_u8Comp_list[eNJPDNum][0]];
        _u16Mcus_per_col[eNJPDNum] = _u16Comp_v_blocks[eNJPDNum][_u8Comp_list[eNJPDNum][0]];
    }
    else
    {
        _u16Mcus_per_row[eNJPDNum] = ( ( ( _u16Image_x_size[eNJPDNum] + 7 ) / 8 ) + ( max_h_samp - 1 ) ) / max_h_samp;
        _u16Mcus_per_col[eNJPDNum] = ( ( ( _u16Image_y_size[eNJPDNum] + 7 ) / 8 ) + ( max_v_samp - 1 ) ) / max_v_samp;
    }

    if ( _u8Comps_in_scan[eNJPDNum] == 1 )
    {
        _u8Mcu_org[eNJPDNum][0] = _u8Comp_list[eNJPDNum][0];

        _u8Blocks_per_mcu[eNJPDNum] = 1;
    }
    else
    {
        _u8Blocks_per_mcu[eNJPDNum] = 0;

        for ( component_num = 0; component_num < _u8Comps_in_scan[eNJPDNum]; component_num++ )
        {
            MS_U8 num_blocks;

            component_id = _u8Comp_list[eNJPDNum][component_num];

            num_blocks = _u8Comp_h_samp[eNJPDNum][component_id] * _u8Comp_v_samp[eNJPDNum][component_id];

            while ( num_blocks-- )
            {
                _u8Mcu_org[eNJPDNum][_u8Blocks_per_mcu[eNJPDNum]++] = component_id;
            }
        }
    }
}
//------------------------------------------------------------------------------
/* Get current access byte address in MRC buffer relative to MRC start address */
JPEG_STATIC MS_U32 JPEG_GetECS(void)
{
    MS_U32 data_end_addr = (MS_U32)_pu8In_buf_ofs[eNJPDNum];
    MS_U32 data_start_addr =(MS_U32) _pu8In_buf[eNJPDNum];

    return (data_end_addr - data_start_addr);
}
//------------------------------------------------------------------------------
//*************************************************
//write symbol table
//*************************************************
//------------------------------------------------------------------------------
JPEG_STATIC void JPEG_WriteSymidx(void)
{
    MS_U16 ac_tbl_num_luma = 0, ac_tbl_num_chroma = 0;
    MS_U16 dc_tbl_num_luma = 0, dc_tbl_num_chroma = 0;
    MS_U8 ci, luma_ci = 0, chroma_ci = 0;
    MS_U16 ac_tbl_num_chroma2 = 0;
    MS_U16 dc_tbl_num_chroma2 = 0;
    MS_U8 chroma2_ci = 0;
    NJPD_Symidx structSymidx;

    JPEG_memset((void *)(&structSymidx), 0, sizeof(structSymidx));

    if (_HeadCheck[eNJPDNum].DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
        {
            if(_u8LumaCi[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
        {
            if(_u8ChromaCi[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        if(TRUE == bIs3HuffTbl[eNJPDNum])
        {
            for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
            {
                if(_u8Chroma2Ci[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
                {
                    chroma2_ci = ci;
                    break;
                }
            }
        }

        ac_tbl_num_luma = _u8Comp_ac_tab[eNJPDNum][luma_ci];
        ac_tbl_num_chroma = _u8Comp_ac_tab[eNJPDNum][chroma_ci];


        dc_tbl_num_luma = _u8Comp_dc_tab[eNJPDNum][luma_ci];
        dc_tbl_num_chroma = _u8Comp_dc_tab[eNJPDNum][chroma_ci];
        if(TRUE == bIs3HuffTbl[eNJPDNum])
        {
            ac_tbl_num_chroma2 = _u8Comp_ac_tab[eNJPDNum][chroma2_ci];
            dc_tbl_num_chroma2 = _u8Comp_dc_tab[eNJPDNum][chroma2_ci];
        }
    }

    structSymidx.DHT = _HeadCheck[eNJPDNum].DHT;
    structSymidx.bUVHuffman = bIs3HuffTbl[eNJPDNum];
    structSymidx.u8DcLumaHuffVal = _Huff_info[eNJPDNum][dc_tbl_num_luma].u8Huff_val;
    structSymidx.u8DcChromaHuffVal = _Huff_info[eNJPDNum][dc_tbl_num_chroma].u8Huff_val;
    structSymidx.u8AcLumaHuffVal = _Huff_info[eNJPDNum][ac_tbl_num_luma].u8Huff_val;
    structSymidx.u8AcChromaHuffVal = _Huff_info[eNJPDNum][ac_tbl_num_chroma].u8Huff_val;
    if(TRUE == bIs3HuffTbl[eNJPDNum])
    {
        structSymidx.u8DcChroma2HuffVal = _Huff_info[eNJPDNum][dc_tbl_num_chroma2].u8Huff_val;
        structSymidx.u8AcChroma2HuffVal = _Huff_info[eNJPDNum][ac_tbl_num_chroma2].u8Huff_val;
    }
    MDrv_NJPD_WriteSymidx(structSymidx);
}
//------------------------------------------------------------------------------
//***************************************************
//write quantization table
//***************************************************
JPEG_STATIC void JPEG_WriteIQTbl(void)
{
    //MS_U8 i, j;
    MS_U8 i;
    MS_U8 com_num = 0;
    MS_U8 comp[JPEG_MAXCOMPONENTS];
    NJPD_IQTbl structIqtbl;

    JPEG_memset((void *)&structIqtbl, 0, sizeof(structIqtbl));

    if (_HeadCheck[eNJPDNum].DQT)
    {
        // Calculate how many valid quantization tables
        JPEG_memset((void *)comp, 0, JPEG_MAXCOMPONENTS);
        for(i = 0; i<_u8Comps_in_frame[eNJPDNum]; i++)
        {
            comp[_u8Comp_quant[eNJPDNum][i]] = 1;
        }

        for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
        {
            if(comp[i]==1)
                com_num++;
        }

    }

    structIqtbl.DQT = _HeadCheck[eNJPDNum].DQT;
    structIqtbl.u8CompNum = com_num;
    structIqtbl.u8CompQuant = _u8Comp_quant[eNJPDNum];
    structIqtbl.QuantTables = (NJPD_QuanTbl *)_QuantTables[eNJPDNum];
    MDrv_NJPD_WriteIQTbl(structIqtbl);
}
//------------------------------------------------------------------------------
//*************************************************
//write group information
//*************************************************
JPEG_STATIC void JPEG_WriteGrpinf(void) //type : luma=>0  chroma=>1
{
    MS_U16 ci, dc_tbl_num_luma = 0, dc_tbl_num_chroma = 0;
    MS_U16 ac_tbl_num_luma = 0, ac_tbl_num_chroma = 0;
    MS_U8 luma_ci = 0, chroma_ci = 0;
    MS_U16 dc_tbl_num_chroma2 = 0;
    MS_U16 ac_tbl_num_chroma2 = 0;
    MS_U8 chroma2_ci = 0;
    NJPD_Grpinf structGrpinf;

    JPEG_memset((void *)&structGrpinf, 0, sizeof(structGrpinf));

    if (_HeadCheck[eNJPDNum].DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
        {
            if(_u8LumaCi[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
        {
            if(_u8ChromaCi[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        if(TRUE == bIs3HuffTbl[eNJPDNum])
        {
            for(ci = 0; ci<_u8Comps_in_frame[eNJPDNum]; ci++)
            {
                if(_u8Chroma2Ci[eNJPDNum]==_u8Comp_ident[eNJPDNum][ci])
                {
                    chroma2_ci = ci;
                    break;
                }
            }
        }

        dc_tbl_num_luma = _u8Comp_dc_tab[eNJPDNum][luma_ci];
        dc_tbl_num_chroma = _u8Comp_dc_tab[eNJPDNum][chroma_ci];

        ac_tbl_num_luma = _u8Comp_ac_tab[eNJPDNum][luma_ci];
        ac_tbl_num_chroma = _u8Comp_ac_tab[eNJPDNum][chroma_ci];

        if(TRUE == bIs3HuffTbl[eNJPDNum])
        {
            dc_tbl_num_chroma2 = _u8Comp_dc_tab[eNJPDNum][chroma2_ci];
            ac_tbl_num_chroma2 = _u8Comp_ac_tab[eNJPDNum][chroma2_ci];
        }
    }

    structGrpinf.DHT = _HeadCheck[eNJPDNum].DHT;
    structGrpinf.bUVHuffman = bIs3HuffTbl[eNJPDNum];
    structGrpinf.u8DcLumaValid= _Huff_info[eNJPDNum][dc_tbl_num_luma].u8Valid;
    structGrpinf.u8DcLumaSymbol = _Huff_info[eNJPDNum][dc_tbl_num_luma].u8Symbol;
    structGrpinf.u16DcLumaCode = _Huff_info[eNJPDNum][dc_tbl_num_luma].u16Code;
    structGrpinf.u8DcChromaValid= _Huff_info[eNJPDNum][dc_tbl_num_chroma].u8Valid;
    structGrpinf.u8DcChromaSymbol = _Huff_info[eNJPDNum][dc_tbl_num_chroma].u8Symbol;
    structGrpinf.u16DcChromaCode = _Huff_info[eNJPDNum][dc_tbl_num_chroma].u16Code;
    structGrpinf.u8AcLumaValid= _Huff_info[eNJPDNum][ac_tbl_num_luma].u8Valid;
    structGrpinf.u8AcLumaSymbol = _Huff_info[eNJPDNum][ac_tbl_num_luma].u8Symbol;
    structGrpinf.u16AcLumaCode = _Huff_info[eNJPDNum][ac_tbl_num_luma].u16Code;
    structGrpinf.u8AcChromaValid= _Huff_info[eNJPDNum][ac_tbl_num_chroma].u8Valid;
    structGrpinf.u8AcChromaSymbol = _Huff_info[eNJPDNum][ac_tbl_num_chroma].u8Symbol;
    structGrpinf.u16AcChromaCode = _Huff_info[eNJPDNum][ac_tbl_num_chroma].u16Code;
    structGrpinf.u8DcLumaCnt = _Huff_info[eNJPDNum][dc_tbl_num_luma].u8SymbolCnt;
    structGrpinf.u8DcChromaCnt = _Huff_info[eNJPDNum][dc_tbl_num_chroma].u8SymbolCnt;
    structGrpinf.u8AcLumaCnt = _Huff_info[eNJPDNum][ac_tbl_num_luma].u8SymbolCnt;
    structGrpinf.u8AcChromaCnt = _Huff_info[eNJPDNum][ac_tbl_num_chroma].u8SymbolCnt;
    if(TRUE == bIs3HuffTbl[eNJPDNum])
    {
        structGrpinf.u8DcChroma2Valid= _Huff_info[eNJPDNum][dc_tbl_num_chroma2].u8Valid;
        structGrpinf.u8DcChroma2Symbol = _Huff_info[eNJPDNum][dc_tbl_num_chroma2].u8Symbol;
        structGrpinf.u16DcChroma2Code = _Huff_info[eNJPDNum][dc_tbl_num_chroma2].u16Code;
        structGrpinf.u8AcChroma2Valid= _Huff_info[eNJPDNum][ac_tbl_num_chroma2].u8Valid;
        structGrpinf.u8AcChroma2Symbol = _Huff_info[eNJPDNum][ac_tbl_num_chroma2].u8Symbol;
        structGrpinf.u16AcChroma2Code = _Huff_info[eNJPDNum][ac_tbl_num_chroma2].u16Code;
        structGrpinf.u8DcChroma2Cnt = _Huff_info[eNJPDNum][dc_tbl_num_chroma2].u8SymbolCnt;
        structGrpinf.u8AcChroma2Cnt = _Huff_info[eNJPDNum][ac_tbl_num_chroma2].u8SymbolCnt;
    }
    MDrv_NJPD_WriteGrpinf(structGrpinf);
}
//------------------------------------------------------------------------------
// Write RLE result
JPEG_STATIC MS_BOOL JPEG_write_RLE(JPEG_SVLD *pVld, MS_BOOL bDecodeNow)
{
    MS_U8 *mrc_buffer = (MS_U8 *) MRC_BUFFER_ADDR;
    MS_U16 status;
    MS_U32 start_time;

    JPEG_memcpy((void *)(mrc_buffer + _u32RLEOffset[eNJPDNum]), (void *)pVld, 4);
    _u32RLEOffset[eNJPDNum] += 4;

    // Check if buffer full
    if((MRC_BUFFER_SIZE == _u32RLEOffset[eNJPDNum])
    || (TRUE == bDecodeNow))
    {
        JPEG_DEBUG_API_MSG("Do RLE, LENG 0x%lx, bDecodeNow = %d\n", _u32RLEOffset[eNJPDNum], bDecodeNow);
        JPEG_DEBUG_API_MSG("CPU Sync and Flush Memory~~~~\n");
        MAsm_CPU_Sync();
        MsOS_FlushMemory();

        if(_bFirstRLE[eNJPDNum] == TRUE)
        {
            // Trigger NJPD decoding
            if(!JPEG_StartDecode())
                return FALSE;
            _bFirstRLE[eNJPDNum] = FALSE;
        }
        if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
        {

    // part 1
            {
                // clear MRC low/high portion read complete event
                MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);

                if(MRC_BUFFER_SIZE/2 >= _u32RLEOffset[eNJPDNum])
                {
                    if(TRUE == bDecodeNow)
                    {
                        JPEG_DEBUG_API_MSG("last one bit enable~~1\n");
                        MDrv_NJPD_ReadLastBuffer();
                    }
                    else
                    {
                        JPEG_DEBUG_API_ERR("something strange~~~\n");
                    }
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
                else
                {
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);

                    if(TRUE == bDecodeNow)
                    {
                        JPEG_DEBUG_API_MSG("last one bit enable~~2\n");
                        MDrv_NJPD_ReadLastBuffer();
                    }
                }
            }

    #if MSOS_GET_SYSTEM_TIME
            start_time = MsOS_GetSystemTime();
    #else
            start_time = 0;
    #endif

    #if 0
            if( bDecodeNow )
            {
                return TRUE;  //wait done in main loop
            }
    #endif

            while(1)
            {
                //status = MDrv_NJPD_ReadNJPDStatus();
                status = MDrv_NJPD_GetEventFlag();
                if(status & E_NJPD_EVENT_DEC_DONE)
                {
                    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_MIU_LAST_Z_PATCH))
                    {
                        MsOS_DelayTask(1);
                    }
                    JPEG_DEBUG_API_MSG("P deocde done\n");
                    break;
                }

                if((status & E_NJPD_EVENT_MINICODE_ERR) || (status & E_NJPD_EVENT_INV_SCAN_ERR)
                    || (status & E_NJPD_EVENT_RES_MARKER_ERR) || (status & E_NJPD_EVENT_RMID_ERR)
    //                || (status & E_NJPD_EVENT_WRITE_PROTECT)
                    )
                {
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Debug();
    //                while(1);
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }

                if(status & E_NJPD_EVENT_MRC0_EMPTY)
                {
                    JPEG_DEBUG_API_MSG("E_NJPD_EVENT_MRC0_EMPTY\n");
                    break;
                }

    #if MSOS_GET_SYSTEM_TIME
                if((MsOS_GetSystemTime() - start_time) >= WRITE_RLE_TIMEOUT_MS)
    #else
                if(start_time++ > WRITE_RLE_TIMEOUT_COUNT)
    #endif
                {
                    JPEG_DEBUG_API_MSG("Partial SVLD decode time out\n");
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Debug();
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }
            }

    // part 2
            {
                // clear MRC low/high portion read complete event
                MDrv_NJPD_SetEventFlag( E_NJPD_EVENT_MRC1_EMPTY);

                if(MRC_BUFFER_SIZE/2 >= _u32RLEOffset[eNJPDNum])
                {
                    // Whether it is last buffer or not, the last buffer status had been set in part 1
                    // So, we do not need to do anything here
                }
                else
                {
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
            }

    #if MSOS_GET_SYSTEM_TIME
            start_time = MsOS_GetSystemTime();
    #else
            start_time = 0;
    #endif

            if( bDecodeNow )
            {
                return TRUE;  //wait done in main loop
            }

            while(1)
            {
                //status = MDrv_NJPD_ReadNJPDStatus();
                status = MDrv_NJPD_GetEventFlag();
                if(status & E_NJPD_EVENT_DEC_DONE)
                {
                    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_MIU_LAST_Z_PATCH))
                    {
                        MsOS_DelayTask(1);
                    }
                    JPEG_DEBUG_API_MSG("P deocde done\n");
                    break;
                }

                if((status & E_NJPD_EVENT_MINICODE_ERR) || (status & E_NJPD_EVENT_INV_SCAN_ERR)
                    || (status & E_NJPD_EVENT_RES_MARKER_ERR) || (status & E_NJPD_EVENT_RMID_ERR)
    //                || (status & E_NJPD_EVENT_WRITE_PROTECT)
                    )
                {
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Debug();
    //                while(1);
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }

                if(status & E_NJPD_EVENT_MRC1_EMPTY)
                {
                    JPEG_DEBUG_API_MSG("E_NJPD_EVENT_MRC1_EMPTY\n");
                    JPEG_DEBUG_API_MSG("Partial SVLD decode done\n");
                    break;
                }

    #if MSOS_GET_SYSTEM_TIME
                if((MsOS_GetSystemTime() - start_time) >= WRITE_RLE_TIMEOUT_MS)
    #else
                if(start_time++ > WRITE_RLE_TIMEOUT_COUNT)
    #endif
                {
                    JPEG_DEBUG_API_MSG("Partial SVLD decode time out\n");
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Debug();
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }
            }


        }
        else
        {
    #if 0 //progressive buffer vld will set here, not in startdecode
            else
    #endif
            {
                // clear MRC low/high portion read complete event
                MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY | E_NJPD_EVENT_MRC1_EMPTY);

                if(MRC_BUFFER_SIZE/2 >= _u32RLEOffset[eNJPDNum])
                {
                    if(TRUE == bDecodeNow)
                    {
                        JPEG_DEBUG_API_MSG("last one bit enable~~ 1\n");
                        MDrv_NJPD_ReadLastBuffer();
                    }
                    else
                    {
                        JPEG_DEBUG_API_ERR("something strange~~~\n");
                    }
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    MsOS_DelayTaskUs(100);
                    JPEG_DEBUG_API_MSG("MRC0 Delay~~~~ 1\n");
                }
                else
                {
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    MsOS_DelayTaskUs(100);
                    JPEG_DEBUG_API_MSG("MRC0 Delay~~~~ 2\n");

                    if(TRUE == bDecodeNow)
                    {
                        JPEG_DEBUG_API_MSG("last one bit enable~~ 2\n");
                        MDrv_NJPD_ReadLastBuffer();
                    }
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
            }

    #if MSOS_GET_SYSTEM_TIME
            start_time = MsOS_GetSystemTime();
    #else
            start_time = 0;
    #endif

            if( bDecodeNow )
            {
                return TRUE;  //wait done in main loop
            }

            while(1)
            {
                //status = MDrv_NJPD_ReadNJPDStatus();
                status = MDrv_NJPD_GetEventFlag();
                if(status & E_NJPD_EVENT_DEC_DONE)
                {
                    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_MIU_LAST_Z_PATCH))
                    {
                        MsOS_DelayTask(1);
                    }
                    JPEG_DEBUG_API_MSG("P deocde done\n");
                    break;
                }

                if((status & E_NJPD_EVENT_MINICODE_ERR) || (status & E_NJPD_EVENT_INV_SCAN_ERR)
                    || (status & E_NJPD_EVENT_RES_MARKER_ERR) || (status & E_NJPD_EVENT_RMID_ERR)
    //                || (status & E_NJPD_EVENT_WRITE_PROTECT)
                    )
                {
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }

                if((status & E_NJPD_EVENT_MRC0_EMPTY) && (status & E_NJPD_EVENT_MRC1_EMPTY))
                {
                    JPEG_DEBUG_API_MSG("Partial SVLD decode done\n");
                    break;
                }

    #if MSOS_GET_SYSTEM_TIME
                if((MsOS_GetSystemTime() - start_time) >= WRITE_RLE_TIMEOUT_MS)
    #else
                if(start_time++ > WRITE_RLE_TIMEOUT_COUNT)
    #endif
                {
                    JPEG_DEBUG_API_MSG("Partial SVLD decode time out\n");
                    // temp patch for protect NJPD from writing to illegal memory
                    JPEG_DEBUG_API_MSG("CurRow = %d, CurCol = %d ",
                            MDrv_NJPD_GetCurRow(),
                            MDrv_NJPD_GetCurCol());
                    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());
                    MDrv_NJPD_Rst();

                    JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                    return FALSE;
                }
            }
        }
        _u32RLEOffset[eNJPDNum] = 0;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Do run length encode of coefficient buffer
//JPEG_STATIC void JPEG_do_RLE(JPEG_BLOCK_TYPE *p, MS_BOOL eop, MS_U8 comp_id)
#if 0
JPEG_STATIC MS_BOOL JPEG_do_RLE(JPEG_BLOCK_TYPE *p, MS_BOOL eop, MS_U8 comp_id, MS_BOOL BlockInRange)
{
    JPEG_SVLD my_vld;
    MS_U8 counter;
    MS_S16 value;
    MS_U16 run;
    MS_U8 cur_blk;
    JPEG_BLOCK_TYPE predictor;

    if(comp_id==0)
        cur_blk = 1;     // Y
    else if(comp_id==1)
        cur_blk = 3;    // U
    else
        cur_blk = 2;    // V

    predictor = _s16dc_pred[eNJPDNum][cur_blk - 1];

    run = 0;
    my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
    my_vld.blk_type = cur_blk;

    //sent DC info
////    my_vld.run = 8;
    if( BlockInRange )//Current block is within display range.
        my_vld.run = 8;
    else
        my_vld.run = 0;

    value = (p[0] - predictor);
    my_vld.sign = (value<0)?1:0;
    my_vld.amp = JPEG_ABS(value);
    my_vld.sym_type = E_RLE_DC;
    if(!JPEG_write_RLE(&my_vld, FALSE))
        return FALSE;

    if( BlockInRange == FALSE )//Current block is not within display range.
        return TRUE;

    my_vld.byte0 = my_vld.byte1 =my_vld.byte2 = my_vld.byte3= 0;
    my_vld.blk_type = cur_blk;

    for(counter = 1;counter<64; counter++)
    {
        if(p[counter]==0)
        {
            run++;
        }
        else
        {
            while(run>15)
            {
                my_vld.sign = 0;
                my_vld.amp = 0;
                my_vld.sym_type = E_RLE_ZRL;
                my_vld.run = 15;
                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
                my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
                my_vld.blk_type = cur_blk;
                run -= 16;
            }

            my_vld.sign = (p[counter]<0)?1:0;
            my_vld.amp = JPEG_ABS(p[counter]);
            my_vld.sym_type = E_RLE_AC;
            my_vld.run = run;

            // Check if the last byte is non-zero. If it's non-zero & EOP, add the EOP flag
            if(counter==63&&eop&&p[63]!=0)
            {
                my_vld.EOP = 1;
                if(!JPEG_write_RLE(&my_vld, TRUE))
                    return FALSE;

                _s16dc_pred[eNJPDNum][cur_blk - 1] = p[0];//update predictor
                return TRUE;
            }
            else
            {
                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
            }

            my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
            my_vld.blk_type = cur_blk;
            run = 0;
        }
    }

    counter = 63;

    if(p[counter]==0)
    {
        my_vld.amp = JPEG_ABS(p[counter]);
        my_vld.sign = p[counter]<0?1:0;
        my_vld.sym_type = E_RLE_EOB;
        my_vld.run = 0;
        if(eop)
        {
            my_vld.EOP = 1;
            if(!JPEG_write_RLE(&my_vld, TRUE))
                return FALSE;
        }
        else
        {
            if(!JPEG_write_RLE(&my_vld, FALSE))
                return FALSE;
        }
    }

    _s16dc_pred[eNJPDNum][cur_blk - 1] = p[0];//update predictor
    return TRUE;
}
#else //nJPD
JPEG_STATIC MS_BOOL JPEG_do_RLE(JPEG_BLOCK_TYPE *p, MS_BOOL eop, MS_U8 comp_id, MS_BOOL BlockInRange)
{
    JPEG_SVLD my_vld;
    MS_U8 counter;
    MS_S16 value;
    MS_U16 run;
    MS_U8 cur_blk;
    JPEG_BLOCK_TYPE predictor;

    if(comp_id==0)
        cur_blk = 1;    // Y
    else if(comp_id==1)
        cur_blk = 3;    // U
    else
        cur_blk = 2;    // V

    predictor = _s16dc_pred[eNJPDNum][cur_blk - 1];

    run = 0;
    my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;

    //sent DC info
////    my_vld.run = 8;
    if( BlockInRange )//Current block is within display range.
        my_vld.run = 8;
    else
        my_vld.run = 0;

    value = (p[0] - predictor);
    my_vld.data = value;

    if(!JPEG_write_RLE(&my_vld, FALSE))
        return FALSE;

    if( BlockInRange == FALSE )//Current block is not within display range.
        return TRUE;

    my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;

    for(counter = 1; counter < 64; counter++)
    {
        if(p[counter]==0)
        {
            run++;
        }
        else
        {
            while(run>15)
            {
                my_vld.data = 0;
                my_vld.run = 15;

                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
                my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
                run -= 16;
            }

            my_vld.data = p[counter];
            my_vld.run = run;

            // Check if the last byte is non-zero. If it's non-zero & EOP, add the EOP flag
            if(counter==63&&eop&&p[63]!=0)
            {
                if(!JPEG_write_RLE(&my_vld, TRUE))
                    return FALSE;

                _s16dc_pred[eNJPDNum][cur_blk - 1] = p[0];//update predictor
                return TRUE;
            }
            else
            {
                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
            }

            my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
            run = 0;
        }
    }

    counter = 63;

    if(p[counter]==0)
    {
        my_vld.data = p[counter];
        my_vld.EOB = 1;
        my_vld.run = 0;
        if(eop)
        {
            if(!JPEG_write_RLE(&my_vld, TRUE))
                return FALSE;
        }
        else
        {
            if(!JPEG_write_RLE(&my_vld, FALSE))
                return FALSE;
        }
    }

    _s16dc_pred[eNJPDNum][cur_blk - 1] = p[0];//update predictor
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
// Starts a new scan.
JPEG_STATIC MS_BOOL JPEG_init_scan(void)
{
    if (!JPEG_locate_sos_marker())
        return FALSE;

    JPEG_calc_mcu_block_order();

    if(_HeadCheck[eNJPDNum].DHT == 0)
    {
        JPEG_read_default_dht();
        _HeadCheck[eNJPDNum].DHT = TRUE;
    }

    if (_HeadCheck[eNJPDNum].DHT)
    {
        if(!JPEG_check_huff_tables())
            return FALSE;
    }

    if (_HeadCheck[eNJPDNum].DQT)
    {
        if(!JPEG_check_quant_tables())
            return FALSE;
    }

#if SUPPORT_PROGRESSIVE_MODE
    JPEG_memset((void *)_u32Last_dc_val[eNJPDNum], 0, _u8Comps_in_frame[eNJPDNum] * sizeof( MS_U32 ) );

    _u32EOB_run[eNJPDNum] = 0;
#endif

    if ( _u16Restart_interval[eNJPDNum] )
    {
        _u16Restarts_left[eNJPDNum] = _u16Restart_interval[eNJPDNum];
        _u16Next_restart_num[eNJPDNum] = 0;
    }

////    _Total_Decoded_Size = (MS_S32)JPEG_GetECS();

    {
#if SW_NJPD_RGB_CMYK
        if(_bProgressive_flag[eNJPDNum]
        || (E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
        || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
#else
        if(_bProgressive_flag[eNJPDNum])
#endif
        {
            // pre-fill bit buffer for later decoding
            _s16Bits_left[eNJPDNum] = 16;
            JPEG_get_bits_2( 16 );
            JPEG_get_bits_2( 16 );
        }
    }

    JPEG_DEBUG_API_MSG("JPEG_init_scan:ECS 0x%08lx\n", JPEG_GetECS());

    return TRUE;
}

#if SW_NJPD_RGB_CMYK
//------------------------------------------------------------------------------
// Create a few tables that allow us to quickly convert YCbCr to RGB.
JPEG_STATIC void msAPI_JPEG_create_look_ups( void )
{
    MS_S16 i, k;
    //kevinhuang, modify
    /*
    for (i = 0; i <= 255; i++)
    {
      //k = (i * 2) - 255;
        k = (i * 2) - 256; // Dec. 28 2001- change so table[128] == 0
      gs32Crr[i] = ( FIX(1.40200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Cbb[i] = ( FIX(1.77200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Crg[i] = (-FIX(0.71414/2)) * k;
      gs32Cbg[i] = (-FIX(0.34414/2)) * k + ONE_HALF;
    }
    */
    for ( i = 0; i <= 255; i++ )
    {
        k = i - 128;

        gs32Crr[i] = ( FIX( 1.40200 ) * k + ONE_HALF ) >> SCALEBITS;
        gs32Cbb[i] = ( FIX( 1.77200 ) * k + ONE_HALF ) >> SCALEBITS;

        gs32Crg[i] = ( -FIX( 0.71414 ) ) * k ; //+ ONE_HALF) >> SCALEBITS;???
        gs32Cbg[i] = ( -FIX( 0.34414 ) ) * k + ONE_HALF; //>> SCALEBITS;???

    }
}

#endif //SW_NJPD_RGB_CMYK
//------------------------------------------------------------------------------
// Starts a frame. Determines if the number of components or sampling factors
// are supported.
JPEG_STATIC MS_BOOL JPEG_init_frame(void) //ok
{
#if SW_NJPD_RGB_CMYK
    MS_U32 i;
    MS_U8 *q;
#endif

    JPEG_DEBUG_API_MSG("JPEG_init_frame:\n");
    if ( _u8Comps_in_frame[eNJPDNum] == 1 )
    {
        gu8Scan_type[eNJPDNum] = E_JPEG_GRAYSCALE;

        _u16Max_blocks_per_mcu[eNJPDNum] = 1;

        gu8Max_mcu_x_size[eNJPDNum] = 8;
        gu8Max_mcu_y_size[eNJPDNum] = 8;
    }
    else if ( _u8Comps_in_frame[eNJPDNum] == 3 )
    {
        if ( ( ( _u8Comp_h_samp[eNJPDNum][1] != 1 ) || ( _u8Comp_v_samp[eNJPDNum][1] != 1 ) ) ||   //support only U_H1V1 & V_H1V1
            ( ( _u8Comp_h_samp[eNJPDNum][2] != 1 ) || ( _u8Comp_v_samp[eNJPDNum][2] != 1 ) ) )
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }

        if ( ( _u8Comp_h_samp[eNJPDNum][0] == 1 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 1 ) )
        {
            //set RGB based jpeg flag
            if(_u8Comp_ident[eNJPDNum][0] == 82 || _u8Comp_ident[eNJPDNum][0] == 71 || _u8Comp_ident[eNJPDNum][0] == 66)
            {
#if SW_NJPD_RGB_CMYK
                if( FALSE == bEnableRGB )
                {
                    JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
                    return FALSE;
                }
                JPEG_DEBUG_API_MSG("Get JPEG_RGB\n");
                gu8Scan_type[eNJPDNum] = E_JPEG_RGB; //RGB
#else
                JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
                return FALSE;
#endif
            }
            else
            {
                JPEG_DEBUG_API_MSG("Get JPEG_YH1V1\n");
                gu8Scan_type[eNJPDNum] = E_JPEG_YH1V1; //4:4:4
            }

            _u16Max_blocks_per_mcu[eNJPDNum] = 3;

            gu8Max_mcu_x_size[eNJPDNum] = 8;
            gu8Max_mcu_y_size[eNJPDNum] = 8;
        }
        else if ( ( _u8Comp_h_samp[eNJPDNum][0] == 2 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 1 ) )
        {
            gu8Scan_type[eNJPDNum] = E_JPEG_YH2V1; //4:2:2

            _u16Max_blocks_per_mcu[eNJPDNum] = 4;

            gu8Max_mcu_x_size[eNJPDNum] = 16;
            gu8Max_mcu_y_size[eNJPDNum] = 8;
        }
        else if ( ( _u8Comp_h_samp[eNJPDNum][0] == 1 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 2 ) )
        {
            gu8Scan_type[eNJPDNum] = E_JPEG_YH1V2;

            _u16Max_blocks_per_mcu[eNJPDNum] = 4;

            gu8Max_mcu_x_size[eNJPDNum] = 8;
            gu8Max_mcu_y_size[eNJPDNum] = 16;
        }
        else if ( ( _u8Comp_h_samp[eNJPDNum][0] == 2 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 2 ) )
        {
            gu8Scan_type[eNJPDNum] = E_JPEG_YH2V2; //4:2:0

            _u16Max_blocks_per_mcu[eNJPDNum] = 6;

            gu8Max_mcu_x_size[eNJPDNum] = 16;
            gu8Max_mcu_y_size[eNJPDNum] = 16;
        }
        else if ( ( _u8Comp_h_samp[eNJPDNum][0] == 4 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 1 ) )
        {
            // 4:1:1
            gu8Scan_type[eNJPDNum] = E_JPEG_YH4V1;

            _u16Max_blocks_per_mcu[eNJPDNum] = 6;

            gu8Max_mcu_x_size[eNJPDNum] = 32;
            gu8Max_mcu_y_size[eNJPDNum] = 8;
        }
        else
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }
    }
#if SW_NJPD_RGB_CMYK
    else if(_u8Comps_in_frame[eNJPDNum] == 4) //handle YCCK & CMYK case, must distinguish YCCK and CMYK later
    {
        if( FALSE == bEnableCMYK )
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
            return FALSE;
        }

        if ( ( _u8Comp_h_samp[eNJPDNum][0] == 1 ) && ( _u8Comp_v_samp[eNJPDNum][0] == 1 ) )
        {
            JPEG_DEBUG_API_MSG("Get JPEG_CMYK\n");
            gu8Scan_type[eNJPDNum] = E_JPEG_CMYK;

            _u16Max_blocks_per_mcu[eNJPDNum] = 4;

            gu8Max_mcu_x_size[eNJPDNum] = 8;
            gu8Max_mcu_y_size[eNJPDNum] = 8;
        }
        else
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }
    }
#endif
    else
    {
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    JPEG_DEBUG_API_MSG("JPEG_init_frame:gu8Scan_type[eNJPDNum] = %d\n", gu8Scan_type[eNJPDNum]);

    gu16Max_mcus_per_row[eNJPDNum] = ( _u16Image_x_size[eNJPDNum] + ( gu8Max_mcu_x_size[eNJPDNum] - 1 ) ) / gu8Max_mcu_x_size[eNJPDNum];
    _u16Max_mcus_per_col[eNJPDNum] = ( _u16Image_y_size[eNJPDNum] + ( gu8Max_mcu_y_size[eNJPDNum] - 1 ) ) / gu8Max_mcu_y_size[eNJPDNum];

#if SW_NJPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        /* these values are for the *destination* pixels: after conversion */

        //Reset image x size by new Max_mcus
        _u16Image_x_size[eNJPDNum] = gu16Max_mcus_per_row[eNJPDNum]*gu8Max_mcu_x_size[eNJPDNum];

        _u16NonAlignmentImage_x_size[eNJPDNum] = _u16Image_x_size[eNJPDNum];

        if ( E_JPEG_GRAYSCALE == gu8Scan_type[eNJPDNum] )
        {
            _u8Dest_bytes_per_pixel = 1;
        }
        else
            //kevinhuang, ToDo 4 -> 3 later
        {
            _u8Dest_bytes_per_pixel = 4;
        }

        _u16Dest_bytes_per_scan_line = ( ( _u16Image_x_size[eNJPDNum] + 15 ) & 0xFFF0 ) * _u8Dest_bytes_per_pixel;
//        _u16Real_dest_bytes_per_scan_line = ( _u16Image_x_size[eNJPDNum] * _u8Dest_bytes_per_pixel );

        // Initialize two scan line buffers.
        // FIXME: Only the V2 sampling factors need two buffers.
        #define DC_ALIGNMENT    16
        //pgu8Scan_line_0         = (U8 *)alloc(_u16Dest_bytes_per_scan_line + 8);
        pgu8Scan_line_0 = ( MS_U8 * )JPEG_alloc( _u16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
        if(pgu8Scan_line_0 == NULL)
            return FALSE;
        JPEG_memset( pgu8Scan_line_0, 0x7F, _u16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE

        //pgu8scan_line_1         = (U8 *)alloc(_u16Dest_bytes_per_scan_line + 8);
        pgu8scan_line_1 = ( MS_U8 * )JPEG_alloc( _u16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
        if(pgu8scan_line_1 == NULL)
            return FALSE;
        JPEG_memset( pgu8scan_line_1, 0x7F, _u16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE
    }
#endif

    _u32Max_blocks_per_row[eNJPDNum] = gu16Max_mcus_per_row[eNJPDNum] * _u16Max_blocks_per_mcu[eNJPDNum];

    // Should never happen
    if ( _u32Max_blocks_per_row[eNJPDNum] > JPEG_MAXBLOCKSPERROW )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return FALSE;
    }

#if SW_NJPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        // Allocate the coefficient buffer, enough for one row's worth of MCU's
        q = ( MS_U8 * )JPEG_alloc( _u32Max_blocks_per_row[eNJPDNum] * 64 * sizeof( JPEG_BLOCK_TYPE ) + 8 );
        if(q == NULL)
            return FALSE;

        // Align to 8-byte boundry, for MMX code
        q = ( MS_U8 * )( ( ( MS_U32 )q + 7 ) & ~7 );

        // The _ps16Block_seg[] array's name dates back to the
        // 16-bit assembler implementation. "seg" stood for "segment".
        for ( i = 0; i < _u32Max_blocks_per_row[eNJPDNum]; i++ )
        {
            _ps16Block_seg[i] = ( JPEG_BLOCK_TYPE * )( (MS_U32)q + i * 64 * sizeof( JPEG_BLOCK_TYPE ) );
        }

        for ( i = 0; i < _u32Max_blocks_per_row[eNJPDNum]; i++ )
        {
            _u8Block_max_zag_set[i] = 64;
        }

        gpu8Sample_buf = ( MS_U8 * )( ( ( MS_U32 )JPEG_alloc( _u32Max_blocks_per_row[eNJPDNum] * 64 + 8 ) + 7 ) & ~7 );
        if(gpu8Sample_buf == NULL)
            return FALSE;

        _u16Total_lines_left[eNJPDNum] = _u16Image_y_size[eNJPDNum];

        msAPI_JPEG_create_look_ups();
    }
#endif

#if SUPPORT_PROGRESSIVE_MODE
    _u16Total_lines_left[eNJPDNum] = _u16Max_mcus_per_col[eNJPDNum] * gu8Max_mcu_y_size[eNJPDNum];
#endif
    return TRUE;
}
//------------------------------------------------------------------------------
#if SUPPORT_PROGRESSIVE_MODE
//------------------------------------------------------------------------------
// Restart interval processing.
JPEG_STATIC MS_BOOL JPEG_process_restart(void)
{
    MS_U16 i, c = 0;

    // Let's scan a little bit to find the marker, but not _too_ far.
    // 1536 is a "fudge factor" that determines how much to scan.
    for ( i = 1536; i > 0; i-- )
    {
        if ( JPEG_get_char() == 0xFF )
        {
            break;
        }
    }

    if ( i == 0 )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    for ( ; i > 0; i-- )
    {
        c = JPEG_get_char();
        if ( c != 0xFF )
        {
            break;
        }
    }

    if ( i == 0 )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    // Is it the expected marker? If not, something bad happened.
    if ( c != ( _u16Next_restart_num[eNJPDNum] + E_JPEG_RST0 ) )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    // Reset each component's DC prediction values.
    JPEG_memset((void *)&_u32Last_dc_val[eNJPDNum], 0, _u8Comps_in_frame[eNJPDNum] * sizeof( MS_U32 ) );

    _u32EOB_run[eNJPDNum] = 0;

    _u16Restarts_left[eNJPDNum] = _u16Restart_interval[eNJPDNum];

    _u16Next_restart_num[eNJPDNum] = ( _u16Next_restart_num[eNJPDNum] + 1 ) & 7;

    // Get the bit buffer going again...
    {
        _s16Bits_left[eNJPDNum] = 16;
        JPEG_get_bits_2( 16 );
        JPEG_get_bits_2( 16 );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// The following methods decode the various types of blocks encountered
// in progressively encoded images.
JPEG_STATIC MS_BOOL JPEG_decode_block_dc_first(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    MS_S32 s, r;
    JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _DC_Coeffs[eNJPDNum][component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    s = JPEG_huff_decode( &_Huff_tbls[eNJPDNum][_u8Comp_dc_tab[eNJPDNum][component_id]] );
    if ( s != 0 )
    {
        r = JPEG_get_bits_2( s );
        s = HUFF_EXTEND_P( r, s );
    }

    // In NJPD mode, the DC coefficient is the difference of nearest DC
    _u32Last_dc_val[eNJPDNum][component_id] = ( s += _u32Last_dc_val[eNJPDNum][component_id] );

    p[0] = s << _u8Successive_low[eNJPDNum];
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_dc_refine(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    if ( JPEG_get_bits_2( 1 ) )
    {
        JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _DC_Coeffs[eNJPDNum][component_id], block_x, block_y );

        if(p == NULL)
        {
            JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        p[0] |= ( 1 << _u8Successive_low[eNJPDNum] );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_ac_first(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    JPEG_BLOCK_TYPE *p;
    MS_S32 k, s, r;

    if ( _u32EOB_run[eNJPDNum] )
    {
        _u32EOB_run[eNJPDNum]--;
        return TRUE;
    }

    p = JPEG_coeff_buf_getp( _AC_Coeffs[eNJPDNum][component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    for ( k = _u8Spectral_start[eNJPDNum]; k <= _u8Spectral_end[eNJPDNum]; k++ )
    {
        s = JPEG_huff_decode( &_Huff_tbls[eNJPDNum][_u8Comp_ac_tab[eNJPDNum][component_id]] );

        r = s >> 4;
        s &= 15;

        if ( s )
        {
            k += r;
            if ( k > 63 )
            {
                JPEG_terminate( E_JPEG_DECODE_ERROR );
                return FALSE;
            }

            r = JPEG_get_bits_2( s );
            s = HUFF_EXTEND_P( r, s );

            // No need to do ZAG order in NJPD mode
#if SW_NJPD_RGB_CMYK
            if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
            || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
            {
                p[_u8ZAG[k]] = s << _u8Successive_low[eNJPDNum];
            }
            else
#endif
            {
                p[k] = s << _u8Successive_low[eNJPDNum];
            }
        }
        else
        {
            if ( r == 15 )
            {
                k += 15;
                if ( k > 63 )
                {
                    JPEG_terminate( E_JPEG_DECODE_ERROR );
                    return FALSE;
                }
            }
            else
            {
                _u32EOB_run[eNJPDNum] = 1 << r;

                if ( r )
                {
                    _u32EOB_run[eNJPDNum] += JPEG_get_bits_2( r );
                }

                _u32EOB_run[eNJPDNum]--;

                break;
            }
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_ac_refine(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    MS_S32 s, k, r;
    MS_S32 p1 = 1 << _u8Successive_low[eNJPDNum];
    MS_S32 m1 = ( -1 ) << _u8Successive_low[eNJPDNum];
    JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _AC_Coeffs[eNJPDNum][component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    k = _u8Spectral_start[eNJPDNum];

    if ( _u32EOB_run[eNJPDNum] == 0 )
    {
        for ( ; (k <= _u8Spectral_end[eNJPDNum]) && ( k < 64 ); k++ )
        {
            s = JPEG_huff_decode( &_Huff_tbls[eNJPDNum][_u8Comp_ac_tab[eNJPDNum][component_id]] );

            r = s >> 4;
            s &= 15;

            if ( s )
            {
                if ( s != 1 )
                {
                    JPEG_terminate( E_JPEG_DECODE_ERROR );
                    return FALSE;
                }

                if ( JPEG_get_bits_2( 1 ) )
                {
                    s = p1;
                }
                else
                {
                    s = m1;
                }
            }
            else
            {
                if ( r != 15 )
                {
                    _u32EOB_run[eNJPDNum] = 1 << r;

                    if ( r )
                    {
                        _u32EOB_run[eNJPDNum] += JPEG_get_bits_2( r );
                    }

                    break;
                }
            }

            do
            {
                // No need to do ZAG order in NJPD mode
                JPEG_BLOCK_TYPE *this_coef;
#if SW_NJPD_RGB_CMYK
                if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
                || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
                {
                    this_coef = p + _u8ZAG[k];
                }
                else
#endif
                {
                    this_coef = p + k;
                }

                if ( *this_coef != 0 )
                {
                    if ( JPEG_get_bits_2( 1 ) )
                    {
                        if ( ( *this_coef & p1 ) == 0 )
                        {
                            if ( *this_coef >= 0 )
                            {
                                *this_coef += p1;
                            }
                            else
                            {
                                *this_coef += m1;
                            }
                        }
                    }
                }
                else
                {
                    if ( --r < 0 )
                    {
                        break;
                    }
                }

                k++;
            }
            while ( (k <= _u8Spectral_end[eNJPDNum]) && ( k < 64 ) );

            if ( ( s ) && ( k < 64 ) )
            {
                // No need to do ZAG order in NJPD mode
#if SW_NJPD_RGB_CMYK
                if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
                || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
                {
                    p[_u8ZAG[k]] = s;
                }
                else
#endif
                {
                    p[k] = s;
                }
            }
        }
    }

    if ( _u32EOB_run[eNJPDNum] > 0 )
    {
        for ( ; (k <= _u8Spectral_end[eNJPDNum]) && ( k < 64 ); k++ )
        {
            // No need to do ZAG order in NJPD mode
            JPEG_BLOCK_TYPE *this_coef;
#if SW_NJPD_RGB_CMYK
            if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
            || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
            {
                this_coef = p + _u8ZAG[k];
            }
            else
#endif
            {
                this_coef = p + k;
            }

            if ( *this_coef != 0 )
            {
                if ( JPEG_get_bits_2( 1 ) )
                {
                    if ( ( *this_coef & p1 ) == 0 )
                    {
                        if ( *this_coef >= 0 )
                        {
                            *this_coef += p1;
                        }
                        else
                        {
                            *this_coef += m1;
                        }
                    }
                }
            }
        }

        _u32EOB_run[eNJPDNum]--;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Decode a scan in a progressively encoded image.
JPEG_STATIC MS_BOOL JPEG_decode_scan(Pdecode_block_func decode_block_func)
{
    MS_U16 mcu_row, mcu_col, mcu_block;
    MS_U32 block_x_mcu[JPEG_MAXCOMPONENTS], block_y_mcu[JPEG_MAXCOMPONENTS];

    JPEG_memset((void *)block_y_mcu, 0, sizeof( block_y_mcu ) );

    for ( mcu_col = 0; mcu_col < _u16Mcus_per_col[eNJPDNum]; mcu_col++ )
    {
        int component_num, component_id;

        JPEG_memset((void *)block_x_mcu, 0, sizeof( block_x_mcu ) );

        for ( mcu_row = 0; mcu_row < _u16Mcus_per_row[eNJPDNum]; mcu_row++ )
        {
            MS_U8 block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;

            if ( ( _u16Restart_interval[eNJPDNum] ) && ( _u16Restarts_left[eNJPDNum] == 0 ) )
            {
                if(!JPEG_process_restart())
                    return FALSE;
            }

            for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu[eNJPDNum]; mcu_block++ )
            {
                component_id = _u8Mcu_org[eNJPDNum][mcu_block];

                if(!decode_block_func( component_id, block_x_mcu[component_id] + block_x_mcu_ofs, block_y_mcu[component_id] + block_y_mcu_ofs ))
                {
                    JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                    return FALSE;
                }

                if ( _u8Comps_in_scan[eNJPDNum] == 1 )
                {
                    block_x_mcu[component_id]++;
                }
                else
                {
                    if ( ++block_x_mcu_ofs == _u8Comp_h_samp[eNJPDNum][component_id] )
                    {
                        block_x_mcu_ofs = 0;

                        if ( ++block_y_mcu_ofs == _u8Comp_v_samp[eNJPDNum][component_id] )
                        {
                            block_y_mcu_ofs = 0;

                            block_x_mcu[component_id] += _u8Comp_h_samp[eNJPDNum][component_id];
                        }
                    }
                }
            }

            _u16Restarts_left[eNJPDNum]--;
        }

        if ( _u8Comps_in_scan[eNJPDNum] == 1 )
        {
            block_y_mcu[_u8Comp_list[eNJPDNum][0]]++;
        }
        else
        {
            for ( component_num = 0; component_num < _u8Comps_in_scan[eNJPDNum]; component_num++ )
            {
                component_id = _u8Comp_list[eNJPDNum][component_num];

                block_y_mcu[component_id] += _u8Comp_v_samp[eNJPDNum][component_id];
            }
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Decode a progressively encoded image.
JPEG_STATIC MS_BOOL JPEG_init_progressive(void)
{
    MS_U8 i;

    if ( _u8Comps_in_frame[eNJPDNum] == 4 )
    {
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    // Allocate the coefficient buffers.
    for ( i = 0; i < _u8Comps_in_frame[eNJPDNum]; i++ )
    {
        _DC_Coeffs[eNJPDNum][i] = JPEG_coeff_buf_open( ((gu16Max_mcus_per_row[eNJPDNum]+0x1)& ~0x1) * _u8Comp_h_samp[eNJPDNum][i], ((_u16Max_mcus_per_col[eNJPDNum]+0x1)& ~0x1) * _u8Comp_v_samp[eNJPDNum][i], 1, 1 );
        if(_DC_Coeffs[eNJPDNum][i] == NULL)
        {
            return FALSE;
        }

        _AC_Coeffs[eNJPDNum][i] = JPEG_coeff_buf_open( ((gu16Max_mcus_per_row[eNJPDNum]+0x1)& ~0x1) * _u8Comp_h_samp[eNJPDNum][i], ((_u16Max_mcus_per_col[eNJPDNum]+0x1)& ~0x1) * _u8Comp_v_samp[eNJPDNum][i], 8, 8 );
        if(_AC_Coeffs[eNJPDNum][i] == NULL)
        {
            return FALSE;
        }
    }

    for ( ; ; )
    {
        MS_BOOL dc_only_scan, refinement_scan;
        Pdecode_block_func decode_block_func;

        if (!JPEG_init_scan())
        {
            break;
        }

        dc_only_scan = ( _u8Spectral_start[eNJPDNum] == 0 );
        refinement_scan = ( _u8Successive_high[eNJPDNum] != 0 );

        if ( ( _u8Spectral_start[eNJPDNum] > _u8Spectral_end[eNJPDNum] ) || ( _u8Spectral_end[eNJPDNum] > 63 ) )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
            return FALSE;
        }

        if ( dc_only_scan )
        {
            if ( _u8Spectral_end[eNJPDNum] )
            {
                JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
                return FALSE;
            }
        }
        else if ( _u8Comps_in_scan[eNJPDNum] != 1 )  /* AC scans can only contain one component */
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
            return FALSE;
        }

        if ( ( refinement_scan ) && ( _u8Successive_low[eNJPDNum] != _u8Successive_high[eNJPDNum] - 1 ) )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SUCCESSIVE );
            return FALSE;
        }

        if ( dc_only_scan )
        {
            if ( refinement_scan )
            {
                decode_block_func = JPEG_decode_block_dc_refine;
            }
            else
            {
                decode_block_func = JPEG_decode_block_dc_first;
            }
        }
        else
        {
            if ( refinement_scan )
            {
                decode_block_func = JPEG_decode_block_ac_refine;
            }
            else
            {
                decode_block_func = JPEG_decode_block_ac_first;
            }
        }

        if(!JPEG_decode_scan( decode_block_func ))
            return FALSE;

        _s16Bits_left[eNJPDNum] = 0;
    }

    _u8Comps_in_scan[eNJPDNum] = _u8Comps_in_frame[eNJPDNum];

    for ( i = 0; i < _u8Comps_in_frame[eNJPDNum]; i++ )
    {
        _u8Comp_list[eNJPDNum][i] = i;
    }

    JPEG_calc_mcu_block_order();
    return TRUE;
}
#endif
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_init_sequential(void)
{
    if ( !JPEG_init_scan() )
    {
        JPEG_DEBUG_API_ERR("JPEG_init_scan - E_JPEG_UNEXPECTED_MARKER\n");
        JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
        return FALSE;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
#if 0
JPEG_STATIC void JPEG_decode_start(void)
{
    JPEG_init_frame();

    if ( _bProgressive_flag[eNJPDNum] )
    {
        JPEG_init_progressive();
    }
    else
    {
        JPEG_init_sequential();
    }
}
#endif
//------------------------------------------------------------------------------
// Find the start of the JPEG file and reads enough data to determine
// its size, number of components, etc.
JPEG_STATIC MS_BOOL JPEG_decode_init(void)
{
    MS_U16 pic_width, pic_height;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    _HeadCheck[eNJPDNum].result = 0;

    if(!JPEG_init())
        return FALSE;

#if SUPPORT_MPO_FORMAT
    _pu8In_buf_ofs[eNJPDNum] = _pu8In_buf_MPO_ofs[eNJPDNum];
    _u32In_buf_left[eNJPDNum] = _u32In_buf_MPO_left[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG_decode_init(), set _pu8In_buf_ofs[eNJPDNum]=0x%lx, _u32In_buf_left[eNJPDNum]=0x%lx========\n", (MS_U32)_pu8In_buf_ofs[eNJPDNum], (MS_U32)_u32In_buf_left[eNJPDNum]);
#endif

    if(!JPEG_locate_sof_marker())
        return FALSE;

    if (bMHEG5[eNJPDNum])
    {
    if(_Error_code[eNJPDNum] == E_JPEG_UNSUPPORTED_MARKER)
        {
            return TRUE;
        }
    }

    if(_u8DecodeType[eNJPDNum] == E_JPEG_TYPE_THUMBNAIL)
    {
        if(_bThumbnailFound[eNJPDNum])
        {
            _bThumbnailAccessMode[eNJPDNum] = TRUE;
            ////MApi_JPEG_Finalize(); //replace with JPEG_free_all_blocks()
            JPEG_free_all_blocks();

            if(!JPEG_init())
                return FALSE;

            // save the original image size, because msAPI_JPEG_init will reset all variables to 0
            if(!JPEG_locate_sof_marker())
                return FALSE;

            if(_bProgressive_flag[eNJPDNum])
            {
                JPEG_DEBUG_API_ERR("Progressive image in thumbnail... do not handle it\n");
                JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
                return FALSE;
            }
        }
        else
        {
            JPEG_terminate( E_JPEG_NO_THUMBNAIL );
            return FALSE;
        }
    }

    // calculate aligned resolution
    JPEG_GetAlignedResolution(&pic_width, &pic_height);
    JPEG_DEBUG_API_MSG("pic_width = %d, pic_height = %d\n",pic_width, pic_height);

    _u16AlignedImagePitch[eNJPDNum] = pic_width;
    _u16AlignedImagePitch_H[eNJPDNum] = pic_height;
    _u16AlignedImageWidth[eNJPDNum] = _u16Image_x_size[eNJPDNum];
    _u16AlignedImageHeight[eNJPDNum] = _u16Image_y_size[eNJPDNum];

#if SUPPORT_MPO_FORMAT
    if(bIsMPOFormat[eNJPDNum])
    {
        JPEG_PRO_MAX_WIDTH[eNJPDNum] = JPEG_MPO_PRO_MAX_WIDTH[eNJPDNum];
        JPEG_PRO_MAX_HEIGHT[eNJPDNum] = JPEG_MPO_PRO_MAX_HEIGHT[eNJPDNum];
        JPEG_MAX_WIDTH[eNJPDNum]  = JPEG_MPO_MAX_WIDTH[eNJPDNum];
        JPEG_MAX_HEIGHT[eNJPDNum]   = JPEG_MPO_MAX_HEIGHT[eNJPDNum];
    }
#endif

    if(_bProgressive_flag[eNJPDNum])
    {
        MAX_JPEG_WIDTH_HD[eNJPDNum] = JPEG_PRO_MAX_WIDTH[eNJPDNum];
        MAX_JPEG_HEIGHT_HD[eNJPDNum] = JPEG_PRO_MAX_HEIGHT[eNJPDNum];
    }
    else
    {
        MAX_JPEG_WIDTH_HD[eNJPDNum] = JPEG_MAX_WIDTH[eNJPDNum];
        MAX_JPEG_HEIGHT_HD[eNJPDNum] = JPEG_MAX_HEIGHT[eNJPDNum];
    }

    // calculate down scale factor
    if ((_u16Image_x_size[eNJPDNum] > MAX_JPEG_WIDTH_HD[eNJPDNum] * 4) || (_u16Image_y_size[eNJPDNum] > MAX_JPEG_HEIGHT_HD[eNJPDNum] * 4))
    {
        JPEG_DEBUG_API_MSG("down scale 1/8!!\n");
        _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_EIGHTH;
        _u8ScaleDownFactor[eNJPDNum] = 8;
    }
    else if ((_u16Image_x_size[eNJPDNum] > MAX_JPEG_WIDTH_HD[eNJPDNum] * 2) || (_u16Image_y_size[eNJPDNum] > MAX_JPEG_HEIGHT_HD[eNJPDNum] * 2))
    {
        JPEG_DEBUG_API_MSG("down scale 1/4!!\n");
        _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_FOURTH;
        _u8ScaleDownFactor[eNJPDNum] = 4;
    }
    else if ((_u16Image_x_size[eNJPDNum] > MAX_JPEG_WIDTH_HD[eNJPDNum]) || (_u16Image_y_size[eNJPDNum] > MAX_JPEG_HEIGHT_HD[eNJPDNum]))
    {
        JPEG_DEBUG_API_MSG("down scale 1/2!!\n");
        _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_HALF;
        _u8ScaleDownFactor[eNJPDNum] = 2;
    }
    else
    {
        JPEG_DEBUG_API_MSG("down scale 1/1!!\n");
        _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_ORG;
        _u8ScaleDownFactor[eNJPDNum] = 1;
    }

#if (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_2==TRUE) \
    || (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_4==TRUE) \
    || (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_8==TRUE)
    //downscale test(only for unit test)

    #if (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_2==TRUE)
    _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_HALF;
    #elif (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_4==TRUE)
    _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_FOURTH;
    #elif (ENABLE_TEST_11_NJPEGScaleDownFunctionTest_8==TRUE)
    _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_EIGHTH;
    #else
    _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_ORG;
    #endif

    if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_HALF)
    {
        _u8ScaleDownFactor[eNJPDNum] = 2;
    }
    else if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_FOURTH)
    {
        _u8ScaleDownFactor[eNJPDNum] = 4;
    }
    else if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_EIGHTH)
    {
        _u8ScaleDownFactor[eNJPDNum] = 8;
    }
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_2
        || MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_4
        || MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_8)
    {
        if(MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_2)
        {
            _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_HALF;
        }
        else if(MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_4)
        {
            _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_FOURTH;
        }
        else if(MDrv_NJPD_GetVerificationMode()==E_NJPD25_DOWNSCALE_1_8)
        {
            _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_EIGHTH;
        }
        else
        {
            _u8DownScaleRatio[eNJPDNum] = E_NJPD_DOWNSCALE_ORG;
        }

        if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_HALF)
        {
            _u8ScaleDownFactor[eNJPDNum] = 2;
        }
        else if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_FOURTH)
        {
            _u8ScaleDownFactor[eNJPDNum] = 4;
        }
        else if(_u8DownScaleRatio[eNJPDNum] == E_NJPD_DOWNSCALE_EIGHTH)
        {
            _u8ScaleDownFactor[eNJPDNum] = 8;
        }
    }
#endif

    if (E_NJPD_DOWNSCALE_ORG != _u8DownScaleRatio[eNJPDNum])
    {
        _u16AlignedImagePitch[eNJPDNum] = pic_width / _u8ScaleDownFactor[eNJPDNum];
        _u16AlignedImagePitch_H[eNJPDNum] =
            ((pic_height + ((MS_U32)_u8ScaleDownFactor[eNJPDNum] - 1)) & (~((MS_U32)_u8ScaleDownFactor[eNJPDNum] - 1)))/ _u8ScaleDownFactor[eNJPDNum];
        _u16AlignedImageWidth[eNJPDNum] = _u16Image_x_size[eNJPDNum] / _u8ScaleDownFactor[eNJPDNum];
        _u16AlignedImageHeight[eNJPDNum] =
                ((_u16AlignedImageHeight[eNJPDNum] + ((MS_U32)_u8ScaleDownFactor[eNJPDNum] - 1)) & (~((MS_U32)_u8ScaleDownFactor[eNJPDNum] - 1))) / _u8ScaleDownFactor[eNJPDNum];
    }

    switch (_u8DownScaleRatio[eNJPDNum])
    {
        case E_NJPD_DOWNSCALE_HALF:
            ROI_width[eNJPDNum] = (pic_width / 16) * 16;
            break;

        case E_NJPD_DOWNSCALE_FOURTH:
            ROI_width[eNJPDNum] = (pic_width / 32) * 32;
            break;

        case E_NJPD_DOWNSCALE_EIGHTH:
            ROI_width[eNJPDNum] = (pic_width / 64) * 64;
            break;

        default:
            ROI_width[eNJPDNum] = pic_width;
            break;
    }
    JPEG_DEBUG_API_MSG("Original ROI_width[eNJPDNum] = %d \n", ROI_width[eNJPDNum]);

    if (ROI_width[eNJPDNum] != pic_width)
    {
        _u16AlignedImagePitch[eNJPDNum] = _u16AlignedImageWidth[eNJPDNum] = ROI_width[eNJPDNum] / _u8ScaleDownFactor[eNJPDNum];

        if(_bProgressive_flag[eNJPDNum])
            _Progressive_ROI_flag[eNJPDNum] = TRUE;
    }
    return TRUE;
}
#if 0 // not implement for new MDDI - harold
//------------------------------------------------------------------------------
JPEG_STATIC void msAPI_MJPEG_decode_init(PJPEG_FILE_FileSystem_t Pstream)
{
    JPEG_init( Pstream );
    MSAPI_MJPEG_Get_Pics();
    JPEG_locate_sof_marker();

    if(_u8DecodeType[eNJPDNum] == E_JPEG_TYPE_THUMBNAIL)
    {
        if(_bThumbnailFound[eNJPDNum])
        {
            _bThumbnailAccessMode[eNJPDNum] = TRUE;
            ////MApi_JPEG_Finalize(); //replace with JPEG_free_all_blocks()
            JPEG_free_all_blocks();
            JPEG_init( Pstream );

            JPEG_locate_sof_marker();

            if(_bProgressive_flag[eNJPDNum])
            {
                JPEG_DEBUG_API_MSG("Progressive image in thumbnail... do not handle it\n");
                JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
            }
        }
        else
        {
            JPEG_terminate( E_JPEG_NO_THUMBNAIL );
        }
    }
}
//------------------------------------------------------------------------------
// Parse header of MJPEG to get total num of pics in the file
JPEG_STATIC void MSAPI_MJPEG_Get_Pics(void)
{
    //Num of pics located at Byte 49~52, hence we need to offset 48 bytes
    MS_U8 ByteOffset, ByteSkip = 48;

    //Total num of pics
    NumPics = 0;

    for(ByteOffset = 0; ByteOffset < ByteSkip; ByteOffset++)
    {
        JPEG_get_char();
    }

    //Handle Little-Ending
    NumPics += (JPEG_get_char());
    NumPics += (JPEG_get_char() << 8);
    NumPics += (JPEG_get_char() << 16);
    NumPics += (JPEG_get_char() << 24);

    JPEG_DEBUG_API_MSG("NumPics = %ld\n",NumPics);
}
#endif // not implement for new MDDI - harold

#if SUPPORT_PROGRESSIVE_MODE
//------------------------------------------------------------------------------
// Loads and dequantizes the next row of (already decoded) coefficients.
// Progressive images only.
JPEG_STATIC MS_BOOL JPEG_load_next_row(void)
{
    JPEG_BLOCK_TYPE p[64];
    MS_BOOL EOF_Flag = FALSE; //CL82399

    MS_U16 mcu_row, mcu_block;
    MS_U8 component_num, component_id;
    MS_U16 block_x_mcu[JPEG_MAXCOMPONENTS];

    JPEG_memset((void *)block_x_mcu, 0, JPEG_MAXCOMPONENTS * sizeof( MS_U16 ) );

    for ( mcu_row = 0; mcu_row < _u16Mcus_per_row[eNJPDNum]; mcu_row++ )
    {
        MS_U16 block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;

        for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu[eNJPDNum]; mcu_block++ )
        {
            JPEG_BLOCK_TYPE *pAC;
            JPEG_BLOCK_TYPE *pDC;

            component_id = _u8Mcu_org[eNJPDNum][mcu_block];

            pAC = JPEG_coeff_buf_getp( _AC_Coeffs[eNJPDNum][component_id], block_x_mcu[component_id] + block_x_mcu_ofs, _u32Block_y_mcu[eNJPDNum][component_id] + block_y_mcu_ofs );
            if(pAC == NULL)
            {
                JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            pDC = JPEG_coeff_buf_getp( _DC_Coeffs[eNJPDNum][component_id], block_x_mcu[component_id] + block_x_mcu_ofs, _u32Block_y_mcu[eNJPDNum][component_id] + block_y_mcu_ofs );
            if(pDC == NULL)
            {
                JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            p[0] = pDC[0];
            JPEG_memcpy((void *)&p[1], (void *)&pAC[1], 63 * sizeof( JPEG_BLOCK_TYPE ) );

            if( _Progressive_ROI_flag[eNJPDNum] == FALSE )
            {
                if((mcu_block == (_u8Blocks_per_mcu[eNJPDNum] - 1))
                && (mcu_row == (_u16Mcus_per_row[eNJPDNum] - 1))
                && (_u16Total_lines_left[eNJPDNum] == gu8Max_mcu_y_size[eNJPDNum]))
                {
                    JPEG_DEBUG_API_MSG("EOF!!!!!ROI enable!!!\n");
                    if(!JPEG_do_RLE(p, TRUE, component_id, TRUE))   // means it is end of picture
                        return FALSE;
                }
                else
                {
                    if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                        return FALSE;
                }
            }
            else
            {
                if( _u16Total_lines_left[eNJPDNum] == gu8Max_mcu_y_size[eNJPDNum] )//Last Line
                {
                    //JPEG_DEBUG_API_MSG("_u16Total_lines_left[eNJPDNum] ==%d,%d,%d \n", gu8Max_mcu_y_size[eNJPDNum], mcu_block, mcu_row);

                    if((mcu_block == (_u8Blocks_per_mcu[eNJPDNum] - 1))
                    && ((mcu_row + 2) * gu8Max_mcu_x_size[eNJPDNum] >  ROI_width[eNJPDNum]))//Last line last block within rang
                    {
                        if( EOF_Flag == FALSE )
                        {
                            EOF_Flag = TRUE;
                            JPEG_DEBUG_API_MSG("EOF!!!!!No ROI!!!\n");
                            if(!JPEG_do_RLE(p, TRUE, component_id, TRUE))   // means it is end of picture
                                return FALSE;
                        }
                    }
                    else
                    {
                         if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                            return FALSE;
                    }
                }
                else
                {
                    if((mcu_row + 1) * gu8Max_mcu_x_size[eNJPDNum] >  ROI_width[eNJPDNum])//ever line out rang block
                    {
                        //JPEG_do_RLE(p, FALSE, component_id, FALSE);
                    }
                    else
                    {
                        if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                            return FALSE;
                    }
                }
            }

            if ( _u8Comps_in_scan[eNJPDNum] == 1 )
            {
                block_x_mcu[component_id]++;
            }
            else
            {
                if ( ++block_x_mcu_ofs == _u8Comp_h_samp[eNJPDNum][component_id] )
                {
                    block_x_mcu_ofs = 0;

                    if ( ++block_y_mcu_ofs == _u8Comp_v_samp[eNJPDNum][component_id] )
                    {
                        block_y_mcu_ofs = 0;

                        block_x_mcu[component_id] += _u8Comp_h_samp[eNJPDNum][component_id];
                    }
                }
            }
        }
    }

    if ( _u8Comps_in_scan[eNJPDNum] == 1 )
    {
        _u32Block_y_mcu[eNJPDNum][_u8Comp_list[eNJPDNum][0]]++;
    }
    else
    {
        for ( component_num = 0; component_num < _u8Comps_in_scan[eNJPDNum]; component_num++ )
        {
            component_id = _u8Comp_list[eNJPDNum][component_num];

            _u32Block_y_mcu[eNJPDNum][component_id] += _u8Comp_v_samp[eNJPDNum][component_id];
        }
    }
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/******************************************************************************/
///Start JPEG decoding
/******************************************************************************/
JPEG_STATIC MS_BOOL JPEG_StartDecode(void)
{
    MS_U16 pic_width, pic_height;
    MS_U8 Y_VSF = _u8Comp_v_samp[eNJPDNum][0];
    MS_U8 Y_HSF = _u8Comp_h_samp[eNJPDNum][0];
    MS_BOOL bUV_en;
    MS_U16 reg_value;
    MS_U8 i;
    MS_U8 com_num = 0;
    MS_U8 comp[JPEG_MAXCOMPONENTS];
    NJPD_BufCfg structBuf;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifdef NJPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return FALSE;
    }
#endif

#if ENABLE_JPEG_NO_SIZE_LOWER_BOUND
    MDrv_NJPD_SetPicDimension(0xFF, 0xFF);
#endif

    // reset NJPD hardware
    //MDrv_NJPD_Reset();
    MDrv_NJPD_Rst();

    // Calculate how many valid quantization tables for components
    JPEG_memset((void *)comp, 0, JPEG_MAXCOMPONENTS);
    for(i = 0; i<_u8Comps_in_frame[eNJPDNum]; i++)
    {
        comp[_u8Comp_quant[eNJPDNum][i]] = 1;
    }

    for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
    {
        if(comp[i]==1)
            com_num++;
    }

    if(_u8Comps_in_frame[eNJPDNum]>1)
        bUV_en = TRUE;
    else
        bUV_en = FALSE;

    //Get Aligned width & height
    JPEG_GetAlignedResolution(&pic_width, &pic_height);

#if 1
    structBuf.bProgressive = _bProgressive_flag[eNJPDNum];
    structBuf.bThumbnailAccessMode = _bThumbnailAccessMode[eNJPDNum];
    structBuf.u32ThumbnailBufAddr = _u32InternalBufferAddr_PA[eNJPDNum];
    structBuf.u32ThumbnailBufSize = JPEG_DEFAULT_EXIF_SIZE;
    structBuf.u32ThumbnailBufOffset = _u32ThumbnailOffset[eNJPDNum] + JPEG_GetECS();
    structBuf.u32MRCBufAddr = _u32ReadBufferAddr_PA[eNJPDNum];
    structBuf.u32MRCBufSize = MRC_BUFFER_SIZE;
    structBuf.u32MRCBufOffset = JPEG_GetECS();
    structBuf.u32MWCBufAddr = _u32WriteBufferAddr_PA[eNJPDNum];
    //HW limitation:if we don't enable write-protect mode, set this value to zero.

#if (ENABLE_TEST_09_NJPEGWriteProtectTest==FALSE)
    structBuf.u16MWCBufLineNum = 0;
//    structBuf.u16MWCBufLineNum = ((pic_height/_u8ScaleDownFactor[eNJPDNum] + 8) < 0x07FF)?(pic_height/_u8ScaleDownFactor[eNJPDNum] + 8):0x7FF;
#else
    structBuf.u16MWCBufLineNum = 32;
#endif

    JPEG_DEBUG_API_MSG("bProgressive = %d, bThumbnailAccessMode = %d\n"
    , structBuf.bProgressive, structBuf.bThumbnailAccessMode);
    JPEG_DEBUG_API_MSG("u32ThumbnailBufAddr = 0x%lX, u32ThumbnailBufSize = 0x%lX, u32ThumbnailBufOffset = 0x%lX\n"
    , structBuf.u32ThumbnailBufAddr, structBuf.u32ThumbnailBufSize, structBuf.u32ThumbnailBufOffset);
    JPEG_DEBUG_API_MSG("u32MRCBufAddr = 0x%lX, u32MRCBufSize = 0x%lX, u32MRCBufOffset = 0x%lX\n"
    , structBuf.u32MRCBufAddr, structBuf.u32MRCBufSize, structBuf.u32MRCBufOffset);
    JPEG_DEBUG_API_MSG("u32MWCBufAddr = 0x%lX, u16MWCBufLineNum = %d\n"
    , structBuf.u32MWCBufAddr, structBuf.u16MWCBufLineNum);

    u32DataOffset[eNJPDNum] += JPEG_GetECS();

    if(structBuf.u32MRCBufOffset >= MRC_BUFFER_SIZE)
    {
        JPEG_terminate(E_JPEG_NOTENOUGHMEM);
        JPEG_DEBUG_API_ERR("%s [%d] not enough MRC size(0x%lx, 0x%lx)\n",
            __FUNCTION__, __LINE__, u32DataOffset[eNJPDNum], MRC_BUFFER_SIZE);
        return FALSE;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s [%d] enough MRC size(0x%lx, 0x%lx)\n",
            __FUNCTION__, __LINE__, u32DataOffset[eNJPDNum], MRC_BUFFER_SIZE);
    }
#if SUPPORT_HIGH_LOW_REVERSE
    bHighLowReverse[eNJPDNum] = MDrv_NJPD_InitBuf(structBuf, __bIsMjpeg[eNJPDNum]);
    //printf_red("bHighLowReverse[eNJPDNum]=%d\n", bHighLowReverse[eNJPDNum]);
    JPEG_DEBUG_API_MSG("bHighLowReverse[eNJPDNum]=%d\n", bHighLowReverse[eNJPDNum]);
#else
    bDifferentHL[eNJPDNum] = MDrv_NJPD_InitBuf(structBuf, __bIsMjpeg[eNJPDNum]);
#endif

//    JPEG_DEBUG_API_MSG("MWCLineNum = %d, pic_height = %d, DwnScaleRatio = %d\n"
//            , structBuf.u16MWCBufLineNum, pic_height, _u8ScaleDownFactor[eNJPDNum]);
#else
    if(_bThumbnailAccessMode[eNJPDNum])
    {
        // Set MRC buffer for NJPD
        MDrv_NJPD_SetReadBuffer(INTERNAL_BUFFER_ADDR, JPEG_DEFAULT_EXIF_SIZE);
        // Set MRC start access byte address
        MDrv_NJPD_SetMRCStartAddr(INTERNAL_BUFFER_ADDR + _u32ThumbnailOffset[eNJPDNum] + JPEG_GetECS());
    }
    else
    {
        if(_bProgressive_flag[eNJPDNum])
        {
            // Set MRC buffer for NJPD
            MDrv_NJPD_SetReadBuffer(MRC_BUFFER_ADDR, MRC_BUFFER_SIZE);
            // Set MRC start access byte address
            MDrv_NJPD_SetMRCStartAddr(MRC_BUFFER_ADDR);
        }
        else
        {
            // Set MRC buffer for NJPD
            MDrv_NJPD_SetReadBuffer(MRC_BUFFER_ADDR, MRC_BUFFER_SIZE);
            // Set MRC start access byte address
            MDrv_NJPD_SetMRCStartAddr(MRC_BUFFER_ADDR + JPEG_GetECS());
        }
    }

    // Set MWC buffer for NJPD
    MDrv_NJPD_SetOutputFrameBuffer(MWC_BUFFER_ADDR);
#endif

    // Set picture width and height
#if (ENABLE_JPEG_NO_SIZE_LOWER_BOUND == 0)
    if(_bProgressive_flag[eNJPDNum])
    {
        // This setting is only needed in NPJD
        // ROI is implemented by HW in NJPD
        MDrv_NJPD_SetPicDimension(ROI_width[eNJPDNum], pic_height);
    }
    else
    {
        MDrv_NJPD_SetPicDimension(pic_width, pic_height);
    }
#endif

    _u16NonAlignmentImage_x_size[eNJPDNum] = pic_width;
    _u16NonAlignmentImage_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum];

    _u16Image_x_size[eNJPDNum] = pic_width/_u8ScaleDownFactor[eNJPDNum];
    _u16Image_y_size[eNJPDNum] = pic_height/_u8ScaleDownFactor[eNJPDNum];
    JPEG_DEBUG_API_MSG("ScaleDownFactor = %d\n", _u8ScaleDownFactor[eNJPDNum]);

#if (ENABLE_TEST_NJPD_21_No_Reset_Table_Test == TRUE)
    static MS_BOOL bFirstTime=0;
    MDrv_NJPD_SetDifferentHTable(bIs3HuffTbl[eNJPDNum]);  // This function must be set before JPEG_WriteGrpinf()
    if(bFirstTime ==0)
    {
        MDrv_NJPD_GTable_Rst(ENABLE);
         JPEG_WriteGrpinf();
         JPEG_WriteSymidx();
         JPEG_WriteIQTbl();
         MsOS_FlushMemory();
        MDrv_NJPD_TableLoadingStart();
        bFirstTime =1;
    }
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD21_NO_RESET_TABLE)
    {
        static MS_BOOL bFirstTime[E_NJPD_NJPD_TOTAL]={0, 0};
        MDrv_NJPD_SetDifferentHTable(bIs3HuffTbl[eNJPDNum]);  // This function must be set before JPEG_WriteGrpinf()
        if(bFirstTime[eNJPDNum] ==0)
        {
            MDrv_NJPD_GTable_Rst(ENABLE);
             JPEG_WriteGrpinf();
             JPEG_WriteSymidx();
             JPEG_WriteIQTbl();
             MsOS_FlushMemory();
            MDrv_NJPD_TableLoadingStart();
            bFirstTime[eNJPDNum] =1;
        }
    }
    else
    {
#if PRINT_NJPD_DECODE_TIME
        u32MeasureDecodeTimeTable[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1);
#endif
        // In NJPD, software VLD mode, we don't need to write huff & symbol tables
        if(_bProgressive_flag[eNJPDNum]==FALSE)
        {
            MDrv_NJPD_SetDifferentHTable(bIs3HuffTbl[eNJPDNum]);  // This function must be set before JPEG_WriteGrpinf()
            MDrv_NJPD_GTable_Rst(ENABLE);
            JPEG_WriteGrpinf();
            JPEG_WriteSymidx();
        }

        JPEG_WriteIQTbl();
#if PRINT_NJPD_DECODE_TIME
        u32MeasureDecodeTimeTable[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1)-u32MeasureDecodeTimeTable[eNJPDNum];
#endif
        MsOS_FlushMemory();
        MDrv_NJPD_TableLoadingStart();
    }
#endif
    Y_VSF -= 1;
    Y_HSF -= 1;

    reg_value = MDrv_NJPD_Get_GlobalSetting00();
    if(_u16Restart_interval[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("RST found! Enable NJPD_RST_EN! Restart_interval = %d\n", _u16Restart_interval[eNJPDNum]);
        //MDrv_Write2Byte(BK_NJPD_RSTINTV, _u16Restart_interval[eNJPDNum] - 1);
        MDrv_NJPD_SetRSTIntv(_u16Restart_interval[eNJPDNum]);
        MDrv_NJPD_SetScalingDownFactor((NJPD_SCALING_DOWN_FACTOR)_u8DownScaleRatio[eNJPDNum]);
        reg_value |= ( NJPD_RST_EN | ((MS_U32) bUV_en) << 3 | ( Y_VSF << 2 ) | Y_HSF );
    }
    else
    {
        MDrv_NJPD_SetScalingDownFactor((NJPD_SCALING_DOWN_FACTOR)_u8DownScaleRatio[eNJPDNum]);
        reg_value |= ( ((MS_U32) bUV_en) << 3 | ( Y_VSF << 2 ) | Y_HSF );
    }

    /* Check the read pointer. If it is in HIGH buffer, we need
       to preload data to LOW buffer and then start decode.
       Progressive mode & thumbnail don't need to check it. */
    if((FALSE == _bProgressive_flag[eNJPDNum])
    && (FALSE == _bThumbnailAccessMode[eNJPDNum]))
    {
        MS_U32 start_offset = JPEG_GetECS();
        JPEG_DEBUG_API_MSG("start offset = 0x%lx\n", start_offset);
        if ( start_offset >= (MRC_BUFFER_SIZE/2) )
        {
            JPEG_DEBUG_API_MSG("ReadPtr is in HIGH, Load LOW!!\n");
            if(_pFillHdrFunc[eNJPDNum])
            {
                MS_S32 byte_read = JPEG_FillHeaderFunction((MS_PHYADDR)MS_VA2PA((MS_U32)_pu8In_buf[eNJPDNum]), (MRC_BUFFER_SIZE/2));
                if(byte_read < 0)
                {
                    JPEG_terminate(E_JPEG_STREAM_READ);
                    return FALSE;
                }
                u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_LOW;
                u32MRCheckCount[eNJPDNum] = 0;
            }
            else
            {
                JPEG_DEBUG_API_MSG("_pFillHdrFunc[eNJPDNum] is not registered!!, still need to start decode.\n");
            }
        }
    }

    if(_bProgressive_flag[eNJPDNum])
    {
        MDrv_NJPD_SetSoftwareVLD(ENABLE);
    }

    // Check if it needs to do ROI
    if(E_NJPD_DOWNSCALE_ORG != _u8DownScaleRatio[eNJPDNum])
    {
        if(ROI_width[eNJPDNum] != pic_width && !_bProgressive_flag[eNJPDNum])
        {
            JPEG_DEBUG_API_MSG("ROI!! ROI_width[eNJPDNum] = %d, ROI_height = %d\n", ROI_width[eNJPDNum], pic_height);
            MDrv_NJPD_SetROI(0, 0, (ROI_width[eNJPDNum]>>3), (pic_height>>3));
            reg_value = reg_value | NJPD_ROI_EN;
        }
        _u16Image_x_size[eNJPDNum] = ROI_width[eNJPDNum]/_u8ScaleDownFactor[eNJPDNum];
    }

#if ENABLE_TEST_NJPD_13_ROI_Test
    MDrv_NJPD_SetROI(20, 20, 100, 80);
    reg_value = reg_value | NJPD_ROI_EN;
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD13_ROI)
    {
        MDrv_NJPD_SetROI(20, 20, 100, 80);
        reg_value = reg_value | NJPD_ROI_EN;
    }
#endif

    if(!_bProgressive_flag[eNJPDNum]) //progressive buffer vld setting is in writeRLE
    {
        MsOS_FlushMemory();
        if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
        {
#if SUPPORT_HIGH_LOW_REVERSE
            if(bHighLowReverse[eNJPDNum])
            {
                MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                MsOS_DelayTaskUs(100);
                MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
            }
            else
#endif
            {
                MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
            }
        }
        else
        {
            MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
            MsOS_DelayTaskUs(100);
            MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
        }
    }

    // enable NJPD decoding
#if ENABLE_JPEG_NO_SIZE_LOWER_BOUND
    MDrv_NJPD_SetPicDimension(pic_width, pic_height);
#endif
    MDrv_NJPD_EnablePowerSaving();

#ifndef MSOS_TYPE_LINUX_KERNEL
    MDrv_NJPD_SetAutoProtect(TRUE);
    #if (ENABLE_TEST_22_AutoProtectFailTest == TRUE)
        MDrv_NJPD_SetWPENStartAddr(_u32WriteBufferAddr_PA[eNJPDNum]);
        MDrv_NJPD_SetWPENEndAddr(_u32WriteBufferAddr_PA[eNJPDNum]+0x50000-1);
        JPEG_DEBUG_API_MSG("Set Auto protect address =0x%lx!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", MWC_BUFFER_ADDR+0x50000);
    #else
        if(MDrv_NJPD_GetVerificationMode()==E_NJPD23_WRITE_PROTECT)
        {
            MDrv_NJPD_SetWPENStartAddr(_u32WriteBufferAddr_PA[eNJPDNum]);
            MDrv_NJPD_SetWPENEndAddr(_u32WriteBufferAddr_PA[eNJPDNum]+0x50000-1);
            JPEG_DEBUG_API_MSG("Set Auto protect address =0x%lx!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", MWC_BUFFER_ADDR+0x50000);
        }
        else
        {
            MDrv_NJPD_SetWPENStartAddr(_u32WriteBufferAddr_PA[eNJPDNum]);
            MDrv_NJPD_SetWPENEndAddr(_u32WriteBufferAddr_PA[eNJPDNum]+MWC_BUFFER_SIZE-1);
        }
    #endif
#else
    MDrv_NJPD_SetAutoProtect(FALSE);
#endif


    MDrv_NJPD_Set_GlobalSetting00(reg_value | NJPD_SWRST);
    MDrv_NJPD_DecodeEnable();
    JPEG_DEBUG_API_MSG("After Setting SCONFIG, NJPD START!!\n");
    //JPEG_GO;
    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
    {
        if(!_bProgressive_flag[eNJPDNum]) //progressive buffer vld setting is in writeRLE
        {
#if SUPPORT_HIGH_LOW_REVERSE
#if 0
            if(bHighLowReverse[eNJPDNum])
            {
                JPEG_DEBUG_API_MSG("before Wait E_NJPD_EVENT_MRC1_EMPTY done!!\n");
                JPEG_DEBUG_API_MSG("Flush~~~\n");
                MsOS_FlushMemory();
                MS_U32 u32Time=MsOS_GetSystemTime();
                MS_U16 u16Event;
                while(1)
                {
                    u16Event = MDrv_NJPD_GetEventFlag();
                    if(u16Event &
                        (E_NJPD_EVENT_MRC1_EMPTY |E_NJPD_EVENT_DEC_DONE))
                    {
                        JPEG_DEBUG_API_MSG("after Wait E_NJPD_EVENT_MRC1_EMPTY done!!\n");
                        break;
                    }
                    else if(u16Event & NJPD_EVENT_ERROR)
                    {
                        JPEG_DEBUG_API_ERR("JPEG DECODE ERROR!!!!!!!!!!!\n");
                        MApi_JPEG_SetErrCode(E_JPEG_DECODE_ERROR);
                        MDrv_NJPD_Debug();
                        break;
                    }
                    else if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("JPEG DECODE TIMEOUT!!!!!!!!!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }

                if(u16Event & E_NJPD_EVENT_MRC1_EMPTY)
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
            }
            else
#endif
#endif
            {
                JPEG_DEBUG_API_MSG("before Wait E_NJPD_EVENT_MRC0_EMPTY done!!\n");
                JPEG_DEBUG_API_MSG("Flush~~~\n");
                MsOS_FlushMemory();
                MS_U32 u32Time=MsOS_GetSystemTime();
                MS_U16 u16Event;
                while(1)
                {
                    u16Event = MDrv_NJPD_GetEventFlag();
                    if(u16Event &
                        (E_NJPD_EVENT_MRC0_EMPTY |E_NJPD_EVENT_DEC_DONE))
                    {
                        JPEG_DEBUG_API_MSG("after Wait E_NJPD_EVENT_MRC0_EMPTY done!!\n");
                        break;
                    }
                    else if(u16Event & NJPD_EVENT_ERROR)
                    {
                        JPEG_DEBUG_API_ERR("JPEG DECODE ERROR!!!!!!!!!!!\n");
                        JPEG_terminate(E_JPEG_DECODE_ERROR);
                        MDrv_NJPD_Debug();
                        break;
                    }
                    else if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("JPEG DECODE TIMEOUT!!!!!!!!!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }

                if(u16Event & E_NJPD_EVENT_MRC0_EMPTY)
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
            }
        }
    }
    return TRUE;
}

#if SUPPORT_PROGRESSIVE_MODE
//------------------------------------------------------------------------------
/******************************************************************************/
///Start Progressive JPEG decode for NJPD
/******************************************************************************/
JPEG_STATIC JPEG_Result JPEG_Progressive_Decode(void)
{
    if(_bProgressive_flag[eNJPDNum]==FALSE)
        return (E_JPEG_FAILED);

    if ( _u16Total_lines_left[eNJPDNum] == 0 )
    {
        return ( E_JPEG_OKAY );
    }

#ifdef NJPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return ( E_JPEG_FAILED );
    }
#endif

    //JPEG_DEBUG_API_MSG("_u16Mcus_per_row[eNJPDNum] is %d!\n",_u16Mcus_per_row[eNJPDNum]);
    //JPEG_DEBUG_API_MSG("_u16Mcus_per_col[eNJPDNum] is %d!\n",_u16Mcus_per_col[eNJPDNum]);
    //JPEG_DEBUG_API_MSG("_u8Blocks_per_mcu[eNJPDNum] is %d!\n",_u8Blocks_per_mcu[eNJPDNum]);
    //JPEG_DEBUG_API_MSG("gu8Max_mcu_x_size[eNJPDNum] is %d!\n",gu8Max_mcu_x_size[eNJPDNum]);
    //JPEG_DEBUG_API_MSG("gu8Max_mcu_y_size[eNJPDNum] is %d!\n",gu8Max_mcu_y_size[eNJPDNum]);

    if( _u16Total_lines_left[eNJPDNum] > 0 )
    {
        JPEG_DEBUG_API_MSG("%s:_u16Total_lines_left[eNJPDNum] = %d\n", __FUNCTION__ , _u16Total_lines_left[eNJPDNum]);
        if(!JPEG_load_next_row())
            return E_JPEG_FAILED;

#if SW_NJPD_RGB_CMYK
        if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
        || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
        {
            if(!msAPI_JPEG_transform_row()) //IDCT
            {
                JPEG_DEBUG_API_MSG("CMYK and RGB decode failed!!\n");
                return ( E_JPEG_FAILED );
            }
        }
#endif

        _u16Total_lines_left[eNJPDNum] -= gu8Max_mcu_y_size[eNJPDNum];
    }

    return (E_JPEG_DONE);
}
#endif

//------------------------------------------------------------------------------
JPEG_STATIC void JPEG_GetAlignedResolution(MS_U16 *width, MS_U16 *height)
{
    MS_U8 mcu_width, mcu_height;
    MS_U8 Y_VSF = _u8Comp_v_samp[eNJPDNum][0];
    MS_U8 Y_HSF = _u8Comp_h_samp[eNJPDNum][0];

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    *width = _u16OriginalImage_x_size[eNJPDNum];
    *height = _u16OriginalImage_y_size[eNJPDNum];

    mcu_width = _u16OriginalImage_x_size[eNJPDNum] % (Y_HSF * 8);
    if (mcu_width)
    {
        *width += (Y_HSF * 8 - mcu_width);
    }

    mcu_height = _u16OriginalImage_y_size[eNJPDNum] % (Y_VSF * 8);
    if (mcu_height)
    {
        *height += (Y_VSF * 8 - mcu_height);
    }

    JPEG_DEBUG_API_MSG("_u8Comp_v_samp[eNJPDNum] = %d, _u8Comp_h_samp[eNJPDNum] = %d\n", _u8Comp_v_samp[eNJPDNum][0], _u8Comp_h_samp[eNJPDNum][0]);
    JPEG_DEBUG_API_MSG("_u16OriginalImage_x_size[eNJPDNum] = %d, _u16OriginalImage_y_size[eNJPDNum] = %d\n", _u16OriginalImage_x_size[eNJPDNum], _u16OriginalImage_y_size[eNJPDNum]);
    JPEG_DEBUG_API_MSG("AlignWidth = %d, AlignHeight = %d\n", *width, *height);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if 0
//------------------------------------------------------------------------------
/******************************************************************************/
///This will set MRC buffer address & size, MWC buffer address, and internal buffer
///address & size.
///@param *pInitParam \b The pointer of information for JPEG Buffer initialization
/// structure {
///         U32 u32MRCBufAddr, \b IN MRC buffer address
///         U32 u32MRCBufSize, \b IN MRC buffer size
///         U32 u32MWCBufAddr, \b IN MWC buffer address
///                                 JPEG decompress data (YUV422)
///         U32 u32InternalBufAddr, \b IN internal buffer address
///                                 (exif:64K + memory pool)
///         U32 u32InternalBufSize, \b IN internal buffer size
///         BOOL bInitMem, \b IN initialize memory pool or not
///         };
///@return TRUE or FALSE
/******************************************************************************/
MS_BOOL MApi_JPEG_SetInitParameter(JPEG_InitParam *pInitParam)
{
    // the buffer size must be multiple of 4 bytes
    if((!pInitParam)
    || (pInitParam->u32MRCBufSize < MIN_READBUFFER_SIZE))
    {
        JPEG_terminate(E_JPEG_READBUFFER_TOOSMALL);
        return FALSE;
    }
    // the buffer size must be multiple of 8 bytes
    pInitParam->u32MRCBufSize = pInitParam->u32MRCBufSize & ~0x7;

    MRC_BUFFER_ADDR = pInitParam->u32MRCBufAddr | AEON_NON_CACHE_MASK;
    MWC_BUFFER_ADDR = pInitParam->u32MWCBufAddr | AEON_NON_CACHE_MASK;
    INTERNAL_BUFFER_ADDR = pInitParam->u32InternalBufAddr | AEON_NON_CACHE_MASK;
    MRC_BUFFER_SIZE = pInitParam->u32MRCBufSize;
    INTERNAL_BUFFER_SIZE = pInitParam->u32InternalBufSize;

    _pu8In_buf[eNJPDNum] = (MS_U8 *) pInitParam->u32MRCBufAddr;

    if (pInitParam->bInitMem)
    {
        JPEG_MEMORY_init_mempool(((MS_U8 *)(pInitParam->u32InternalBufAddr + JPEG_DEFAULT_EXIF_SIZE))
                                , (pInitParam->u32InternalBufSize - JPEG_DEFAULT_EXIF_SIZE));
    }

    return TRUE;
}
//------------------------------------------------------------------------------
// Call get_error_code() after constructing to determine if the stream
// was valid or not. You may call the get_width(), get_height(), etc.
// methods after the constructor is called.
// You may then either destruct the object, or begin decoding the image
// by calling begin(), then decode().
void MApi_JPEG_Constructor( PJPEG_FILE_FileSystem_t Pstream, MS_U8 decode_type)
{
    if ( setjmp( _jmp_state ) )
    {
        return;
    }

    _u8DecodeType[eNJPDNum] = decode_type;
    JPEG_init_thumbnail();
    JPEG_decode_init( Pstream);

}
#endif

#if SW_NJPD_RGB_CMYK
JPEG_STATIC MS_BOOL MApi_JPEG_Is_RGB_CMYK(void)
{
    if (_u8Comps_in_frame[eNJPDNum] == 3)
    {
        //support only U_H1V1 & V_H1V1
        if ((_u8Comp_h_samp[eNJPDNum][1] != 1 || _u8Comp_v_samp[eNJPDNum][1] != 1)
            || (_u8Comp_h_samp[eNJPDNum][2] != 1 || _u8Comp_v_samp[eNJPDNum][2] != 1))
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }

        if (_u8Comp_h_samp[eNJPDNum][0] == 1 && _u8Comp_v_samp[eNJPDNum][0] == 1)
        {
            if (_u8Comp_ident[eNJPDNum][0] == 82 || _u8Comp_ident[eNJPDNum][0] == 71 || _u8Comp_ident[eNJPDNum][0] == 66)
                return TRUE; // RGB
        }
    }
    else
    if (_u8Comps_in_frame[eNJPDNum] == 4) //handle YCCK & CMYK case, must distinguish YCCK and CMYK later
    {
        if (_u8Comp_h_samp[eNJPDNum][0] == 1 && _u8Comp_v_samp[eNJPDNum][0] == 1)
            return TRUE; // CMYK
    }

    return FALSE;
}
#endif
//------------------------------------------------------------------------------
#if SUPPORT_OJPD
#define _USE_OJPD(fun, args...) \
   do { \
       if(bUseOJPD) \
       {    \
           return fun##_OJPD(args);\
       }    \
   }while(0)
#else
#define _USE_OJPD(fun, args...)
#endif

static MS_BOOL bUseOJPD =FALSE;
static MS_BOOL bOJPDOn = FALSE;
static MS_BOOL bForceNJPD2 = FALSE;

void _MApi_JPEG_EnableOJPD(MS_BOOL bOnOff)
{
    bOJPDOn = bOnOff;
}

JPEG_Result _MApi_JPEG_Init_UsingOJPD(JPEG_InitParam *pInitParam)
{
    bUseOJPD = TRUE;
    _USE_OJPD(MApi_JPEG_Init, pInitParam);
    return MApi_JPEG_Init(pInitParam);
}


/********************************************************************/
///This function will\n
///1. set MRC buffer address & size, MWC buffer address, and internal buffer address & size.\n
///2. power on NJPD\n
///3. reset thumbnail parameters\n
///4. set the stream and its size, the number of components, etc.\n
///Call MApi_JPEG_GetErrorCode() after it to determine if the stream was valid or not.\n
///@param -pInitParam \b IN : \n
/// structure {\n
///     MS_PHYADDR \b u32MRCBufAddr : MRC buffer address\n
///     MS_U32 \b u32MRCBufSize : MRC buffer size\n
///     MS_PHYADDR \b u32MWCBufAddr : MWC buffer address(JPEG decompress data (YUV422))\n
///     MS_U32 \b u32MWCBufSize : MWC buffer size\n
///     MS_PHYADDR \b u32InternalBufAddr : Internal buffer address(exif:64K + memory pool)\n
///     MS_U32 \b u32InternalBufSize : Internal buffer size\n
///     MS_U32 \b u32DecByteRead : how many byte read for JPEG decoder initialization\n
///     MS_BOOL \b bEOF : has read to the end of file\n
///     MS_U8 \b u8DecoderType : 3 types: JPEG main, thumbnail, or MJPEG\n
///     MS_BOOL \b bInitMem : initialize memory pool or not\n
///     JPEG_FillHdrFunc \b pFillHdrFunc : the function for fill header information\n
/// };
///@return \b JPEG_Result : JPEG init status
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_Init(JPEG_InitParam *pInitParam)
{
    _USE_OJPD(MApi_JPEG_Init, pInitParam);

#if 0
    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_ALL);
#endif

#if PRINT_NJPD_DECODE_TIME
    //printf("init WDT and timer1....\n");
    MDrv_WDT_Init((WDT_DbgLv)E_WDT_DBGLV_ERR_ONLY);
    MDrv_TIMER_Count(E_TIMER_1, 1);
#endif

#if LOG_DATA_TO_USB
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','l','o', 'g', '0', '0', '0', '.', 'b', 'i', 'n',0};
    fileName2[15]++;
    if(fileName2[15] > '9')
    {
        fileName2[14]++;
        fileName2[15] = '0';
    }
    if(fileName2[14] > '9')
    {
        fileName2[13]++;
        fileName2[14] = '0';
    }

    if((logBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        printf("Can't open /usb/sda1/log.bin\n");
    }
    else
    {
        printf("open /usb/sda1/log.bin success\n");
    }
#endif

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    // jpeg constructor
#ifdef NJPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return  E_JPEG_FAILED;
    }
#endif


    if(_bIsInit[eNJPDNum] == TRUE)
    {
#if SUPPORT_MPO_FORMAT
        if(MApi_JPEG_IsMPOFormat()==FALSE)
        {
            JPEG_terminate(E_JPEG_RE_INIT);
            return E_JPEG_FAILED;
        }
#else
        JPEG_terminate(E_JPEG_RE_INIT);
        return E_JPEG_FAILED;
#endif
    }
    else
    {
        if(s32NJPEGMutexID[eNJPDNum] == -1)
        {
            if(_NJPEG_EX_API_MutexCreate(eNJPDNum) != TRUE)
            {
                JPEG_DEBUG_API_ERR("[%s]Mutex create failed!\n",__FUNCTION__);
                return E_JPEG_FAILED;
            }
        }
        _NJPEG_EX_API_MutexLock(eNJPDNum);
        _bIsInit[eNJPDNum] = TRUE;
    }

    if(!pInitParam)
    {
        JPEG_terminate(E_JPEG_DECODE_ERROR);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }

//    if(pInitParam->u32MRCBufSize < MIN_READBUFFER_SIZE)
    if(pInitParam->u32MRCBufSize < JPEG_DEFAULT_EXIF_SIZE)
    {
        JPEG_terminate(E_JPEG_READBUFFER_TOOSMALL);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }

    // the buffer size must be multiple of 8 bytes
    pInitParam->u32MRCBufSize = pInitParam->u32MRCBufSize & ~0x7;

    _u32ReadBufferAddr_PA[eNJPDNum] = pInitParam->u32MRCBufAddr;
    _u32WriteBufferAddr_PA[eNJPDNum] = pInitParam->u32MWCBufAddr;
    _u32InternalBufferAddr_PA[eNJPDNum] = pInitParam->u32InternalBufAddr;

    MRC_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32MRCBufAddr);// | AEON_NON_CACHE_MASK;
#ifndef MSOS_TYPE_LINUX_KERNEL
    MWC_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32MWCBufAddr);// | AEON_NON_CACHE_MASK;
#endif
    INTERNAL_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32InternalBufAddr);// | AEON_NON_CACHE_MASK;

#if ENABLE_NJPD_DEBUG_MSG
#ifndef MSOS_TYPE_LINUX_KERNEL
    JPEG_memset((void*)(MWC_BUFFER_ADDR), 0x37, pInitParam->u32MWCBufSize);
    JPEG_DEBUG_API_MSG("set output buffer to 0x37............................................\n");
#endif
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE
        || MDrv_NJPD_GetVerificationMode()==E_NJPD23_WRITE_PROTECT
        || (_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API))
    {
#ifndef MSOS_TYPE_LINUX_KERNEL
        JPEG_memset((void*)(MWC_BUFFER_ADDR), 0x37, pInitParam->u32MWCBufSize);
        JPEG_DEBUG_API_MSG("set output buffer to 0x37............................................\n");
#endif
    }
#endif

    if (__bIsMjpeg[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG ("MJPEG Read buffer address CONVERT : 0x%lx - 0x%lx + 0x%lx ",
                _u32ReadBufferAddr_PA[eNJPDNum], MS_VA2PA(__u32RealMjpegBase[eNJPDNum]), MS_VA2PA(__u32TrickyMjpegBase[eNJPDNum]));
#if MJPEG_SW_PARSING_IN_MIU0
        MRC_BUFFER_ADDR -= __u32RealMjpegBase[eNJPDNum];
        MRC_BUFFER_ADDR += __u32TrickyMjpegBase[eNJPDNum];
#endif
        JPEG_DEBUG_API_MSG ("= 0x%lx\n", _u32ReadBufferAddr_PA[eNJPDNum]);
    }
    _pu8In_buf[eNJPDNum] = (MS_U8 *)MRC_BUFFER_ADDR;//pInitParam->u32MRCBufAddr;

    MRC_BUFFER_SIZE = pInitParam->u32MRCBufSize;
    MWC_BUFFER_SIZE = pInitParam->u32MWCBufSize;
    INTERNAL_BUFFER_SIZE = pInitParam->u32InternalBufSize;


    JPEG_DEBUG_API_MSG("MRC_ADDR = 0x%lX, MRC_BUFFER_SIZE = 0x%lX\n", _u32ReadBufferAddr[eNJPDNum], _u32ReadBufferSize[eNJPDNum]);
#ifndef MSOS_TYPE_LINUX_KERNEL
    JPEG_DEBUG_API_MSG("MWC_ADDR = 0x%lX, MWC_BUFFER_SIZE = 0x%lX\n", _u32WriteBufferAddr[eNJPDNum], _u32WriteBufferSize[eNJPDNum]);
#endif
    JPEG_DEBUG_API_MSG("INTERNAL_ADDR = 0x%lX, INTERNAL_BUFFER_SIZE = 0x%lX\n", _u32InternalBufferAddr[eNJPDNum], _u32InternalBufferSize[eNJPDNum]);
    JPEG_DEBUG_API_MSG ("MRC_PA= 0x%lx\n", _u32ReadBufferAddr_PA[eNJPDNum]);
#ifndef MSOS_TYPE_LINUX_KERNEL
    JPEG_DEBUG_API_MSG ("MWC_PA= 0x%lx\n", _u32WriteBufferAddr_PA[eNJPDNum]);
#endif
    JPEG_DEBUG_API_MSG ("INTER_PA= 0x%lx\n", _u32InternalBufferAddr_PA[eNJPDNum]);

#ifdef MSOS_TYPE_LINUX_KERNEL
    _pFillHdrFunc[eNJPDNum] = JPEG_FillHdrFunc_KernlDummy;
#else
    _pFillHdrFunc[eNJPDNum] = pInitParam->pFillHdrFunc;
#endif

#if SW_NJPD_RGB_CMYK
    u8Out_buf = (MS_U8 *) MWC_BUFFER_ADDR;
    u32_Decode_Line = 0;
#endif

    //JPEG_GO;

    // powner on NJPD
    MDrv_NJPD_PowerOn();
    JPEG_DEBUG_API_MSG("MDrv_NJPD_PowerOn OK!!\n");

    _u8DecodeType[eNJPDNum] = pInitParam->u8DecodeType;

    _u32In_buf_left[eNJPDNum] = pInitParam->u32DecByteRead;
    _bEOF_flag[eNJPDNum] = pInitParam->bEOF;

    JPEG_DEBUG_API_MSG("@1,  _pu8In_buf_ofs[eNJPDNum]=0x%lx, _u32In_buf_left[eNJPDNum]=0x%lx, _bEOF_flag[eNJPDNum]=%d========\n",
    (MS_U32)_pu8In_buf_ofs[eNJPDNum], (MS_U32)_u32In_buf_left[eNJPDNum], _bEOF_flag[eNJPDNum]);
        // Try to get more bytes.
        if(!JPEG_force_fill_read_buffer(_u32In_buf_left[eNJPDNum]))
        {
            JPEG_DEBUG_API_MSG("JPEG_force_fill_read_buffer failed!!!\n");
            _u32In_buf_left[eNJPDNum] = pInitParam->u32DecByteRead;
        }
    JPEG_DEBUG_API_MSG("@2,  _pu8In_buf_ofs[eNJPDNum]=0x%lx, _u32In_buf_left[eNJPDNum]=0x%lx, _bEOF_flag[eNJPDNum]=%d========\n",
    (MS_U32)_pu8In_buf_ofs[eNJPDNum], (MS_U32)_u32In_buf_left[eNJPDNum], _bEOF_flag[eNJPDNum]);

#if SUPPORT_MPO_FORMAT
    if(_u32In_buf_MPO_left[eNJPDNum] ==0xFFFFFFFFUL)
    {
        _pu8In_buf_MPO_ofs[eNJPDNum] = (MS_U8*)_pu8In_buf[eNJPDNum];
        _u32In_buf_MPO_left[eNJPDNum] = _u32In_buf_left[eNJPDNum];
        mpo_load_data[eNJPDNum] = TRUE;
        bIsMPOFormat[eNJPDNum] = FALSE;
        u32MPFOffset[eNJPDNum] = 0;
        _u32App2MarkerOffset[eNJPDNum] = 0;

        //Init MPO database
        JPEG_InitMPO();
    }
#endif

    u32DataOffset[eNJPDNum] = 0;
    u32SOFOffset[eNJPDNum] = 0;

    JPEG_init_thumbnail();

    if (_u8DecodeType[eNJPDNum] == E_JPEG_TYPE_MJPEG)
    {
        __bIsMjpeg[eNJPDNum] = TRUE;
#if MJPEG_SW_PARSING_IN_MIU0
        __u32RealMjpegBase[eNJPDNum] = MRC_BUFFER_ADDR;
        __u32TrickyMjpegBase[eNJPDNum] = INTERNAL_BUFFER_ADDR;
#endif

        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_OKAY;
    }

    if(!JPEG_decode_init())
    {
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }

    if (bMHEG5[eNJPDNum])
    {
        if(_Error_code[eNJPDNum] == E_JPEG_UNSUPPORTED_MARKER)
        {
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_FAILED;
        }
    }

    //init mempool for progressive decoding.
#if SW_NJPD_RGB_CMYK
    if (pInitParam->bInitMem && (_bProgressive_flag[eNJPDNum] || MApi_JPEG_Is_RGB_CMYK()))
#else
    if (pInitParam->bInitMem && _bProgressive_flag[eNJPDNum])
#endif
    {
        if(!JPEG_MEMORY_init_mempool((void *)(INTERNAL_BUFFER_ADDR + JPEG_DEFAULT_EXIF_SIZE)
                                , (INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE)))
        {
            JPEG_DEBUG_API_MSG("JPEG_MEMORY_init_mempool FAIL!!\n");
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_FAILED;
        }
        JPEG_DEBUG_API_MSG("JPEG_MEMORY_init_mempool OK!!\n");
    }

    //JPEG_GO;

    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Decode JPEG header
///@return \b JPEG_Result : JPEG header decode status
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_DecodeHdr(void)
{
    _USE_OJPD(MApi_JPEG_DecodeHdr);
#if SW_NJPD_RGB_CMYK
    MS_U8 mcu_width, mcu_height;
    MS_U16 pic_width = _u16Image_x_size[eNJPDNum];
    MS_U16 pic_height = _u16Image_y_size[eNJPDNum];
    MS_U8 Y_VSF = _u8Comp_v_samp[eNJPDNum][0];
    MS_U8 Y_HSF = _u8Comp_h_samp[eNJPDNum][0];
#endif

#if PRINT_NJPD_DECODE_TIME
    u32MeasureDecodeTime[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1);
    u32MeasureDecodeTimeSW[eNJPDNum] = 0;
    u32MeasureDecodeTimeHW[eNJPDNum] = 0;
    u32MeasureDecodeTimeTable[eNJPDNum] = 0;
#endif

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

#ifdef NJPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return ( E_JPEG_FAILED );
    }
#endif

    if(!JPEG_init_frame())
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }

    if ( _bProgressive_flag[eNJPDNum] )
    {
#if SUPPORT_PROGRESSIVE_MODE
        if(!JPEG_init_progressive())
        {
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_FAILED;
        }
#endif
    }
    else
    {
        if(!JPEG_init_sequential())
        {
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_FAILED;
        }
    }

#if SW_NJPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        if ( ( mcu_width = pic_width % ( Y_HSF * 8 ) ) )
        {
            pic_width += ( Y_HSF * 8 - mcu_width );
        }

        if ( ( mcu_height = pic_height% ( Y_VSF * 8 ) ) )
        {
            pic_height += ( Y_VSF * 8 - mcu_height );
        }

        JPEG_DEBUG_API_MSG("Y_HSF = %d and Y_VSF = %d\n",Y_HSF, Y_VSF);

        JPEG_DEBUG_API_MSG("mcu_width = %d and mcu_height = %d\n",mcu_width, mcu_height);

        _u16NonAlignmentImage_x_size[eNJPDNum] = pic_width;
        _u16NonAlignmentImage_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum];
        _u16Image_x_size[eNJPDNum] = pic_width;
        _u16Image_y_size[eNJPDNum] = pic_height;

        JPEG_DEBUG_API_MSG("Width = %d and Height = %d\n",JPEG_MAX_WIDTH[eNJPDNum], JPEG_MAX_HEIGHT[eNJPDNum]);

        //Set the Scale down variable
        if(_u16Image_x_size[eNJPDNum] > JPEG_MAX_WIDTH[eNJPDNum]*4 || _u16Image_y_size[eNJPDNum]>JPEG_MAX_HEIGHT[eNJPDNum]*4)
        {
            _u8ScaleDownFactor[eNJPDNum] = E_NJPD_DOWNSCALE_EIGHTH;
            _u16Image_x_size[eNJPDNum]= (_u16Image_x_size[eNJPDNum]/ 64)*64;
            _u16Image_x_size[eNJPDNum] = _u16Image_x_size[eNJPDNum]/ 8;
            _u16Image_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum]/ 8;
            JPEG_DEBUG_API_MSG("down scale 1/8!!\n");
        }
        else if(_u16Image_x_size[eNJPDNum] > JPEG_MAX_WIDTH[eNJPDNum]*2 || _u16Image_y_size[eNJPDNum]>JPEG_MAX_HEIGHT[eNJPDNum]*2)
        {
            _u8ScaleDownFactor[eNJPDNum] = E_NJPD_DOWNSCALE_FOURTH;
            _u16Image_x_size[eNJPDNum]= (_u16Image_x_size[eNJPDNum]/ 32)*32;
            _u16Image_x_size[eNJPDNum] = _u16Image_x_size[eNJPDNum]/ 4;
            _u16Image_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum]/ 4;
             JPEG_DEBUG_API_MSG("down scale 1/4!!\n");
        }
        else if (_u16Image_x_size[eNJPDNum] > JPEG_MAX_WIDTH[eNJPDNum] || _u16Image_y_size[eNJPDNum]>JPEG_MAX_HEIGHT[eNJPDNum])
        {
            _u8ScaleDownFactor[eNJPDNum] = E_NJPD_DOWNSCALE_HALF;
            _u16Image_x_size[eNJPDNum]= (_u16Image_x_size[eNJPDNum]/ 16)*16;
            _u16Image_x_size[eNJPDNum] = _u16Image_x_size[eNJPDNum]/ 2;
            _u16Image_y_size[eNJPDNum] = _u16Image_y_size[eNJPDNum]/ 2;
             JPEG_DEBUG_API_MSG("down scale 1/2!!\n");
        }
        else
        {
             JPEG_DEBUG_API_MSG("down scale 1/1!!\n");
            _u8ScaleDownFactor[eNJPDNum] = E_NJPD_DOWNSCALE_ORG;

        }

        _u16PaddingMcuNumber = (_u16Image_x_size[eNJPDNum] << _u8ScaleDownFactor[eNJPDNum])/gu8Max_mcu_x_size[eNJPDNum];
        _u32Max_blocks_per_row[eNJPDNum] = _u16PaddingMcuNumber* _u16Max_blocks_per_mcu[eNJPDNum];

        JPEG_DEBUG_API_MSG("down scale width : %d\n",_u16Image_x_size[eNJPDNum]);
        JPEG_DEBUG_API_MSG("down scale height : %d\n",_u16Image_y_size[eNJPDNum]);
        JPEG_DEBUG_API_MSG("down scale MCU : %d\n",_u16PaddingMcuNumber);
        JPEG_DEBUG_API_MSG("down scale MCU : %ld\n",_u32Max_blocks_per_row[eNJPDNum]);
    }
#endif

    if (__bIsMjpeg[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG ("MJPEG Read buffer address RECOVER : 0x%lx - 0x%lx + 0x%lx ",
                _u32ReadBufferAddr_PA[eNJPDNum], MS_VA2PA(__u32TrickyMjpegBase[eNJPDNum]), MS_VA2PA(__u32RealMjpegBase[eNJPDNum]));
#if MJPEG_SW_PARSING_IN_MIU0
        MRC_BUFFER_ADDR -= __u32TrickyMjpegBase[eNJPDNum];
        MRC_BUFFER_ADDR += __u32RealMjpegBase[eNJPDNum];
#endif
        JPEG_DEBUG_API_MSG ("= 0x%lx\n", _u32ReadBufferAddr_PA[eNJPDNum]);
    }

    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return ( E_JPEG_OKAY );
}
//------------------------------------------------------------------------------
/********************************************************************/
///Decode JPEG data (baseline & progressive)
///@return \b JPEG_Result : JPEG decode status
///     - E_JPEG_DONE : For progressive decoding one line done.
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_Decode(void)
{
    _USE_OJPD(MApi_JPEG_Decode);
    JPEG_Result retVal = E_JPEG_FAILED;

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s: _bProgressive_flag[eNJPDNum] = %d\n", __FUNCTION__ , _bProgressive_flag[eNJPDNum]);

    if(_bProgressive_flag[eNJPDNum])
    {
#if SUPPORT_PROGRESSIVE_MODE
        retVal = JPEG_Progressive_Decode();
#endif
    }
    else
    {
#if SW_NJPD_RGB_CMYK
        if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
        || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
        {
            retVal = msAPI_JPEG_Baseline_Decode();
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return retVal;
        }
#endif

        JPEG_StartDecode();
#if PRINT_NJPD_DECODE_TIME
        u32MeasureDecodeTimeSW[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1)-u32MeasureDecodeTime[eNJPDNum];
        u32MeasureDecodeTime[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1);
#endif
        if(_Error_code[eNJPDNum] == E_JPEG_NO_ERROR)
            retVal = E_JPEG_OKAY;
        else
            retVal = E_JPEG_FAILED;
    }
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return retVal;
}

void _MApi_NJPD_PrintMem(MS_U32 u32Addr, MS_U32 u32Size)
{
    MS_U32 u32i;
    printf("===========================================================\n");
    printf("print memory addr=0x%lx, size=0x%lx\n", u32Addr, u32Size);
    printf("===========================================================\n");

#if 0
    for(u32i=0; u32i<u32Size/8+((u32Size%8)? 1:0); u32i++)
    {
        NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*8)), *((MS_U8*)(u32Addr+u32i*8+1)), *((MS_U8*)(u32Addr+u32i*8+2)), *((MS_U8*)(u32Addr+u32i*8+3)),
        *((MS_U8*)(u32Addr+u32i*8+4)), *((MS_U8*)(u32Addr+u32i*8+5)), *((MS_U8*)(u32Addr+u32i*8+6)), *((MS_U8*)(u32Addr+u32i*8+7))
        );
    }
#else
    for(u32i=0; u32i<u32Size/16+((u32Size%16)? 1:0); u32i++)
    {
        printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*16)), *((MS_U8*)(u32Addr+u32i*16+1)), *((MS_U8*)(u32Addr+u32i*16+2)), *((MS_U8*)(u32Addr+u32i*16+3)),
        *((MS_U8*)(u32Addr+u32i*16+4)), *((MS_U8*)(u32Addr+u32i*16+5)), *((MS_U8*)(u32Addr+u32i*16+6)), *((MS_U8*)(u32Addr+u32i*16+7)),
        *((MS_U8*)(u32Addr+u32i*16+8)), *((MS_U8*)(u32Addr+u32i*16+9)), *((MS_U8*)(u32Addr+u32i*16+10)), *((MS_U8*)(u32Addr+u32i*16+11)),
        *((MS_U8*)(u32Addr+u32i*16+12)), *((MS_U8*)(u32Addr+u32i*16+13)), *((MS_U8*)(u32Addr+u32i*16+14)), *((MS_U8*)(u32Addr+u32i*16+15))
        );
    }
#endif
    printf("===========================================================\n");
}

#if LOG_DATA_TO_USB
static void _PrintOutputMem(MS_U32 u32addr, MS_U32 u32Size)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','0','0', '0', '0', 'x','0','0', '0', '0', '_','0', '0', '0', '.', 'y', 'u', 'y', 'v',0};
    fileName2[10]=MApi_JPEG_GetWidth()/1000+'0';
    fileName2[11]=(MApi_JPEG_GetWidth()/100)%10+'0';
    fileName2[12]=(MApi_JPEG_GetWidth()/10)%10+'0';
    fileName2[13]=MApi_JPEG_GetWidth()%10+'0';
    fileName2[15]=MApi_JPEG_GetHeight()/1000+'0';
    fileName2[16]=(MApi_JPEG_GetHeight()/100)%10+'0';
    fileName2[17]=(MApi_JPEG_GetHeight()/10)%10+'0';
    fileName2[18]=MApi_JPEG_GetHeight()%10+'0';

    fileName2[22]++;
    if(fileName2[22] > '9')
    {
        fileName2[21]++;
        fileName2[22] = '0';
    }
    if(fileName2[21] > '9')
    {
        fileName2[20]++;
        fileName2[21] = '0';
    }

    FILE *outBinfp;
    if((outBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        printf("Can't open output yuyv file\n");
    }
    else
    {
        printf("open output yuyv file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, outBinfp);
    fclose(outBinfp);

}
#endif

//------------------------------------------------------------------------------
/********************************************************************/
/// Free all block of NJPD. Complete destroy the decoder object.
/// It could be called at any time.
///@return None
/********************************************************************/
void _MApi_JPEG_Exit(void)
{

#if SUPPORT_OJPD
    if(bUseOJPD)
    {
        bUseOJPD = 0;
        MApi_JPEG_Exit_OJPD();
        return;
    }
#endif

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }

    _NJPEG_EX_API_MutexLock(eNJPDNum);

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
    JPEG_DEBUG_API_MSG("MApi_JPEG_GetCurReadStatus : \nCurRow = %d, CurCol = %d ",
        MDrv_NJPD_GetCurRow(),
        MDrv_NJPD_GetCurCol());
    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());

    MDrv_NJPD_PowerOff();
    JPEG_free_all_blocks();

#if SW_NJPD_RGB_CMYK
    bEnableCMYK = TRUE;
    bEnableRGB = TRUE;
#endif

#if SUPPORT_MPO_FORMAT
    _u32In_buf_MPO_left[eNJPDNum] = 0xFFFFFFFFUL;         // reset _u32In_buf_MPO_left[eNJPDNum] to U32_MAX
    bIsMPOFormat[eNJPDNum] = FALSE;
#endif

#if LOG_DATA_TO_USB
fclose(logBinfp);
logBinfp = NULL;
#endif

    _bIsInit[eNJPDNum] = FALSE;
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
#if 0
    _NJPEG_EX_API_MutexDelete(eNJPDNum);
#endif
    JPEG_DEBUG_API_MSG("%s: done!!\n", __FUNCTION__);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get JPEG decoder error code
///@return JPEG decoder error code
/********************************************************************/
JPEG_ErrCode _MApi_JPEG_GetErrorCode(void)
{
    _USE_OJPD(MApi_JPEG_GetErrorCode);

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_NOT_INIT;
    }
    JPEG_DEBUG_API_MSG("%s: ErrCode = %d\n", __FUNCTION__ , _Error_code[eNJPDNum]);
    return _Error_code[eNJPDNum];
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get NJPD EVENT FLAG
///@return \b JPEG_Event : JPEG event flag
/********************************************************************/
JPEG_Event _MApi_JPEG_GetJPDEventFlag(void)
{
    _USE_OJPD(MApi_JPEG_GetJPDEventFlag);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return (E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK);
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);

#if SW_NJPD_RGB_CMYK
    // It's pure software decode
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_EVENT_DEC_DONE;
    }
#endif

    {
        NJPD_Event reg_val = (NJPD_Event)MDrv_NJPD_GetEventFlag();
        JPEG_Event reg_val_org = E_JPEG_EVENT_DEC_NONE;
        if(reg_val!=0)
        {
            JPEG_DEBUG_API_MSG("%s: reg_val = 0x%04X\n", __FUNCTION__ , reg_val);
        }
        if(E_NJPD_EVENT_DEC_DONE & reg_val)
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_MIU_LAST_Z_PATCH))
            {
                NJPD_Status *pDrvStatus = MDrv_NJPD_GetStatus();
                if(pDrvStatus->bIsrEnable == 0)
                {
                    // delay task is not allowed in ISR
                    MsOS_DelayTask(1);
                }
            }

#if PRINT_NJPD_DECODE_TIME
            u32MeasureDecodeTimeHW[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1)-u32MeasureDecodeTime[eNJPDNum];
            JPEG_DEBUG_API_MSG("SW decode header:[%ld]us, HW decode: [%ld]us, table:[%ld]us\n", u32MeasureDecodeTimeSW[eNJPDNum],
                u32MeasureDecodeTimeHW[eNJPDNum], u32MeasureDecodeTimeTable[eNJPDNum]);
#endif
            if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API)
            {
#ifndef MSOS_TYPE_LINUX_KERNEL
                verJPD_CRC32_Init();
                verJPD_CRC32_Update((MS_U8 *)MWC_BUFFER_ADDR,
                    (MS_U32)MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2);
                JPEG_DEBUG_API_MSG("[%dx%d], u32CRCResult=0x%lx\n",MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), verJPD_CRC32_GetResult());

                _MApi_NJPD_Debug();
                _MApi_NJPD_PrintMem(MWC_BUFFER_ADDR, 0x100);
#endif
            }

#if LOG_DATA_TO_USB
            if(MApi_JPEG_GetWidth() <=JPEG_MAX_WIDTH[eNJPDNum]
                && MApi_JPEG_GetHeight() <=JPEG_MAX_HEIGHT[eNJPDNum])
            {
                MS_U32 u32Size;
                u32Size = MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2;
                JPEG_DEBUG_API_MSG("[%dx%d], MWC_BUFFER_ADDR=0x%lx, size=0x%lx", MApi_JPEG_GetWidth(),
                    MApi_JPEG_GetHeight(), MS_VA2PA(MWC_BUFFER_ADDR),u32Size);
                _PrintOutputMem(MWC_BUFFER_ADDR, u32Size);
            }
#endif
        }
        // For the compatibility, the NJPD's event should be converted
        if(reg_val & E_NJPD_EVENT_DEC_DONE)
        {
            reg_val_org |=E_JPEG_EVENT_DEC_DONE;
#if 0
            printf("=======================================================\n");
            printf("Decode done!!! dump stuck message!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("=======================================================\n");
            MApi_NJPD_Debug();
            while(1);
#endif
        }
        if(reg_val & NJPD_EVENT_ERROR)
        {
            JPEG_terminate(E_JPEG_DECODE_ERROR);
            reg_val_org &= ~E_JPEG_EVENT_DEC_DONE;
            MDrv_NJPD_Debug();
        }
        if(reg_val & E_NJPD_EVENT_MINICODE_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]mini-code error!!\n", __FUNCTION__);
            reg_val_org |=E_JPEG_EVENT_DEC_ECS_ERROR;
        }
        if(reg_val & E_NJPD_EVENT_INV_SCAN_ERR )
        {
            JPEG_DEBUG_API_ERR("[%s]inverse scan error!!\n", __FUNCTION__);
            reg_val_org |=E_JPEG_EVENT_DEC_IS_ERROR;
        }
        if(reg_val & E_NJPD_EVENT_RES_MARKER_ERR )
        {
            JPEG_DEBUG_API_ERR("[%s]restart marker error!!\n", __FUNCTION__);
            reg_val_org |=E_JPEG_EVENT_DEC_RST_ERROR;
        }
        if(reg_val & E_NJPD_EVENT_MRC0_EMPTY )
        {
            JPEG_DEBUG_API_MSG("%s : Get buffer 0 empty done with reg_val = 0x%04X\n", __FUNCTION__ , reg_val);
            reg_val_org |=E_JPEG_EVENT_DEC_MRBL_DONE;
        }
        if(reg_val & E_NJPD_EVENT_MRC1_EMPTY )
        {
            JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , reg_val);
            reg_val_org |=E_JPEG_EVENT_DEC_MRBH_DONE;
        }
        if(reg_val & E_NJPD_EVENT_WRITE_PROTECT )
        {
            JPEG_DEBUG_API_MSG("[%s]Write protect!!\n", __FUNCTION__);
            reg_val_org |=E_JPEG_EVENT_DEC_MWB_FULL;
        }
        if(reg_val & E_NJPD_EVENT_IBUF_LOAD_DONE)
        {
        }

    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return reg_val_org;
    }
}
//------------------------------------------------------------------------------
/********************************************************************/
///Set NJPD EVENT FLAG
///@param -eEvtVal \b IN : JPEG_Event
///@return None
/********************************************************************/
void _MApi_JPEG_SetJPDEventFlag(JPEG_Event eEvtVal)
{
    _USE_OJPD(MApi_JPEG_SetJPDEventFlag, eEvtVal);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }

    _NJPEG_EX_API_MutexLock(eNJPDNum);
#if SW_NJPD_RGB_CMYK
    // It's pure software decode
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return;
    }
#endif

    NJPD_Event reg_val =E_NJPD_EVENT_DEC_NONE;
    if(eEvtVal & E_JPEG_EVENT_DEC_DONE)
    {
        reg_val |=E_NJPD_EVENT_DEC_DONE;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_ECS_ERROR)
    {
        reg_val |=E_NJPD_EVENT_MINICODE_ERR;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_IS_ERROR)
    {
        reg_val |=E_NJPD_EVENT_INV_SCAN_ERR;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_RST_ERROR)
    {
        reg_val |=E_NJPD_EVENT_RES_MARKER_ERR;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_MRBL_DONE)
    {
        reg_val |=E_NJPD_EVENT_MRC0_EMPTY;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_MRBH_DONE)
    {
        reg_val |=E_NJPD_EVENT_MRC1_EMPTY;
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_MRB_DONE)
    {
    }
    if(eEvtVal & E_JPEG_EVENT_DEC_MWB_FULL)
    {
        reg_val |=E_NJPD_EVENT_WRITE_PROTECT;
    }
    MDrv_NJPD_SetEventFlag((MS_U16)reg_val);
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Reset NJPD -- Reset must be called before trigger NJPD\n
///This function always issue pause command and wait for both MRC & MWC\n
///becoming inactive, then reset NJPD.\n
///\b NOTE: The pause function can be only used by Pluto/T1/T2... not include ERIS.
///@return None
/********************************************************************/
void _MApi_JPEG_Rst(void)
{
    _USE_OJPD(MApi_JPEG_Rst);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
    JPEG_DEBUG_API_MSG("MApi_JPEG_GetCurReadStatus : \nCurRow = %d, CurCol = %d ",
        MDrv_NJPD_GetCurRow(),
        MDrv_NJPD_GetCurCol());
    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_NJPD_GetCurMRCAddr());

    MDrv_NJPD_Rst();
    JPEG_DEBUG_API_MSG("%s:done!!\n", __FUNCTION__);
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Power On JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_PowerOn(void)
{
    _USE_OJPD(MApi_JPEG_PowerOn);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
    MDrv_NJPD_PowerOn();
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Power Off JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_PowerOff(void)
{
    _USE_OJPD(MApi_JPEG_PowerOff);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
    MDrv_NJPD_PowerOff();
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get current vertical line index written to memory
///@return Current vertical line index written to memory
/********************************************************************/
MS_U16 _MApi_JPEG_GetCurVidx(void) //new
{
    _USE_OJPD(MApi_JPEG_GetCurVidx);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#if 1
    // NJPD do not support the register vidx
    // use other method to calculate this value
    if(MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE)
    {
        return _u16Image_y_size[eNJPDNum];
    }
    else
    {

        MS_U32 u32MCURowCnt;
        u32MCURowCnt = (MDrv_NJPD_GetWritePtrAddr()-MDrv_NJPD_GetMWCStartAddr())/2/_u16Image_x_size[eNJPDNum]/16;

        if(u32MCURowCnt > 2)
        {
            return (u32MCURowCnt-2)*16;
        }
        else
        {
            return 0;
        }
    }
#else
    return MDrv_NJPD_GetCurVidx();
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
///Check current JPEG image is progressive or not
///@return TRUE / FALSE
/// - TRUE : progressive
/// - FALSE : baseline
/********************************************************************/
MS_BOOL _MApi_JPEG_IsProgressive(void)
{
    _USE_OJPD(MApi_JPEG_IsProgressive);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return FALSE;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _bProgressive_flag[eNJPDNum]);
    return (_bProgressive_flag[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Check current JPEG image has thumbnail or not
///@return TRUE / FALSE
/// - TRUE : thumbnail found
/// - FALSE : no thumbnail
/********************************************************************/
MS_BOOL _MApi_JPEG_ThumbnailFound(void)
{
    _USE_OJPD(MApi_JPEG_ThumbnailFound);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return FALSE;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _bThumbnailFound[eNJPDNum]);
    return (_bThumbnailFound[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///The width may be the thumbnail or original image size, it based on decoding mode
///@return width
/********************************************************************/
MS_U16 _MApi_JPEG_GetWidth(void)
{
    _USE_OJPD(MApi_JPEG_GetWidth);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16Image_x_size[eNJPDNum]);
    return (_u16Image_x_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///The height may be the thumbnail or original image size, it based on decoding mode
///@return height
/********************************************************************/
MS_U16 _MApi_JPEG_GetHeight(void)
{
    _USE_OJPD(MApi_JPEG_GetHeight);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16Image_y_size[eNJPDNum]);
    return (_u16Image_y_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original width of this JPEG file after alignment
///@return original width
/********************************************************************/
MS_U16 _MApi_JPEG_GetOriginalWidth(void)
{
    _USE_OJPD(MApi_JPEG_GetOriginalWidth);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16OriginalImage_x_size[eNJPDNum]);
    return (_u16OriginalImage_x_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original height of this JPEG file after alignment
///@return original height
/********************************************************************/
MS_U16 _MApi_JPEG_GetOriginalHeight(void)
{
    _USE_OJPD(MApi_JPEG_GetOriginalHeight);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16OriginalImage_y_size[eNJPDNum]);
    return (_u16OriginalImage_y_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original width before alignment
///@return non-alignment width
/********************************************************************/
MS_U16 _MApi_JPEG_GetNonAlignmentWidth(void)
{
    _USE_OJPD(MApi_JPEG_GetNonAlignmentWidth);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16NonAlignmentImage_x_size[eNJPDNum]);
    return (_u16NonAlignmentImage_x_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original height before alignment
///@return non-alignment height
/********************************************************************/
MS_U16 _MApi_JPEG_GetNonAlignmentHeight(void)
{
    _USE_OJPD(MApi_JPEG_GetNonAlignmentHeight);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16NonAlignmentImage_y_size[eNJPDNum]);
    return (_u16NonAlignmentImage_y_size[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed pitch of JPEG image
///@return displayed pitch
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedPitch(void)
{
    _USE_OJPD(MApi_JPEG_GetAlignedPitch);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImagePitch[eNJPDNum]);
    return (_u16AlignedImagePitch[eNJPDNum]);
}

MS_U16 _MApi_JPEG_GetAlignedPitch_H(void)
{
    _USE_OJPD(MApi_JPEG_GetAlignedPitch_H);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImagePitch_H[eNJPDNum]);
    return (_u16AlignedImagePitch_H[eNJPDNum]);
}


//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed width of JPEG image
///@return displayed width
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedWidth(void)
{
    _USE_OJPD(MApi_JPEG_GetAlignedWidth);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImageWidth[eNJPDNum]);
    return (_u16AlignedImageWidth[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed height of JPEG image
///@return displayed height
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedHeight(void)
{
    _USE_OJPD(MApi_JPEG_GetAlignedHeight);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImageHeight[eNJPDNum]);
    return (_u16AlignedImageHeight[eNJPDNum]);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get scale down factor\n
///Depending on the real picture width & height, it will automatically set scale down\n
///factor to meet maximum JPEG width & height allowed.
///@return scale down factor
/********************************************************************/
MS_U8 _MApi_JPEG_GetScaleDownFactor(void)
{
    _USE_OJPD(MApi_JPEG_GetScaleDownFactor);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }

#if SW_NJPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u8ScaleDownFactor[eNJPDNum]*2);
        return (_u8ScaleDownFactor[eNJPDNum]*2);
    }
#endif
    {
        JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u8ScaleDownFactor[eNJPDNum]);
        return (_u8ScaleDownFactor[eNJPDNum]);
    }
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution
///@param -u16Width \b IN : indicate max decoding width
///@param -u16Height \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
    _USE_OJPD(MApi_JPEG_SetMaxDecodeResolution, u16Width, u16Height);
    JPEG_DEBUG_API_MSG("%s: Width = %d, Height = %d\n", __FUNCTION__, u16Width, u16Height);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_MAX_WIDTH[eNJPDNum] = u16Width;
    JPEG_MAX_HEIGHT[eNJPDNum] = u16Height;
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution of Progressive JPEG
///@param -u16ProWidth \b IN : indicate max decoding width
///@param -u16ProHeight \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
    _USE_OJPD(MApi_JPEG_SetProMaxDecodeResolution, u16ProWidth, u16ProHeight);
    JPEG_DEBUG_API_MSG("%s: ProWidth = %d, ProHeight = %d\n", __FUNCTION__, u16ProWidth, u16ProHeight);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_PRO_MAX_WIDTH[eNJPDNum] = u16ProWidth;
    JPEG_PRO_MAX_HEIGHT[eNJPDNum] = u16ProHeight;
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Valid the JPEG read buffer flag
///@param -u8MRBuffType \b IN : the type of JPEG read buffer
///@return None
/********************************************************************/
void _MApi_JPEG_SetMRBufferValid(JPEG_BuffLoadType u8MRBuffType)
{
    _USE_OJPD(MApi_JPEG_SetMRBufferValid, u8MRBuffType);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s: MRBuffType = %d\n", __FUNCTION__ , u8MRBuffType);
    MsOS_FlushMemory();
    MS_U32 u32Time=MsOS_GetSystemTime();

#if SUPPORT_HIGH_LOW_REVERSE
    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
    {
        switch(u8MRBuffType)
        {
            case E_JPEG_BUFFER_HIGH:
                JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_HIGH\n");
                if(bHighLowReverse[eNJPDNum])
                {
                    if(_bEOF_flag[eNJPDNum])
                    {
                        MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                        MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    }
                }
                else
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);

                    while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC0_EMPTY)==0)
                    {
                        if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                        {
                            break;
                        }
                        if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                        {
                            JPEG_DEBUG_API_ERR("Timeout @1!!!\n");
                            MDrv_NJPD_Debug();
                            break;
                        }
                    }
                    JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());

                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
                break;
            case E_JPEG_BUFFER_LOW:
                JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_LOW ==> do nothing\n");
                if(bHighLowReverse[eNJPDNum])
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);

                    while(MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC1_EMPTY)
                    {
                        if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                        {
                            break;
                        }
                        if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                        {
                            JPEG_DEBUG_API_ERR("Timeout @2!!!\n");
                            MDrv_NJPD_Debug();
                            break;
                        }
                    }
                    JPEG_DEBUG_API_MSG("%s : Get buffer 0 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());

                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
                else
                {
                    if(_bEOF_flag[eNJPDNum])
                    {
                        MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                        MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    }
                }
                break;
            default:
                break;
        }
    }
    else
    {
        switch(u8MRBuffType)
        {
            case E_JPEG_BUFFER_HIGH:
                if(bHighLowReverse[eNJPDNum])
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
                else
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
                break;
            case E_JPEG_BUFFER_LOW:
                if(bHighLowReverse[eNJPDNum])
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                }
                else
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
                break;
            default:
                break;
        }
    }
#else
    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
    {
        switch(u8MRBuffType)
        {
            case E_JPEG_BUFFER_HIGH:
                JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_HIGH\n");
                MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);

                while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC0_EMPTY)==0)
                {
                    if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                    {
                        break;
                    }
                    if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("Timeout @3!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }
                JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
                MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                break;
            case E_JPEG_BUFFER_LOW:
                JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_LOW ==> do nothing\n");
                if(_bEOF_flag[eNJPDNum])
                {
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                }
                break;
            default:
                break;
        }
    }
    else
    {
        if(bDifferentHL[eNJPDNum])
        {
            switch(u8MRBuffType)
            {
                case E_JPEG_BUFFER_HIGH:
                    JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_HIGH\n");
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);

                    while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC0_EMPTY)==0)
                    {
                        if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                        {
                            break;
                        }
                        if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                        {
                            JPEG_DEBUG_API_ERR("Timeout @4!!!\n");
                            MDrv_NJPD_Debug();
                            break;
                        }
                    }
                    JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                    break;
                case E_JPEG_BUFFER_LOW:
                    JPEG_DEBUG_API_MSG("parameter: E_JPEG_BUFFER_LOW ==> do nothing\n");
                    if(_bEOF_flag[eNJPDNum])
                    {
                        MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                        MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch(u8MRBuffType)
            {
                case E_JPEG_BUFFER_HIGH:
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC1_VALID);
                    break;
                case E_JPEG_BUFFER_LOW:
                    MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
                    MDrv_NJPD_SetMRC_Valid(NJPD_MRC0_VALID);
                    break;
                default:
                    break;
            }
        }
    }

#endif
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG EOF flag and the total byte-read of JPEG file
///@param -u32DataRead \b IN : the amount of byte of file reading
///@param -bEOFflag \b IN : the EOF flag of file reading
///@return None
/********************************************************************/
void _MApi_JPEG_UpdateReadInfo(MS_U32 u32DataRead, MS_BOOL bEOFflag)
{
    _USE_OJPD(MApi_JPEG_UpdateReadInfo, u32DataRead, bEOFflag);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
// Note: this function is used in callback function, we CANNOT add mutex lock on it!!!
//    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s: DataRead = %ld, EOFflag = %d\n", __FUNCTION__ , u32DataRead, bEOFflag);
////    _Total_Decoded_Size += data_read;
    _u32Total_bytes_read[eNJPDNum] += u32DataRead;
    _bEOF_flag[eNJPDNum] = bEOFflag;

//    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Process JPEG read buffer flag when EOF.
///@param -u8MRBuffType \b IN : The type of JPEG read buffer
///@return None
/********************************************************************/
void _MApi_JPEG_ProcessEOF(JPEG_BuffLoadType u8MRBuffType)
{
    _USE_OJPD(MApi_JPEG_ProcessEOF, u8MRBuffType);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_MSG("%s: end of file!! MRBuffType = %d\n", __FUNCTION__ , u8MRBuffType);
////    MDrv_UART_PutChar('t'); // DO NOT MARK THIS, THIS FUNCTION MUST BE CALLED. OR THE NJPD DECODE WILL FAIL for LARGE FILES
    if(u8MRBuffType == E_JPEG_BUFFER_HIGH)
    {
        // clear MRC high portion read complete event
#if SUPPORT_HIGH_LOW_REVERSE
        if(bHighLowReverse[eNJPDNum])
        {
            MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
        }
        else
#endif
        {
            MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
        }
    }
    else
    {
        // clear MRC low portion read complete event
#if SUPPORT_HIGH_LOW_REVERSE
        if(bHighLowReverse[eNJPDNum])
        {
            MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC1_EMPTY);
        }
        else
#endif
        {
            MDrv_NJPD_SetEventFlag(E_NJPD_EVENT_MRC0_EMPTY);
        }
    }
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG Error Code if something error on upper layer.
///@param -ErrCode \b IN : The error type of JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_SetErrCode(JPEG_ErrCode ErrCode)
{
    _USE_OJPD(MApi_JPEG_SetErrCode, ErrCode);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    JPEG_DEBUG_API_ERR("%s: ErrCode = %d\n", __FUNCTION__ , ErrCode);
    _Error_code[eNJPDNum] = ErrCode;
////    JPEG_free_all_blocks();
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG debug level.
///@param -u8DbgLevel \b IN : The JPEG debug level, refer to JPEG_DbgLevel.
///@return None
/********************************************************************/
void _MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _USE_OJPD(MApi_JPEG_SetDbgLevel, u8DbgLevel);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    _u8JPEG_ApiDbgLevel = u8DbgLevel;
    JPEG_DEBUG_API_MSG("%s: DbgLevel = 0x%04X\n", __FUNCTION__, u8DbgLevel);
    MDrv_NJPD_SetDbgLevel(u8DbgLevel);
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG debug level.
///@return JPEG debug level, refer to JPEG_DbgLevel.
/********************************************************************/
MS_U8 _MApi_JPEG_GetDbgLevel(void)
{
    _USE_OJPD(MApi_JPEG_GetDbgLevel);
    JPEG_DEBUG_API_MSG("%s: DbgLevel = 0x%04X\n", __FUNCTION__, _u8JPEG_ApiDbgLevel);
    return _u8JPEG_ApiDbgLevel;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set MHEG5 Flag.
///@param -bEnable \b IN :
///@return None
/********************************************************************/
void _MApi_JPEG_SetMHEG5(MS_BOOL bEnable)
{
    _USE_OJPD(MApi_JPEG_SetMHEG5, bEnable);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    bMHEG5[eNJPDNum] = bEnable;
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG information
///@param -pJPEG_Info \b OUT : JPEG information\n
/// structure {\n
///    MS_U8* \b pu8DrvVer : JPEG DRV version\n
///    MS_U8* \b pu8HalVer : JPEG HAL version\n
///    MS_U8* \b pu8FwVer : JPEG FW version\n
///    JPEG_DrvCap \b stDrvCap : JPEG driver capability\n
///           };
///@return None
/********************************************************************/
void _MApi_JPEG_GetInfo(JPEG_Info *pJPEG_Info)
{
    _USE_OJPD(MApi_JPEG_GetInfo, pJPEG_Info);
    NJPD_Info DrvInfo;
    const MSIF_Version *DrvVer;
    MDrv_NJPD_GetInfo(&DrvInfo);
    MDrv_NJPD_GetLibVer(&DrvVer);
    pJPEG_Info->pu8DrvVer = DrvVer;
    pJPEG_Info->pu8HalVer = DrvInfo.pu8HalVer;
    pJPEG_Info->pu8FwVer = DrvInfo.pu8FwVer;
    pJPEG_Info->stDrvCap.bBaseline = DrvInfo.stCap.bBaseline;
    pJPEG_Info->stDrvCap.bProgressive = DrvInfo.stCap.bProgressive;
    pJPEG_Info->stDrvCap.bMJPEG = DrvInfo.stCap.bMJPEG;
    pJPEG_Info->stDrvCap.u16MaxWidth = JPEG_MAX_WIDTH[eNJPDNum];
    pJPEG_Info->stDrvCap.u16MaxHeight = JPEG_MAX_HEIGHT[eNJPDNum];
    pJPEG_Info->stDrvCap.u16MaxProWidth = JPEG_PRO_MAX_WIDTH[eNJPDNum];
    pJPEG_Info->stDrvCap.u16MaxProHeight = JPEG_PRO_MAX_HEIGHT[eNJPDNum];

    JPEG_DEBUG_API_MSG("NJPD %04x, %04x, %04x, %s\n",
        DrvVer->DDI.customer, DrvVer->DDI.model, DrvVer->DDI.chip, &(DrvVer->DDI.cpu));
    JPEG_DEBUG_API_MSG("pu8HalVer = %s, pu8FwVer = %s\n", pJPEG_Info->pu8HalVer, pJPEG_Info->pu8FwVer);
    JPEG_DEBUG_API_MSG("%s: DrvCap_BL = %d, DrvCap_PRO = %d, DrvCap_MJPEG = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.bBaseline, pJPEG_Info->stDrvCap.bProgressive, pJPEG_Info->stDrvCap.bMJPEG);
    JPEG_DEBUG_API_MSG("%s: MaxWidth = %d, MaxHeight = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.u16MaxWidth, pJPEG_Info->stDrvCap.u16MaxHeight);
    JPEG_DEBUG_API_MSG("%s: MaxProWidth = %d, MaxProHeight = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.u16MaxProWidth, pJPEG_Info->stDrvCap.u16MaxProHeight);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG status.
///@param -pJPEG_Status \b OUT : JPEG status\n
/// structure {\n
///    MS_U32 \b u32CurMRCAddr : NJPD current decode address\n
///    MS_U16 \b u16CurVidx : NJPD current decode vertical index\n
///    MS_U16 \b u16CurRow : NJPD current decode row\n
///    MS_U16 \b u16CurCol : NJPD current decode column\n
///    MS_BOOL \b bDrvBusy : JPEG DRV busy status\n
///    MS_BOOL \b bIsrEnable : NJPD ISR status\n
///           };
///@return None
/********************************************************************/
void _MApi_JPEG_GetStatus(JPEG_Status *pJPEG_Status)
{
    _USE_OJPD(MApi_JPEG_GetStatus, pJPEG_Status);
    NJPD_Status *pDrvStatus = MDrv_NJPD_GetStatus();
    pJPEG_Status->u32CurMRCAddr = pDrvStatus->u32CurMRCAddr;
    pJPEG_Status->u16CurVidx = pDrvStatus->u16CurVidx;
    pJPEG_Status->u16CurRow = pDrvStatus->u16CurRow;
    pJPEG_Status->u16CurCol = pDrvStatus->u16CurCol;
    pJPEG_Status->bDrvBusy = pDrvStatus->bIsBusy;
    pJPEG_Status->bIsrEnable = pDrvStatus->bIsrEnable;
    JPEG_DEBUG_API_MSG("%s: Drv_busy = %d, Isr_status = %d\n", __FUNCTION__ , pJPEG_Status->bDrvBusy, pJPEG_Status->bIsrEnable);
    JPEG_DEBUG_API_MSG("%s: CurVidx = %d, CurRow = %d, CurCol = %d\n", __FUNCTION__ ,
                        pJPEG_Status->u16CurVidx, pJPEG_Status->u16CurRow, pJPEG_Status->u16CurCol);
    JPEG_DEBUG_API_MSG("%s: CurMRCAddr = 0x%lx\n", __FUNCTION__ , pJPEG_Status->u32CurMRCAddr);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG Version.
///@param -ppVersion \b OUT : JPEG Version
///@return \b JPEG_Result
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_GetLibVer(const MSIF_Version **ppVersion)
{
    _USE_OJPD(MApi_JPEG_GetLibVer, ppVersion);
    if (!ppVersion)
    {
        return E_JPEG_FAILED;
    }
    *ppVersion = &_api_jpeg_version;
    JPEG_DEBUG_API_MSG("%s: JPEG %04x, %04x, %04x, %s\n", __FUNCTION__ ,
        _api_jpeg_version.DDI.customer, _api_jpeg_version.DDI.model, _api_jpeg_version.DDI.chip, &(_api_jpeg_version.DDI.cpu));
    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// For H/W bug, some cases can not exit after decode done, Check
/// Vidx to exit.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : Vidx checking is successful.
/// - E_JPEG_FAILED : Vidx checking is failed.
/********************************************************************/
JPEG_Result _MApi_JPEG_HdlVidxChk(void)
{
    // This function is only reserved for upper layer to call without change APIs.
    _USE_OJPD(MApi_JPEG_HdlVidxChk);

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get Preload buffer information.
///@param -pBuffLoadType \b OUT : JPEG preload buffer type
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/// - E_JPEG_RETRY : we need to re-get preload buffer type
/********************************************************************/
JPEG_Result _MApi_JPEG_GetBuffLoadType(JPEG_BuffLoadType *pBuffLoadType)
{
    _USE_OJPD(MApi_JPEG_GetBuffLoadType, pBuffLoadType);
    MS_U16 reg_val = MDrv_NJPD_GetEventFlag();
    MS_U32 u32Time=MsOS_GetSystemTime();

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);
#if SUPPORT_HIGH_LOW_REVERSE
    if(bHighLowReverse[eNJPDNum])
    {
        if((E_NJPD_EVENT_MRC0_EMPTY & reg_val)
        && (E_JPEG_BUFFER_HIGH != u8PreLHFlag[eNJPDNum]))
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC1_EMPTY)==0)
                {
                    if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                    {
                        break;
                    }
                    if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("Timeout 1!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }
                JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
            }
            JPEG_DEBUG_API_MSG("%s : @@MRBuffer Load HIGH!!!!\n", __FUNCTION__);
            u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_HIGH;
            u32MRCheckCount[eNJPDNum] = 0;
            *pBuffLoadType = u8PreLHFlag[eNJPDNum];
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_OKAY;
        }
        else if((E_NJPD_EVENT_MRC1_EMPTY & reg_val)
        && (E_JPEG_BUFFER_LOW != u8PreLHFlag[eNJPDNum]))
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC0_EMPTY)==0)
                {
                    if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                    {
                        break;
                    }
                    if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("Timeout 2!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }
                JPEG_DEBUG_API_MSG("%s : Get buffer 0 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
            }
            JPEG_DEBUG_API_MSG("%s : @@MRBuffer Load LOW!!!!\n", __FUNCTION__);
            u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_LOW;
            u32MRCheckCount[eNJPDNum] = 0;
            *pBuffLoadType = u8PreLHFlag[eNJPDNum];
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_OKAY;
        }
        else
        {
            JPEG_DEBUG_API_MSG("%s : MRBuffer Load NONE!!!!\n", __FUNCTION__);
            *pBuffLoadType = E_JPEG_BUFFER_NONE;
            u32MRCheckCount[eNJPDNum]++;
            if(u32MRCheckCount[eNJPDNum] >= MRC_CHECK_COUNT)
            {
                JPEG_DEBUG_API_ERR("Decoding time out!!\n");
                u32MRCheckCount[eNJPDNum] = 0;
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_FAILED;
            }
        }
    }
    else
#endif
    {
        if((E_NJPD_EVENT_MRC0_EMPTY & reg_val)
        && (E_JPEG_BUFFER_LOW != u8PreLHFlag[eNJPDNum]))
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC1_EMPTY)==0)
                {
                    if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                    {
                        break;
                    }
                    if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("Timeout 3!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }
                JPEG_DEBUG_API_MSG("%s : Get buffer 1 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
            }
            JPEG_DEBUG_API_MSG("%s : MRBuffer Load LOW!!!!\n", __FUNCTION__);
            u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_LOW;
            u32MRCheckCount[eNJPDNum] = 0;
            *pBuffLoadType = u8PreLHFlag[eNJPDNum];
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_OKAY;
        }
        else if((E_NJPD_EVENT_MRC1_EMPTY & reg_val)
        && (E_JPEG_BUFFER_HIGH != u8PreLHFlag[eNJPDNum]))
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                while((MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_MRC0_EMPTY)==0)
                {
                    if(_bEOF_flag[eNJPDNum] || (MDrv_NJPD_GetEventFlag() & E_NJPD_EVENT_DEC_DONE))
                    {
                        break;
                    }
                    if((MsOS_GetSystemTime() - u32Time) >= MRC_DECODE_TIMEOUT)
                    {
                        JPEG_DEBUG_API_ERR("Timeout 4!!!\n");
                        MDrv_NJPD_Debug();
                        break;
                    }
                }
                JPEG_DEBUG_API_MSG("%s : Get buffer 0 empty done with reg_val = 0x%04X\n", __FUNCTION__ , MDrv_NJPD_GetEventFlag());
            }
            JPEG_DEBUG_API_MSG("%s : MRBuffer Load HIGH!!!!\n", __FUNCTION__);
            u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_HIGH;
            u32MRCheckCount[eNJPDNum] = 0;
            *pBuffLoadType = u8PreLHFlag[eNJPDNum];
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_OKAY;
        }
        else
        {
            JPEG_DEBUG_API_MSG("%s : MRBuffer Load NONE!!!!\n", __FUNCTION__);
            *pBuffLoadType = E_JPEG_BUFFER_NONE;
            u32MRCheckCount[eNJPDNum]++;
            if(u32MRCheckCount[eNJPDNum] >= MRC_CHECK_COUNT)
            {
                JPEG_DEBUG_API_ERR("Decoding time out!!\n");
                u32MRCheckCount[eNJPDNum] = 0;
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_FAILED;
            }
        }
    }
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return E_JPEG_RETRY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Enable NJPD ISR and register callback function.
///@param -IsrCb \b IN : NJPD ISR callback function
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_EnableISR(JPEG_IsrFuncCb IsrCb)
{
    _USE_OJPD(MApi_JPEG_EnableISR, IsrCb);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    if(TRUE == MDrv_NJPD_EnableISR((NJPD_IsrFuncCb)IsrCb))
    {
        JPEG_DEBUG_API_MSG("%s : Success!!!!\n", __FUNCTION__);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_OKAY;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s : Fail!!!!\n", __FUNCTION__);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Disable NJPD ISR and unregister callback function.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_DisableISR(void)
{
    _USE_OJPD(MApi_JPEG_DisableISR);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    if(TRUE == MDrv_NJPD_DisableISR())
    {
        JPEG_DEBUG_API_MSG("%s : Success!!!!\n", __FUNCTION__);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_OKAY;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s : Fail!!!!\n", __FUNCTION__);
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_FAILED;
    }
}
//------------------------------------------------------------------------------
/********************************************************************/
/// JPEG wait done function, call it to check JPEG decoding status.
///@return \b JPEG_DecodeStatus
/// - E_JPEG_DEC_DONE : JPEG decode done.
/// - E_JPEG_DEC_FAILED : JPEG decode failed.
/// - E_JPEG_DEC_MRBL_DONE : MRB low done, need to feed data.
/// - E_JPEG_DEC_MRBH_DONE : MRB high done, need to feed data.
/// - E_JPEG_DEC_DECODING : JPEG decoding
/********************************************************************/
JPEG_DecodeStatus _MApi_JPEG_WaitDone(void)
{
    _USE_OJPD(MApi_JPEG_WaitDone);
    MS_U16 reg_val = E_NJPD_EVENT_DEC_DONE;

    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_ERR("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_DEC_FAILED;
    }
    _NJPEG_EX_API_MutexLock(eNJPDNum);

#if SW_NJPD_RGB_CMYK
    // It's pure software decode
    if((E_JPEG_CMYK == gu8Scan_type[eNJPDNum])
    || (E_JPEG_RGB == gu8Scan_type[eNJPDNum]))
    {
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_DEC_DONE;
    }
#endif

    reg_val = MDrv_NJPD_GetEventFlag();

    if(reg_val & NJPD_EVENT_ERROR)
    {
        JPEG_terminate(E_JPEG_DECODE_ERROR);
        if(reg_val & NJPD_EVENT_WRITE_PROTECT)
        {
            JPEG_DEBUG_API_MSG("[%s]Write protect!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_RES_MARKER_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]restart marker error!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_RMID_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]restart marker index disorder error!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_MINICODE_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]mini-code error!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_INV_SCAN_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]inverse scan error!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_DATA_LOSS_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]data loss error!!\n", __FUNCTION__);
        }
        if(reg_val & NJPD_EVENT_HUFF_TABLE_ERR)
        {
            JPEG_DEBUG_API_ERR("[%s]Huffman table error!!\n", __FUNCTION__);
        }
        #if ENABLE_NJPD_DEBUG_MSG
        MDrv_NJPD_Debug();
        #else
        if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
        {
            MDrv_NJPD_Debug();
        }
        #endif
        if(reg_val & (NJPD_EVENT_RES_MARKER_ERR|NJPD_EVENT_RMID_ERR))
        {
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_DEC_RST_ERROR;
        }
        if(reg_val & (NJPD_EVENT_MINICODE_ERR | NJPD_EVENT_INV_SCAN_ERR
            | NJPD_EVENT_DATA_LOSS_ERR | NJPD_EVENT_HUFF_TABLE_ERR))
        {
            _NJPEG_EX_API_MutexUnlock(eNJPDNum);
            return E_JPEG_DEC_BITSTREAM_ERROR;
        }
    }

    if(E_NJPD_EVENT_DEC_DONE & reg_val)
    {
        JPEG_DEBUG_API_MSG("%s:Decode Done!!\n", __FUNCTION__);
        if(MDrv_NJPD_IsNeedToPatch(E_NJPD_MIU_LAST_Z_PATCH))
        {
            MsOS_DelayTask(1);
        }
#if PRINT_NJPD_DECODE_TIME
        u32MeasureDecodeTimeHW[eNJPDNum] = MDrv_TIMER_GetUs(E_TIMER_1)-u32MeasureDecodeTime[eNJPDNum];
        JPEG_DEBUG_API_MSG("SW decode header:[%ld]us, HW decode: [%ld]us, table:[%ld]us\n", u32MeasureDecodeTimeSW[eNJPDNum],
            u32MeasureDecodeTimeHW[eNJPDNum], u32MeasureDecodeTimeTable[eNJPDNum]);
#endif
        if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API)
        {
#ifndef MSOS_TYPE_LINUX_KERNEL
            verJPD_CRC32_Init();
            verJPD_CRC32_Update((MS_U8 *)MWC_BUFFER_ADDR,
                (MS_U32)MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2);
            JPEG_DEBUG_API_MSG("[%dx%d], u32CRCResult=0x%lx\n",MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), verJPD_CRC32_GetResult());

            _MApi_NJPD_Debug();
            _MApi_NJPD_PrintMem(MWC_BUFFER_ADDR, 0x100);
#endif
        }
#if LOG_DATA_TO_USB
            if(MApi_JPEG_GetWidth() <=JPEG_MAX_WIDTH[eNJPDNum]
                && MApi_JPEG_GetHeight() <=JPEG_MAX_HEIGHT[eNJPDNum])
            {
                MS_U32 u32Size;
                u32Size = MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2;
                JPEG_DEBUG_API_MSG("[%dx%d], MWC_BUFFER_ADDR=0x%lx, size=0x%lx", MApi_JPEG_GetWidth(),
                    MApi_JPEG_GetHeight(), MS_VA2PA(MWC_BUFFER_ADDR),u32Size);
                _PrintOutputMem(MWC_BUFFER_ADDR, u32Size);
            }
#endif
        _NJPEG_EX_API_MutexUnlock(eNJPDNum);
        return E_JPEG_DEC_DONE;
    }
    else
    {
#if SUPPORT_HIGH_LOW_REVERSE
        if(bHighLowReverse[eNJPDNum])
        {
            if((E_NJPD_EVENT_MRC1_EMPTY & reg_val)
            && (E_JPEG_BUFFER_LOW != u8PreLHFlag[eNJPDNum]))
            {
                u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_LOW;
                JPEG_DEBUG_API_MSG("[%s]MRBL done!!\n", __FUNCTION__);
                if(_bEOF_flag[eNJPDNum])
                {
                    MDrv_NJPD_ReadLastBuffer();
                }
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_DEC_MRBL_DONE;
            }
            else if((E_NJPD_EVENT_MRC0_EMPTY & reg_val)
            && (E_JPEG_BUFFER_HIGH != u8PreLHFlag[eNJPDNum]))
            {
                u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_HIGH;
                JPEG_DEBUG_API_MSG("[%s]MRBH done!!\n", __FUNCTION__);
                if(_bEOF_flag[eNJPDNum])
                {
                    MDrv_NJPD_ReadLastBuffer();
                }
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_DEC_MRBH_DONE;
            }
        }
        else
#endif
        {
            if((E_NJPD_EVENT_MRC0_EMPTY & reg_val)
            && (E_JPEG_BUFFER_LOW != u8PreLHFlag[eNJPDNum]))
            {
                u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_LOW;
                JPEG_DEBUG_API_MSG("[%s]MRBL done!!\n", __FUNCTION__);
                if(_bEOF_flag[eNJPDNum])
                {
                    MDrv_NJPD_ReadLastBuffer();
                }
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_DEC_MRBL_DONE;
            }
            else if((E_NJPD_EVENT_MRC1_EMPTY & reg_val)
            && (E_JPEG_BUFFER_HIGH != u8PreLHFlag[eNJPDNum]))
            {
                u8PreLHFlag[eNJPDNum] = E_JPEG_BUFFER_HIGH;
                JPEG_DEBUG_API_MSG("[%s]MRBH done!!\n", __FUNCTION__);
                if(_bEOF_flag[eNJPDNum])
                {
                    MDrv_NJPD_ReadLastBuffer();
                }
                _NJPEG_EX_API_MutexUnlock(eNJPDNum);
                return E_JPEG_DEC_MRBH_DONE;
            }
        }
    }
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
    return E_JPEG_DEC_DECODING;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get datetime info of JPEG EXIF
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_GetEXIFDateTime(JPEG_EXIF_DateTime *DateTime)
{
    _USE_OJPD(MApi_JPEG_GetEXIFDateTime, DateTime);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    DateTime->bHasDataTime = _stEXIF_DateTime[eNJPDNum].bHasDataTime;
    DateTime->u32Year = _stEXIF_DateTime[eNJPDNum].u32Year;
    DateTime->u32Month = _stEXIF_DateTime[eNJPDNum].u32Month;
    DateTime->u32Day = _stEXIF_DateTime[eNJPDNum].u32Day;
    DateTime->u32Hour = _stEXIF_DateTime[eNJPDNum].u32Hour;
    DateTime->u32Minute = _stEXIF_DateTime[eNJPDNum].u32Minute;
    DateTime->u32Second = _stEXIF_DateTime[eNJPDNum].u32Second;

    JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime======= Exist = %d \n", _stEXIF_DateTime[eNJPDNum].bHasDataTime);
    JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Year, _stEXIF_DateTime[eNJPDNum].u32Month, _stEXIF_DateTime[eNJPDNum].u32Day);
    JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime[eNJPDNum].u32Hour, _stEXIF_DateTime[eNJPDNum].u32Minute, _stEXIF_DateTime[eNJPDNum].u32Second);

    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get orientation info of JPEG EXIF
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_GetEXIFOrientation(JPEG_EXIF_Orientation *eOrientation)
{
    _USE_OJPD(MApi_JPEG_GetEXIFOrientation, eOrientation);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *eOrientation = _eEXIF_Orientation[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation[eNJPDNum]);
#if ENABLE_NJPD_DEBUG_MSG
    MDrv_NJPD_Debug();
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        MDrv_NJPD_Debug();
    }
#endif
    return E_JPEG_OKAY;
}

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_Result _MApi_JPEG_GetEXIFManufacturer(MS_U8 *pu8Manufacturer, MS_U8 u8size)
{
    _USE_OJPD(MApi_JPEG_GetEXIFManufacturer, pu8Manufacturer, u8size);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    MS_U8 u8i=0;
    JPEG_memset((void*)(pu8Manufacturer), 0, u8size);
    JPEG_DEBUG_API_MSG("JPEG EXIF Manufacturer =");
    while(_u8EXIF_Manufacturer[eNJPDNum][u8i] && u8i<JPEG_MANUFACTURER_SIZE)
    {
        *(pu8Manufacturer+u8i) =_u8EXIF_Manufacturer[eNJPDNum][u8i];
        JPEG_DEBUG_API_MSG("%c", *(pu8Manufacturer+u8i));
        u8i++;
        if(u8i>=u8size)
        {
            JPEG_DEBUG_API_ERR("%s: NOT enough space!!\n", __FUNCTION__);
            return E_JPEG_FAILED;
        }
    }
    JPEG_DEBUG_API_MSG("\n");
//    JPEG_DEBUG_API_MSG("JPEG EXIF Manufacturer = %02x %02x %02x %02x %02x %02x %02x %02x\n",
//        *pu8Manufacturer, *(pu8Manufacturer+1), *(pu8Manufacturer+2), *(pu8Manufacturer+3),
//        *(pu8Manufacturer+4),*(pu8Manufacturer+5),*(pu8Manufacturer+6),*(pu8Manufacturer+7));
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFModel(MS_U8 *pu8Model, MS_U8 u8size)
{
    _USE_OJPD(MApi_JPEG_GetEXIFModel, pu8Model, u8size);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    MS_U8 u8i=0;
    JPEG_memset((void*)(pu8Model), 0, u8size);
    JPEG_DEBUG_API_MSG("JPEG EXIF Model =");
    while(_u8EXIF_Model[eNJPDNum][u8i] && u8i<JPEG_MODEL_SIZE)
    {
        *(pu8Model+u8i) =_u8EXIF_Model[eNJPDNum][u8i];
        JPEG_DEBUG_API_MSG("%c", *(pu8Model+u8i));
        u8i++;
        if(u8i>=u8size)
        {
            JPEG_DEBUG_API_ERR("%s: NOT enough space!!\n", __FUNCTION__);
            return E_JPEG_FAILED;
        }
    }
    JPEG_DEBUG_API_MSG("\n");

//    JPEG_DEBUG_API_MSG("JPEG EXIF Model = %02x %02x %02x %02x %02x %02x %02x %02x\n",
//        *pu8Model, *(pu8Model+1), *(pu8Model+2), *(pu8Model+3),
//        *(pu8Model+4),*(pu8Model+5),*(pu8Model+6),*(pu8Model+7));
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFFlash(MS_U16 *pu16Flash)
{
    _USE_OJPD(MApi_JPEG_GetEXIFFlash, pu16Flash);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu16Flash = _u16EXIF_Flash[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG EXIF Flash = %d \n", *pu16Flash);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFISOSpeedRatings(MS_U32 *pu32ISOSpeedRatings)
{
    _USE_OJPD(MApi_JPEG_GetEXIFISOSpeedRatings, pu32ISOSpeedRatings);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ISOSpeedRatings = _u32EXIF_ISOSpeedRatings[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG EXIF ISO = %ld \n", *pu32ISOSpeedRatings);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFShutterSpeedValue(JPEG_RATIONAL *pShutterSpeedValue)
{
    _USE_OJPD(MApi_JPEG_GetEXIFShutterSpeedValue, pShutterSpeedValue);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pShutterSpeedValue->numerator = _stEXIF_ShutterSpeedValue[eNJPDNum].numerator;
    pShutterSpeedValue->denominator= _stEXIF_ShutterSpeedValue[eNJPDNum].denominator;
    JPEG_DEBUG_API_MSG("ShutterSpeedValue numerator=%ld, denominator=%ld\n", pShutterSpeedValue->numerator, pShutterSpeedValue->denominator);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFApertureValue(JPEG_RATIONAL *pApertureValue)
{
    _USE_OJPD(MApi_JPEG_GetEXIFApertureValue, pApertureValue);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pApertureValue->s_numerator = _stEXIF_ApertureValue[eNJPDNum].s_numerator;
    pApertureValue->s_denominator= _stEXIF_ApertureValue[eNJPDNum].s_denominator;
    JPEG_DEBUG_API_MSG("ApertureValue numerator=%ld, denominator=%ld\n", pApertureValue->s_numerator, pApertureValue->s_denominator);
    return E_JPEG_OKAY;
}


JPEG_Result _MApi_JPEG_GetEXIFExposureBiasValue(JPEG_RATIONAL *pExposureBiasValue)
{
    _USE_OJPD(MApi_JPEG_GetEXIFExposureBiasValue, pExposureBiasValue);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pExposureBiasValue->numerator = _stEXIF_ExposureBiasValue[eNJPDNum].numerator;
    pExposureBiasValue->denominator= _stEXIF_ExposureBiasValue[eNJPDNum].denominator;
    JPEG_DEBUG_API_MSG("ExposureBiasValue numerator=%ld, denominator=%ld\n", pExposureBiasValue->numerator, pExposureBiasValue->denominator);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFFocalLength(JPEG_RATIONAL *pFocalLength)
{
    _USE_OJPD(MApi_JPEG_GetEXIFFocalLength, pFocalLength);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pFocalLength->s_numerator = _stEXIF_FocalLength[eNJPDNum].s_numerator;
    pFocalLength->s_denominator= _stEXIF_FocalLength[eNJPDNum].s_denominator;
    JPEG_DEBUG_API_MSG("FocalLength numerator=%ld, denominator=%ld\n", pFocalLength->s_numerator, pFocalLength->s_denominator);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFImageWidth(MS_U32 *pu32ImageWidth)
{
    _USE_OJPD(MApi_JPEG_GetEXIFImageWidth, pu32ImageWidth);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ImageWidth = _u32EXIF_ImageWidth[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG EXIF Image Width = %ld \n", *pu32ImageWidth);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFImageHeight(MS_U32 *pu32ImageHeight)
{
    _USE_OJPD(MApi_JPEG_GetEXIFImageHeight, pu32ImageHeight);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ImageHeight = _u32EXIF_ImageHeight[eNJPDNum];
    JPEG_DEBUG_API_MSG("JPEG EXIF Image Height = %ld \n", *pu32ImageHeight);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFExposureTime(JPEG_RATIONAL *pExposureTime)
{
    _USE_OJPD(MApi_JPEG_GetEXIFApertureValue, pExposureTime);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pExposureTime->s_numerator = _stEXIF_ExposureTime[eNJPDNum].s_numerator;
    pExposureTime->s_denominator= _stEXIF_ExposureTime[eNJPDNum].s_denominator;
    JPEG_DEBUG_API_MSG("ExposureTime numerator=%ld, denominator=%ld\n", pExposureTime->s_numerator, pExposureTime->s_denominator);
    return E_JPEG_OKAY;
}


JPEG_Result _MApi_JPEG_GetEXIFFNumber(JPEG_RATIONAL *pFNumber)
{
    _USE_OJPD(MApi_JPEG_GetEXIFApertureValue, pFNumber);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pFNumber->s_numerator = _stEXIF_FNumber[eNJPDNum].s_numerator;
    pFNumber->s_denominator= _stEXIF_FNumber[eNJPDNum].s_denominator;
    JPEG_DEBUG_API_MSG("FNumber numerator=%ld, denominator=%ld\n", pFNumber->s_numerator, pFNumber->s_denominator);
    return E_JPEG_OKAY;
}


#endif

JPEG_Result _MApi_JPEG_GetControl(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32size)
{
    JPEG_Result eResult = E_JPEG_OKAY;
    _USE_OJPD(MApi_JPEG_GetControl, eGetID, param, u32size);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    switch(eGetID)
    {
        case E_JPEG_GET_EXIF_MANUFACTURER:
            eResult = MApi_JPEG_GetEXIFManufacturer((MS_U8 *)param, u32size*4);
            JPEG_DEBUG_API_MSG("@JPEG EXIF Manufacturer = %c%c%c%c%c%c%c%c%c%c%c%c\n",
                *((MS_U8 *)param), *((MS_U8 *)param+1), *((MS_U8 *)param+2), *((MS_U8 *)param+3),
                *((MS_U8 *)param+4),*((MS_U8 *)param+5),*((MS_U8 *)param+6),*((MS_U8 *)param+7),
                *((MS_U8 *)param+8),*((MS_U8 *)param+9),*((MS_U8 *)param+10),*((MS_U8 *)param+11)
                );
            break;
        case E_JPEG_GET_EXIF_MODEL:
            eResult = MApi_JPEG_GetEXIFModel((MS_U8 *)param, u32size*4);
            JPEG_DEBUG_API_MSG("@JPEG EXIF Model = %c%c%c%c%c%c%c%c%c%c%c%c\n",
                *((MS_U8 *)param), *((MS_U8 *)param+1), *((MS_U8 *)param+2), *((MS_U8 *)param+3),
                *((MS_U8 *)param+4),*((MS_U8 *)param+5),*((MS_U8 *)param+6),*((MS_U8 *)param+7),
                *((MS_U8 *)param+8),*((MS_U8 *)param+9),*((MS_U8 *)param+10),*((MS_U8 *)param+11)
                );
            break;
        case E_JPEG_GET_EXIF_FLASH:
            *param = (MS_U32)_u16EXIF_Flash[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG EXIF Flash = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_ISO_SPEED_RATINGS:
            *param = _u32EXIF_ISOSpeedRatings[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG EXIF ISO = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = _stEXIF_ShutterSpeedValue[eNJPDNum].numerator;
            *(param+1) = _stEXIF_ShutterSpeedValue[eNJPDNum].denominator;
            JPEG_DEBUG_API_MSG("@ShutterSpeedValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_APERTURE_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_ApertureValue[eNJPDNum].s_numerator;
            *(param+1) = (MS_U32)_stEXIF_ApertureValue[eNJPDNum].s_denominator;
            JPEG_DEBUG_API_MSG("@ApertureValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = _stEXIF_ExposureBiasValue[eNJPDNum].numerator;
            *(param+1) = _stEXIF_ExposureBiasValue[eNJPDNum].denominator;
            JPEG_DEBUG_API_MSG("@ExposureBiasValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_FOCAL_LENGTH:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_FocalLength[eNJPDNum].s_numerator;
            *(param+1) = (MS_U32)_stEXIF_FocalLength[eNJPDNum].s_denominator;
            JPEG_DEBUG_API_MSG("@FocalLength numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_IMAGE_WIDTH:
            *param = _u32EXIF_ImageWidth[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG EXIF Image Width = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_IMAGE_HEIGHT:
            *param = _u32EXIF_ImageHeight[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG EXIF Image Height = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_TIME:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_ExposureTime[eNJPDNum].s_numerator;
            *(param+1) = (MS_U32)_stEXIF_ExposureTime[eNJPDNum].s_denominator;
            JPEG_DEBUG_API_MSG("@ExposureTime numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_F_NUMBER:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_FNumber[eNJPDNum].s_numerator;
            *(param+1) = (MS_U32)_stEXIF_FNumber[eNJPDNum].s_denominator;
            JPEG_DEBUG_API_MSG("@FNumber numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_PROGRAM:
            *param = (MS_U32)_u16EXIF_Exposureprogram[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG EXIF Exposure Program = %ld \n", *param);
            break;
        case E_JPEG_GET_Y_SAMPLING_FACTOR_HORIZONTAL:
            *param = (MS_U32)_u8Comp_h_samp[eNJPDNum][0];
            JPEG_DEBUG_API_MSG("@JPEG Horizontal sampling factor(Y) = %ld \n", *param);
            break;
        case E_JPEG_GET_Y_SAMPLING_FACTOR_VERTICAL:
            *param = (MS_U32)_u8Comp_v_samp[eNJPDNum][0];
            JPEG_DEBUG_API_MSG("@JPEG Vertical sampling factor(Y) = %ld \n", *param);
            break;
        case E_JPEG_GET_COMPONENTS_NUM:
            *param = (MS_U32)_u8Comps_in_frame[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG componemts num = %ld \n", *param);
            break;
        case E_JPEG_GET_IS_PROGRESSIVE:
            *param = (MS_U32)_bProgressive_flag[eNJPDNum];
            JPEG_DEBUG_API_MSG("@JPEG is progressive = %ld \n", *param);
            break;
        case E_JPEG_GET_COLOR_FORMAT:
            switch(gu8Scan_type[eNJPDNum])
            {
                case E_JPEG_GRAYSCALE:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_GRAY_SCALE;
                    break;
                case E_JPEG_YH1V1:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_YUV444;
                    break;
                case E_JPEG_YH2V1:
                case E_JPEG_YH1V2:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_YUV422;
                    break;
                case E_JPEG_YH2V2:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_YUV420;
                    break;
                case E_JPEG_YH4V1:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_YUV411;
                    break;
#if SW_NJPD_RGB_CMYK
                case E_JPEG_RGB:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_RGB;
                    break;
                case E_JPEG_CMYK:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_CMYK;
                    break;
#endif
                default:
                    *param = (MS_U32)E_JPEG_INPUT_FORMAT_GRAY_SCALE;
                    break;
            }
            JPEG_DEBUG_API_MSG("@JPEG input color format = %ld \n", *param);
            break;

        default:
            eResult = E_JPEG_FAILED;
            break;
    }
    return eResult;
}

////////////////////////////////////////////////////////////////////////////////////////
///   current unused function, maybe removed in new MDDI
////////////////////////////////////////////////////////////////////////////////////////


#if SW_NJPD_RGB_CMYK
// Decodes and dequantizes the next row of coefficients.
JPEG_STATIC MS_BOOL msAPI_JPEG_decode_next_row( void )
{
    MS_U16 row_block = 0;
    MS_U16 mcu_row, mcu_block, k;

    // Clearing the entire row block buffer can take a lot of time!
    // Instead of clearing the entire buffer each row, keep track
    // of the number of nonzero entries written to each block and do
    // selective clears.
    //memset(_ps16Block_seg[0], 0, _u16Mcus_per_row[eNJPDNum] * _u8Blocks_per_mcu[eNJPDNum] * 64 * sizeof(BLOCK_TYPE));

    for ( mcu_row = 0; mcu_row < _u16Mcus_per_row[eNJPDNum]; mcu_row++ )
    {
        if ( ( _u16Restart_interval[eNJPDNum] ) && ( _u16Restarts_left[eNJPDNum] == 0 ) )
        {
            if(!JPEG_process_restart())
                return FALSE;
        }

        for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu[eNJPDNum]; mcu_block++ )
        {
            MS_U8 component_id = _u8Mcu_org[eNJPDNum][mcu_block];
            MS_U8 prev_num_set;
            JPEG_HuffTbl *Ph;

            JPEG_BLOCK_TYPE *p = _ps16Block_seg[row_block];
            JPEG_QUANT_TYPE *q = _QuantTables[eNJPDNum][_u8Comp_quant[eNJPDNum][component_id]].s16Value;
            MS_S32 r, s;

            if ( ( s = JPEG_huff_decode( &_Huff_tbls[eNJPDNum][_u8Comp_dc_tab[eNJPDNum][component_id]] ) ) != 0 )
            {
                r = JPEG_get_bits_2( s );
                s = HUFF_EXTEND( r, s );
            }

            //printf("r : %d s : %d\n",s,r);

            _u32Last_dc_val[eNJPDNum][component_id] = ( s += _u32Last_dc_val[eNJPDNum][component_id] );

            p[0] = s * q[0];

            prev_num_set = _u8Block_max_zag_set[row_block];

            Ph = &_Huff_tbls[eNJPDNum][_u8Comp_ac_tab[eNJPDNum][component_id]];

            for ( k = 1; k < 64; k++ )
            {
                s = JPEG_huff_decode( Ph );

                //printf("Decode s :%d\n",s);

                r = s >> 4;
                s &= 15;

                if ( s )
                {
                    if ( r )
                    {
                        if ( ( k + r ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                            return FALSE;
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U32 n = MIN( r, prev_num_set - k );
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += r;
                    }

                    r = JPEG_get_bits_2( s );
                    s = HUFF_EXTEND( r, s );

                    //assert(k < 64);

                    p[_u8ZAG[k]] = s * q[k];
                }
                else
                {
                    if ( r == 15 )
                    {
                        if ( ( k + 15 ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                            return FALSE;
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U16 n = MIN( 16, prev_num_set - k );        //bugfix Dec. 19, 2001 - was 15!
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += 15;
                    }
                    else
                    {
                        //while (k < 64)
                        //  p[ZAG[k++]] = 0;

                        break;
                    }
                }
            }

            if ( k < prev_num_set )
            {
                MS_U16 kt = k;
                while ( kt < prev_num_set && kt < 64)           // kt < 64 is for coverity check
                {
                    p[_u8ZAG[kt++]] = 0;
                }
            }

            _u8Block_max_zag_set[row_block] = k;

            //block_num[row_block++] = k;
            row_block++;
        }

        _u16Restarts_left[eNJPDNum]--;
    }
    return TRUE;
}

/*----------------------------------------------------------------------------*/
JPEG_STATIC void msAPI_JPEG_idct( JPEG_BLOCK_TYPE *data, MS_U8 *Pdst_ptr )
{
    MS_S32 t0, t1, t2, t3;
    MS_S32 t10, t11, t12, t13;
    MS_S32 a1, a2, a3, a4, a5;
    MS_S32 rowctl;
    register JPEG_BLOCK_TYPE *pdata;

    //kevinhuang, use an internal array in idct to avoid memcpy to save time
    JPEG_BLOCK_TYPE workspace[64];
    JPEG_BLOCK_TYPE *wsptr;

    pdata = data;
    wsptr = workspace;
    for ( rowctl = 8 - 1; rowctl >= 0; rowctl-- )
    {
        if ( ( pdata[1] | pdata[2] | pdata[3] | pdata[4] | pdata[5] | pdata[6] | pdata[7] ) == 0 )
        {
            MS_S16 dc_val = ( MS_S16 )( pdata[0] << 2 );

            wsptr[0] = dc_val;
            wsptr[1] = dc_val;
            wsptr[2] = dc_val;
            wsptr[3] = dc_val;
            wsptr[4] = dc_val;
            wsptr[5] = dc_val;
            wsptr[6] = dc_val;
            wsptr[7] = dc_val;

            pdata += 8;       /* advance pointer to next row */
            wsptr += 8;
            continue;
        }

        a2 = ( MS_S32 )pdata[2];
        a3 = ( MS_S32 )pdata[6];

        a1 = (FIX_0_5411) * (a2 + a3);
        t2 = (-FIX_1_8477) * a3 + a1;
        t3 = (FIX_0_7653) * a2 + a1;

        t0 = ( ( MS_S32 )pdata[0] + ( MS_S32 )pdata[4] ) << 13;
        t1 = ( ( MS_S32 )pdata[0] - ( MS_S32 )pdata[4] ) << 13;

        t10 = t0 + t3;
        t13 = t0 - t3;
        t11 = t1 + t2;
        t12 = t1 - t2;

        t0 = ( MS_S32 )pdata[7];
        t1 = ( MS_S32 )pdata[5];
        t2 = ( MS_S32 )pdata[3];
        t3 = ( MS_S32 )pdata[1];

        a1 = t0 + t3;
        a2 = t1 + t2;
        a3 = t0 + t2;
        a4 = t1 + t3;
        a5 = (FIX_1_1758) * (a3 + a4);

        a1 = (-FIX_0_8999) * a1;
        a2 = (-FIX_2_5629) * a2;
        a3 = (-FIX_1_9615) * a3;
        a4 = (-FIX_0_3901) * a4;
        t0 = (FIX_0_2986) * t0;
        t1 = (FIX_2_0531) * t1;
        t2 = (FIX_3_0727) * t2;
        t3 = (FIX_1_5013) * t3;

        a3 += a5;
        a4 += a5;

        t0 += a1 + a3;
        t1 += a2 + a4;
        t2 += a2 + a3;
        t3 += a1 + a4;

        wsptr[0] = ( MS_S16 )DESCALE( t10 + t3, 13 - 2 );
        wsptr[1] = ( MS_S16 )DESCALE( t11 + t2, 13 - 2 );
        wsptr[2] = ( MS_S16 )DESCALE( t12 + t1, 13 - 2 );
        wsptr[3] = ( MS_S16 )DESCALE( t13 + t0, 13 - 2 );
        wsptr[4] = ( MS_S16 )DESCALE( t13 - t0, 13 - 2 );
        wsptr[5] = ( MS_S16 )DESCALE( t12 - t1, 13 - 2 );
        wsptr[6] = ( MS_S16 )DESCALE( t11 - t2, 13 - 2 );
        wsptr[7] = ( MS_S16 )DESCALE( t10 - t3, 13 - 2 );

        pdata += 8;
        wsptr += 8;
    }

    pdata = workspace;
    for ( rowctl = 8 - 1; rowctl >= 0; rowctl-- )
    {
        MS_S16 i;

        if ( ( pdata[8 * 1] | pdata[8 * 2] | pdata[8 * 3] | pdata[8 * 4] | pdata[8 * 5] | pdata[8 * 6] | pdata[8 * 7] ) == 0 )
        {
            MS_S16 dc_val = ( MS_S16 )DESCALE( ( MS_S32 )pdata[0], 2 + 3 ) + 128;

            jpeg_clamp( dc_val );

            Pdst_ptr[8 * 0] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor[eNJPDNum] == 3)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 4] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor[eNJPDNum] == 2)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 2] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 6] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor[eNJPDNum] == 1)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 1] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 5] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 3] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 7] = ( MS_U8 )dc_val;

            pdata++;
            Pdst_ptr++;
            continue;
        }

        a2 = ( MS_S32 )pdata[8 * 2];
        a3 = ( MS_S32 )pdata[8 * 6];

        a1 = (FIX_0_5411) * (a2 + a3);
        t2 = (-FIX_1_8477) * a3 + a1;
        t3 = (FIX_0_7653) * a2 + a1;

        t0 = ( ( MS_S32 )pdata[8 * 0] + ( MS_S32 )pdata[8 * 4] ) << 13;
        t1 = ( ( MS_S32 )pdata[8 * 0] - ( MS_S32 )pdata[8 * 4] ) << 13;

        t10 = t0 + t3;
        t13 = t0 - t3;
        t11 = t1 + t2;
        t12 = t1 - t2;

        t0 = ( MS_S32 )pdata[8 * 7];
        t1 = ( MS_S32 )pdata[8 * 5];
        t2 = ( MS_S32 )pdata[8 * 3];
        t3 = ( MS_S32 )pdata[8 * 1];

        a1 = t0 + t3;
        a2 = t1 + t2;
        a3 = t0 + t2;
        a4 = t1 + t3;
        a5 = (FIX_1_1758) * (a3 + a4);

        a1 = (-FIX_0_8999) * a1;
        a2 = (-FIX_2_5629) * a2;
        a3 = (-FIX_1_9615) * a3;
        a4 = (-FIX_0_3901) * a4;
        t0 = (FIX_0_2986) * t0;
        t1 = (FIX_2_0531) * t1;
        t2 = (FIX_3_0727) * t2;
        t3 = (FIX_1_5013) * t3;

        a3 += a5;
        a4 += a5;

        t0 += a1 + a3;
        t1 += a2 + a4;
        t2 += a2 + a3;
        t3 += a1 + a4;

        i = ( MS_S16 )DESCALE( t10 + t3, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 0] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor[eNJPDNum]== 3)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t13 - t0, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 4] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor[eNJPDNum]== 2)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t12 + t1, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 2] = ( MS_U8 )i;

        i = ( MS_S16 )DESCALE( t11 - t2, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 6] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor[eNJPDNum]== 1)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t11 + t2, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 1] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t13 + t0, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 3] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t12 - t1, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 5] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t10 - t3, 13 + 2 + 3 ) + 128;
        jpeg_clamp( i )
        Pdst_ptr[8 * 7] = ( MS_U8 )i;

        pdata++;
        Pdst_ptr++;
    }
}

JPEG_STATIC MS_BOOL msAPI_JPEG_Idct_CMYKConvert(void)
{
    JPEG_BLOCK_TYPE *Psrc_ptr = _ps16Block_seg[0];
    MS_U8 *Pdst_ptr = gpu8Sample_buf;
    MS_U8 *u8Start_Each_Block, *u8Start_Each_Row;
    MS_U32 u32_y_Blcok_Pos = 0, Max_Pixel_Per_Row;
    MS_S32 i;
    MS_U8  u8Row_in_block, k;
    MS_U16 u16Pixel_Per_Block_x = 8, u16Pixel_Per_Block_y = 8;
    MS_U16 u16Pixel_Per_Pack_row = 16, u16TotalBlock_Per_Row, u16ScaleDownStep = 1 << _u8ScaleDownFactor[eNJPDNum];

    u16TotalBlock_Per_Row = (_u32Max_blocks_per_row[eNJPDNum])/2;

    u16Pixel_Per_Pack_row = u16Pixel_Per_Pack_row >> _u8ScaleDownFactor[eNJPDNum];
    u16Pixel_Per_Block_x = u16Pixel_Per_Block_x/* >> _u8ScaleDownFactor[eNJPDNum]*/;
    u16Pixel_Per_Block_y = u16Pixel_Per_Block_y >> _u8ScaleDownFactor[eNJPDNum];


    Max_Pixel_Per_Row = (u16TotalBlock_Per_Row*u16Pixel_Per_Block_x >> _u8ScaleDownFactor[eNJPDNum]);

    //Get start position of each row
    u8Start_Each_Row = (u8Out_buf + (u32_Decode_Line*(Max_Pixel_Per_Row*u16Pixel_Per_Block_y)));

    for ( i = (MS_S32)_u32Max_blocks_per_row[eNJPDNum]; i > 0;)
    {
        //===================GET Y1 BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Skip U. V block for 422 format
        Psrc_ptr += 256;
        Pdst_ptr += 256;
        i-=4;

        if(i == 0)
        {
            //Already decoded all blocks
            continue;
        }
        else if(i < 0)
        {
            printf("GET BLOCK NUMBER ERROR!\n");
            JPEG_terminate( E_JPEG_DECODE_ERROR );
            return FALSE;
        }

        //Move y block position in a row
        u32_y_Blcok_Pos++;
        //===================GET Y2 BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos--;
        //===================GET U BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 1) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET V BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 3) ) =  255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 128;
        Pdst_ptr += 128;
        i-=2;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos+=2;

    }

    u32_Decode_Line++;
    return TRUE;
}

JPEG_STATIC MS_BOOL msAPI_JPEG_Idct_RGBConvert(void)
{
    JPEG_BLOCK_TYPE *Psrc_ptr = _ps16Block_seg[0];
    MS_U8 *Pdst_ptr = gpu8Sample_buf;
    MS_U8 *u8Start_Each_Block, *u8Start_Each_Row;
    MS_U32 u32_y_Blcok_Pos = 0, Max_Pixel_Per_Row;
    MS_S32 i;
    MS_U8  u8Row_in_block, k;
    MS_U16 u16Pixel_Per_Block_x = 8, u16Pixel_Per_Block_y = 8;
    MS_U16 u16Pixel_Per_Pack_row = 16, u16TotalBlock_Per_Row, u16ScaleDownStep = 1 << _u8ScaleDownFactor[eNJPDNum];

    u16TotalBlock_Per_Row = (_u32Max_blocks_per_row[eNJPDNum]*2)/3;

    u16Pixel_Per_Pack_row = u16Pixel_Per_Pack_row >> _u8ScaleDownFactor[eNJPDNum];
    u16Pixel_Per_Block_x = u16Pixel_Per_Block_x/* >> _u8ScaleDownFactor[eNJPDNum]*/;
    u16Pixel_Per_Block_y = u16Pixel_Per_Block_y >> _u8ScaleDownFactor[eNJPDNum];


    Max_Pixel_Per_Row = (u16TotalBlock_Per_Row*u16Pixel_Per_Block_x >> _u8ScaleDownFactor[eNJPDNum]);

    //Get start position of each row
    u8Start_Each_Row = (u8Out_buf + (u32_Decode_Line*(Max_Pixel_Per_Row*u16Pixel_Per_Block_y)));

    for ( i = (MS_S32)_u32Max_blocks_per_row[eNJPDNum]; i > 0;)
    {

        //===================GET R BLOCKs========================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Skip U. V block for 422 format
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET G BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 1) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET B BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 3) ) =  Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        if(i == 0)
        {
            //Already decoded all blocks
            continue;
        }
        else if(i < 0)
        {
            printf("GET BLOCK NUMBER ERROR!\n");
            JPEG_terminate( E_JPEG_DECODE_ERROR );
            return FALSE;
        }

        //Move y block position in a row
        u32_y_Blcok_Pos++;
        //===================GET R BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 192;
        Pdst_ptr += 192;
        i-=3;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos++;

    }

    u32_Decode_Line++;
    return TRUE;
}

//------------------------------------------------------------------------------
// Performs a 2D IDCT over the entire row's coefficient buffer.
JPEG_STATIC MS_BOOL msAPI_JPEG_transform_row( void )
{
    JPEG_DEBUG_API_MSG("Max block : %ld\n", _u32Max_blocks_per_row[eNJPDNum]);

    switch ( gu8Scan_type[eNJPDNum] )
    {
        case E_JPEG_CMYK:
            if(!msAPI_JPEG_Idct_CMYKConvert())
                return FALSE;
            break;
        case E_JPEG_RGB:
            if(!msAPI_JPEG_Idct_RGBConvert())
                return FALSE;
            break;
        default :
            JPEG_DEBUG_API_ERR("System don`t support this format!\n");
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
            break;
    }
    return TRUE;

}

JPEG_STATIC JPEG_Result msAPI_JPEG_Baseline_Decode(void)
{

    if(_bProgressive_flag[eNJPDNum]==TRUE)
        return (E_JPEG_FAILED);

    if ( _u16Total_lines_left[eNJPDNum] == 0 )
    {
        return ( E_JPEG_OKAY );
    }

#ifdef NJPD_LONGJUMP_SUPPORT
if ( setjmp( _jmp_state ) )
    {
        return ( E_JPEG_FAILED );
    }
#endif

    if( _u16Total_lines_left[eNJPDNum] > 0 )
    {

        if(!msAPI_JPEG_decode_next_row())
            return E_JPEG_FAILED;

        JPEG_DEBUG_API_MSG("Max line num : %d\n",(_u16Total_lines_left[eNJPDNum]));

        if(!msAPI_JPEG_transform_row()) //IDCT
            return E_JPEG_FAILED;

        _u16Total_lines_left[eNJPDNum] -= gu8Max_mcu_y_size[eNJPDNum];
    }

    return (E_JPEG_DONE);
}
#endif //SW_NJPD_RGB_CMYK



void MApi_JPEG_DisableAddressConvert(void)
{
    _USE_OJPD(MApi_JPEG_DisableAddressConvert);
    _NJPEG_EX_API_MutexLock(eNJPDNum);
    // reset bForceNJPD2 to FALSE when MJPEG stop
    bForceNJPD2 = FALSE;

    JPEG_DEBUG_API_MSG("%s\n", __FUNCTION__);
    __bIsMjpeg[eNJPDNum] = FALSE;
    _bIsInit[eNJPDNum] = FALSE;
    _NJPEG_EX_API_MutexUnlock(eNJPDNum);
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the free memory for temporary usage.
///@param size \b IN indicate the size of wanted free memory
///@return the physical address of free memory
/********************************************************************/
MS_PHYADDR _MApi_JPEG_GetFreeMemory(MS_U32 size)
{
    _USE_OJPD(MApi_JPEG_GetFreeMemory, size);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bProgressive_flag[eNJPDNum] == TRUE)
    {
        return 0;
    }

    if(size > (INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE))
    {
        return 0;
    }

    INTERNAL_BUFFER_SIZE = INTERNAL_BUFFER_SIZE - size;

    return (_u32InternalBufferAddr_PA[eNJPDNum] + INTERNAL_BUFFER_SIZE);
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the data offset of JPEG file.
///@return the data offset of JPEG file
/********************************************************************/
MS_U32 _MApi_JPEG_GetDataOffset(void)
{
    _USE_OJPD(MApi_JPEG_GetDataOffset);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bThumbnailAccessMode[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("%s: Thumbnail mode!!\n", __FUNCTION__);
        return 0;
    }

    JPEG_DEBUG_API_MSG("%s : u32DataOffset[eNJPDNum] =  0x%lx\n", __FUNCTION__ , u32DataOffset[eNJPDNum]);
    return u32DataOffset[eNJPDNum];
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the sof marker offset of JPEG file.
///@return the sof marker offset of JPEG file
/********************************************************************/
MS_U32 _MApi_JPEG_GetSOFOffset(void)
{
    _USE_OJPD(MApi_JPEG_GetSOFOffset);
    if(_bIsInit[eNJPDNum] == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: NJPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bThumbnailAccessMode[eNJPDNum])
    {
        JPEG_DEBUG_API_MSG("%s: Thumbnail mode!!\n", __FUNCTION__);
        return 0;
    }

    JPEG_DEBUG_API_MSG("%s : u32SOFOffset[eNJPDNum] =  0x%lx\n", __FUNCTION__ , u32SOFOffset[eNJPDNum]);
    return u32SOFOffset[eNJPDNum];
}

// This function is only for MJPEG use only!!!!
void _MApi_JPEG_SetNJPDInstance(MS_U8 JPDNum)
{
    if(bOJPDOn || bForceNJPD2)
    {
        JPEG_DEBUG_API_MSG("OJPD is in use, force NJPD to E_NJPD_NJPD2\n");
        bForceNJPD2 = TRUE;
        JPDNum = E_NJPD_NJPD2;
    }

    if(JPDNum==E_NJPD_MJPEG_ON)
    {
        __bIsMjpeg[E_NJPD_NJPD1] = TRUE;
        __bIsMjpeg[E_NJPD_NJPD2] = TRUE;
        return;
    }
    if(JPDNum==E_NJPD_MJPEG_OFF)
    {
        __bIsMjpeg[E_NJPD_NJPD1] = FALSE;
        __bIsMjpeg[E_NJPD_NJPD2] = FALSE;
        return;
    }

    if(JPDNum >= E_NJPD_NJPD_TOTAL)
    {
        JPEG_DEBUG_API_ERR("Error!!!! input parameter of JPDNum out of range!!!!\n");
        JPDNum %= E_NJPD_NJPD_TOTAL;
    }
    eNJPDNum = (JPD_Number)JPDNum;
    MDrv_NJPD_SetNJPDInstance(eNJPDNum);
}


//------------------------------------------------------------------------------
/********************************************************************/
///Enable/Disable NJPD CMYK support.
///@param bEnable \b IN enable/disable supporting NJPD CMYK
///@return E_JPEG_OKAY/E_JPEG_FAILED
///Note: It need to be called before init. The default is NJPD CMYK support.
/********************************************************************/
JPEG_Result _MApi_JPEG_SupportCMYK(MS_BOOL bEnable)
{
    _USE_OJPD(MApi_JPEG_SupportCMYK, bEnable);
#if SW_NJPD_RGB_CMYK
    bEnableCMYK = bEnable;
    return E_JPEG_OKAY;
#else
    JPEG_DEBUG_API_MSG("Not Support CMYK formant!!!\n");
    UNUSED(bEnable);
    return E_JPEG_FAILED;
#endif
}

//------------------------------------------------------------------------------
/********************************************************************/
///Enable/Disable NJPD RGB support.
///@param bEnable \b IN enable/disable supporting NJPD RGB
///@return E_JPEG_OKAY/E_JPEG_FAILED
///Note: It need to be called before init. The default is NJPD RGB support.
/********************************************************************/
JPEG_Result _MApi_JPEG_SupportRGB(MS_BOOL bEnable)
{
    _USE_OJPD(MApi_JPEG_SupportRGB, bEnable);
#if SW_NJPD_RGB_CMYK
    bEnableRGB = bEnable;
    return E_JPEG_OKAY;
#else
    JPEG_DEBUG_API_MSG("Not Support RGB formant!!!\n");
    UNUSED(bEnable);
    return E_JPEG_FAILED;
#endif
}


#if SUPPORT_MPO_FORMAT
MS_BOOL _MApi_JPEG_IsMPOFormat( void )
{
    _USE_OJPD(MApi_JPEG_IsMPOFormat);
    JPEG_DEBUG_API_MSG("MApi_JPEG_IsMPOFormat=%d\n", bIsMPOFormat[eNJPDNum]);
    return bIsMPOFormat[eNJPDNum];
}

MS_BOOL _MApi_JPEG_GetMPOIndex(JPEG_MPO_INDEX_INFO **ppMPOIndex)
{
    _USE_OJPD(MApi_JPEG_GetMPOIndex, ppMPOIndex);
    if(_stMPOIndex[eNJPDNum].num_of_image == 0xFFFFFFFF)
    {
        JPEG_DEBUG_API_MSG("No MPO Index\n");
        return FALSE;
    }

    *ppMPOIndex = &_stMPOIndex[eNJPDNum];
    return TRUE;
}

MS_BOOL _MApi_JPEG_GetMPOAttr(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr)
{
    _USE_OJPD(MApi_JPEG_GetMPOAttr, image_no, ppMPOAttr);
    if((JPEG_MPO_MAX_SUPPORT_IMAGE < image_no)
    || (FALSE == _stMPOAttri[eNJPDNum][image_no - 1].used))
    {
        JPEG_DEBUG_API_MSG("No MPO Attribute with image no. %ld\n", image_no);
        return FALSE;
    }

    *ppMPOAttr = &_stMPOAttri[eNJPDNum][image_no - 1];
    return TRUE;
}

void _MApi_JPEG_DumpMPO(void)
{
    _USE_OJPD(MApi_JPEG_DumpMPO);
    MS_U32 i = 0;

    if(_stMPOIndex[eNJPDNum].num_of_image == 0xFFFFFFFF)
    {
        JPEG_DEBUG_API_MSG("No MPO Index\n");
        return;
    }

    JPEG_DEBUG_API_MSG("start_of_offset = %ld, num_of_image = %ld\n", _stMPOIndex[eNJPDNum].start_of_offset, _stMPOIndex[eNJPDNum].num_of_image);
    for(i = 0; i<JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:attribute = 0x%lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].attribute);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:size = 0x%lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].size);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:offset = 0x%lx\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].offset);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:image1_no = 0x%x\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].image1_no);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:image2_no = 0x%x\n", i, _stMPOIndex[eNJPDNum].mp_entry[i].image2_no);

        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:MPIndividualNum = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].MPIndividualNum);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PanOrientation = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PanOrientation);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PanOverlap_H.numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PanOverlap_H.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PanOverlap_H.denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PanOverlap_H.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PanOverlap_V.numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PanOverlap_V.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PanOverlap_V.denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PanOverlap_V.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:BaseViewpointNum = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].BaseViewpointNum);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:ConvergenceAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].ConvergenceAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:ConvergenceAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].ConvergenceAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:BaselineLength.numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].BaselineLength.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:BaselineLength.denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].BaselineLength.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:VerticalDivergence.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].VerticalDivergence.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:VerticalDivergence.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].VerticalDivergence.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_X.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_X.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_X.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_X.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_Y.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_Y.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_Y.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_Y.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_Z.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_Z.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:AxisDistance_Z.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].AxisDistance_Z.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:YawAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].YawAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:YawAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].YawAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PitchAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PitchAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:PitchAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].PitchAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:RollAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].RollAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:RollAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[eNJPDNum][i].RollAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[eNJPDNum][%ld]:used = 0x%d\n", i, _stMPOAttri[eNJPDNum][i].used);
    }
    return;
}

MS_BOOL _MApi_JPEG_SetMPOBuffer(MS_U32 read_offset, MS_U32 output_start)
{
    _USE_OJPD(MApi_JPEG_SetMPOBuffer, read_offset, output_start);
    read_offset = MS_PA2KSEG1((MS_U32)read_offset);// | AEON_NON_CACHE_MASK;
    output_start = MS_PA2KSEG1((MS_U32)output_start);// | AEON_NON_CACHE_MASK;

    if((read_offset < MRC_BUFFER_ADDR)
    || (read_offset > (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE)))
    {
        JPEG_DEBUG_API_MSG("JPEG_SetMPOBuffer, read_offset invalid\n");
        return FALSE;
    }

    if(read_offset > (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE/2))
    {
        mpo_load_data[eNJPDNum] = FALSE;
    }

#ifndef MSOS_TYPE_LINUX_KERNEL
    if(output_start < MWC_BUFFER_ADDR)
    {
        JPEG_DEBUG_API_MSG("JPEG_SetMPOBuffer, output_start invalid\n");
        return FALSE;
    }
#endif

    _pu8In_buf_MPO_ofs[eNJPDNum] = (MS_U8*)read_offset;
    _u32In_buf_MPO_left[eNJPDNum] = (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE) - read_offset;
    JPEG_DEBUG_API_MSG("After MApi_JPEG_SetMPOBuffer(), _pu8In_buf_MPO_ofs[eNJPDNum]=0x%lx, _u32In_buf_MPO_left[eNJPDNum]=0x%lx=============\n", (MS_U32)_pu8In_buf_MPO_ofs[eNJPDNum], (MS_U32)_u32In_buf_MPO_left[eNJPDNum]);
#ifndef MSOS_TYPE_LINUX_KERNEL
    MWC_BUFFER_ADDR = output_start;
#endif
    return TRUE;
}

/********************************************************************/
/// Set Max decoding resolution for MPO
///@param -u16Width \b IN : indicate max decoding width
///@param -u16Height \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMPOMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
    _USE_OJPD(MApi_JPEG_SetMPOMaxDecodeResolution, u16Width, u16Height);
    JPEG_DEBUG_API_MSG("%s: Width = %d, Height = %d\n", __FUNCTION__, u16Width, u16Height);
    JPEG_MPO_MAX_WIDTH[eNJPDNum] = u16Width;
    JPEG_MPO_MAX_HEIGHT[eNJPDNum] = u16Height;
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution of Progressive MPO
///@param -u16ProWidth \b IN : indicate max decoding width
///@param -u16ProHeight \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMPOProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
    _USE_OJPD(MApi_JPEG_SetMPOProMaxDecodeResolution, u16ProWidth, u16ProHeight);
    JPEG_DEBUG_API_MSG("%s: ProWidth = %d, ProHeight = %d\n", __FUNCTION__, u16ProWidth, u16ProHeight);
    JPEG_MPO_PRO_MAX_WIDTH[eNJPDNum] = u16ProWidth;
    JPEG_MPO_PRO_MAX_HEIGHT[eNJPDNum] = u16ProHeight;
    return;
}

#endif

#ifdef VDEC_UTOPIA_2K
JPEG_Result MApi_JPEG_GetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//user
{
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetCbEvent, (void*)(CbEventParam)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return E_JPEG_OKAY;
}

JPEG_Result MApi_JPEG_SetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//user
{
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetCbEvent, (void*)(CbEventParam)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return E_JPEG_OKAY;
}
#endif

JPEG_Result _MApi_JPEG_GetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//kernel
{
#ifndef MSOS_TYPE_LINUX_KERNEL
    UNUSED(CbEventParam);
    return E_JPEG_FAILED;
#else
    if(CbEventParam->type == JPEG_CB_EVENT_FILL_HDR)
    {
        if(s32FillHdrIdx!=-1)
        {
            CbEventParam->param = s32FillHdrIdx;
            CbEventParam->BufAddr = FillHdrPhyAddr[CbEventParam->param];
            CbEventParam->BufLength = u32FillHdrLength[CbEventParam->param];
        }
        else
        {
            CbEventParam->param = -1;
            CbEventParam->BufAddr = 0;
            CbEventParam->BufLength = 0;
        }
    }
    else if(CbEventParam->type == JPEG_CB_EVENT_ISR)
    {
        CbEventParam->param = JPEG_Get_ISR();
    }
    return E_JPEG_OKAY;
#endif
}

JPEG_Result _MApi_JPEG_SetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//kernel
{
#ifndef MSOS_TYPE_LINUX_KERNEL
    UNUSED(CbEventParam);
    return E_JPEG_FAILED;
#else
    if(CbEventParam->type == JPEG_CB_EVENT_FILL_HDR)
    {
        if(s32FillHdrIdx!=-1)
        {
            s32FillHdrIdx = -1;
            s32FillHdrRes = CbEventParam->param;
        }
    }
    else if(CbEventParam->type == JPEG_CB_EVENT_ISR)
    {
        JPEG_Set_ISR(FALSE);
    }
    return E_JPEG_OKAY;
#endif
}

#ifdef VDEC_UTOPIA_2K
void _MApi_JPEG_Callback_Proc(void)//user space
{
    while(bJpegCallbackTaskRunning)
    {
        JPEG_CB_EVENT_PARAM CbEventParam;

//////////////////////////////////////////////////////////////////////////
// polling fill_header, for feeding data use
        memset(&CbEventParam,0,sizeof(JPEG_CB_EVENT_PARAM));
        CbEventParam.type = JPEG_CB_EVENT_FILL_HDR;
        MApi_JPEG_GetEvent(&CbEventParam);

        if(CbEventParam.param != -1)//get event
        {
            //invoke callback
            if(_pFillHdrFunc[CbEventParam.param])
                CbEventParam.param = _pFillHdrFunc[CbEventParam.param](CbEventParam.BufAddr, CbEventParam.BufLength);
            MApi_JPEG_SetEvent(&CbEventParam);
        }

//////////////////////////////////////////////////////////////////////////
// IsrCb, for ISR use (independent with fill header function)
        memset(&CbEventParam,0,sizeof(JPEG_CB_EVENT_PARAM));
        CbEventParam.type = JPEG_CB_EVENT_ISR;
        MApi_JPEG_GetEvent(&CbEventParam);

        if(CbEventParam.param > 0)
        {
            //invoke callback
            gIsrCB();
            MApi_JPEG_SetEvent(FALSE);
        }
//////////////////////////////////////////////////////////////////////////

        MsOS_DelayTask(100);
    }
}



MS_BOOL _MApi_JPEG_Create_Callback_task(void)
{
    // Create callback task in user space
    JPEG_DEBUG_API_MSG("_MApi_JPEG_Create_Callback_task\n");

    bJpegCallbackTaskRunning = TRUE;
    if(_s32JPEG_Callback_TaskId == -1)
    {
        MS_U32 u32Id = 0;
        _s32JPEG_Callback_TaskId = MsOS_CreateTask((TaskEntry)_MApi_JPEG_Callback_Proc,
                                              u32Id,
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)u8JpegCallbackStack,
                                              JPEG_STACK_SIZE,
                                              "MAPI_JPEG_Callback_Task");

        if (_s32JPEG_Callback_TaskId < 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void _MApi_JPEG_Delete_Callback_Task(void)
{
    if(_s32JPEG_Callback_TaskId != -1 )
    {
        bJpegCallbackTaskRunning = FALSE;
        MsOS_DeleteTask(_s32JPEG_Callback_TaskId);
        _s32JPEG_Callback_TaskId = -1;
    }
}
#endif


void _MApi_JPEG_SetVerificationMode(NJPEG_VerificationMode VerificationMode)
{
    MDrv_NJPD_SetVerificationMode((NJPD_VerificationMode)VerificationMode);
}
NJPEG_VerificationMode _MApi_JPEG_GetVerificationMode(void)
{
    return (NJPEG_VerificationMode)MDrv_NJPD_GetVerificationMode();
}

void _MApi_NJPD_Debug(void)
{
    MDrv_NJPD_Debug();

}

#else

JPEG_STATIC JPEG_FillHdrFunc _pFillHdrFunc[E_NJPD_NJPD_TOTAL];
JPEG_STATIC JPD_Number eNJPDNum = E_NJPD_NJPD1;
static MS_BOOL bOJPDOn = FALSE;

#ifdef VDEC_UTOPIA_2K
#ifndef JPEG_STACK_SIZE
#define JPEG_STACK_SIZE 4096
#endif
MS_S32 _s32JPEG_Callback_TaskId = -1;
MS_U8   u8JpegCallbackStack[JPEG_STACK_SIZE];
MS_BOOL bJpegCallbackTaskRunning = FALSE;

JPEG_IsrFuncCb gIsrCB;

MS_BOOL _MApi_JPEG_Create_Callback_task(void);
MS_BOOL _MApi_JPEG_Delete_Callback_task(void);
void _MApi_JPEG_Callback_Proc(void);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_S32 s32FillHdrIdx = -1;
MS_S32 s32FillHdrRes = 0;
MS_PHYADDR FillHdrPhyAddr[E_NJPD_NJPD_TOTAL];
MS_U32 u32FillHdrLength[E_NJPD_NJPD_TOTAL];
MS_BOOL JPEG_Get_ISR(void);
void JPEG_Set_ISR(MS_BOOL val);
MS_S32 JPEG_FillHdrFunc_KernlDummy(MS_PHYADDR BufAddr, MS_U32 BufLength)
{
    return 0;
}
#endif

MS_U32 JPEG_FillHeaderFunction(MS_PHYADDR mFillHdrPhyAddr,MS_U32 mu32FillHdrLength)//kernel
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    FillHdrPhyAddr[eNJPDNum] = mFillHdrPhyAddr;
    u32FillHdrLength[eNJPDNum] = mu32FillHdrLength;
    s32FillHdrIdx = eNJPDNum;
    int timeout = 1000;
    while(s32FillHdrIdx != -1)
    {
        timeout--;
        MsOS_DelayTask(1);
        if(timeout<=0)
        {
            return -1;
        }
    }
    return s32FillHdrRes;
#else
    return _pFillHdrFunc[eNJPDNum](mFillHdrPhyAddr,mu32FillHdrLength);
#endif
}

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL JPEG_Get_ISR()//kernel
{
    return MDrv_NJPD_Get_ISR();
}

void JPEG_Set_ISR(MS_BOOL val)
{
    MDrv_NJPD_Set_ISR(val);
}
#endif

JPEG_STATIC MS_U8 _u8JPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;


#ifdef VDEC_UTOPIA_2K
JPEG_Result MApi_JPEG_GetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//user
{
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetCbEvent, (void*)(CbEventParam)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return E_JPEG_OKAY;
}

JPEG_Result MApi_JPEG_SetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//user
{
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetCbEvent, (void*)(CbEventParam)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return E_JPEG_OKAY;
}
#endif

JPEG_Result _MApi_JPEG_GetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//kernel
{
#ifndef MSOS_TYPE_LINUX_KERNEL
    UNUSED(CbEventParam);
    return E_JPEG_FAILED;
#else
    if(CbEventParam->type == JPEG_CB_EVENT_FILL_HDR)
    {
        if(s32FillHdrIdx!=-1)
        {
            CbEventParam->param = s32FillHdrIdx;
            CbEventParam->BufAddr = FillHdrPhyAddr[CbEventParam->param];
            CbEventParam->BufLength = u32FillHdrLength[CbEventParam->param];
        }
        else
        {
            CbEventParam->param = -1;
            CbEventParam->BufAddr = 0;
            CbEventParam->BufLength = 0;
        }
    }
    else if(CbEventParam->type == JPEG_CB_EVENT_ISR)
    {
        CbEventParam->param = JPEG_Get_ISR();
    }
    return E_JPEG_OKAY;
#endif
}

JPEG_Result _MApi_JPEG_SetEvent(JPEG_CB_EVENT_PARAM* CbEventParam)//kernel
{
#ifndef MSOS_TYPE_LINUX_KERNEL
    UNUSED(CbEventParam);
    return E_JPEG_FAILED;
#else
    if(CbEventParam->type == JPEG_CB_EVENT_FILL_HDR)
    {
        if(s32FillHdrIdx!=-1)
        {
            s32FillHdrIdx = -1;
            s32FillHdrRes = CbEventParam->param;
        }
    }
    else if(CbEventParam->type == JPEG_CB_EVENT_ISR)
    {
        JPEG_Set_ISR(FALSE);
    }
    return E_JPEG_OKAY;
#endif
}

#ifdef VDEC_UTOPIA_2K
void _MApi_JPEG_Callback_Proc(void)//user space
{
    while(bJpegCallbackTaskRunning)
    {
        JPEG_CB_EVENT_PARAM CbEventParam;

//////////////////////////////////////////////////////////////////////////
// polling fill_header, for feeding data use
        memset(&CbEventParam,0,sizeof(JPEG_CB_EVENT_PARAM));
        CbEventParam.type = JPEG_CB_EVENT_FILL_HDR;
        MApi_JPEG_GetEvent(&CbEventParam);

        if(CbEventParam.param != -1)//get event
        {
            //invoke callback
            if(_pFillHdrFunc[CbEventParam.param])
                CbEventParam.param = _pFillHdrFunc[CbEventParam.param](CbEventParam.BufAddr, CbEventParam.BufLength);
            MApi_JPEG_SetEvent(&CbEventParam);
        }

//////////////////////////////////////////////////////////////////////////
// IsrCb, for ISR use (independent with fill header function)
        memset(&CbEventParam,0,sizeof(JPEG_CB_EVENT_PARAM));
        CbEventParam.type = JPEG_CB_EVENT_ISR;
        MApi_JPEG_GetEvent(&CbEventParam);

        if(CbEventParam.param > 0)
        {
            //invoke callback
            gIsrCB();
            MApi_JPEG_SetEvent(FALSE);
        }
//////////////////////////////////////////////////////////////////////////

        MsOS_DelayTask(100);
    }
}



MS_BOOL _MApi_JPEG_Create_Callback_task(void)
{
    // Create callback task in user space
    JPEG_DEBUG_API_MSG("_MApi_JPEG_Create_Callback_task\n");

    bJpegCallbackTaskRunning = TRUE;
    if(_s32JPEG_Callback_TaskId == -1)
    {
        MS_U32 u32Id = 0;
        _s32JPEG_Callback_TaskId = MsOS_CreateTask((TaskEntry)_MApi_JPEG_Callback_Proc,
                                              u32Id,
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)u8JpegCallbackStack,
                                              JPEG_STACK_SIZE,
                                              "MAPI_JPEG_Callback_Task");

        if (_s32JPEG_Callback_TaskId < 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void _MApi_JPEG_Delete_Callback_Task(void)
{
    if(_s32JPEG_Callback_TaskId != -1 )
    {
        bJpegCallbackTaskRunning = FALSE;
        MsOS_DeleteTask(_s32JPEG_Callback_TaskId);
        _s32JPEG_Callback_TaskId = -1;
    }
}
#endif

#if 1
JPEG_Result _MApi_JPEG_Init(JPEG_InitParam *pInitParam)
{
    UNUSED(pInitParam);
    return E_JPEG_FAILED;
}
JPEG_Result _MApi_JPEG_DecodeHdr(void)
{
    return E_JPEG_FAILED;
}
JPEG_Result _MApi_JPEG_Decode(void)
{
    return E_JPEG_FAILED;
}
void _MApi_JPEG_Exit(void)
{
    return;
}
JPEG_ErrCode _MApi_JPEG_GetErrorCode(void)
{
    return E_JPEG_NO_ERROR;
}
JPEG_Event _MApi_JPEG_GetJPDEventFlag(void)
{
    return E_JPEG_EVENT_DEC_NONE;
}
void _MApi_JPEG_Rst(void)
{
    return;
}

MS_U16 _MApi_JPEG_GetCurVidx(void)
{
    return 0;
}
MS_U16 _MApi_JPEG_GetWidth(void)
{
    return 0;
}
MS_U16 _MApi_JPEG_GetHeight(void)
{
    return 0;
}

void _MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
    UNUSED(u16Width);
    UNUSED(u16Height);
    return;
}
void _MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
    UNUSED(u8DbgLevel);
    return;
}
MS_U16 _MApi_JPEG_GetAlignedPitch(void)
{
    return 0;
}
void _MApi_JPEG_SetNJPDInstance(MS_U8 JPDNum)
{
    UNUSED(JPDNum);
    return;
}

#endif
#endif

MS_BOOL MApi_JPEG_IsNJPD(void)
{
    return TRUE;
}

//=====================================================================

void MApi_JPEG_EnableOJPD(MS_BOOL bOnOff)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL && bOnOff==TRUE)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD); //coverity [90015] var_deref_model: Passing null pointer "pInstantNJPD" to function "UtopiaClose(void *)", which dereferences it.
        return;
    }

    JPEG_BOOL_PARAM bPARA;
    bPARA.bValue = bOnOff;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_EnableOJPD\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_EnableOJPD, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
    else if(bOnOff==FALSE)
    {
        UTOPIA_20_DBG("%s(): UtopiaClose()\n", __FUNCTION__);
        if(UtopiaClose(pInstantNJPD) != UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Utopia Close NJPD failed\n");
            return;
        }
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_EnableOJPD(bOnOff);
#endif
}
JPEG_Result MApi_JPEG_Init_UsingOJPD(JPEG_InitParam *pInitParam)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return E_JPEG_FAILED;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return E_JPEG_FAILED;
    }
#ifdef VDEC_UTOPIA_2K
    _pFillHdrFunc[eNJPDNum] = pInitParam->pFillHdrFunc;
    _MApi_JPEG_Create_Callback_task();
#endif


    JPEG_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(JPEG_INIT_PARAM));
    InitPara.pInitParam = pInitParam;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Init_UsingOJPD\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Init_UsingOJPD, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
        return E_JPEG_FAILED;
    }
    return InitPara.retVal;
#else
    return _MApi_JPEG_Init_UsingOJPD(pInitParam);
#endif
}

void msAPI_JPEG_get_APP0_info( MS_U8 *unit,MS_U16 *x, MS_U16 *y )
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETAPP0INFO_PARAM App0Info;
    memset(&App0Info , 0 ,sizeof(JPEG_GETAPP0INFO_PARAM));
    App0Info.unit= unit;
    App0Info.x= x;
    App0Info.y= y;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_get_APP0_info\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_get_APP0_info, (void*)(&App0Info)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _msAPI_JPEG_get_APP0_info(unit, x, y );
#endif
}

JPEG_Result MApi_JPEG_Init(JPEG_InitParam *pInitParam)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return E_JPEG_FAILED;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return E_JPEG_FAILED;
    }
#ifdef VDEC_UTOPIA_2K
    // _pFillHdrFunc() in user space, create callback task in user space
    _pFillHdrFunc[eNJPDNum] = pInitParam->pFillHdrFunc;
    _MApi_JPEG_Create_Callback_task();
#endif
    JPEG_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(JPEG_INIT_PARAM));
    InitPara.pInitParam = pInitParam;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Init\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Init, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        InitPara.retVal= E_JPEG_FAILED;
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
    return InitPara.retVal;
#else
    return _MApi_JPEG_Init(pInitParam);
#endif
}

JPEG_Result MApi_JPEG_DecodeHdr(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DecodeHdr\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_DecodeHdr, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_DecodeHdr();
#endif
}

JPEG_Result MApi_JPEG_Decode(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Decode\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Decode, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_Decode();
#endif
}

void MApi_JPEG_Exit(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
#ifdef VDEC_UTOPIA_2K
    _MApi_JPEG_Delete_Callback_Task();
#endif
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Exit\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Exit, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    else
    {
        if(bOJPDOn==FALSE)
        {
            UTOPIA_20_DBG("%s(): UtopiaClose()\n", __FUNCTION__);
            if(UtopiaClose(pInstantNJPD) != UTOPIA_STATUS_SUCCESS)
            {
                JPEG_DEBUG_API_ERR("Utopia Close NJPD failed\n");
                return;
            }
            pInstantNJPD = NULL;
        }
    }

#else
    _MApi_JPEG_Exit();
#endif
}

JPEG_ErrCode MApi_JPEG_GetErrorCode(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_NOT_INIT;
    }

    JPEG_GETERRORCODE_PARAM GetErrorCode;
    memset(&GetErrorCode , 0 ,sizeof(JPEG_GETERRORCODE_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetErrorCode\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetErrorCode, (void*)(&GetErrorCode)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_NOT_INIT;
    }
    return GetErrorCode.ErrCode;
#else
    return _MApi_JPEG_GetErrorCode();
#endif
}

JPEG_Event MApi_JPEG_GetJPDEventFlag(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_JPEGEVENT_PARAM JpegEvent;
    memset(&JpegEvent , 0 ,sizeof(JPEG_JPEGEVENT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetJPDEventFlag\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetJPDEventFlag, (void*)(&JpegEvent)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return JpegEvent.eEvtVal;
#else
    return _MApi_JPEG_GetJPDEventFlag();
#endif
}

void MApi_JPEG_SetJPDEventFlag(JPEG_Event eEvtVal)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_JPEGEVENT_PARAM JpegEvent;
    memset(&JpegEvent , 0 ,sizeof(JPEG_JPEGEVENT_PARAM));
    JpegEvent.eEvtVal = eEvtVal;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetJPDEventFlag\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetJPDEventFlag, (void*)(&JpegEvent)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetJPDEventFlag(eEvtVal);
#endif
}

void MApi_JPEG_Rst(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Rst\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Rst, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_Rst();
#endif
}

void MApi_JPEG_PowerOn(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_PowerOn\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_PowerOn, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_PowerOn();
#endif
}

void MApi_JPEG_PowerOff(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_PowerOff\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_PowerOff, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_PowerOff();
#endif
}

MS_U16 MApi_JPEG_GetCurVidx(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetCurVidx\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetCurVidx, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetCurVidx();
#endif
}

MS_BOOL MApi_JPEG_IsProgressive(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_IsProgressive\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_IsProgressive, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_IsProgressive();
#endif
}

MS_BOOL MApi_JPEG_ThumbnailFound(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_ThumbnailFound\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_ThumbnailFound, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_ThumbnailFound();
#endif
}

MS_U16 MApi_JPEG_GetWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetWidth\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetWidth();
#endif
}

MS_U16 MApi_JPEG_GetHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetHeight\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetHeight();
#endif
}

MS_U16 MApi_JPEG_GetOriginalWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetOriginalWidth\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetOriginalWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetOriginalWidth();
#endif
}

MS_U16 MApi_JPEG_GetOriginalHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetOriginalHeight\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetOriginalHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetOriginalHeight();
#endif
}

MS_U16 MApi_JPEG_GetNonAlignmentWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetNonAlignmentWidth\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetNonAlignmentWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetNonAlignmentWidth();
#endif
}

MS_U16 MApi_JPEG_GetNonAlignmentHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetNonAlignmentHeight\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetNonAlignmentHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetWidth();
#endif
}

MS_U16 MApi_JPEG_GetAlignedPitch(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedPitch\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetAlignedPitch, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedPitch();
#endif
}

MS_U16 MApi_JPEG_GetAlignedPitch_H(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedPitch_H\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetAlignedPitch_H, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedPitch_H();
#endif
}

MS_U16 MApi_JPEG_GetAlignedWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedWidth\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetAlignedWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedWidth();
#endif
}

MS_U16 MApi_JPEG_GetAlignedHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedHeight\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetAlignedHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedHeight();
#endif
}

MS_U8 MApi_JPEG_GetScaleDownFactor(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetScaleDownFactor\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetScaleDownFactor, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U8PARA.u8Value;
#else
    return _MApi_JPEG_GetScaleDownFactor();
#endif
}

void MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16Width;
    Resol.u16Height= u16Height;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetMaxDecodeResolution(u16Width, u16Height);
#endif
}

void MApi_JPEG_SetProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16ProWidth;
    Resol.u16Height= u16ProHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetProMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetProMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetProMaxDecodeResolution(u16ProWidth, u16ProHeight);
#endif
}

void MApi_JPEG_SetMRBufferValid(JPEG_BuffLoadType u8MRBuffType)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_SETMBUFFERVALID_PARAM MBuffValid;
    memset(&MBuffValid , 0 ,sizeof(JPEG_SETMBUFFERVALID_PARAM));
    MBuffValid.u8MRBuffType = u8MRBuffType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMRBufferValid\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMRBufferValid, (void*)(&MBuffValid)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetMRBufferValid(u8MRBuffType);
#endif
}

void MApi_JPEG_UpdateReadInfo(MS_U32 u32DataRead, MS_BOOL bEOFflag)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_UPDATEREADINFO_PARAM ReadInfo;
    memset(&ReadInfo , 0 ,sizeof(JPEG_UPDATEREADINFO_PARAM));
    ReadInfo.u32DataRead = u32DataRead;
    ReadInfo.bEOFflag = bEOFflag;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_UpdateReadInfo\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_UpdateReadInfo, (void*)(&ReadInfo)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_UpdateReadInfo(u32DataRead, bEOFflag);
#endif
}

void MApi_JPEG_ProcessEOF(JPEG_BuffLoadType u8MRBuffType)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_PROCESSEOF_PARAM EofPara;
    memset(&EofPara , 0 ,sizeof(JPEG_PROCESSEOF_PARAM));
    EofPara.u8MRBuffType = u8MRBuffType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_ProcessEOF\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_ProcessEOF, (void*)(&EofPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_ProcessEOF(u8MRBuffType);
#endif
}

void MApi_JPEG_SetErrCode(JPEG_ErrCode ErrStatus)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_SETERRORCODE_PARAM ErrCode;
    memset(&ErrCode , 0 ,sizeof(JPEG_SETERRORCODE_PARAM));
    ErrCode.ErrStatus = ErrStatus;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetErrCode\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetErrCode, (void*)(&ErrCode)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetErrCode(ErrStatus);
#endif
}

void MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
#if (JPEG_UTOPIA20)
    _u8JPEG_ApiDbgLevel = u8DbgLevel;
    JPEG_DEBUG_API_MSG("%s: DbgLevel = 0x%04X\n", __FUNCTION__, u8DbgLevel);
    JPEG_SetDbgLevel_V2(u8DbgLevel);
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    U8PARA.u8Value = u8DbgLevel;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetDbgLevel\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetDbgLevel, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetDbgLevel(u8DbgLevel);
#endif
}

MS_U8 MApi_JPEG_GetDbgLevel(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetDbgLevel\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetDbgLevel, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U8PARA.u8Value;
#else
    return _MApi_JPEG_GetDbgLevel();
#endif
}

void MApi_JPEG_GetInfo(JPEG_Info *pJPEG_Info)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETINFO_PARAM eGETINFO;
    memset(&eGETINFO , 0 ,sizeof(JPEG_GETINFO_PARAM));
    eGETINFO.pJPEG_Info = pJPEG_Info;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetInfo\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetInfo, (void*)(&eGETINFO)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_GetInfo(pJPEG_Info);
#endif
}

void MApi_JPEG_GetStatus(JPEG_Status *pJPEG_Status)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETSTATUS_PARAM eGETSTATUS;
    memset(&eGETSTATUS , 0 ,sizeof(JPEG_GETSTATUS_PARAM));
    eGETSTATUS.pJPEG_Status = pJPEG_Status;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetStatus\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetStatus, (void*)(&eGETSTATUS)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_GetStatus(pJPEG_Status);
#endif
}

JPEG_Result MApi_JPEG_GetLibVer(const MSIF_Version **ppVersion)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETLIBVER_PARAM eLIBVER;
    memset(&eLIBVER , 0 ,sizeof(JPEG_GETLIBVER_PARAM));
    eLIBVER.ppVersion = ppVersion;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetLibVer\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetLibVer, (void*)(&eLIBVER)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eLIBVER.retVal;
#else
    return _MApi_JPEG_GetLibVer(ppVersion);
#endif
}

JPEG_Result MApi_JPEG_HdlVidxChk(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_HdlVidxChk\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_HdlVidxChk, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_HdlVidxChk();
#endif
}

JPEG_Result MApi_JPEG_GetBuffLoadType(JPEG_BuffLoadType *pBuffLoadType)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETBUFFLOADTYPE_PARAM eBufferLoad;
    memset(&eBufferLoad , 0 ,sizeof(JPEG_GETBUFFLOADTYPE_PARAM));
    eBufferLoad.pBuffLoadType = pBuffLoadType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetBuffLoadType\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetBuffLoadType, (void*)(&eBufferLoad)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eBufferLoad.retVal;
#else
    return _MApi_JPEG_GetBuffLoadType(pBuffLoadType);
#endif
}

JPEG_Result MApi_JPEG_EnableISR(JPEG_IsrFuncCb IsrCb)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_ENABLEISR_PARAM eISR;
    memset(&eISR , 0 ,sizeof(JPEG_ENABLEISR_PARAM));
    eISR.IsrCb = IsrCb;
#ifdef VDEC_UTOPIA_2K
    gIsrCB = IsrCb;
#endif

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_EnableISR\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_EnableISR, (void*)(&eISR)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eISR.retVal;
#else
    return _MApi_JPEG_EnableISR(IsrCb);
#endif
}

JPEG_Result MApi_JPEG_DisableISR(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DisableISR\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_DisableISR, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#ifdef VDEC_UTOPIA_2K
    gIsrCB = NULL;
#endif
    return ResPara.retVal;
#else
    return _MApi_JPEG_DisableISR();
#endif
}

JPEG_DecodeStatus MApi_JPEG_WaitDone(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_DEC_DECODING;
    }

    JPEG_WAITDONE_PARAM eWaitDone;
    memset(&eWaitDone , 0 ,sizeof(JPEG_WAITDONE_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_WaitDone\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_WaitDone, (void*)(&eWaitDone)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eWaitDone.retVal;
#else
    return _MApi_JPEG_WaitDone();
#endif
}

JPEG_Result MApi_JPEG_GetEXIFDateTime(JPEG_EXIF_DateTime *DateTime)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFDATETIME_PARAM eEXIFDate;
    memset(&eEXIFDate , 0 ,sizeof(JPEG_GETEXIFDATETIME_PARAM));
    eEXIFDate.DateTime = DateTime;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFDateTime\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFDateTime, (void*)(&eEXIFDate)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFDate.retVal;
#else
    return _MApi_JPEG_GetEXIFDateTime(DateTime);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFOrientation(JPEG_EXIF_Orientation *eOrientation)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFORIENTATION_PARAM eEXIFOri;
    memset(&eEXIFOri , 0 ,sizeof(JPEG_GETEXIFORIENTATION_PARAM));
    eEXIFOri.eOrientation = eOrientation;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFOrientation\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFOrientation, (void*)(&eEXIFOri)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFOri.retVal;
#else
    return _MApi_JPEG_GetEXIFOrientation(eOrientation);
#endif
}

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_Result MApi_JPEG_GetEXIFManufacturer(MS_U8 *pu8Manufacturer, MS_U8 u8size)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFSTRING_PARAM eEXIFStr;
    memset(&eEXIFStr , 0 ,sizeof(JPEG_GETEXIFSTRING_PARAM));
    eEXIFStr.pu8String = pu8Manufacturer;
    eEXIFStr.u8size = u8size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFManufacturer\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFManufacturer, (void*)(&eEXIFStr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFStr.retVal;
#else
    return _MApi_JPEG_GetEXIFManufacturer(pu8Manufacturer, u8size);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFModel(MS_U8 *pu8Model, MS_U8 u8size)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFSTRING_PARAM eEXIFStr;
    memset(&eEXIFStr , 0 ,sizeof(JPEG_GETEXIFSTRING_PARAM));
    eEXIFStr.pu8String = pu8Model;
    eEXIFStr.u8size = u8size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFModel\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFModel, (void*)(&eEXIFStr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFStr.retVal;
#else
    return _MApi_JPEG_GetEXIFModel(pu8Model, u8size);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFlash(MS_U16 *pu16Flash)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_GETEXIFU16_PARAM));
    U16PARA.pu16Value = pu16Flash;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFlash\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFFlash, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U16PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFlash(pu16Flash);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFISOSpeedRatings(MS_U32 *pu32ISOSpeedRatings)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ISOSpeedRatings;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFISOSpeedRatings\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFISOSpeedRatings, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFISOSpeedRatings(pu32ISOSpeedRatings);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFShutterSpeedValue(JPEG_RATIONAL *pShutterSpeedValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pShutterSpeedValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFShutterSpeedValue\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFShutterSpeedValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFShutterSpeedValue(pShutterSpeedValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFApertureValue(JPEG_RATIONAL *pApertureValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pApertureValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFApertureValue\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFApertureValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFApertureValue(pApertureValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFExposureBiasValue(JPEG_RATIONAL *pExposureBiasValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pExposureBiasValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFExposureBiasValue\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFExposureBiasValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFExposureBiasValue(pExposureBiasValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFocalLength(JPEG_RATIONAL *pFocalLength)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pFocalLength;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFocalLength\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFFocalLength, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFocalLength(pFocalLength);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFImageWidth(MS_U32 *pu32ImageWidth)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ImageWidth;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFImageWidth\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFImageWidth, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFImageWidth(pu32ImageWidth);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFImageHeight(MS_U32 *pu32ImageHeight)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ImageHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFImageHeight\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFImageHeight, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFImageHeight(pu32ImageHeight);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFExposureTime(JPEG_RATIONAL *pExposureTime)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pExposureTime;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFExposureTime\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFExposureTime, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFExposureTime(pExposureTime);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFNumber(JPEG_RATIONAL *pFNumber)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pFNumber;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFNumber\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetEXIFFNumber, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFNumber(pFNumber);
#endif
}

#endif

MS_PHYADDR MApi_JPEG_GetFreeMemory(MS_U32 size)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETFREEMEMORY_PARAM MEMORYPARA;
    memset(&MEMORYPARA , 0 ,sizeof(JPEG_GETFREEMEMORY_PARAM));
    MEMORYPARA.size = size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetFreeMemory\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetFreeMemory, (void*)(&MEMORYPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return MEMORYPARA.addr;
#else
    return _MApi_JPEG_GetFreeMemory(size);
#endif
}


MS_U32 MApi_JPEG_GetDataOffset(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_U32_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetDataOffset\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetDataOffset, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U32PARA.u32Value;
#else
    return _MApi_JPEG_GetDataOffset();
#endif
}

MS_U32 MApi_JPEG_GetSOFOffset(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_U32_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetSOFOffset\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetSOFOffset, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U32PARA.u32Value;
#else
    return _MApi_JPEG_GetSOFOffset();
#endif
}

void MApi_JPEG_SetNJPDInstance(MS_U8 JPDNum)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }
#ifdef VDEC_UTOPIA_2K
    eNJPDNum = (JPD_Number)JPDNum;
#endif
    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetNJPDInstance\n");
    U8PARA.u8Value = JPDNum;
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetNJPDInstance, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
        return;
    }
    return;
#else
    _MApi_JPEG_SetNJPDInstance(JPDNum);
#endif
}

JPEG_Result MApi_JPEG_SupportCMYK(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return E_JPEG_FAILED;
        }
    }

    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return E_JPEG_FAILED;
    }

    JPEG_SUPPORTCMYK_PARAM CMYKRGB;
    memset(&CMYKRGB , 0 ,sizeof(JPEG_SUPPORTCMYK_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SupportCMYK\n");
    CMYKRGB.bEnable = bEnable;
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SupportCMYK, (void*)(&CMYKRGB)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
        return E_JPEG_FAILED;
    }
    return CMYKRGB.retVal;
#else
    return _MApi_JPEG_SupportCMYK(bEnable);
#endif
}

JPEG_Result MApi_JPEG_SupportRGB(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return E_JPEG_FAILED;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return E_JPEG_FAILED;
    }

    JPEG_SUPPORTCMYK_PARAM CMYKRGB;
    memset(&CMYKRGB , 0 ,sizeof(JPEG_SUPPORTCMYK_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SupportRGB\n");
    CMYKRGB.bEnable = bEnable;
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SupportRGB, (void*)(&CMYKRGB)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
        return E_JPEG_FAILED;
    }
    return CMYKRGB.retVal;
#else
    return _MApi_JPEG_SupportRGB(bEnable);
#endif
}

void MApi_JPEG_SetMHEG5(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    bPARA.bValue= bEnable;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMHEG5\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMHEG5, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    _MApi_JPEG_SetMHEG5(bEnable);
#endif
}


#if SUPPORT_MPO_FORMAT
//For MPO
MS_BOOL MApi_JPEG_IsMPOFormat(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_IsMPOFormat\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_IsMPOFormat, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_IsMPOFormat();
#endif
}


MS_BOOL MApi_JPEG_GetMPOIndex(JPEG_MPO_INDEX_INFO **ppMPOIndex)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETMPOINDEX_PARAM eMPOIndex;
    memset(&eMPOIndex , 0 ,sizeof(JPEG_GETMPOINDEX_PARAM));
    eMPOIndex.ppMPOIndex = ppMPOIndex;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetMPOIndex\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetMPOIndex, (void*)(&eMPOIndex)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOIndex.bretVal;
#else
    return _MApi_JPEG_GetMPOIndex(ppMPOIndex);
#endif
}

MS_BOOL MApi_JPEG_GetMPOAttr(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETMPOATTR_PARAM eMPOAttr;
    memset(&eMPOAttr , 0 ,sizeof(JPEG_GETMPOATTR_PARAM));
    eMPOAttr.image_no = image_no;
    eMPOAttr.ppMPOAttr = ppMPOAttr;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetMPOAttr\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetMPOAttr, (void*)(&eMPOAttr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOAttr.bretVal;
#else
    return _MApi_JPEG_GetMPOAttr(image_no, ppMPOAttr);
#endif
}

void MApi_JPEG_DumpMPO(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DumpMPO\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_DumpMPO, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_DumpMPO();
#endif
}

MS_BOOL MApi_JPEG_SetMPOBuffer(MS_U32 read_offset, MS_U32 output_start)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_SETMPOBUFFER_PARAM eMPOBuff;
    memset(&eMPOBuff , 0 ,sizeof(JPEG_SETMPOBUFFER_PARAM));
    eMPOBuff.read_offset = read_offset;
    eMPOBuff.output_start = output_start;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOBuffer\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMPOBuffer, (void*)(&eMPOBuff)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOBuff.bretVal;
#else
    return _MApi_JPEG_SetMPOBuffer(read_offset, output_start);
#endif
}

void MApi_JPEG_SetMPOMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16Width;
    Resol.u16Height= u16Height;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMPOMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetMPOMaxDecodeResolution(u16Width, u16Height);
#endif
}

void MApi_JPEG_SetMPOProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16ProWidth;
    Resol.u16Height= u16ProHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOProMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetMPOProMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetMPOProMaxDecodeResolution(u16ProWidth, u16ProHeight);
#endif
}

#endif

void MApi_JPEG_SetVerificationMode(NJPEG_VerificationMode VerificationMode)
{
#if (JPEG_UTOPIA20)
    if (pInstantNJPD == NULL)
    {
        UTOPIA_20_DBG("%s(): UtopiaOpen()\n", __FUNCTION__);
        if(UtopiaOpen(MODULE_NJPEG_EX | VDEC_DRIVER_BASE, &pInstantNJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open NJPD fail\n");
            return;
        }
    }
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        //UtopiaClose(pInstantNJPD);
        return;
    }

    JPEG_VERIFICATION_PARAM VERIFY;
    memset(&VERIFY , 0 ,sizeof(JPEG_VERIFICATION_PARAM));
    VERIFY.VerificationMode = VerificationMode;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetVerificationMode\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_SetVerificationMode, (void*)(&VERIFY)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantNJPD);
        pInstantNJPD = NULL;
    }
#else
    _MApi_JPEG_SetVerificationMode(VerificationMode);
#endif
}

NJPEG_VerificationMode MApi_JPEG_GetVerificationMode(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_NJPEG00_NONE;
    }

    JPEG_VERIFICATION_PARAM VERIFY;
    memset(&VERIFY , 0 ,sizeof(JPEG_VERIFICATION_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetVerificationMode\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetVerificationMode, (void*)(&VERIFY)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_NJPEG00_NONE;
    }
    return VERIFY.VerificationMode;
#else
    return _MApi_JPEG_GetVerificationMode();
#endif
}

void MApi_NJPD_Debug(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Debug\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_Debug, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_NJPD_Debug();
#endif
}

JPEG_Result MApi_JPEG_GetControl(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32size)
{
#if (JPEG_UTOPIA20)
    if(pInstantNJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GET_CTRL_PARAM eGetCTRL;
    memset(&eGetCTRL , 0 ,sizeof(JPEG_GET_CTRL_PARAM));
    eGetCTRL.eGetID = eGetID;
    eGetCTRL.param = param;
    eGetCTRL.u32size = u32size;

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetControl\n");
    if(UtopiaIoctl(pInstantNJPD,MApi_CMD_JPEG_GetControl, (void*)(&eGetCTRL)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return eGetCTRL.retVal;
#else
    return _MApi_JPEG_GetControl(eGetID, param, u32size);
#endif
}


