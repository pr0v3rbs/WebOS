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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stddef.h>   /* for size_t */
#include "MsCommon.h"
#include "drvMMIO.h"
#include "gif_lib.h"
#include "apiGPD.h"
#include "drvgpd.h"
#include "gpd.h"
#include "drvMIU.h"
#include "halCHIP.h"

#include "utopia.h"
#include "apiGPD_priv.h"
#include "apiGPD_v2.h"

void* pInstantGPD = NULL;

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------

#ifdef ANDROID
#ifndef LOGD
#define printf(format, args...)          ALOGI(format, ##args);
#else
#define printf(format, args...)          LOGI(format, ##args);
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#undef printf
#define printf(format, args...)          printk(format, ##args);
#endif
#endif

#if ENABLE_GPD_MESSAGE
#define DEBUG_GPD(y)   y
#else
#define DEBUG_GPD(y)   // y
#endif

#define LOG_DATA_TO_USB         FALSE

#ifdef GPD_UTOPIA_2K
    #define GPD_DRIVER_BASE KERNEL_MODE
#else
    #define GPD_DRIVER_BASE 0
#endif

//#define VERIFY


#ifndef VERIFY


#define GPD_XIU_BASE  (0x13F00 * 2)
#define GPD_RIU_BASE  (0x13E00 * 2)


#define MIU_CLI_BASE  (0x01200 * 2)
#define MIU_SEL_BASE  (0x00600 * 2)

#define STB_GPD_XIU_BASE  (0x10F00 * 2)
#define STB_GPD_RIU_BASE  (0x10E00 * 2)

#else

#define CBUFFER_SIZE         (1024*1024)
#define ZBUFFER_SIZE         (1024*1024)
#define STREAMBUFFER_SIZE  (4*1024*1024) //4MB
#define FRAMEBUFFER_SIZE  (22*1024*1024) //22MB


#define CBUFFER            (DRAMBASE + 0x11000000)
#define ZBUFFER            (CBUFFER + CBUFFER_SIZE)
#define STREAMBUFFER       (ZBUFFER + ZBUFFER_SIZE)
#define FRAMEBUFFER        (STREAMBUFFER + STREAMBUFFER_SIZE)
#define ARGBBUFFER         (FRAMEBUFFER + FRAMEBUFFER_SIZE)

#if (defined(__arm__))

#define HK_REG_BASE   0xFD200000

#else

#define HK_REG_BASE   0xBF200000

#endif

#define GPD_XIU_BASE  (HK_REG_BASE + 0x13F00 * 2)
#define GPD_RIU_BASE  (HK_REG_BASE + 0x13E00 * 2)

#if 0

#define CLK_GEN_BASE  (HK_REG_BASE + 0x00B00 * 2)

#else

#define CLK_GEN_BASE  (HK_REG_BASE + 0x03300 * 2)

#endif

#define MIU_CLI_BASE  (HK_REG_BASE + 0x01200 * 2)


#endif

#define HW_MAX_SUPPORTED_WIDTH          8192
#define HW_MAX_SUPPORTED_HEIGHT         8192
#define GPD_API_DUMP  0
//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
#define MMIO8(addr) (*((volatile unsigned char*)(addr)))

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
gpd_addr_info GpdAddrInfo;
gpd_pic_info Pparam;
U32 MIUAllocation = 0;
U32 g_gpd_set_access_region = FALSE;
U32 g_gpd_clock;
U32 u32CLK_OFFSET_BIT;
U32 RIU_REG_BASE;
//static U32 paMIUAddress = 0xFFFFFFFF;
//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------


extern void mdrv_gpd_setbuf(gpd_addr_info* addr_info);
extern S32 mdrv_gpd_init(gpd_addr_info* param);
extern S32 mdrv_gpd_decode(gpd_pic_info* pic_info, U32 Ocolor, U32 mode,gpd_roi_info* roi);
extern void mdrv_gpd_scaling_init(U32 enable, U32 mode);
U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size);

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
static MS_U32 u32FileIndex=0;
#endif

#if LOG_DATA_TO_USB
void GPD_WRITE_INPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','I','n', 'p', 'u', 't','_','0', '0', '0', '.', 'p', 'n', 'g',0};
    MS_U8 u8FilenameIndex = 10;

    fileName2[u8FilenameIndex+6]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Index%10+'0';

    FILE *inBinfp;
    if((inBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        printf("Can't open input png file\n");
    }
    else
    {
        printf("open input png file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, inBinfp);
    fclose(inBinfp);

}

void GPD_WRITE_OUTPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Width, MS_U32 u32Height, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','0','0', '0', '0', 'x','0','0', '0', '0', '_','0', '0', '0', '.', 'y', 'u', 'y', 'v',0};
    MS_U8 u8FilenameIndex = 10;
    fileName2[u8FilenameIndex]=u32Width/1000+'0';
    fileName2[u8FilenameIndex+1]=(u32Width/100)%10+'0';
    fileName2[u8FilenameIndex+2]=(u32Width/10)%10+'0';
    fileName2[u8FilenameIndex+3]=u32Width%10+'0';
    fileName2[u8FilenameIndex+5]=u32Height/1000+'0';
    fileName2[u8FilenameIndex+6]=(u32Height/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Height/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Height%10+'0';

    fileName2[u8FilenameIndex+10]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+11]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+12]=u32Index%10+'0';


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

static U32 _GPD_SetCacheable(U32 param)
{

    U32 cacheable;
    cacheable = (param == TRUE) ? TRUE: FALSE;
    drv_gpd_set_cacheable(cacheable);

    return TRUE;
}

static U32 _GPD_SetAccessRegion(gpd_access_region *param)
{
    U32 BankBaseAddr;
    U32 BankSize;
    U32 u32GPDRIUBase, u32GPDXIUBase;

    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);

    if(FALSE == MDrv_MMIO_GetBASE((MS_U32*)&BankBaseAddr, (MS_U32*)&BankSize, MS_MODULE_GPD))
    {
        printf("MDrv_MMIO_GetBASE FAILED !! \n");
        return FALSE;
    }

    GpdAddrInfo.RegBaseAddr = BankBaseAddr + u32GPDRIUBase;
    mdrv_gpd_setbuf(&GpdAddrInfo);
    drv_gpd_set_access_region(param->u32PA_StartAddr >> WORDALIGN, param->u32PA_EndAddr >> WORDALIGN);
    g_gpd_set_access_region = TRUE;

    DEBUG_GPD(printf("Set g_gpd_set_access_region to TRUE!!!!\n"));
    return TRUE;

}

static U32 _GPD_GetPitch(gpd_get_pitch_info *para)
{
    if(para == NULL)
    {
        printf("_GPD_GetPitch FAILED !! para error\n");
        return FALSE;
    }
    if(para->ocolor > GRAY_ALPHA)
    {
        printf("_GPD_GetPitch FAILED !! ocolor error\n");
        return FALSE;
    }
    if(para->u32Width == 0)
    {
        printf("_GPD_GetPitch FAILED !! u32Width error\n");
        return FALSE;
    }
    switch(para->ocolor)
    {
        case ARGB8888:
            para->u32Pitch = (((para->u32Width) + 1) & ~1);     // 2-align
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            para->u32Pitch = (((para->u32Width) + 3) & ~3);     // 4-align
            break;
        case GRAY:
            para->u32Pitch = (((para->u32Width) + 7) & ~7);     // 8-align
            break;
    }
    return TRUE;

}


#ifndef VERIFY
void verGPD_ISR_CB(void)
{
    printf("verGPD_ISR_CB()!!!!!!!!!\n");
}


void MApi_GPD_EnableISR(GPD_IsrFuncCb IsrCb)
{
    if(TRUE == MDrv_GPD_EnableISR((GPD_IsrFuncCb)IsrCb))
    {
    }
    else
    {
        printf("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Disable GPD ISR and unregister callback function.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
void MApi_GPD_DisableISR(void)
{
    if(TRUE == MDrv_GPD_DisableISR())
    {
    }
    else
    {
        printf("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}


void *_MApi_GPD_Init(U32 SrcBuf)
{
    U32 BankBaseAddr;
    U32 BankSize;
    U32 GPD_CLK_BASE, GPD_CLK_OFFSET;
    U32 u32GPDRIUBase, u32GPDXIUBase;

    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);
    DEBUG_GPD(printf("MApi_GPD_Init\n"));
    DEBUG_GPD(printf("GPD version:%x\n", GPD_VERSION));

    if(FALSE == MDrv_MMIO_GetBASE((MS_U32*)&BankBaseAddr, (MS_U32*)&BankSize, MS_MODULE_GPD))
    {
        printf("MDrv_MMIO_GetBASE FAILED !! \n");
        return FALSE;
    }

    RIU_REG_BASE = BankBaseAddr;
    DEBUG_GPD(printf("[%s] [%d] BankBaseAddr = %lx, BankSize = %lx\n", __FUNCTION__, __LINE__, BankBaseAddr, BankSize));

    //enable gpd clck

    GPD_CLK_BASE = drv_gpd_get_clkbase();

    GPD_CLK_OFFSET = drv_gpd_get_clkoffset();
    u32CLK_OFFSET_BIT = GPD_CLK_OFFSET>>24;
    GPD_CLK_OFFSET &= 0xffffff;

    DEBUG_GPD(printf("GPD_CLK_BASE=%lx, GPD_CLK_OFFSET=%lx\n", (MS_U32)GPD_CLK_BASE, (MS_U32)GPD_CLK_OFFSET));

    if (TRUE == g_gpd_set_access_region)
    {
        g_gpd_clock = (BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET);
        DEBUG_GPD(printf("Set g_gpd_clock to %lx!!!!(%lx %lx %lx)\n", (MS_U32)g_gpd_clock,
            BankBaseAddr, GPD_CLK_BASE, GPD_CLK_OFFSET));
    }
    else
    {
        MS_U32 u32RegValue;
        MS_U32 u32Mask;
#if 1
        DEBUG_GPD(printf("@@@set gpd_clk to 216MHz\n"));
        u32RegValue = *((volatile U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET));
        u32Mask = ~(0x1<<u32CLK_OFFSET_BIT | 0x1<<(u32CLK_OFFSET_BIT+1) | 0x1<<(u32CLK_OFFSET_BIT+2) | 0x1<<(u32CLK_OFFSET_BIT+3) | 0x1<<(u32CLK_OFFSET_BIT+4));
        u32RegValue &= u32Mask;

        *((volatile U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = u32RegValue;
#elif 0
        DEBUG_GPD(printf("@@@set gpd_clk to 192MHz\n"));
        *((volatile U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0x4;
#else
        DEBUG_GPD(printf("@@@set gpd_clk to 144MHz\n"));
        *((volatile U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0xc;
#endif
    }

    drv_gpd_init_outside_reg(BankBaseAddr);

#if 0
    U32 u32Offset = 0;
    _phy_to_miu_offset(MIUAllocation, u32Offset, SrcBuf);
    if(u32Offset == 0)
    {
    }
    switch(MIUAllocation)
    {
        case E_CHIP_MIU_0:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
        case E_CHIP_MIU_1:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU1);
            break;
        case E_CHIP_MIU_2:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU2);
            break;
        case E_CHIP_MIU_3:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU3);
            break;
        default:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
    }
#else
    unsigned int tmp = 0;
    U8 u8MIU_Offset;
    U16 u16MIU_Mask;
    MDrv_GPD_MIU_Client(&u8MIU_Offset, &u16MIU_Mask);

    //check miu sel is controlled by HW or MIU
    //GPD MIU select control is
    tmp = *((volatile U32 *)(BankBaseAddr + MIU_SEL_BASE + (((U32)u8MIU_Offset)<< 1)));
    tmp &= (~u16MIU_Mask);
    *((volatile U32 *)(BankBaseAddr + MIU_SEL_BASE + (((U32)u8MIU_Offset) << 1))) = tmp;

    tmp = *((volatile U32 *)(BankBaseAddr + MIU_CLI_BASE + (((U32)u8MIU_Offset) << 1)));

    DEBUG_GPD(printf("GPD MIU value = %x\n", tmp));
    DEBUG_GPD(printf("SrcBuf :0x%lx\n", SrcBuf));

    MIUAllocation = MDrv_GPD_MIU_Select(SrcBuf);
    if(MIUAllocation)
    {
        tmp |= (u16MIU_Mask);
        DEBUG_GPD(printf("Select MIU1\n"));
    }
    else
    {
        tmp &= (~u16MIU_Mask);
        DEBUG_GPD(printf("Select MIU0\n"));
    }

    *((volatile U32 *)(BankBaseAddr + MIU_CLI_BASE +(((U32)u8MIU_Offset) << 1))) = tmp;
#endif

    GpdAddrInfo.RegBaseAddr = BankBaseAddr + u32GPDRIUBase;
    GpdAddrInfo.ReadBitBaseAddr = BankBaseAddr + u32GPDXIUBase;

    GpdAddrInfo.u32PA_ColorBufferAddr = SrcBuf;

    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ColorBufferAddr);
        GpdAddrInfo.u32PA_DepthBufferAddr = GpdAddrInfo.u32PA_ColorBufferAddr + 512 * 1024;
        GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_DepthBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_ColorBufferAddr);
        GpdAddrInfo.u32PA_DepthBufferAddr = GpdAddrInfo.u32PA_ColorBufferAddr + 512 * 1024;
        GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_DepthBufferAddr);
    }
#if GPD_API_DUMP
    memset((void *)GpdAddrInfo.u32VA_ColorBufferAddr, 0x77, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x10), 0x88, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x20), 0x99, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x30), 0xaa, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x40), 0xbb, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x50), 0xcc, 0x10);
    MsOS_FlushMemory();
#endif
    mdrv_gpd_init(&GpdAddrInfo);
#if 0
    MApi_GPD_DisableISR();
    MApi_GPD_EnableISR((GPD_IsrFuncCb)verGPD_ISR_CB);
    MDrv_GPD_SetISRMask();
#endif
    #if GPD_API_DUMP
    memset((void *)GpdAddrInfo.u32VA_ColorBufferAddr, 0x34, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x10), 0x45, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x20), 0x56, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x30), 0x67, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x40), 0x78, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x50), 0x89, 0x10);
    MsOS_FlushMemory();
    verifyBitstream3(GpdAddrInfo.u32PA_ColorBufferAddr, 0x50);

    memset((void *)GpdAddrInfo.u32VA_ColorBufferAddr, 0x43, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x10), 0x54, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x20), 0x65, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x30), 0x76, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x40), 0x87, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x50), 0x98, 0x10);
    verifyBitstream3(GpdAddrInfo.u32PA_ColorBufferAddr, 0x50);

    #endif
    return &GpdAddrInfo;

}

S32 _MApi_GPD_InputSource(gpd_pic_info* param, U32 InputBuf, U32 Size)
{
    U8 u8MiuSel = 0;
    U32 u32Offset = 0;
#if GPD_API_DUMP
    verifyBitstream3(GpdAddrInfo.u32PA_ColorBufferAddr, 0x50);
    memset((void *)GpdAddrInfo.u32VA_ColorBufferAddr, 0x9a, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x10), 0xab, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x20), 0xbc, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x30), 0xcd, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x40), 0xde, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x50), 0xef, 0x10);
    MsOS_FlushMemory();
    verifyBitstream3(GpdAddrInfo.u32PA_ColorBufferAddr, 0x50);
#endif
    _phy_to_miu_offset(u8MiuSel, u32Offset, InputBuf);
    if(u32Offset == 0)
    {
    }
    if(MIUAllocation != u8MiuSel)
    {
        DEBUG_GPD(printf("Wrong Memory Allocation\n"));
        return -1;
    }

    DEBUG_GPD(printf("MApi_GPD_ParseHeader\n"));
    GpdAddrInfo.u32PA_ReadBufferAddr = InputBuf;

    DEBUG_GPD(printf("InputBuf=%lx\n", InputBuf));

    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ReadBufferAddr);
        DEBUG_GPD(printf("MsOS_PA2KSEG1(InputBuf)=%lx\n", MsOS_PA2KSEG1(InputBuf)));
    }
    else
    {
        GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_ReadBufferAddr);
        DEBUG_GPD(printf("MsOS_PA2KSEG0(InputBuf)=%lx\n", MsOS_PA2KSEG0(InputBuf)));
    }
    GpdAddrInfo.u32ReadBufferSize = Size;

#if GPD_API_DUMP

    memset((void *)GpdAddrInfo.u32VA_ColorBufferAddr, 0x11, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x10), 0x22, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x20), 0x33, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x30), 0x44, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x40), 0x55, 0x10);
    memset((void *)(GpdAddrInfo.u32VA_ColorBufferAddr+0x50), 0x66, 0x10);
    MsOS_FlushMemory();
    verifyBitstream3(GpdAddrInfo.u32PA_ColorBufferAddr, 0x50);
    verifyBitstream3(InputBuf, 16);
    verifyBitstream3(InputBuf, 32);
    verifyBitstream3(InputBuf, 48);
    verifyBitstream3(InputBuf, 64);
    verifyBitstream3(InputBuf, 80);
#endif


#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
    u32FileIndex++;
#if LOG_DATA_TO_USB
    GPD_WRITE_INPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)Size, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    printf("[%s][addr=0x%lx, size=0x%lx] Input CRC result=0x%lx\n", __FUNCTION__,
        (MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)GpdAddrInfo.u32ReadBufferSize,
        (MS_U32)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        (MS_U32)GpdAddrInfo.u32ReadBufferSize));
#endif
#endif


    mdrv_gpd_setbuf(&GpdAddrInfo);
    mdrv_gpd_decode(param,0,1,NULL);
    if(param->u32Height > HW_MAX_SUPPORTED_HEIGHT || param->u32Width >HW_MAX_SUPPORTED_WIDTH)
    {
        param->enGPDStatus = -2;
    }
    memcpy((void*)(&Pparam),(void*)(param),sizeof(gpd_pic_info));
    return param->enGPDStatus;

}

#if ENABLE_GPD_MESSAGE
static void MApi_GPD_PrintMem(MS_U32 u32Addr, MS_U32 u32Size)
{
    MS_U32 u32i;
    printf("===========================================================\n");
    printf("print memory addr=0x%lx, size=0x%lx\n", u32Addr, u32Size);
    printf("===========================================================\n");

#if 0
    for(u32i=0; u32i<u32Size/8+((u32Size%8)? 1:0); u32i++)
    {
        printf("%02x %02x %02x %02x %02x %02x %02x %02x \n",
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
#endif


S32 _MApi_GPD_OutputDecode(U32 DecodeBuf, U32 ocolor, U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    U8 u8MiuSel = 0;
    U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    if(u32Offset == 0)
    {
    }
    if(MIUAllocation != u8MiuSel)
    {
        DEBUG_GPD(printf("Wrong Memory Allocation\n"));
        return -1;
    }
    gpd_roi_info roi;
    DEBUG_GPD(printf("MApi_GPD_Decode\n"));
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    GpdAddrInfo.u32WriteBufferSize = Size;

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);

    DEBUG_GPD(printf("GPDBaseAddr=0x%lx\n",GpdAddrInfo.RegBaseAddr));
    DEBUG_GPD(printf("READBITBaseAddr=0x%lx\n",GpdAddrInfo.ReadBitBaseAddr));
    DEBUG_GPD(printf("Read  buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ReadBufferAddr,GpdAddrInfo.u32VA_ReadBufferAddr));
    DEBUG_GPD(printf("Write buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32VA_WriteBufferAddr));
    DEBUG_GPD(printf("Color buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ColorBufferAddr,GpdAddrInfo.u32VA_ColorBufferAddr));
    DEBUG_GPD(printf("Depth buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_DepthBufferAddr,GpdAddrInfo.u32VA_DepthBufferAddr));
    DEBUG_GPD(printf("Read  buf size=0x%lx\n", GpdAddrInfo.u32ReadBufferSize));
    DEBUG_GPD(printf("Write buf size=0x%lx\n", GpdAddrInfo.u32WriteBufferSize));

#if ENABLE_GPD_MESSAGE
    memset((void *)GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)GpdAddrInfo.u32WriteBufferSize);
#endif

    mdrv_gpd_decode(&Pparam, ocolor, 0, &roi);

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(Pparam.u32Height)*(Pparam.u32Width)*u8BytesPerPixel, Pparam.u32Width, Pparam.u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    printf("[%s, %ld, %ldx%ld], CRC result=0x%lx\n", __FUNCTION__, u32FileIndex, Pparam.u32Width, Pparam.u32Height,
        MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr,(MS_U32) (Pparam.u32Height)*(Pparam.u32Width)*u8BytesPerPixel, Pparam.u32Width, ocolor));
    printf("[%s][addr=0x%lx, size=0x%lx] Input CRC result=0x%lx\n", __FUNCTION__,
        (MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)GpdAddrInfo.u32ReadBufferSize,
        (MS_U32)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        (MS_U32)GpdAddrInfo.u32ReadBufferSize));


#endif
#endif

    return Pparam.enGPDStatus;


}

S32 _MApi_GPD_OutputDecodeMGIF(U32 DecodeBuf, U32 ocolor, U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    U8 u8MiuSel = 0;
    U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    if(u32Offset == 0)
    {
    }
    if(MIUAllocation != u8MiuSel)
    {
        DEBUG_GPD(printf("Wrong Memory Allocation\n"));
        return -1;
    }
    gpd_roi_info roi;
    DEBUG_GPD(printf("MApi_GPD_Decode\n"));
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    GpdAddrInfo.u32WriteBufferSize = Size;

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);

    DEBUG_GPD(printf("GPDBaseAddr=0x%lx\n",GpdAddrInfo.RegBaseAddr));
    DEBUG_GPD(printf("READBITBaseAddr=0x%lx\n",GpdAddrInfo.ReadBitBaseAddr));
    DEBUG_GPD(printf("Read  buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ReadBufferAddr,GpdAddrInfo.u32VA_ReadBufferAddr));
    DEBUG_GPD(printf("Write buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32VA_WriteBufferAddr));
    DEBUG_GPD(printf("Color buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ColorBufferAddr,GpdAddrInfo.u32VA_ColorBufferAddr));
    DEBUG_GPD(printf("Depth buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_DepthBufferAddr,GpdAddrInfo.u32VA_DepthBufferAddr));
    DEBUG_GPD(printf("Read  buf size=0x%lx\n", GpdAddrInfo.u32ReadBufferSize));
    DEBUG_GPD(printf("Write buf size=0x%lx\n", GpdAddrInfo.u32WriteBufferSize));

    mdrv_gpd_decode(&Pparam, ocolor, 3, &roi);

#if (ENABLE_GPD_MESSAGE ||LOG_DATA_TO_USB)
    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(Pparam.u32Height)*(Pparam->u32Width)*u8BytesPerPixel, Pparam->u32Width, Pparam->u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    printf("[%s, %ld, %ldx%ld], CRC result=0x%lx\n", __FUNCTION__, u32FileIndex, Pparam.u32Width, Pparam.u32Height,
        MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr,(MS_U32) (Pparam.u32Height)*(Pparam.u32Width)*u8BytesPerPixel, Pparam.u32Width, ocolor));
    printf("[%s][addr=0x%lx, size=0x%lx] Input CRC result=0x%lx\n", __FUNCTION__,
        (MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)GpdAddrInfo.u32ReadBufferSize,
        (MS_U32)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        (MS_U32)GpdAddrInfo.u32ReadBufferSize));

#endif

#endif

    return Pparam.enGPDStatus;


}

S32 _MApi_GPD_GetDuration(void)
{
    return MstDGifGetDuration();
}

void _MApi_GPD_SetGIFMode(int mode)
{
    MstDGifSetMode(mode);
}



S32 _MApi_GPD_OutputDecodeROI(U32 DecodeBuf, U32 ocolor, U32 Size, gpd_roi_info *roi)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    U8 u8MiuSel = 0;
    U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    if(u32Offset == 0)
    {
    }
    if(MIUAllocation != u8MiuSel)
    {
        DEBUG_GPD(printf("Wrong Memory Allocation\n"));
        return -1;
    }

    DEBUG_GPD(printf("MApi_GPD_DecodeROI\n"));
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    GpdAddrInfo.u32WriteBufferSize = Size;

    mdrv_gpd_setbuf(&GpdAddrInfo);

    DEBUG_GPD(printf("GPDBaseAddr=0x%lx\n",GpdAddrInfo.RegBaseAddr));
    DEBUG_GPD(printf("READBITBaseAddr=0x%lx\n",GpdAddrInfo.ReadBitBaseAddr));
    DEBUG_GPD(printf("Read  buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ReadBufferAddr,GpdAddrInfo.u32VA_ReadBufferAddr));
    DEBUG_GPD(printf("Write buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32VA_WriteBufferAddr));
    DEBUG_GPD(printf("Color buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_ColorBufferAddr,GpdAddrInfo.u32VA_ColorBufferAddr));
    DEBUG_GPD(printf("Depth buf PA=0x%lx,VA=0x%lx\n",GpdAddrInfo.u32PA_DepthBufferAddr,GpdAddrInfo.u32VA_DepthBufferAddr));
    DEBUG_GPD(printf("Read  buf size=0x%lx\n", GpdAddrInfo.u32ReadBufferSize));
    DEBUG_GPD(printf("Write buf size=0x%lx\n", GpdAddrInfo.u32WriteBufferSize));

#if ENABLE_GPD_MESSAGE
    memset((void *)GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)GpdAddrInfo.u32WriteBufferSize);
#endif
    mdrv_gpd_decode(&Pparam, ocolor, 2, roi);

#if (ENABLE_GPD_MESSAGE || LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(roi->height)*(roi->width)*u8BytesPerPixel, roi->width, roi->height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    MS_U32 u32OutputCRC = MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr,(MS_U32) (roi->height)*(roi->width)*u8BytesPerPixel, roi->width, ocolor);
    printf("[%s, %ld, %ldx%ld], CRC result=0x%lx\n", __FUNCTION__, u32FileIndex, roi->width, roi->height,
        u32OutputCRC);
    printf("[%s][addr=0x%lx, size=0x%lx] Input CRC result=0x%lx\n", __FUNCTION__,
        (MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)GpdAddrInfo.u32ReadBufferSize,
        (MS_U32)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        (MS_U32)GpdAddrInfo.u32ReadBufferSize));

#endif
#endif

    return Pparam.enGPDStatus;


}

#else

void* MAdp_GPD_Init(void)
{
    S32 err;
    //U32 size=0;
    S32 tmp;

    console_printf("MAdp_GPD_Init\n");

    GpdAddrInfo.RegBaseAddr = (U32)GPD_RIU_BASE;
    GpdAddrInfo.ReadBitBaseAddr = (U32)GPD_XIU_BASE;

#if 0

   *((volatile U32 *)(CLK_GEN_BASE + (0x5E << 1))) = 0;   // 0x2f << 2

#else

    *((volatile U32 *) (CLK_GEN_BASE + (0x50 << 1))) = 0;   // 0x28 << 2
    //*((volatile U32 *) (CLK_GEN_BASE + (0x50 << 1))) = (2 << 2);   // 0x28 << 2

#endif

    //set gpd miu channel

#if 1
    tmp = *((volatile U32 *)((U32)MIU_CLI_BASE + (0xf4 << 1)));


    if ((U32)CBUFFER & (1 << 29))  //512mb
    {
        tmp |= (0x40);  // bit 6
        console_printf("Select MIU1\n");
    }
    else
    {
        tmp &= (~0x40);
        console_printf("Select MIU0\n");

    }

    *((volatile U32 *)((U32)MIU_CLI_BASE +(0xf4 << 1))) = tmp;
#endif

#if 0
    *((volatile U32 *)((U32)MIU_CLI_BASE +(0x67 << 2))) = 0x26;

    *((volatile U32 *)((U32)MIU_CLI_BASE +(0x68 << 2))) = 0x8800;

    *((volatile U32 *)((U32)MIU_CLI_BASE +(0x69 << 2))) = 0x9800;

    *((volatile U32 *)((U32)MIU_CLI_BASE +(0x60 << 2))) = 0x2;

    console_printf("Enable MIU Protection\n");
#endif

    GpdAddrInfo.u32PA_ColorBufferAddr = (U32)CBUFFER;
    GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ColorBufferAddr);

    console_printf("Get Color buffer\n");

    GpdAddrInfo.u32PA_DepthBufferAddr = (U32)ZBUFFER;
    GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_DepthBufferAddr);

    console_printf("Get Depth buffer\n");

    GpdAddrInfo.u32PA_ReadBufferAddr = (U32)STREAMBUFFER;
    GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ReadBufferAddr);

    console_printf("Get Read buffer\n");

    GpdAddrInfo.u32PA_WriteBufferAddr = (U32)FRAMEBUFFER;
    GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);

    console_printf("Get Write buffer\n");

    GpdAddrInfo.u32WriteBufferSize = FRAMEBUFFER_SIZE;


    console_printf("GPDBaseAddr=0x%08x\n",GpdAddrInfo.RegBaseAddr);
    console_printf("READBITBaseAddr=0x%08x\n",GpdAddrInfo.ReadBitBaseAddr);
    console_printf("Read  buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_ReadBufferAddr,GpdAddrInfo.u32VA_ReadBufferAddr);
    console_printf("Write buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32VA_WriteBufferAddr);
    console_printf("Color buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_ColorBufferAddr,GpdAddrInfo.u32VA_ColorBufferAddr);
    console_printf("Depth buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_DepthBufferAddr,GpdAddrInfo.u32VA_DepthBufferAddr);
    console_printf("Read  buf size=0x%08x\n", GpdAddrInfo.u32ReadBufferSize);
    console_printf("Write buf size=0x%08x\n", GpdAddrInfo.u32WriteBufferSize);


    //Call mdrv_gpd_init, This should include Color-buffer, Z-buffer, Bitstream buffer, Frame Buffer.
    err = mdrv_gpd_init(&GpdAddrInfo);



    return &GpdAddrInfo;

}

S32 MAdp_GPD_Play(gpd_pic_info* param, U32 ocolor, U8 *srcbuffer, U32 size)
{

    extern void *memcpy(void *destination, const void *source, size_t num);
    //FILE *hFile = 0;
    //long lSize,i=0;
    //size_t result;
    //int i = 0;
    gpd_roi_info roi;
    //U8 sig[10];
    //FILE *fp;

    console_printf("MAdp_GPD_Play\n");

    Pparam = param;
    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);


    #if 1

    console_printf("size:%d\n",size);

    memcpy((void *)GpdAddrInfo.u32VA_ReadBufferAddr, (void *)srcbuffer, size);

    GpdAddrInfo.u32ReadBufferSize = size;

    console_printf("copy image done\n");

    #endif


    #if 0
    hFile = fopen(param->filename, "rb");
    if(!hFile)
    {
        console_printf("Open file error=%s!!\n",param->filename);
        return -1;
    }
    // obtain file size:
    fseek (hFile , 0 , SEEK_END);
    lSize = ftell (hFile);
    rewind (hFile);

    //fread file into bitstream buffer
    result = fread ((char *)GpdAddrInfo.u32VA_ReadBufferAddr,1,lSize,hFile);

    #endif

#if 0
    sig[0] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr);
    sig[1] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+1);
    sig[2] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+2);
    sig[3] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+3);
    console_printf("Signaute = %x,%x,%x,%x\n",sig[0],sig[1],sig[2],sig[3]);
#endif
    //Call mdrv_gpd_decode, specify output format;ARGB888/YUV422/RGB565

    #if 0
    roi.hstart = 0;
    roi.vstart = 0;
    roi.width = 64;
    roi.height = 64;
    #endif

    mdrv_gpd_decode(Pparam, ocolor, 0, &roi);

    return Pparam.enGPDStatus;
}


S32 MAdp_GPD_Compare(U32 ocolor_typ)
{

    extern int memcmp(const void *ptr1, const void *ptr2, size_t num);

    unsigned int frameptr, argbptr;

    unsigned int hw_extra = 0, sw_extra = 0;

    unsigned int pixelbytes = 0;

    unsigned int width, height;

    unsigned int result = 0;

    frameptr = MsOS_PA2KSEG1(FRAMEBUFFER);

    argbptr = MsOS_PA2KSEG1(ARGBBUFFER);

    console_printf("MAdp_GPD_Compare\n");

    #if 0

    console_printf("size:%d\n",size);

    memcpy((void *)ARGBBUFFER,(void *)srcbuffer,size);

    console_printf("copy image done\n");

    #endif

    width = Pparam.u32Width;

    height = Pparam.u32Height;

    console_printf("height:%d width:%d\n", height, width);


    switch (ocolor_typ)
    {
    case ARGB8888:
        pixelbytes = 4;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case YUV422:
    case YVYU422:
        if(width & 1)
            sw_extra = 2;
    case RGB565:
    case GRAY_ALPHA:
        pixelbytes = 2;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case GRAY:
        pixelbytes = 1;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    default:
        console_printf("Unsupported output color %d\n", ocolor_typ);
        return NULL;
    }


    while(height)
    {

        result = memcmp((void *) frameptr,(void *) argbptr, width * pixelbytes);

        if(result)
        {
            console_printf("compare failed\n");

            return result;
        }

        frameptr += width * pixelbytes + hw_extra;

        argbptr += width * pixelbytes + sw_extra;

        height--;
    }

    return 0;

}


#endif

void _MApi_GPD_ScalingEnable(U32 mode)
{


    mdrv_gpd_scaling_init(1, mode);

    DEBUG_GPD(printf("GPD Scaling Enable, mode = %ld\n", mode));
}



void _MApi_GPD_ScalingDisable(void)
{

    mdrv_gpd_scaling_init(0, 0);

    DEBUG_GPD(printf("GPD Scaling Disable\n"));
}


U32 _MApi_GPD_SetControl(gpd_user_cmd cmd_id, U32 param)
{
    switch (cmd_id)
    {
        case E_GPD_USER_CMD_SET_ACCESS_REGION:
            return _GPD_SetAccessRegion((gpd_access_region *)param);

        case E_GPD_USER_CMD_SET_CACHEABLE:
            return _GPD_SetCacheable((U32)param);

        default:
            return FALSE;

    }

}


U32 _MApi_GPD_GetControl(gpd_get_cmd cmd_id, U32* param)
{
    switch (cmd_id)
    {
        case E_GPD_GET_CMD_GET_PITCH:
            return _GPD_GetPitch((gpd_get_pitch_info *)param);
        default:
            return FALSE;
    }

}

static unsigned long  u32CRC;

static void verGPD_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verGPD_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Init_CRC_Table(MS_U32 *u32CRCtemptable)
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

static void verGPD_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, unsigned int ocolor)
{
    if (pu8Data == NULL || u32Size == 0) return;
    MS_U32  u32LoopCounter;
    MS_U8 u8BytesPerPixel;
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 *u32CRCTable = (MS_U32 *)kmalloc(sizeof(MS_U32)*256, GFP_KERNEL);
#else
    MS_U32 *u32CRCTable = (MS_U32 *)malloc(sizeof(MS_U32)*256);
#endif

    if(u32CRCTable == NULL) return;

    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

    verGPD_Init_CRC_Table(u32CRCTable);

    if((u32Width*u8BytesPerPixel)%8 ==0)
    {
        for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
        {
            u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (unsigned char)(u32CRC & 0xFF) ];
        }
    }
    else
    {
        MS_U32 u32LoopCounter2;
        MS_U32 u32WidthAlign;
        u32WidthAlign = u32Width + ((u32Width*u8BytesPerPixel)%8)/u8BytesPerPixel;

        for (u32LoopCounter=0; u32LoopCounter<u32Size/(u32Width*u8BytesPerPixel)*(u32WidthAlign*u8BytesPerPixel); u32LoopCounter+=u32WidthAlign*u8BytesPerPixel)
            for(u32LoopCounter2=0; u32LoopCounter2<u32Width*4; u32LoopCounter2++)
            {
                u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter+u32LoopCounter2] ^ (unsigned char)(u32CRC & 0xFF) ];
            }
    }
#ifdef MSOS_TYPE_LINUX_KERNEL
    kfree(u32CRCTable);
#else
    free(u32CRCTable);
#endif
}

U32 _MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, unsigned int ocolor)
{
    verGPD_CRC32_Init();
    verGPD_CRC32_Update(pu8Data, u32Size, u32Width, ocolor);
    return verGPD_CRC32_GetResult();
}



static void verGPD_Input_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verGPD_Input_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Input_Init_CRC_Table(MS_U32 *u32CRCtemptable)
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

static void verGPD_Input_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    if (pu8Data == NULL || u32Size == 0) return;
    MS_U32  u32LoopCounter;
#ifdef MSOS_TYPE_LINUX_KERNEL
        MS_U32 *u32CRCTable = (MS_U32 *)kmalloc(sizeof(MS_U32)*256, GFP_KERNEL);
#else
        MS_U32 *u32CRCTable = (MS_U32 *)malloc(sizeof(MS_U32)*256);
#endif
    if (u32CRCTable == NULL) return;

    verGPD_Input_Init_CRC_Table(u32CRCTable);

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
#ifdef MSOS_TYPE_LINUX_KERNEL
        kfree(u32CRCTable);
#else
        free(u32CRCTable);
#endif

}

U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    verGPD_Input_CRC32_Init();
    verGPD_Input_CRC32_Update(pu8Data, u32Size);
    return verGPD_Input_CRC32_GetResult();
}

void *MApi_GPD_Init(U32 SrcBuf)
{
#if (GPD_UTOPIA20)
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            printf("Open GPD fail\n");
            return FALSE;
        }
    }

    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    //MS_U32 u32Temp = 0;
    GPD_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(GPD_INIT_PARAM));
    InitPara.SrcBuf = SrcBuf;
    //InitPara.retVal = (void *)&u32Temp;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_Init\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_Init, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return FALSE;
#else
    return _MApi_GPD_Init(SrcBuf);
#endif

}


S32 MApi_GPD_InputSource(gpd_pic_info* param, U32 InputBuf, U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_INPUTSOURCE_PARAM InputPara;
    memset(&InputPara , 0 ,sizeof(GPD_INPUTSOURCE_PARAM));
    InputPara.param = param;
    InputPara.InputBuf = InputBuf;
    InputPara.Size = Size;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_InputSource\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_InputSource, (void*)(&InputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return InputPara.retVal;
#else
    return _MApi_GPD_InputSource(param, InputBuf, Size);
#endif
}

S32 MApi_GPD_OutputDecode(U32 DecodeBuf, U32 ocolor, U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_OutputDecode\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecode, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        #if 0
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        else
        {
            pInstantGPD = NULL;
        }
        #endif
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecode(DecodeBuf, ocolor, Size);
#endif
}

S32 MApi_GPD_OutputDecodeMGIF(U32 DecodeBuf, U32 ocolor, U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeMGIF\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeMGIF, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        #if 0
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        else
        {
            pInstantGPD = NULL;
        }
        #endif
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecodeMGIF(DecodeBuf, ocolor, Size);
#endif
}

S32 MApi_GPD_GetDuration(void)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETDURATION_PARAM GetDuration;
    memset(&GetDuration , 0 ,sizeof(GPD_GETDURATION_PARAM));
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_GetDuration\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetDuration, (void*)(&GetDuration)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return GetDuration.retVal;
#else
    return _MApi_GPD_GetDuration();
#endif

}


void MApi_GPD_SetGIFMode(int mode)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SETGIFMODE_PARAM GIFMode;
    memset(&GIFMode , 0 ,sizeof(GPD_SETGIFMODE_PARAM));
    GIFMode.mode = mode;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_SetGIFMode\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetGIFMode, (void*)(&GIFMode)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_SetGIFMode(mode);
#endif

}

S32 MApi_GPD_OutputDecodeROI(U32 DecodeBuf, U32 ocolor, U32 Size, gpd_roi_info *roi)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODEROI_PARAM OutputParaROI;
    memset(&OutputParaROI , 0 ,sizeof(GPD_OUTPUTDECODEROI_PARAM));
    OutputParaROI.DecodeBuf = DecodeBuf;
    OutputParaROI.ocolor = ocolor;
    OutputParaROI.Size = Size;
    OutputParaROI.roi = roi;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeROI\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeROI, (void*)(&OutputParaROI)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        #if 0
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Close GPD failed\n");
            return OutputParaROI.retVal;
        }
        else
        {
            pInstantGPD = NULL;
        }
        #endif
    }
    return OutputParaROI.retVal;
#else
    return _MApi_GPD_OutputDecodeROI(DecodeBuf, ocolor, Size, roi);
#endif

}

void MApi_GPD_ScalingEnable(U32 mode)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SCALINGENABLE_PARAM ScalingEnable;
    memset(&ScalingEnable , 0 ,sizeof(GPD_SCALINGENABLE_PARAM));
    ScalingEnable.mode = mode;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_ScalingEnable\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingEnable, (void*)(&ScalingEnable)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_ScalingEnable(mode);
#endif

}

void MApi_GPD_ScalingDisable(void)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_ScalingDisable\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingDisable, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_ScalingDisable();
#endif
}

U32 MApi_GPD_SetControl(gpd_user_cmd cmd_id, U32 param)
{
#if (GPD_UTOPIA20)
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            printf("Open GPD fail\n");
            return FALSE;
        }
    }

    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_SetControl\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_SetControl(cmd_id, param);
#endif

}

U32 MApi_GPD_GetControl(gpd_get_cmd cmd_id, U32* param)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GET_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_GET_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_GetControl\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_GetControl(cmd_id, param);
#endif

}

U32 MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, unsigned int ocolor)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETCRCRESULT_PARAM CRCPara;
    memset(&CRCPara , 0 ,sizeof(GPD_GETCRCRESULT_PARAM));
    CRCPara.pu8Data = pu8Data;
    CRCPara.u32Size = u32Size;
    CRCPara.u32Width = u32Width;
    CRCPara.ocolor = ocolor;
    DEBUG_GPD(printf("UtopiaIoctl(): MApi_CMD_GPD_GetCRCResult\n"));
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetCRCResult, (void*)(&CRCPara)) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CRCPara.retVal;
#else
    return _MApi_GPD_GetCRCResult(pu8Data, u32Size, u32Width, ocolor);
#endif

}

