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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// Common Definition
#ifndef MHAL_DYNAMICSCALING_C
#define MHAL_DYNAMICSCALING_C

#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "halCHIP.h"
#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_dynamicscaling.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "mhal_menuload.h"
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#else
#define assert(b)   \
    do {    \
        if (!(b)){  \
            printf("assert at [%s][%d] \n", __FUNCTION__, __LINE__);   \
            *((int*)0) = 0; \
        }   \
    } while(0)
#endif

#define DS_DBG(x)   //x
#define SLZ_DBG(x)  //x
#if SUPPORT_SEAMLESS_ZAPPING
static void _Hal_SC_ResetDSCommand(SCALER_WIN eWindow, MS_U8 u8DSIndex)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;
    MS_U32 u32IndexBase = pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr + (MS_U32)u8DSIndex * pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT;
    MS_U32 *pu32Data = NULL;
    int i = 0;

    SLZ_DBG(printf("@@#@@[%s]pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr=%lx, pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth=%u\n",
        __FUNCTION__,
        pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr,
        pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel]));
    SLZ_DBG(printf("@@#@@u32IndexBase=%lx\n",u32IndexBase));
    for(i = 0; i < (pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT)/4; i++)
    {
        if(i % 8 < 4)
        {
            if(MAIN_WINDOW == eWindow)
            {
                if(i % 4 < 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_1 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                    }
                    *pu32Data = 0xFFFF0000;
                }
            }
            else //SUB_WINDOW == eWindow
            {
                if(i % 4 >= 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_2 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                    }
                    *pu32Data = 0xFFFF0000;
                }
            }
        }
        else
        {
            if(MAIN_WINDOW == eWindow)
            {
                if(i % 4 < 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_3 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                    }
                    *pu32Data = 0xFFFF0000;//0;
                }
            }
            else //SUB_WINDOW == eWindow
            {
                if(i % 4 >= 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_4 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                    }
                    *pu32Data = 0xFFFF0000;
                }
            }
        }
    }
}

static void _Hal_SC_WriteDSCommand(SCALER_WIN eWindow, MS_U16 u16CmdRegAddr, MS_U16 u16CmdRegValue)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;
    MS_U32 u32Addr = pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr + pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[eWindow] * pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT;
    MS_U32 *pu32Data = NULL;

    MS_U8 u8Count = pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel];
    MS_U32 u32Command = 0;

    if(SUB_WINDOW == eWindow)
    {
        u32Addr += 8; // Move to OP sub command
    }

    u32Command = u16CmdRegValue + ((u16CmdRegAddr & 0x00FF) << 15) + ((u16CmdRegAddr & 0xFF00) << 16);

    while(u8Count > 0)
    {
        pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32Addr);
        if(0 == pu32Data)
        {
            printf("%s :pu32Data = 0!,error!!!\n", __FUNCTION__);
            assert(pu32Data != 0);
        }
        // Found null command
        if(*pu32Data == 0xFFFF0000)
        {
            break;
        }
        // Found duplicated command
        if(*pu32Data == u32Command)
        {
            DS_DBG(printf("Command duplicated\n"));
            return;
        }
        u32Addr += MS_MLOAD_MEM_BASE_UNIT;
        u8Count--;
    }

    if(*pu32Data == 0xFFFF0000)
    {
        *pu32Data = u32Command;
    }
    else
    {
        printf("DS buffer is full for this index %u!\n", pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[eWindow]);
    }
    //printf("@@[%s][%u]u16CmdRegAddr=%x, u16CmdRegValue=%x\n",__FUNCTION__, __LINE__, u16CmdRegAddr, u16CmdRegValue);
}

void Hal_SC_PrepareSeamlessZapping(SCALER_WIN eWindow )
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));


    SLZ_DBG(printf("@@#@@1.[%s]@@FRZ=%x,pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[MAIN_WINDOW]=%x, pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[MAIN_WINDOW]=%x\n",
        __FUNCTION__,
        Hal_SC_get_freezeimg(MAIN_WINDOW),
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[MAIN_WINDOW],
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[MAIN_WINDOW]));

    /// Reset the DS Memory

    /// for 2nd Seamless Zapping operating
    if(pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[eWindow] == 0)
    {
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[eWindow] = 1;
    }
    else
    {
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[eWindow] = 0;
    }
    SLZ_DBG(printf("@@#@@2.[%s]@@FRZ=%x,pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[MAIN_WINDOW]=%x, pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[MAIN_WINDOW]=%x\n",
        __FUNCTION__,
        Hal_SC_get_freezeimg(MAIN_WINDOW),
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[MAIN_WINDOW],
        pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[MAIN_WINDOW]));

    {
        MS_U8 u8index = 0;
        for(u8index=0; u8index<(sizeof(pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex)/sizeof(pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[0])); u8index++)
        {
            _Hal_SC_ResetDSCommand(eWindow, u8index);
        }
    }
 }
#endif

#if ENABLE_SW_DS

//clear DS Buffer
void Hal_SC_ResetSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U8 u8DSIndex)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;
    MS_U8 u8MiuSel = (pXCResourcePrivate->sthal_SC.u32DynamicScalingMemAddr & HAL_MIU1_BASE) ? 1 : 0;

    MS_U32 u32IndexBase = pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel] + (MS_U32)u8DSIndex * pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT;

    MS_U32 *pu32Data = NULL;
    int i = 0;

    //XC_PRINTF("[%s,%5d]  u32DSBaseAddr:%lx  u32IndexBase:%lx  u8DSIndexDepth:%d \n",__func__,__LINE__,pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel],u32IndexBase,pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel]);

    //Remove hardcoding
    MsOS_MPool_Mapping_Dynamic(u8MiuSel,pXCResourcePrivate->sthal_SC.u32DynamicScalingMemAddr,pXCResourcePrivate->sthal_SC.u32DynamicScalingMemSize,1);

    for(i = 0; i < (pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT)/4; i++) //need check
    {
        if(i % 8 < 4)
        {
            if(MAIN_WINDOW == eWindow)
            {
                if(i % 4 < 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_1 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                        return;
                    }
                    *pu32Data = 0xFFFF0000;
                }
            }
            else //SUB_WINDOW == eWindow
            {
                if(i % 4 >= 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_2 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                        return;
                    }
                    *pu32Data = 0xFFFF0000;
                }
            }
        }
        else
        {
            if(MAIN_WINDOW == eWindow)
            {
                if(i % 4 < 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_3 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                        return;
                    }
                    else
                    {
                        *pu32Data = 0xFFFF0000;//0;
                    }

                    //printf("[szuhua] [%s,%5d] [IP] 0xFFFF0000 \n",__func__,__LINE__);
                }
            }
            else //SUB_WINDOW == eWindow
            {
                if(i % 4 >= 2)
                {
                    pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IndexBase + (i*4));
                    if(0 == pu32Data)
                    {
                        printf("%s :pu32Data_4 = 0!,error!!!\n", __FUNCTION__);
                        assert(pu32Data != 0);
                        return;
                    }
                    else
                    {
                	*pu32Data = 0xFFFF0000;
		    }
                }
            }
        }
    }
}

void Hal_SC_WriteSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U16 u16CmdRegAddr, MS_U16 u16CmdRegValue,ds_reg_ip_op_sel IPOP_Sel)
{
    //XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));

    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;

    MS_U32 u32Addr = pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel] + pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel] * pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] * MS_MLOAD_MEM_BASE_UNIT;

    //XC_PRINTF("[%s,%5d]  u32DynamicScalingBaseAddr:%x  CurIdx:%d  IdxDepth:%d  BYTE_PER_WORD:%d \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel],(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel],(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel],(int)BYTE_PER_WORD);
    //XC_PRINTF("[%s,%5d]  u32Addr:%x  \n",__func__,__LINE__,(int)u32Addr);

    if(IPOP_Sel == DS_OP)
    {
        MS_U32 u32OPAddr = u32Addr;
        MS_U32 *pu32Data = NULL;
        MS_U8 u8OPCount = pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel];
       // MS_U8 u8OPCount = IdxDepth;
        MS_U32 u32Command = 0;

        //XC_PRINTF("[%s,%5d]  OP cmd  u32OPAddr:%x  u8OPCount:%d  \n",__func__,__LINE__,(int)u32OPAddr,(int)u8OPCount);

        u32Command = u16CmdRegValue + ((u16CmdRegAddr & 0x00FF) << 15) + ((u16CmdRegAddr & 0xFF00) << 16);

        while(u8OPCount > 0)
        {
            if(0 == u32OPAddr)
            {   
                printf("%s :u32OPAddr = 0!,error!!!\n", __FUNCTION__);
                return;
            }
            pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32OPAddr);
            if(0 == pu32Data)
            {
                printf("%s :pu32Data = 0!,error!!!\n", __FUNCTION__);
                return;
            }
            // Found null command
            if(*pu32Data == 0xFFFF0000||*pu32Data == 0xFFFFFFFF)
            {
                break;
            }
            // Found duplicated command
            if(*pu32Data == u32Command)
            {
                //SC_DBG(printf("Command duplicated\n"));

                return;
            }
            u32OPAddr += MS_MLOAD_MEM_BASE_UNIT;
            u8OPCount--;
        }

        if(*pu32Data == 0xFFFF0000||*pu32Data == 0xFFFFFFFF)
        {
            *pu32Data = u32Command;
        }
        else
        {
           //XC_PRINTF("DS OP command buffer is full for this index %u!\n", pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel]);
        }
        //printf("@@[%s][%u]u16CmdRegAddr=%x, u16CmdRegValue=%x\n",__FUNCTION__, __LINE__, u16CmdRegAddr, u16CmdRegValue);
    }

    else
    {
        // (OP : IP : Unused : Unused) (4 byte : 4 byte : 4 byte : 4 byte) if MIU 128bit
        // (OPmain : IPmain : OPsub : IPsub) (4 byte : 4 byte : 4 byte : 4 byte) if MIU 128bit

        MS_U32 u32IPAddr = u32Addr + DS_OP_CMD_LEN;
        MS_U32 *pu32Data = NULL;
        MS_U8 u8IPCount = pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel];
        //MS_U8 u8IPCount = IdxDepth;
        MS_U32 u32Command = 0;

        //XC_PRINTF("[%s,%5d]  IP cmd  u32IPAddr:%x  u8IPCount:%d \n",__func__,__LINE__,(int)u32IPAddr,(int)u8IPCount);

        u32Command = u16CmdRegValue + ((u16CmdRegAddr & 0x00FF) << 15) + ((u16CmdRegAddr & 0xFF00) << 16);

        while(u8IPCount > 0)
        {
            pu32Data = (MS_U32*)MsOS_PA2KSEG1(u32IPAddr);
            if(0 == pu32Data)
            {
                printf("%s :pu32Data = 0!,error!!!\n", __FUNCTION__);

                assert(pu32Data != 0);
                return;
            }
            // Found null command
            if(*pu32Data == 0xFFFF0000||*pu32Data == 0xFFFFFFFF)
            {
                break;
            }
            // Found duplicated command
            if(*pu32Data == u32Command)
            {
                return;
            }
            u32IPAddr += MS_MLOAD_MEM_BASE_UNIT;
            u8IPCount--;
        }

        if(*pu32Data == 0xFFFF0000||*pu32Data == 0xFFFFFFFF)
        {
            *pu32Data = u32Command;
        }
        else
        {
           //XC_PRINTF("DS IP command buffer is full for this index %u!\n", pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel]);
        }
        //printf("@@[%s][%u]u16CmdRegAddr=%x, u16CmdRegValue=%x\n",__FUNCTION__, __LINE__, u16CmdRegAddr, u16CmdRegValue);
    }

}

void Hal_SC_UpdateDynamicScalingCurrentIndex(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    MS_U16 mvop_sel = (pXCResourcePrivate->sthal_SC.bIsMainMVOP)?0:1;
    (pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel])++;
    XC_PRINTF("[%s,%5d]  CurIdx:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel]);

    pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel] % DS_BUFFER_NUM_EX;
    XC_PRINTF("[%s,%5d] CurIdx_Ring:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel]);
    (pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel]) %= DS_BUFFER_NUM_EX;

    //if( pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel] >= DS_BUFFER_NUM_EX )
    {
        Hal_SC_ResetSWDSCommand(pInstance,MAIN_WINDOW, pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel]);
    }
}

void Hal_SC_UpdateDynamicScalingIPcommand(void *pInstance, SCALER_WIN eWindow, MS_U16 u8index)
{
    MS_U32 ActiveSWClearEn = 0;
    MS_U32 u32SW_Reset_Enable = 0;
    MS_U32 u32SW_Reset_IPM = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    ActiveSWClearEn = SC_R2BYTEMSK(0, REG_SC_BK1F_17_L, 0xFFFF);
    //ActiveSWClearEn enable in first DS IP command ; patch for IPM tune after DS
    u32SW_Reset_Enable = 0x400; // bit10: SW_Reset_Enable
    ActiveSWClearEn |= u32SW_Reset_Enable;
    if (u8index == 1)
    {
        //toggle reg_ds_ipm_active_set in last DS IP command  ; patch for IPM tune after DS
        u32SW_Reset_IPM = 0x0100 ;//bit8 ip main
        ActiveSWClearEn |= u32SW_Reset_IPM;
    }
    Hal_SC_WriteSWDSCommand(pInstance,eWindow,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP);

}


#endif


MS_BOOL MHAL_SC_Set_DynamicScaling(void *pInstance, MS_U32 u32DSMemAddr, MS_U32 u32DSMemSize, MS_U32 u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    MS_U32 u32Base = u32MemBaseAddr;// / BYTE_PER_WORD;
    MS_U16 u16OnOff = 0;
    MS_U16 u16MIU_Sel_bit1 = 0;
    MS_WINDOW_TYPE pWin;

   // XC_PRINTF("[%s,%5d] u32DSBufSize:%ld u8MIU_Select:%d   \n",__func__,__LINE__,u32DSBufSize,u8MIU_Select);
    XC_PRINTF("[%s,%5d] bOP_On:%d  bIPS_On:%d bIPM_On:%d \n",__func__,__LINE__,bOP_On,bIPS_On,bIPM_On);

    if( u32Base > HAL_MIU2_BASE)
    {
        u32Base = u32Base - HAL_MIU2_BASE;
    }
    else if( u32Base > HAL_MIU1_BASE)
    {
        u32Base = u32Base - HAL_MIU1_BASE;
    }
    u32Base = u32Base/MS_MLOAD_MEM_BASE_UNIT; //BYTE_PER_WORD;


    SC_W2BYTEMSK(0, REG_SC_BK12_02_L, BIT(14), BIT(14)); //rd request disable DNR read before ds enabled/disabled
    if(bOP_On || bIPM_On)
    {
    	SC_W2BYTEMSK(0, REG_SC_BK1F_05_L, 0, BIT(15)|BIT(14));//rd request disable watchdog before ds enabled/disabled
    }

    // disable DS before modifying DS configuration
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, 0, 0x0E00);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_ENTRY();
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, 0, 0x0E00);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, 0, 0x0E00);
    }

    // enable/disable
    // we only memorize riu enable bit because mload and ds shares the same riu enable bit
    u16OnOff = SC_R2BYTE(0, REG_SC_BK1F_10_L) & 0x1000;

    u16OnOff |= ((bOP_On << 9) | (bIPS_On << 10) | (bIPM_On << 11));

    pXCResourcePrivate->sthal_SC.bDynamicScalingForceI[MAIN_WINDOW] = FALSE;
    pXCResourcePrivate->sthal_SC.bDynamicScalingForceI[SUB_WINDOW] = FALSE;
    
#if ENABLE_SW_DS

    MS_U16 mvop_sel = 0;
    XC_PRINTF("[%s,%5d] gSrcInfo[eWindow].enInputSourceType:%u \n",__func__,__LINE__,gSrcInfo[eWindow].enInputSourceType);

    if ((gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_STORAGE) || (gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_DTV))
    {
        mvop_sel = 0;
    }
    else
    {
        mvop_sel = 1;
    }
    pXCResourcePrivate->sthal_SC.bIsMainMVOP = (mvop_sel == 0)? TRUE: FALSE;
    pXCResourcePrivate->sthal_SC.u32DynamicScalingMemAddr = u32DSMemAddr;
    pXCResourcePrivate->sthal_SC.u32DynamicScalingMemSize = u32DSMemSize;
    pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel] = u32MemBaseAddr;
    pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel] = u8IdxDepth; //MaxDepth(IP/OP)
    pXCResourcePrivate->sthal_SC.u32DynamicScalingBufSize[mvop_sel] = BYTE_PER_WORD * u8IdxDepth * DS_MAX_INDEX;


    XC_PRINTF("[%s,%5d] u32DynamicScalingBaseAddr:%x  u8DynamicScalingIndexDepth:%d u32DynamicScalingBufSize:%ld  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr[mvop_sel],(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth[mvop_sel],pXCResourcePrivate->sthal_SC.u32DynamicScalingBufSize[mvop_sel]);

    //Befor DS On, Clean the DS memory
    if(u16OnOff & 0x0E00)
    {
        MS_U8 u8index = 0;
        //MS_U8 u8IdxCount = 0;

        //u8IdxCount = (MS_U8)(u32DSBufSize / (u8IdxDepth * BYTE_PER_WORD));

        XC_PRINTF(" [%s,%5d] u8IdxDepth:%d DS_MAX_INDEX:%d BYTE_PER_WORD:%d \n",__func__,__LINE__,u8IdxDepth,DS_MAX_INDEX,BYTE_PER_WORD);

        //if(bFirstTimeSetDS==0)

            pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow][mvop_sel] = 0;
            pXCResourcePrivate->sthal_SC.u8DynamicScalingNextIndex[eWindow][mvop_sel] = 0;

            for(u8index=0; u8index < DS_MAX_INDEX; u8index++)
            {
                Hal_SC_ResetSWDSCommand(pInstance,eWindow, u8index);
            }

            // MVD HW reset DS index to F
            // Fill in DS Dram region with unlock ipm command to avoid ipm lock when index is F
            for (u8index=0; u8index < FW_DS_MAX_INDEX; u8index++)
            {
                pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel] = u8index; 
                Hal_SC_UpdateDynamicScalingIPcommand(pInstance,eWindow,1);
            }
            
            pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow][mvop_sel] = 0;

    }
#endif



    if(u16OnOff & 0x0E00)
    {
        // ENABLE DS
        u16OnOff |= BIT(12);     // enable write register through RIU
#if !SUPPORT_DS_FAST_SWITCHING
        pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = TRUE;
#endif

        // currently, these 3 will use same MIU
        u16OnOff |= ((u8MIU_Select & 0x01) << 13);                       // MIU select of OP
        u16OnOff |= ((u8MIU_Select & 0x01) << 14);                       // MIU select of IPS
        u16OnOff |= ((u8MIU_Select & 0x01) << 15);                       // MIU select of IPM

        u16MIU_Sel_bit1 |= ((u8MIU_Select & 0x02) << 12);                // MIU select of OP
        u16MIU_Sel_bit1 |= ((u8MIU_Select & 0x02) << 13);                // MIU select of IPS
        u16MIU_Sel_bit1 |= ((u8MIU_Select & 0x02) << 14);                // MIU select of IPM

        // patch IP1F2_21[15:14] r/w bank
        pXCResourcePrivate->sthal_SC.su16OldValue = SC_R2BYTE(0, REG_SC_BK01_21_L);
        pXCResourcePrivate->sthal_SC.sbOldValueReaded = TRUE;

#if (HW_DESIGN_4K2K_VER == 4)
        MDrv_XC_MLoad_get_IP_trig_p(pInstance, &pXCResourcePrivate->sthal_SC.u16OldIPTrain, &pXCResourcePrivate->sthal_SC.u16OldIPDisp);
#endif

        SC_W2BYTEMSK(0, REG_SC_BK01_21_L, 0x4000, 0xC000);
        SC_W2BYTEMSK(0, REG_SC_BK57_1A_L, 0x01 , 0x0FFF);// set reg_atp_trig_dly
        SC_W2BYTEMSK(0, REG_SC_BK57_1B_L, 0x02 , 0x0FFF);// set reg_disp_trig_dly

        Hal_XC_MLoad_set_opm_lock(pInstance, OPM_LOCK_DS);
    }
    else
    {
#if !SUPPORT_DS_FAST_SWITCHING
        pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = FALSE;
#endif
        // DISABLE DS
		MHal_SC_DynamicScaling_SWReset();
        if(pXCResourcePrivate->sthal_SC.sbOldValueReaded)
        {
            SC_W2BYTE(0, REG_SC_BK01_21_L, pXCResourcePrivate->sthal_SC.su16OldValue);

#if (HW_DESIGN_4K2K_VER == 4)
            MDrv_XC_MLoad_set_IP_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16OldIPTrain, pXCResourcePrivate->sthal_SC.u16OldIPDisp);
#endif

            pXCResourcePrivate->sthal_SC.sbOldValueReaded = FALSE;
        }
        Hal_XC_MLoad_set_opm_lock(pInstance, OPM_LOCK_INIT_STATE);
    }

    // if disabled, need to set dynamic scaling ratio to 1:1
    if(!bOP_On)
    {
        SC_W4BYTE(0, REG_SC_BK23_1C_L, 0x00100000);
        SC_W4BYTE(0, REG_SC_BK23_1E_L, 0x00100000);
    }

    if(u16OnOff & 0x0E00)
    {
        if (FALSE == pXCResourcePrivate->sthal_SC.bTrig_pStored)
        {
            //set DS write bank
            SC_W2BYTEMSK(0, REG_SC_BK1F_74_L, 0x4000 , 0xF000);

            // set base address
            SC_W2BYTE(0, REG_SC_BK1F_11_L, u32Base & 0xFFFF);
            SC_W2BYTEMSK(0, REG_SC_BK1F_12_L, (u32Base >> 16) , 0xFFFF);

            // set index depth
            SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, u8IdxDepth , 0x00FF);

            // MIU sel
            SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, u16OnOff & 0xE000, 0xE000);
            SC_W2BYTEMSK(0, REG_SC_BK1F_71_L, u16MIU_Sel_bit1 & 0xE000, 0xE000);

            // set DMA threthold, length, fixed value, no need to change
            SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, 0x88, 0x00FF);

            // store
            Hal_XC_MLoad_get_trig_p(pInstance, &pXCResourcePrivate->sthal_SC.u16OldTrain, &pXCResourcePrivate->sthal_SC.u16OldDisp);
            pXCResourcePrivate->sthal_SC.bTrig_pStored = TRUE;

            // set the trigger point from delayed line,  DS: 0x08, Train:0x14, Disp:0x18
            SC_W2BYTEMSK(0, REG_SC_BK20_21_L, 0x08, 0xFF);  // ds_trig_dly

            Hal_SC_get_disp_de_window(pInstance, &pWin);
            if(pWin.width == 1366 && pWin.height == 768)    //for HD model youtube video broken
            {
                Hal_XC_MLoad_set_trig_p(pInstance, 0x0C, 0x11);
            }
            else
            {
            Hal_XC_MLoad_set_trig_p(pInstance, 0x14, 0x18);
            }

            // set trigger source to trigger
            SC_W2BYTEMSK(0, REG_SC_BK20_19_L, 0x08B8, 0x08BC);

            //set the delay line cnt to vfde end
            SC_W2BYTEMSK(0, REG_SC_BK20_19_L, BIT(6), BIT(6)|BIT(0));
        }
		SC_W2BYTEMSK(0, REG_SC_BK1F_17_L, 0, BIT(10));

    }
    else
    {
        // restore
        if (pXCResourcePrivate->sthal_SC.bTrig_pStored)
        {
            pXCResourcePrivate->sthal_SC.bTrig_pStored = FALSE;
            Hal_XC_MLoad_set_trig_p(pInstance, pXCResourcePrivate->sthal_SC.u16OldTrain, pXCResourcePrivate->sthal_SC.u16OldDisp);

            //set the delay line cnt to V sync start
            SC_W2BYTEMSK(0, REG_SC_BK20_19_L, 0, BIT(6)|BIT(0));
        }
    }

#if SUPPORT_DS_FAST_SWITCHING
// No control OP[9],IPS[10],IPM[11] here.
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x1100, 0x1100);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_ENTRY();
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x1100, 0x1100);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, u16OnOff & 0x1100, 0x1100);
    }
#else
    // set DS in IPS[10],ipm[11] On/Off
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x1D00, 0x1D00);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_ENTRY();
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x1D00, 0x1D00);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, u16OnOff & 0x1D00, 0x1D00);
    }

    // set DS in OP[9] On/Off
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x0200, 0x0200);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_ENTRY();
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x0200, 0x0200);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, u16OnOff & 0x0200, 0x0200);
    }
#endif
    return TRUE;
}

void MHAL_SC_Set_DynamiScaling_OPIP_OnOff(void *pInstance, MS_BOOL bOP_On,MS_BOOL bIPS_On,MS_BOOL bIPM_On, SCALER_WIN eWindow)
{
#if SUPPORT_DS_FAST_SWITCHING

    if (eWindow != MAIN_WINDOW)
		return;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
	MS_U16 u16OnOff = 0;
	u16OnOff |= ((bOP_On << 9) | (bIPS_On << 10) | (bIPM_On << 11));

	if (u16OnOff & 0x0E00)
	{
		pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = TRUE;
	}
	else
	{
		pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = FALSE;
	}

	if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
	{
#if SC2_ENABLE
		XC_PRINTF(" [%s,%5d] bOP_On:%d  bIPS_On:%d bIPM_On:%d \n",__func__,__LINE__,bOP_On,bIPS_On,bIPM_On);
		_MLOAD_ENTRY(pInstance);
		MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x0E00, 0x0E00);
		MDrv_XC_MLoad_Fire(pInstance, TRUE);
		_MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
		_MLOAD_ENTRY();
		MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK1F_10_L, u16OnOff & 0x0E00, 0x0E00);
		MDrv_XC_MLoad_Fire(pInstance, TRUE);
		_MLOAD_RETURN();
#endif //#if SC2_ENABLE

	}
	else
	{
		XC_PRINTF(" [%s,%5d] bOP_On:%d  bIPS_On:%d bIPM_On:%d \n",__func__,__LINE__,bOP_On,bIPS_On,bIPM_On);
		SC_W2BYTEMSK(0, REG_SC_BK1F_10_L, u16OnOff & 0x0E00, 0x0E00);
	}
#endif
}

void MHAL_SC_Set_DynamicScalingFlag(void *pInstance, MS_BOOL bEnable)
{
// using MHAL_SC_Set_DynamicScaling
//return;


#if 1
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = bEnable;
#endif
}

MS_BOOL MHAL_SC_Get_DynamicScaling_Status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(0)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_SC.bDynamicScalingEnable;
}

MS_BOOL MHAL_SC_Enable_IPMTuneAfterDS(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // ENABLE IPM TUNE AFTER DS
    SC_W2BYTEMSK(0, REG_SC_BK12_06_L, BIT(7), BIT(7));
    if(TRUE == bEnable)
    {
        // Clear DS active pulse every vsync
        SC_W2BYTEMSK(0, REG_SC_BK1F_17_L, BIT(6), BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1F_17_L, 0x0, BIT(6));
    }
    return TRUE;
}

void MHal_SC_DynamicScaling_SWReset(void)
{
    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x400, 0x400);
    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x800, 0x800);
    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x1000, 0x1000);

    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x0, 0x1000);
    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x0, 0x800);
    SC_W2BYTEMSK(0, REG_SC_BK1F_13_L, 0x0, 0x400);
}
MS_BOOL MHal_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow)
{
    if (eWindow >= MAX_WINDOW)
    {
        printf("[%s,%5d] maximum window exceeded",__FUNCTION__,__LINE__);
        return FALSE;
    }

    return TRUE;
}

void MHal_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow >= MAX_WINDOW)
    {
        printf("[%s,%5d] maximum window exceeded",__FUNCTION__,__LINE__);
        return;
    }

    if (eWindow == MAIN_WINDOW)
    {
        // reg_idxg_force_en
        SC_W2BYTEMSK(0, REG_SC_BK00_37_L, bEnable?BIT(2):0 , BIT(2));

        // reg_idxg_force_idx_f2
        SC_W2BYTEMSK(0, REG_SC_BK00_36_L, (MS_U16)u8Index << 8 , 0xFF00);
    }
    else // eWindow == SUB_WINDOW
    {
        SC_W2BYTEMSK(0, REG_SC_BK00_37_L, bEnable?BIT(10):0 , BIT(10));

        // reg_idxg_force_idx_f1
        SC_W2BYTEMSK(0, REG_SC_BK00_35_L, (MS_U16)u8Index << 8 , 0xFF00);
    }
}

void MHal_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow >= MAX_WINDOW)
    {
        printf("[%s,%5d] maximum window exceeded",__FUNCTION__,__LINE__);
        return;
    }

    if (eDSIdxSrc >= E_XC_DS_INDEX_MAX)
    {
        printf("[%s,%5d] Maximum index exceeded, the index is (%d)\n",__FUNCTION__,__LINE__,eDSIdxSrc);
        return;
    }

    if(eWindow == MAIN_WINDOW)
    {
        // reg_idxg_en_f2
        SC_W2BYTEMSK(0, REG_SC_BK00_37_L, eDSIdxSrc, BIT(0));
    }
    else
    {
        // reg_idxg_en_f1
        SC_W2BYTEMSK(0, REG_SC_BK00_37_L, eDSIdxSrc << 8, BIT(8));
    }
}


void MHal_XC_Set_DS_ForceI_DeInterlaceMode(void *pInstance, MS_BOOL bPmode,  MS_BOOL bIsTopField, MS_U16 u16FrameRate, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow >= MAX_WINDOW)
    {
        printf("[%s,%5d] maximum window exceeded",__FUNCTION__,__LINE__);
        return;
    }
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_SC.bDynamicScalingForceI[eWindow] = TRUE;
    pXCResourcePrivate->sthal_SC.bCurrentPmode[eWindow] = bPmode;
    pXCResourcePrivate->sthal_SC.bIsTopField[eWindow] = bIsTopField;
    pXCResourcePrivate->sthal_SC.u16CurrentFramerate[eWindow] = u16FrameRate;

    //Disable HW film22
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK0A_10_L, 0, BIT(14));

    //printf("\33[0;35m [SWDS] %s %d: , bPmode[%u],  bIsTopField[%u], u16FrameRate[%u], eWindow[%u]   \33[m \n", __FUNCTION__,  __LINE__,  bPmode,   bIsTopField,  u16FrameRate,  eWindow);
}


#endif // MHAL_DYNAMICSCALING_C
