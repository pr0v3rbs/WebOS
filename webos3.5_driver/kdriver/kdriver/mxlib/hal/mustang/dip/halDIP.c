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
///////////////////////////////////////////////////////////////////////////////
//
//      File name: drvDIP.c
//      Company: MStarSemi Inc.
//
//      Description:  DIP driver implementation.
//                    1. NR part : noise reduction
//                    2. DI part : De-interlace
//
///////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
// Common Definition
#include <string.h> //for memcpy, memset
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "drvDIP.h"
#include "halDIP.h"


static MS_U32 u32RiuBaseAdd;



//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------

#define CLKGEN1_REG_BASE        (0x1980)
#define NR_REG_BASE             (0x8700)
#define DI_REG_BASE             (0x8780)
#define NR_HSD_REG_BASE         (0x8580)

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000


#define DIP_NR_DISABLE      1 // disable SNR/TNR

#define DIP_NR_HSD_MODE     (BIT0|BIT1)
#define DIP_NR_HSD_FACTOR   (0x230000)

#define DIP_VD_WIDTH        (640)
#define DIP_VD_HEIGHT_NTSC  (480)
#define DIP_VD_HEIGHT_PAL   (576)
#define DIP_VD_START_X      (0x72)
#define DIP_VD_START_Y_NTSC (0x09)
#define DIP_VD_START_Y_PAL  (0x0D)

#define DIP_DI_UV_SWAP      0

#define DIP_INPUT_UNKNOWN   0
#define DIP_INPUT_NTSC      1
#define DIP_INPUT_PAL       2
#define DIP_INPUT_WEBCAM    3

#define DIP_PATCH_HAIER_MIU_WAIT 0

#define TILE_BLOCK 512
#define USE_NEON
//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------

#ifdef  DIP_DEBUG

#define DIP_DBG(x1, args...)            printf(x1, ## args)
#define assert(p)   do {\
                        if (!(p)) {\
                            DIP_DBG("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #p);\
                            while(1);\
                        }\
                    } while (0)

#define ASSERT(arg)                  assert((arg))
#else

#define DIP_DBG(x1, args...)
#define ASSERT(arg) do {\
                        if (!(arg)) {\
                            printf("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #arg);\
                        }\
                    } while (0)


#endif


#define UNUSED(x)       (x=x)



#define RIU_MAP u32RiuBaseAdd  //obtain in init

#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))

#define BITMASK(x) _BITMASK(1?x, 0?x)
#define WORD2REAL(W)                ((W)<<2)
#define X1BYTE(addr)                *(volatile MS_U8*)(RIU_MAP + (addr))
#define X2BYTE(addr)                *(volatile MS_U16*)(RIU_MAP + (addr))

/* Write/Read method invalid */
#define _MHal_R1B( u32Reg ) \
        X1BYTE(u32Reg)

#define _MHal_W1B( u32Reg, u08Val ) \
        (X1BYTE(u32Reg) = u08Val)

#define _MHal_W1BM( u32Reg, u08Val, u08Mask ) \
        (X1BYTE(u32Reg) = (X1BYTE(u32Reg) & ~(u08Mask)) | ((u08Val) & (u08Mask)))

#define _MHal_W1Rb( u32Reg, bBit, u08BitPos ) \
        (X1BYTE(u32Reg) = (bBit) ? (X1BYTE(u32Reg) | (u08BitPos)) : (X1BYTE(u32Reg) & ~(u08BitPos)))

#define _MHal_R1Rb( u32Reg, u08BitPos ) \
        (X1BYTE(u32Reg) & (u08BitPos))

#define _MHal_R2B( u32Reg ) \
        X2BYTE(u32Reg)

#define _MHal_W2B( u32Reg, u16Val ) \
        (X2BYTE(u32Reg) = (u16Val))

#define _MHal_W2BM( u32Reg, u08Val, u08Mask ) \
        (X2BYTE(u32Reg) = (X2BYTE(u32Reg) & ~(u08Mask)) | ((u08Val) & (u08Mask)))

#define _MHal_W2Bb( u32Reg, bBit, u08BitPos ) \
        (X2BYTE(u32Reg) = (bBit) ? (X2BYTE(u32Reg) | (u08BitPos)) : (X2BYTE(u32Reg) & ~(u08BitPos)))

#define _MHal_R2Bb( u32Reg, u08BitPos ) \
        (X2BYTE(u32Reg) & (u08BitPos))


#define CLKGEN1_REG(x)                  WORD2REAL((CLKGEN1_REG_BASE) + (x))
#define NR_HSD_REG(x)                   WORD2REAL((NR_HSD_REG_BASE) + (x))
#define DI_REG(x)                       WORD2REAL((DI_REG_BASE) + (x))
#define NR_REG(x)                       WORD2REAL((NR_REG_BASE) + (x))

#if 0
#define LOG_TAG "DIP Driver"
#define LOG_NDEBUG 0
#include <cutils/log.h>
#endif
#if 0
#include "/home/peifen.chen/AEGIS/android/ics/system/core/include/cutils/log.h"
#define ms_dLOGE(level, fmt, arg...) \
    do { \
        if (level & DEBUG_LEVEL) LOGE(fmt, ##arg); \
    } while (0)
#endif
//----------------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------------


DIP_MGR     DIP_mgr;

#if FRAME_CHECK
#define     CHECK_LENGTH        2
MS_U32      u32CheckWord[CHECK_LENGTH] = {0x01234567, 0x76543210};
#endif

extern void MsOS_FlushMemory(void);
extern void MsOS_ReadMemory(void);
#ifdef USE_NEON
extern void yc_separate_tile_neon_256(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_256_y_only(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_128(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_128_y_only(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_64(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_64_y_only(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_32(void* pDestY, void* pDestCbCr, void* pSrc);
extern void yc_separate_tile_neon_32_y_only(void* pDestY, void* pDestCbCr, void* pSrc);
#endif
#if ENABLE_DI_BUFFER_VERIFY | DIP_PATCH_HAIER_MIU_WAIT
static MS_U32 u32VerifyDiBufStartAddress;
static MS_U32 u32FrameSize;
#endif

#if ENABLE_DI_BUFFER_VERIFY
static MS_U8* Y_TailBuf;
static MS_U8* UV_TialBuf;
static MS_U8* Y_SequencyBuf;
static MS_U8* U_SequencyBuf;
static MS_U8* V_SequencyBuf;
static MS_U8* OutBuf;
#endif
static MS_U8  BufferIndex =0;
static MS_U8  u8VerifyDiBufCnt;

static MS_U8 _u8DipInput = DIP_INPUT_UNKNOWN;


str_DIPSharedInfo *g_pDIPCtxLocal = NULL;
str_DIP_DBG_SharedInfo *gDBG_pDIPCtxLocal =NULL;

MS_U8 HANDLER_DONE =0;

//----------------------------------------------------------------------------
// Local Function Prototypes
//----------------------------------------------------------------------------
static void BuffIndexCnt(void)
{
    DIP_mgr.DI_Info.u16Valid_DIBuf |= (1 << g_pDIPCtxLocal->u32BufIndex);
    g_pDIPCtxLocal->u32BufIndex ++;
    if(g_pDIPCtxLocal->u32BufIndex == g_pDIPCtxLocal->DstBlk.u32DstCnt)
        g_pDIPCtxLocal->u32BufIndex =0;
}

#if (_INT_Handler)
static void __ARM_INT_handler(MHAL_SavedRegisters *pHalReg, MS_U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);

    BuffIndexCnt();
    MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_BEON);
    HANDLER_DONE = TRUE;
    gDBG_pDIPCtxLocal->u32DIPHandler = 0x1111;
 }
#endif

#if FRAME_CHECK
#if 0
void MHal_DIP_SetIdentifyInfo(MS_U8 u8DiIndex)
{
    MS_U8* pu8Start = NULL;

    ASSERT(DIP_mgr.Init==TRUE);
    ASSERT(u8DiIndex<DIP_mgr.DI_Info.u8DI_BufCnt);
    //DIP_DBG("Set %d.\n", u8DiIndex);

    pu8Start = (MS_U8*)(DIP_mgr.DI_Info.u32DI_YBuf[u8DiIndex] | 0xA0000000);
    memcpy(pu8Start, (MS_U8*)u32CheckWord, sizeof(u32CheckWord));
    MsOS_FlushMemory();
}


MS_BOOL MHal_DIP_CheckIdentifyInfo(MS_U8 u8DiIndex)
{
    MS_U32* pu32Start = NULL;

    ASSERT(DIP_mgr.Init==TRUE);
    ASSERT(u8DiIndex<DIP_mgr.DI_Info.u8DI_BufCnt);


    pu32Start = (MS_U32*)(DIP_mgr.DI_Info.u32DI_YBuf[u8DiIndex] | 0xA0000000);

    MsOS_ReadMemory();
    if((pu32Start[0] != u32CheckWord[0])||(pu32Start[1] != u32CheckWord[1]))
        return FALSE;

    return TRUE;
}
#else

MS_BOOL MHal_DIP_SetIdentifyInfo(MS_U8 u8DiIndex)
{
    UNUSED(u8DiIndex);
    return TRUE;
}

MS_BOOL MHal_DIP_CheckIdentifyInfo(MS_U8 u8DiIndex)
{
    UNUSED(u8DiIndex);
    return TRUE;
}
#endif
#endif


MS_BOOL DIP_ISR_Control(MS_BOOL bEnable, InterruptCb pCallback)
{
    static MS_BOOL bISRstates = FALSE;

    if(bEnable && bISRstates == FALSE)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DISPI, pCallback);// LGE drmyung 081013
        // enable interrupt here
        MsOS_EnableInterrupt(E_INT_IRQ_DISPI);
        // ...
        //
        bISRstates = TRUE;
    }
    else if(bISRstates == TRUE && bEnable == FALSE)
    {
        // disable interrupt here
        MsOS_DisableInterrupt(E_INT_IRQ_DISPI);
        // ...
        //
        MsOS_DetachInterrupt(E_INT_IRQ_DISPI);
        bISRstates = FALSE;
    }
    return TRUE;
}


#if FRAME_CHECK
void DIP_HandleIsr(InterruptNum eIntNum)
{
    MS_U8  u8IntStatus = 0;
    //static MS_U8 prev_index = 0xFF;

    //Read int 19~16
    /*
        The IRQ status [15:0] for disp_ipath
        # [0] : the rising edge for NR Vsync output
        # [1] : the falling edge for NR Vsync output
        # [2] : the rising edge for NR Hsync output
        # [3] : the falling edge for NR Hsync output
        # [4] : the rising edge for NR Field output
        # [5] : the falling edge for NR Field output
        # [6] : No Vsync / Hsync as video source input
        # [7] : Abnormal Vertical Line Numbers as video source input
        # [8] : Abnormal Horizontal Pixel Numbers as video source input
        # [9] : NR Write Motion Ratio FIFO Full
        # [10] : NR Write Data FIFO Full
        # [11] : NR Read History Mode Motion Ratio FIFO Empty
        # [12] : NR Read Motion Ratio FIFO Empty
        # [13] : NR Read Data FIFO Empty
        # [14] : DI Read FIFO Empty
        # [15] : DI Write C FIFO Full
        The IRQ status [19:16] for disp_ipath
        # [16] : DI Write FIFO Full
        # [17]:  NR Write MIU Finish
        # [18]:  DI Write MIU Finish
        # [19]:  SNR Frost Algorithm Statistic Ready
        # [23:20]:  DI Write MIU Finish Index
        # [19:17] : reserved
    */

    printf("\33[0;31m  [%s] [%d] =====)========    \33[m \n",__FUNCTION__,__LINE__);



    u8IntStatus = _MHal_R1B(NR_REG(0x0E));
    //printf("u8IntStatus = %x", u8IntStatus);
    if(u8IntStatus != 0)
    {
        // DI Write MIU Finish
        if(u8IntStatus & BIT2)
        {
                DIP_mgr.DI_Info.u16Valid_DIBuf |= (1 << BufferIndex);
                BufferIndex++;
                if (BufferIndex == u8VerifyDiBufCnt)
                    BufferIndex = 0;
        }
        // clear int status
        _MHal_W1B(NR_REG(0x0C), u8IntStatus);
        _MHal_W1B(NR_REG(0x0C), 0);
    }

    MsOS_EnableInterrupt(E_INT_IRQ_DISPI);
}
#endif //FRAME_CHECK


void MHAL_DIP_RegSetBase(MS_U32 u32Base)
{
    u32RiuBaseAdd = u32Base;
}


DIP_ERRCODE MHal_DIP_Init(MS_U32 u32InputMode)
{
    DIP_mgr.Init = TRUE;

#if DIP_PATCH_HAIER_MIU_WAIT
    BufferIndex = 0;
#endif

   	_u8DipInput = u32InputMode;

    switch(_u8DipInput)
    {
        case DIP_INPUT_NTSC:
        case DIP_INPUT_PAL:
        case DIP_INPUT_WEBCAM:
             break;
        default:
            _u8DipInput = DIP_INPUT_UNKNOWN;

            DIP_DBG("==== DIP Mode error ====.\n");
            return DIP_ERR_FAIL;
            break;
    }

    //g_pDIPCtxLocal = (str_DIPSharedInfo*)MsOS_PA2KSEG1(0xC800000);       //Charka2
    //Android
    if( MsOS_MPool_Mapping(0, 0x1ED000, (0x1ED000 + 0x10000+ 0x400), 1)!=TRUE)    //1:NonCache 0:Cache
    {
        printf("mpool mapping fali\n");
    }
    g_pDIPCtxLocal = (str_DIPSharedInfo*)MsOS_PA2KSEG1(0x1FD000);       //DRAM 0x1ED000+0x1000*/
    gDBG_pDIPCtxLocal= (str_DIP_DBG_SharedInfo*)MsOS_PA2KSEG1(0x1FD000+sizeof(str_DIPSharedInfo));

    #if (_INT_Handler)
        MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_BEON, (InterruptCb)__ARM_INT_handler);
        MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_BEON);
        gDBG_pDIPCtxLocal->u32DIP_INTType = 0xD0CE;
    #else
        gDBG_pDIPCtxLocal->u32DIP_INTType = 0xD0FA;
    #endif


    //R2 inital status
    if(g_pDIPCtxLocal->u32R2Initialized == 0)
    {
        printf("\n====EAGLE R2 Init Fail ====.\n");
    }

    g_pDIPCtxLocal->u32DIPInitialized = DIP_mgr.Init;

    g_pDIPCtxLocal->SrcBlk.u32BufAddrStart = 0;
    g_pDIPCtxLocal->SrcBlk.u32BufWidth = 0;
    g_pDIPCtxLocal->SrcBlk.u32BufHeight = 0;

    g_pDIPCtxLocal->DstBlk.u32YBufAddrStart = 0;
    g_pDIPCtxLocal->DstBlk.u32CBufAddrStart = 0;
    g_pDIPCtxLocal->DstBlk.u32DstCnt = 0;

    g_pDIPCtxLocal->u32BufIndex = 0;
    g_pDIPCtxLocal->u32DIPOrder = 0;
    g_pDIPCtxLocal->u32TriggerStatus = 0;

    HANDLER_DONE = TRUE;

    return DIP_ERR_OK;
}

DIP_ERRCODE MHal_DIP_SetFrameInfo(MS_U32 u32FrameX, MS_U32 u32FrameY,
                MS_U32 u32FrameWidth, MS_U32 u32FrameHeight, MS_BOOL bInterLace)
{
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_U32 u32StartX;
    MS_U32 u32StartY;

    if (_u8DipInput == DIP_INPUT_NTSC)
    {
        u32Width = DIP_VD_WIDTH;
        u32Height = DIP_VD_HEIGHT_NTSC;
        u32StartX = DIP_VD_START_X;
        u32StartY = DIP_VD_START_Y_NTSC;
    }
    else if (_u8DipInput == DIP_INPUT_PAL)
    {
        u32Width = DIP_VD_WIDTH;
        u32Height = DIP_VD_HEIGHT_PAL;
        u32StartX = DIP_VD_START_X;
        u32StartY = DIP_VD_START_Y_PAL;

    }
    else if (_u8DipInput == DIP_INPUT_WEBCAM)//WebCam Mode
    {
        u32Width = u32FrameWidth;
        u32Height = u32FrameHeight;
        u32StartX = 0;
        u32StartY = 0;
    }
    else
    {
        DIP_DBG("==== warning! unknown input ====.\n");
        return DIP_ERR_FAIL;
    }

    DIP_DBG("====MHal_DIP_SetFrameInfo====.\n");
    DIP_DBG("Frame width: %d, height: %d, Interlace: %d.\n", (unsigned int)u32Width, (unsigned int)u32Height, bInterLace);

    return DIP_ERR_OK;
}

DIP_ERRCODE MHal_DIP_SetIntputMode(MS_U32 u32InputMode)
{
    DIP_DBG("==== DIP Mode %d ====.\n", (unsigned int)u32InputMode);

    _u8DipInput = u32InputMode;


    switch(_u8DipInput)
    {
        case DIP_INPUT_NTSC:
        case DIP_INPUT_PAL:
        case DIP_INPUT_WEBCAM:
             break;
        default:
            _u8DipInput = DIP_INPUT_UNKNOWN;

            DIP_DBG("==== DIP Mode error ====.\n");
            return DIP_ERR_FAIL;
            break;
    }

    return DIP_ERR_OK;
}

DIP_ERRCODE MHal_DIP_SetYUVOrder(MS_U32 dipOrder)
{
    g_pDIPCtxLocal->u32DIPOrder = dipOrder;

    return DIP_ERR_OK;
}

DIP_ERRCODE MHal_DIP_SetWebCamBuff(MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{
    g_pDIPCtxLocal->SrcBlk.u32BufAddrStart = u32BufStart;
    g_pDIPCtxLocal->SrcBlk.u32BufWidth = u32BufWidth;
    g_pDIPCtxLocal->SrcBlk.u32BufHeight = u32BufHeight;

    return DIP_ERR_OK;
}

static void DIP(str_DIP_Info *p_DIP_Info)
{
   	#define CEILING_ALIGN(value, align) (((unsigned int)(value)+((align)-1)) & ~((align)-1))

    MS_U16 nSrcWidth =0, nSrcHeight =0;
    MS_U16 AlighWidth = 0,AlignHeight = 0;
    //MS_U32 ConverCnt=0;

    nSrcWidth = g_pDIPCtxLocal->SrcBlk.u32BufWidth;
    nSrcHeight = g_pDIPCtxLocal->SrcBlk.u32BufHeight;

    p_DIP_Info->TilewWidth =16;  /*Unit =pixel*/
    p_DIP_Info->TileHeight =32;
    AlighWidth  = CEILING_ALIGN(nSrcWidth, p_DIP_Info->TilewWidth);
    AlignHeight = CEILING_ALIGN(nSrcHeight, (p_DIP_Info->TileHeight));
    p_DIP_Info->TileWidthCnt = AlighWidth / p_DIP_Info->TilewWidth;
    p_DIP_Info->TileHeightCnt = AlignHeight / p_DIP_Info->TileHeight;

    p_DIP_Info->tile_block = (p_DIP_Info->TilewWidth <<1) * (p_DIP_Info->TileHeight); //Unit:Byte
    p_DIP_Info->offset_block = (p_DIP_Info->tile_block) * (p_DIP_Info->TileWidthCnt);
    p_DIP_Info->offset_Cnt = (p_DIP_Info->tile_block) * (p_DIP_Info->TileWidthCnt-1);
}

#ifdef USE_NEON
static void yc_separate(void* pDestY, void* pDestCbCr, void* pSrc, unsigned int alignedwidth,unsigned int alignedheight)
{
    unsigned char* destY, * destYBase, *destYinner;
    unsigned char* destCbCr,* destCbCrBase,* destCbCrinner;
    unsigned char* src;
    unsigned int i;
    unsigned int line, linecount;
    unsigned int count;//, size;
    unsigned int yblockfill = 0,cblockfill = 0;
    unsigned int neon_data=0,neon_num=0;

    destY = (unsigned char*)pDestY;
    destCbCr = (unsigned char*)pDestCbCr;
    src = (unsigned char*)pSrc;
    destYBase = destY ;
    destCbCrBase = destCbCr;
    destYinner = destY ;
    destCbCrinner = destCbCr;

    if( (alignedwidth%128) == 0)
    	neon_data = 256;
    else if( (alignedwidth%64) == 0)
    	neon_data = 128;
    else if( (alignedwidth%32) == 0)
    	neon_data = 64;
    else
    	neon_data = 32;

    neon_num = neon_data/32;

    linecount = ( alignedwidth * 2) /neon_data;
    count = ( alignedwidth * alignedheight * 2) /neon_data;

    line = 0;
    for(i = 0; i < count; i ++)
    {
        if( (line % 2) != 0 )
        {
        		if(neon_data == 256)
            	yc_separate_tile_neon_256(destYinner, destCbCrinner, src);
            else if(neon_data == 128)
            	yc_separate_tile_neon_128(destYinner, destCbCrinner, src);
            else if(neon_data == 64)
            	yc_separate_tile_neon_64(destYinner, destCbCrinner, src);
            else
            	yc_separate_tile_neon_32(destYinner, destCbCrinner, src);

            destYinner += neon_num*TILE_BLOCK;
            destCbCrinner +=  neon_num*TILE_BLOCK;
        }
        else
        {
        		if(neon_data == 256)
            	yc_separate_tile_neon_256_y_only(destYinner, destCbCrinner, src);
            else if(neon_data == 128)
            	yc_separate_tile_neon_128_y_only(destYinner, destCbCrinner, src);
            else if(neon_data == 64)
            	yc_separate_tile_neon_64_y_only(destYinner, destCbCrinner, src);
            else
            	yc_separate_tile_neon_32_y_only(destYinner, destCbCrinner, src);

            destYinner +=  neon_num*TILE_BLOCK;
        }
        src += neon_data;
        if( ((i + 1) % linecount) == 0 )
        {
            yblockfill ++;
            if( (yblockfill % 32) == 0 )
            {
                destYBase +=  alignedwidth * 32;
                destY = destYBase;
                destYinner = destY ;
            }
            else
            {
                destY += 16;
                destYinner = destY ;
            }

            if((line % 2) != 0)
            {
                cblockfill ++;
                if((cblockfill % 32) == 0 )
                {
                    destCbCrBase +=  alignedwidth * 32;
                    destCbCr = destCbCrBase;
                    destCbCrinner = destCbCr;
                }
                else
                {
                    destCbCr += 16;
                    destCbCrinner = destCbCr;
                }
            }
            line ++;
        }
    }
    return ;
}
static void DIP_YUYV(str_DIP_Info DIP_Info)
{
    unsigned char *Src,*DstY, *DstUV;

    //Src = (unsigned char*)MsOS_PA2KSEG0(g_pDIPCtxLocal->SrcBlk.u32BufAddrStart);
    Src = (unsigned char*)g_pDIPCtxLocal->SrcBlk.u32BufAddrStart;       //VA
    DstY = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32YBufAddrStart);
    DstUV = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32CBufAddrStart);

    yc_separate(DstY,DstUV,Src,g_pDIPCtxLocal->SrcBlk.u32BufWidth,g_pDIPCtxLocal->SrcBlk.u32BufHeight);
}
#else
static void DIP_YUYV(str_DIP_Info DIP_Info)
{
    str_SrcYUYV *Src;
    unsigned char *DstY, *DstUV;

    MS_U16 x=0,y=0,row=0,col=0;

    //Already MPOOL Mapping By DIP_Dev;
    //Src = (str_SrcYUYV*)MsOS_PA2KSEG0(g_pDIPCtxLocal->SrcBlk.u32BufAddrStart);
    Src = (str_SrcYUYV*)g_pDIPCtxLocal->SrcBlk.u32BufAddrStart;       //VA
    DstY = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32YBufAddrStart);
    DstUV = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32CBufAddrStart);

    //Read continue; write not continue;
    for(y=0; y< DIP_Info.TileHeightCnt; y++)
    {
        for(col =0; col< DIP_Info.TileHeight; col++)
        {
            for(x=0; x< DIP_Info.TileWidthCnt; x++)
            {
                for(row =0; row< 8; row++)
                {
                    *DstY = (Src[0].Y0);
                    DstY++;
                    *DstY = (Src[0].Y1);
                    DstY++;
                    if( !(col & 0x0001))
                    {
                        *DstUV = (Src[0].U) ;
                        DstUV++;
                        *DstUV = (Src[0].V) ;
                        DstUV++;
                    }
                    Src++;
                }
                DstY -= 16;
                if( !(col & 0x0001))
	                DstUV -= 16;
                DstY += DIP_Info.tile_block >>1;
                DstUV += DIP_Info.tile_block >>1;
            }
            DstY -= DIP_Info.offset_block >>1;
            DstUV -= DIP_Info.offset_block >>1;
            DstY += 16;
            if( !(col & 0x0001) )
	            DstUV += 16;
        }
        DstY += DIP_Info.offset_Cnt >>1;
        if( y & 0x0001)
            DstUV += DIP_Info.offset_Cnt >>1;
    }
}
#endif
static void DIP_UYUV(str_DIP_Info DIP_Info)
{
    str_SrcUYVY *Src;
    unsigned char *DstY, *DstUV;

    MS_U16 x=0,y=0,row=0,col=0;

    //Already MPOOL Mapping By DIP_Dev;
    //Src = (str_SrcUYVY*)MsOS_PA2KSEG0(g_pDIPCtxLocal->SrcBlk.u32BufAddrStart);
    Src = (str_SrcUYVY*)g_pDIPCtxLocal->SrcBlk.u32BufAddrStart;       //VA
    DstY = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32YBufAddrStart);
    DstUV = (unsigned char*)MsOS_PA2KSEG1(g_pDIPCtxLocal->DstBlk.u32CBufAddrStart);

    //Read continue; write not continue;
    for(y=0; y< DIP_Info.TileHeightCnt; y++)
    {
        for(col =0; col< DIP_Info.TileHeight; col++)
        {
            for(x=0; x< DIP_Info.TileWidthCnt; x++)
            {
                for(row =0; row< 8; row++)
                {
                    *DstY = (Src[0].Y0);
                    DstY++;
                    *DstY = (Src[0].Y1);
                    DstY++;
                    if( !(col & 0x0001))
                    {
                        *DstUV = (Src[0].U) ;
                        DstUV++;
                        *DstUV = (Src[0].V) ;
                        DstUV++;
                    }
                    Src++;
                }
                DstY -= 16;
                if( !(col & 0x0001))
                DstUV -= 16;
                DstY += DIP_Info.tile_block >>1;
                DstUV += DIP_Info.tile_block >>1;
            }
            DstY -= DIP_Info.offset_block >>1;
            DstUV -= DIP_Info.offset_block >>1;
            DstY += 16;
            if( !(col & 0x0001) )
	            DstUV += 16;
        }
       DstY += DIP_Info.offset_Cnt >>1;
       if( y & 0x0001)
	       DstUV += DIP_Info.offset_Cnt >>1;
    }

}
static void LinearTo420Tile(str_DIP_Info DIP_Info)
{
    if(g_pDIPCtxLocal->u32DIPOrder)
    {
        DIP_YUYV(DIP_Info);
    }
    else
    {
        DIP_UYUV(DIP_Info);
    }
}

void MHal_DIP_Trigger(void)
{
    str_DIP_Info DIP_Info = {0,0,0,0,0,0,0,0,0};

    g_pDIPCtxLocal->DstBlk.u32YBufAddrStart = DIP_mgr.DI_Info.u32DI_YBuf[g_pDIPCtxLocal->u32BufIndex];
    g_pDIPCtxLocal->DstBlk.u32CBufAddrStart = DIP_mgr.DI_Info.u32DI_CBuf[g_pDIPCtxLocal->u32BufIndex];

    DIP(&DIP_Info);
    LinearTo420Tile(DIP_Info);
    BuffIndexCnt();
}

DIP_ERRCODE MHal_DIP_SetNRBuff(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{
    MS_U32 u32Temp = 0;
    MS_U8 i;

    MS_U32 u32Width;
    MS_U32 u32Height;

    //Patch for when u8BufCnt == 0, It means dont use NR Buff.

    if (_u8DipInput == DIP_INPUT_NTSC)
    {
        u32Width = DIP_VD_WIDTH;
        u32Height = DIP_VD_HEIGHT_NTSC;
    }
    else if (_u8DipInput == DIP_INPUT_PAL)
    {
        u32Width = DIP_VD_WIDTH;
        u32Height = DIP_VD_HEIGHT_PAL;
    }
    else if (_u8DipInput == DIP_INPUT_WEBCAM)//WebCam Mode
    {
        u32Width = u32BufWidth;
        u32Height = u32BufHeight;
    }
    else
    {
        DIP_DBG("==== warning! unknown input ====.\n");
        return DIP_ERR_FAIL;
    }


    DIP_DBG("====MHal_DIP_SetNRBuff====\n");
    DIP_DBG("Buffer count: %d, width: %d, height: %d.\n", u8BufCnt, (unsigned int)u32Width, (unsigned int)u32Height);
    DIP_DBG("Buffer address start: 0x%x, end: 0x%x.\n", (unsigned int)u32BufStart, (unsigned int)u32BufEnd);

    // Auto gen buffer
    if(u8BufCnt == 0xFF)
    {
        #if 0
        u8BufCnt = 1;
        u32BufWidth = 720;
        u32BufHeight = 480;
        u32BufStart = (U32)phys_to_virt((U32)DIP_NR_BUF_ADR);
        u32BufEnd= (U32)phys_to_virt((U32)(DIP_NR_BUF_ADR+DIP_NR_BUF_LEN));
        #endif

        return  DIP_ERR_INVALID_BUFFER_CNT;
    }

    if (u8BufCnt > MAX_NR_BUFF_CNT)
    {
        return DIP_ERR_INVALID_BUFFER_CNT;
    }
    // T3 DIP need 16 bytes align.
    //if (u32BufStart % 8)
    if (u32BufStart % 16)
    {
        return DIP_ERR_INVALID_BUFFER_START;
    }
    if ((u32Width % 16) || (u32Width > MAX_BUFF_WIDTH))
    {
        return DIP_ERR_INVALID_BUFFER_WIDTH;
    }
    //if ((u32BufHeight % 16) || (u32BufHeight > MAX_BUFF_HEIGHT))
   if ((u32Height > MAX_BUFF_HEIGHT))
    {
        return DIP_ERR_INVALID_BUFFER_HEIGHT;
    }
    u32Temp = u32Width*u32Height*(u8BufCnt*2+1);
    if (u32Temp > (u32BufEnd - u32BufStart))
    {
        return DIP_ERR_INVALID_BUFFER_SZIE;
    }

    // NR BUFFER
    // |--> NrRatioBuf
    // *
    // [NR Ration = u32BufWidth*u32BufHeight] [NR = u32BufWidth*u32BufHeight*2] [ ... ]
    //                                         *                                *
    //                                         |--> NrDataBuff[0]               |-->NrDataBuff[1]
    //

    //Initial data buffer count, buffer width, buffer count
    DIP_mgr.NR_Info.u8NR_BufCnt = u8BufCnt;
    DIP_mgr.NR_Info.u32NR_BufWidth = u32Width;
    DIP_mgr.NR_Info.u32NR_BufHeight = u32Height;
    //Initial ratio buffer address
    DIP_mgr.NR_Info.u32NR_RationBuf = u32BufStart;
    DIP_DBG("Ratio Buff start: 0x%x.\n", (unsigned int)u32BufStart);
    u32BufStart += u32Width*u32Height;// 1 Pixel 1 Byte in Motion Ratio
    //Initial data buffer address
    u32Temp = u32Width*u32Height*2;
    for (i=0; i<u8BufCnt; i++)
    {
        DIP_mgr.NR_Info.u32NR_DataBuf[i] = u32BufStart;
        u32BufStart += u32Temp;
        DIP_DBG("NR buffer %d, start: 0x%x.\n", i, (unsigned int)DIP_mgr.NR_Info.u32NR_DataBuf[i]);
    }

    //Set NR buffer flag
    //u32DIPFlag |= E_SET_NR_BUFFER;

    return DIP_ERR_OK;
}

void MHal_DIP_GetDIInfo(DI_INFO *pDIInfo)
{
    ASSERT(pDIInfo != NULL);
    if(pDIInfo != NULL)
    {
        memcpy(pDIInfo, &DIP_mgr.DI_Info, sizeof(DI_INFO));
    }
}


void MHal_DIP_GetNRInfo(NR_INFO *pNRInfo)
{
    ASSERT(pNRInfo != NULL);
    if(pNRInfo != NULL)
    {
        memcpy(pNRInfo, &DIP_mgr.NR_Info, sizeof(NR_INFO));
    }
}

DIP_ERRCODE MHal_DIP_SetDIBuff(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
                               MS_U32 u32BufStart, MS_U32 u32BufEnd)
{
    #define CEILING_ALIGN(value, align) (((unsigned int)(value)+((align)-1)) & ~((align)-1))

    MS_U32 u32Temp = 0;
    MS_U8 i;

    MS_U32 u32Width;
    MS_U32 u32YHeight, u32CHeight;
    MS_U32 u32BufSize;

    if (_u8DipInput == DIP_INPUT_NTSC)
    {
        u32Width = DIP_VD_WIDTH;
        u32YHeight = DIP_VD_HEIGHT_NTSC;
    }
    else if (_u8DipInput == DIP_INPUT_PAL)
    {
        u32Width = DIP_VD_WIDTH;
        u32YHeight = DIP_VD_HEIGHT_PAL;
    }
    else if (_u8DipInput == DIP_INPUT_WEBCAM)//WebCam Mode
    {
        u32Width = u32BufWidth;
        u32YHeight = u32BufHeight;
    }
    else
    {
        DIP_DBG("==== warning! unknown input ====.\n");
        return DIP_ERR_FAIL;
    }

    //Align cause of UV [ 16*32 tile ]
    u32YHeight = CEILING_ALIGN(u32YHeight, 32);
    u32CHeight = CEILING_ALIGN(u32YHeight, 32*2);

    DIP_DBG("====MHal_DIP_SetDIBuff====.\n");
    DIP_DBG("Buffer count: %d, width: %d, height: %d.\n", u8BufCnt, (unsigned int)u32Width, (unsigned int)u32YHeight);
    DIP_DBG("Buffer address start: 0x%x, end: 0x%x.\n", (unsigned int)u32BufStart, (unsigned int)u32BufEnd);

    // Auto gen buffer
    if(u8BufCnt == 0xFF)
    {
        #if 0
        u8BufCnt = 5;
        u32BufWidth = 720;
        u32BufHeight = 480;
        u32BufStart = (U32)phys_to_virt((U32)DIP_DI_BUF_ADR);
        u32BufEnd= (U32)phys_to_virt((U32)(DIP_DI_BUF_ADR+DIP_DI_BUF_LEN));
        #endif
        return  DIP_ERR_INVALID_BUFFER_CNT;
    }

#if ENABLE_DI_BUFFER_VERIFY
    u32VerifyDiBufStartAddress = u32BufStart | 0xA0000000;
    u8VerifyDiBufCnt = u8BufCnt;
    DIP_DBG("u32VerifyDiBufStartAddress: 0x%x, u8VerifyDiBufCnt: 0x%x.\n", u32VerifyDiBufStartAddress, u8VerifyDiBufCnt);
#endif

    if (u8BufCnt > MAX_DATA_BUFF_CNT-1)
    {
        return DIP_ERR_INVALID_BUFFER_CNT;
    }
    if (u32BufStart % 16)
    {
        return DIP_ERR_INVALID_BUFFER_START;
    }
    if ((u32Width % 16) || (u32Width > MAX_BUFF_WIDTH))
    {
        return DIP_ERR_INVALID_BUFFER_WIDTH;
    }
    if ((u32YHeight %32) ||( u32YHeight > CEILING_ALIGN(MAX_BUFF_HEIGHT,32*2)))
    {
        return DIP_ERR_INVALID_BUFFER_HEIGHT;
    }

     //(Width*YHeight*1 + Width*CHeight*0.5)*BufCnt
    u32Temp =  (u32Width*u32YHeight) + (u32Width*(u32CHeight>>1)) ;
    u32BufSize = u32Temp * u8BufCnt;
    if (u32BufSize > (u32BufEnd - u32BufStart))
    {
        return DIP_ERR_INVALID_BUFFER_SZIE;
    }

    // DI BUFFER
    // |--> sDiDataBuff[0]                                                      |--> sDiDataBuff[1]
    // *                                                                        *
    // [DI Y = u32BufWidth*u32BufHeight | DI UV= 0.5*u32BufWidth*u32BufHeight ] [ ... ]
    //

    //Initial data buffer count, buffer width, buffer count
    DIP_mgr.DI_Info.u8DI_BufCnt = u8BufCnt;
    DIP_mgr.DI_Info.u32DI_BufWidth = u32Width;
    DIP_mgr.DI_Info.u32DI_BufHeight = u32YHeight;
    //Initial YUV420 buffer address , 4Pixels 6 Bytes,1Pixel 1.5 Byte
    for (i=0; i<u8BufCnt; i++)
    {
        DIP_mgr.DI_Info.u32DI_YBuf[i] = u32BufStart;
        DIP_mgr.DI_Info.u32DI_CBuf[i] = (u32BufStart + u32Width*u32YHeight); // Y size is u32BufWidth*u32BufHeight
        u32BufStart += u32Temp;
        #if FRAME_CHECK
        MHal_DIP_SetIdentifyInfo(i);
        #endif
        DIP_DBG("DI buffer %d, Y star: 0x%x, UV strat: 0x%x.\n", i, (unsigned int)DIP_mgr.DI_Info.u32DI_YBuf[i], (unsigned int)DIP_mgr.DI_Info.u32DI_CBuf[i]);
    }

    g_pDIPCtxLocal->DstBlk.u32DstCnt = (MS_U32)u8BufCnt;

    return DIP_ERR_OK;
}

DIP_ERRCODE MHal_DIP_EnableNRandDI(MS_BOOL bEnableNR, MS_BOOL bEnableSNR, MS_BOOL bEnableTNR, MS_BOOL bEnableDI)
{
    MS_U16 u16Tmp = 0;

    DIP_mgr.DI_Info.u16Valid_DIBuf = 0;
    DIP_mgr.DI_Info.u32DI_FrameCnt = 0;

    if (bEnableNR)
    {
        if (bEnableSNR)
        {
            //MDrv_WriteByteMask(NR_REG_CTRL_L, (REG_NR_SNR_EN | REG_NR_SNR_LINE_BUFF_EN), (REG_NR_SNR_EN | REG_NR_SNR_LINE_BUFF_EN));
            u16Tmp |= (BIT1 | BIT6);
        }

        if (bEnableTNR)
        {
            //MDrv_WriteByteMask(NR_REG_CTRL_L, (REG_NR_TNR_RD_DATA_EN | REG_NR_TNR_RD_RATIO_EN), (REG_NR_TNR_RD_DATA_EN | REG_NR_TNR_RD_RATIO_EN));
            //MDrv_WriteByteMask(NR_REG_CTRL_H, (REG_NR_TNR_ADJUST_RATIO_KY_EN | REG_NR_TNR_ADJUST_RATIO_KC_EN | REG_NR_TNR_EN),
            //    (REG_NR_TNR_ADJUST_RATIO_KY_EN | REG_NR_TNR_ADJUST_RATIO_KC_EN | REG_NR_TNR_EN));
            u16Tmp |= (BIT2 | BIT3 |BIT4 | BIT5 |BIT13 | BIT14 | BIT15 );
        }

        DIP_DBG("Enable NR: 0x%x\n", u16Tmp);
/*
        MDrv_WriteByteMask(NR_REG_CTRL_H, (REG_NR_DATA_ACCESS_MIU_EN | REG_NR_RATIO_ACCESS_MIU_EN | REG_NR_MOTION_HISTORY_EN),
            (REG_NR_DATA_ACCESS_MIU_EN | REG_NR_RATIO_ACCESS_MIU_EN | REG_NR_MOTION_HISTORY_EN));
        DIP_DBG(KDBG("Enable NR 1 : 0x%x.\n", MDrv_Read2Byte(NR_REG_CTRL_L)));
        MDrv_WriteByteMask(NR_REG_CTRL_L, (REG_NR_MIU_WR_DATA_EN | REG_NR_MIU_WR_RATIO_EN | REG_NR_EN),
            (REG_NR_MIU_WR_DATA_EN | REG_NR_MIU_WR_RATIO_EN | REG_NR_EN));
        DIP_DBG(KDBG("Enable NR 2 : 0x%x.\n", MDrv_Read2Byte(NR_REG_CTRL_L)));
*/
        //RobertYang
        //u16Tmp |= ((REG_NR_DATA_ACCESS_MIU_EN | REG_NR_RATIO_ACCESS_MIU_EN | REG_NR_MOTION_HISTORY_EN)<<8);
        u16Tmp |= (BIT8 | BIT9 |BIT0);
        #if DIP_NR_DISABLE   // disable NR
            //u16Tmp&=~REG_NR_EN;
			u16Tmp&=~(0x01);
        #endif
        DIP_DBG("Enable NR 1: 0x%x\n", u16Tmp);

        DIP_DBG("Enable NR 2: 0x%x\n", _MHal_R2B(NR_REG(0x00)));

        u16Tmp = (DIP_NR_HSD_FACTOR & 0xFFFF);
        DIP_DBG("Enable NR-HSD Factor (L)%d\n", u16Tmp);

        u16Tmp = ((DIP_NR_HSD_FACTOR >> 16) & 0x00FF);
        DIP_DBG("Enable NR-HSD Factor (H)%d\n", u16Tmp);

        u16Tmp = DIP_NR_HSD_MODE;
        DIP_DBG("Enable NR-HSD (mode 0x%x)\n", u16Tmp);

    }

    return DIP_ERR_OK;
}

MS_U8 MHal_DIP_GetDiBuffCount(void)
{
    return DIP_mgr.DI_Info.u8DI_BufCnt;
}

MS_U16 MHal_DIP_GetDiBuffStatus(void)
{
    return DIP_mgr.DI_Info.u16Valid_DIBuf;
}

DIP_ERRCODE MHal_DIP_ClearDiBuffStatus(MS_U8 BufferIndex)// BufferIndex from 0 ~ (u8DiBuffCnt-1)
{

    ASSERT(BufferIndex<DIP_mgr.DI_Info.u8DI_BufCnt);

    // Disable DIP IRQ to protect DIP_mgr.DI_Info.u16Valid_DIBuf.
    // It is modified by DIP interrupt.
    //MsOS_DisableInterrupt(E_INT_IRQ_DISPI);

     DIP_mgr.DI_Info.u16Valid_DIBuf &= ~(0x01 << BufferIndex );

    // Enable DIP IRQ
    //MsOS_EnableInterrupt(E_INT_IRQ_DISPI);


    return DIP_ERR_OK;
}

MS_U32 MHal_DIP_GetDiBuffFrameCount(void)
{
    return DIP_mgr.DI_Info.u32DI_FrameCnt;
}

/*****Below functions are for Test Only*****/

#if ENABLE_DI_BUFFER_VERIFY

void MHal_DIP_ShowFieldInfo(void)
{
    //DIP_DBG("TO YUV\n");
    void MHal_DIP_TailYUV420ToSequenceYUVBlock1(void);

    // Disable MIU Access for DI
    _MHal_W1BM(DI_REG(0x01), 0, BIT0);

    MHal_DIP_SetDIVerBuff(0);
    MHal_DIP_TailYUV420ToSequenceYUVBlock();
    MHal_DIP_SequenceYUVBlockToYUV422();

    // Enable MIU Access for DI
    _MHal_W1BM(DI_REG(0x01),, BIT0, BIT0);
}

void MHal_DIP_SetDIVerBuff(MS_U32 u32BufStart)
{
    U32 i;
    u32BufStart = 0xA5A00000;

    OutBuf =(U8*)(u32BufStart);
    memset(OutBuf, 0x0, 0x200000);


    BufferIndex = 5;

    u32BufStart = 0xA5C00000;
    Y_SequencyBuf = (MS_U8*)(u32BufStart + (720*480*2));
    U_SequencyBuf = (MS_U8*)(u32BufStart + (720*480*2) + (720*480) );
    V_SequencyBuf = (MS_U8*)(u32BufStart + (720*480*2) + (720*480)+(360*240) );

    // Green Y 0x72 U 0x48 V 0x3B
    // Red   Y 0x3B U 0x71 V 0xD0
    // Blue  Y 0x18 U 0xcD V 0x7A
    for( i=0;i<(720*480);i++)
        Y_SequencyBuf[i] = 0x80;
    for( i=0;i<(360*240);i++)
        U_SequencyBuf[i] = 0x36;
    for( i=0;i<(360*240);i++)
        V_SequencyBuf[i] = 0x22;
}

//static U8 Yvalue = 0x72;
void MHal_DIP_SequenceYUVBlockToYUV422(void)
{
    MS_U32 i,j;
    MS_U32 InputPos,OutputPos;
    MS_BOOL UVflag=TRUE;//U Output First // U:True V:False

    //MDrv_UART0_Switch(UART_SWITCH_AEON, TRUE);
    //KDBG("====MDrv_DIP_SequenceYUVBlockToYUV422====.\n");
    //MDrv_UART0_Switch(UART_SWITCH_51, TRUE);

#if 0
    Yvalue ++;
    if( Yvalue==1 )//Red
    {
        for( i=0;i<(720*480);i++)
            Y_SequencyBuf[i] = 0x3B;
        for( i=0;i<(360*240);i++)
            U_SequencyBuf[i] = 0x71;
        for( i=0;i<(360*240);i++)
            V_SequencyBuf[i] = 0xD0;
    }
    else if( Yvalue==2 )//Green
    {
        for( i=0;i<(720*480);i++)
            Y_SequencyBuf[i] = 0x72;
        for( i=0;i<(360*240);i++)
            U_SequencyBuf[i] = 0x3B;
        for( i=0;i<(360*240);i++)
            V_SequencyBuf[i] = 0x48;
    }
    else//Blue
    {   Yvalue =0;
        for( i=0;i<(720*480);i++)
            Y_SequencyBuf[i] = 0x18;
        for( i=0;i<(360*240);i++)
            U_SequencyBuf[i] = 0xCD;
        for( i=0;i<(360*240);i++)
            V_SequencyBuf[i] = 0x7A;
    }
#endif
    for( i=0; i<480 ;i++)
    {
        for( j=0; j<720*2 ;j++) //1Pixel 2Byte
        {
            OutputPos = (i*720*2)+j;
            if( j % 2 ==0 )//Y Ouput
            {
                InputPos = i*720+j/2;

                //OutBuf[OutputPos] = Y_SequencyBuf[InputPos];
                OutBuf[OutputPos] = Y_SequencyBuf[InputPos];
            }
            else//UV Output
            {
                //InputPos = (i/2*720/2)+(j/4+1);
                InputPos = ((i/2)*(360))+ (j/4);
                if( UVflag == TRUE )
                {
                    OutBuf[OutputPos] = U_SequencyBuf[InputPos];
                    //OutBuf[OutputPos] = 0x80;
                    UVflag = FALSE;
                }
                else
                {
                    OutBuf[OutputPos] = V_SequencyBuf[InputPos];
                    //OutBuf[OutputPos] = 0x80;
                    UVflag = TRUE;
                }
            }
        }
    }
}
void MHal_DIP_TailYUV420ToSequenceYUVBlock1(void)
{
    MS_U32 sed, i, j;
    MS_U32 Y_H_Block,Y_V_Block;

    MS_U32 YLineAddr[720];
    MS_U32 UVLineAddr[360];
    MS_U32 VIndx[480];
    MS_U32 VCIndx[240];
    MS_U8* V_TialBuf;
    if( BufferIndex >= u8VerifyDiBufCnt )
        BufferIndex = 0;

    Y_TailBuf = (U8*)(u32VerifyDiBufStartAddress+((U32)BufferIndex*(720*480*3/2)));
    UV_TialBuf = (U8*)((u32VerifyDiBufStartAddress+720*480)+((U32)BufferIndex*(720*480*3/2)));
    V_TialBuf = (U8*)((u32VerifyDiBufStartAddress+720*480)+360*240 + ((U32)BufferIndex*(720*480*3/2)));


    sed = 0x0;
    for(i=0; i<360; i++)
    {
        YLineAddr[i*2] = sed;
        sed = sed + 0x08;
        YLineAddr[i*2+1] = sed;
        sed = sed + 0x08;
    }

    sed = 0x0;
    for(i=0; i<180; i++)
    {
        UVLineAddr[i*2] = sed;
        sed = sed + 0x08;
        UVLineAddr[i*2+1] = sed;
        sed = sed + 0x08;
    }


    sed = 0x0;
    for(i=0; i<480/8; i++)
    {
        VIndx[i*8+0] = sed;
        VIndx[i*8+1] = sed+1;
        VIndx[i*8+2] = sed+2;
        VIndx[i*8+3] = sed+3;
        VIndx[i*8+4] = sed+4;
        VIndx[i*8+5] = sed+5;
        VIndx[i*8+6] = sed+6;
        VIndx[i*8+7] = sed+7;
        sed = sed + 0x08*2;
    }

    sed = 0x0;
    for(i=0; i<240/8; i++)
    {
        VCIndx[i*8+0] = sed;
        VCIndx[i*8+1] = sed+1;
        VCIndx[i*8+2] = sed+2;
        VCIndx[i*8+3] = sed+3;
        VCIndx[i*8+4] = sed+4;
        VCIndx[i*8+5] = sed+5;
        VCIndx[i*8+6] = sed+6;
        VCIndx[i*8+7] = sed+7;
        sed = sed + 0x8;
    }

    //BufferIndex++;
    Y_H_Block = 720;
    Y_V_Block = (480);
    for(j=0;j<Y_V_Block;j++)
    {
        for(i=0;i<Y_H_Block;i++)
        {
            Y_SequencyBuf[i+720*j] = Y_TailBuf[YLineAddr[i]+VIndx[j]];
        }
    }
#if 0
    Y_H_Block = 360;
    Y_V_Block = 240;
    for(j=0;j<Y_V_Block;j++)
    {
        for(i=0;i<Y_H_Block;i++)
        {
            U_SequencyBuf[i+360*j] = UV_TialBuf[UVLineAddr[i]+VCIndx[j]];
        }
    }

    Y_H_Block = 360;
    Y_V_Block = 240;
    for(j=0;j<Y_V_Block;j++)
    {
        for(i=0;i<Y_H_Block;i++)
        {
            V_SequencyBuf[i+360*j] = V_TialBuf[UVLineAddr[i]+VCIndx[j]];
        }
    }
#endif
}

void MHal_DIP_TailYUV420ToSequenceYUVBlock(void)
{
    U32 i,j,sub_i,sub_j;
    U32 Y_H_Block,Y_V_Block;
    U32 UV_H_Block,UV_V_Block;
    U32 BlockStartPos=0,SequencePos=0,TailPos=0;

    if( BufferIndex >= u8VerifyDiBufCnt )
        BufferIndex = 0;

    Y_TailBuf = (U8*)(u32VerifyDiBufStartAddress+((U32)BufferIndex*(720*480*3/2)));
    UV_TialBuf = (U8*)((u32VerifyDiBufStartAddress+720*480)+((U32)BufferIndex*(720*480*3/2)));

    //BufferIndex++;

    Y_H_Block = (720/16);
    Y_V_Block = (480/16);
    TailPos=0;

    for(i=0;i<Y_V_Block;i++)
        for(j=0;j<Y_H_Block;j++)
    {
        {
            //Y First Block Size 8*8 0f 16*16
            BlockStartPos = i*(Y_H_Block*256)+(j*16);
            for(sub_i=0;sub_i<8;sub_i++)
            {
                SequencePos = BlockStartPos + sub_i*720;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    Y_SequencyBuf[SequencePos+sub_j] = Y_TailBuf[TailPos];
                    TailPos++;
                }
            }

            //Y Second Block Size 8*8 0f 16*16
            BlockStartPos = i*(Y_H_Block*256)+(j*16)+8;
            for(sub_i=0;sub_i<8;sub_i++)
            {
                SequencePos = BlockStartPos + sub_i*720;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    Y_SequencyBuf[SequencePos+sub_j] = Y_TailBuf[TailPos];
                    TailPos++;
                }
            }

            //Y Third Block Size 8*8 0f 16*16
            BlockStartPos = (i*Y_H_Block*256)+(j*16)+(720*8);
            for(sub_i=0;sub_i<8;sub_i++)
            {
                SequencePos = BlockStartPos + sub_i*720;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    Y_SequencyBuf[SequencePos+sub_j] = Y_TailBuf[TailPos];
                    TailPos++;
                }
            }

            //Y Fourth Block Size 8*8 0f 16*16
            BlockStartPos = (i*Y_H_Block*256)+(j*16)+(720*8)+8;
            for(sub_i=0;sub_i<8;sub_i++)
            {
                SequencePos = BlockStartPos + sub_i*720;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    Y_SequencyBuf[SequencePos+sub_j] = Y_TailBuf[TailPos];
                    TailPos++;
                }
            }
        }//for(j=0;j<Y_H_Block;j++)
    }//for(i=0;i<Y_V_Block;i++)


    UV_H_Block = (360/8);
    UV_V_Block = (240/8);

    TailPos=0;
    for(i=0;i<UV_V_Block;i++)
        for(j=0;j<UV_H_Block;j++)
    {
        {
            BlockStartPos = i*(UV_H_Block*64)  + (j*8);

            for(sub_i=0;sub_i<8;sub_i++)//UV Block Size is 8*8
            {
                SequencePos = BlockStartPos + sub_i*360;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    U_SequencyBuf[SequencePos+sub_j] = UV_TialBuf[TailPos];
                    TailPos++;
                }

            }

            BlockStartPos = i*(UV_H_Block*64)  + (j*8);

            for(sub_i=0;sub_i<8;sub_i++)//UV Block Size is 8*8
            {
                SequencePos = BlockStartPos + sub_i*360;
                for(sub_j=0;sub_j<8;sub_j++)
                {
                    V_SequencyBuf[SequencePos+sub_j] = UV_TialBuf[TailPos];
                    TailPos++;
                }

            }
        }
    }
}

#endif//#if ENABLE_DI_BUFFER_VERIFY

