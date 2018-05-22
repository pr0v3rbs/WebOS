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
// (!§MStar Confidential Information!‥) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
#define DRV_SC_MENULOAD_C

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
//#include "Debug.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "drvXC_IOPort.h"
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
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "mhal_menuload.h"
#include "drvMIU.h"
#include "halCHIP.h"
#include "drv_sc_scaling.h"

#define  MLDBG_WARN(x)   x // open when debug time for serious issue.
#define  MLDBG(x) //(printf("[MLOAD] "), x)
#define  MLG(x) //(printf("[MLG] "), x)

#define _AlignTo(value, align)  ( ((value) + ((align)-1)) & ~((align)-1) )

typedef struct
{
    union
    {
        struct
        {
        	MS_U16 u16Data;
        	MS_U8 u8Addr;
        	MS_U8 u8Bank;
        };
        MS_U32 u32Cmd;
    };
}MS_MLoad_Data;

static ST_DRVXC_MENULOAD _stdrvXC_Menuload[MAX_XC_DEVICE_NUM];

#if 0
void dumpcmd(void)
{
    MS_U16 i, j;
    MS_U8 *p = (MS_U8 *)MS_PA2KSEG1(_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.PhyAddr);

    MLDBG(printf("dump cmd buffer: %d\n", _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint));
    for (i = 0; i < _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint; i++)
    {
        printf("%06d: ", i);
        for (j = 0; j < MS_MLOAD_CMD_LEN; j++)
        {
            printf("%02x", p[i*MS_MLOAD_CMD_LEN + j]);
        }
        printf(" \n");
    }
}
#endif

MS_BOOL MDrv_XC_MLoad_Check_Done(void *pInstance)
{
    MS_BOOL bEn;

    if(Hal_XC_MLoad_get_status(pInstance))
        bEn = FALSE;
    else
        bEn = TRUE;
    return bEn;
}

void MDrv_XC_MLoad_Wait_HW_Done(void *pInstance)
{
    MS_U32 u32Delayms = 0;
    while((MDrv_XC_MLoad_Check_Done(pInstance) == FALSE) && u32Delayms < 600)
    {
        MsOS_DelayTask(1);
        u32Delayms++;
    }
}

void MDrv_XC_MLoad_Init(void *pInstance, MS_PHY phyAddr)
{
    MS_PHY u32StartOffset;
    MS_U8 u8MIUSel = 0;
    MS_U16 u16Cmd;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MLDBG(printf("%s: 0x%lx\n", __FUNCTION__, phyAddr));
    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, u32StartOffset, phyAddr);
    UNUSED(u32StartOffset);
    Hal_XC_MLoad_set_opm_arbiter_bypass(pInstance, TRUE);
    //Hal_XC_MLoad_set_on_off(DISABLE);
    Hal_XC_MLoad_set_trigger_timing(pInstance, TRIG_SRC_DELAY_LINE); //By REG_MLOAD_TRIG_DLY
    Hal_XC_MLoad_set_trigger_delay(pInstance, 0x05);
    Hal_XC_MLoad_set_trig_p(pInstance, 0x08, 0x0C);
    Hal_XC_MLoad_get_trig_p(pInstance, &pXCResourcePrivate->sthal_SC.u16OldTrain, &pXCResourcePrivate->sthal_SC.u16OldDisp);
#if (HW_DESIGN_4K2K_VER == 4)
    // set ip trig as hw default value
    MDrv_XC_MLoad_set_IP_trig_p(pInstance, 0x03, 0x05);
    MDrv_XC_MLoad_get_IP_trig_p(pInstance, &pXCResourcePrivate->sthal_SC.u16OldIPTrain, &pXCResourcePrivate->sthal_SC.u16OldIPDisp);
#endif
    Hal_XC_MLoad_set_miu_bus_sel(pInstance, MS_MLOAD_MIU_BUS_SEL);
    Hal_XC_MLoad_set_miusel(pInstance, u8MIUSel);
    Hal_XC_MLG_set_miusel(pInstance, u8MIUSel);
    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].u8XCMLoadMIUSel = u8MIUSel;

    if (MDrv_XC_MLoad_GetCaps(pInstance))
    {
        for (u16Cmd = 0; u16Cmd < 32; u16Cmd++)
        {
            MDrv_XC_MLoad_AddCmd(pInstance, MS_MLOAD_NULL_CMD);
        }

        MDrv_XC_MLoad_AddNull(pInstance);
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint - 1;

        // when system is power on, the read count of MLoad may be not 0.
        // If we enable MLoad RIU_En, the garbage data in fifo will over-write
        // unexpected register.
        // Hence, add below code before Kickoff to avoid it.

        // 1001_02[4] = 1, RIU_XARB不收menuload過來的register
        Hal_XC_MLoad_set_riu_cs(pInstance, TRUE);
        MsOS_DelayTask(1);

        // menuload a RIU_XARB enable, FIFO 裡面的東西因為無法寫到RIU_XARB, 所以還沒法寫
        Hal_XC_MLoad_Set_riu(pInstance, 1);

        // 切bank到0xFF, 搶bank switch的主動權
        MDrv_WriteByte(0x102F00, 0xFF);

        // 1001_02[4] = 0, FIFO裡面的東西開始往RIU_XARB吐, 因為menuload是disable的, 所以bank切換的主動權不在menuload
        Hal_XC_MLoad_set_riu_cs(pInstance, FALSE);

        MsOS_DelayTask(1);

        MDrv_XC_MLoad_KickOff(pInstance);
    }
}

MS_BOOL MDrv_XC_MLoad_GetCaps(void *pInstance)
{
    return Hal_XC_MLoad_GetCaps(pInstance);
}

void MDrv_XC_MLoad_Trigger(void *pInstance, MS_PHY startAddr, MS_U16 u16CmdCnt)
{
    MS_PHY u32StartOffset = 0;
    MS_U8 u8MIUSel = 0;
    MLDBG(printf("%s 0x%lx, %u\n\n",__FUNCTION__, startAddr, u16CmdCnt));

    //dumpcmd();
    #if 1
    _phy_to_miu_offset(u8MIUSel, u32StartOffset, startAddr);
    UNUSED(u8MIUSel);
    Hal_XC_MLoad_set_base_addr(pInstance, u32StartOffset);
    Hal_XC_MLoad_set_depth(pInstance, u16CmdCnt);
    Hal_XC_MLoad_set_len(pInstance, MS_MLOAD_REG_LEN);//length of DMA request
    Hal_XC_MLoad_set_on_off(pInstance, ENABLE);
    MsOS_DelayTaskUs(3);
    Hal_XC_MLoad_Set_riu(pInstance, ENABLE);

    #else
    Hal_XC_MLoad_trigger(startAddr, MS_MLOAD_REG_LEN, (MS_U8)u16CmdCnt);
    #endif
}

void MDrv_XC_MLoad_AddCmd(void *pInstance, MS_U32 u32Cmd)
{
    MS_VIRT *pu32Addr;
    MS_PHYADDR DstAddr;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    DstAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint * MS_MLOAD_CMD_LEN;
    pu32Addr = (MS_VIRT *)MS_PA2KSEG1(DstAddr);
    *pu32Addr = u32Cmd;
    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint++;
}

void MDrv_XC_MLoad_SetCmd(MS_PHY DstAddr, MS_U32 u32Cmd)
{
    MS_VIRT *pu32Addr;

    pu32Addr = (MS_VIRT *)MS_PA2KSEG1(DstAddr);
    *pu32Addr = u32Cmd;
}

MS_U32 MDrv_XC_MLoad_GetCmd(MS_PHY DstAddr)
{
    MS_VIRT u32Addr;

    u32Addr = MS_PA2KSEG1(DstAddr);
    return (MS_U32)(*(MS_U32 *)u32Addr);
}

MS_BOOL MDrv_XC_MLoad_BufferEmpty(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(MDrv_XC_MLoad_Check_Done(pInstance) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint+1))
        return TRUE;
    else
        return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Write command to the Menuload buffer
/// @param  u32Cmd                 \b IN: the command to write into the buffer
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Cmd)
{
    MS_BOOL bRet;
    MS_U16 u16DummyCmdIdx;
    MS_U16 u16QueueCmd;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#ifdef ENABLE_SCALING_WO_MUTE
    if ((_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint) &&
        (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint+1))
    {
        MDrv_XC_MLoad_Wait_HW_Done(pInstance);
    }
#endif

    if( MDrv_XC_MLoad_BufferEmpty(pInstance) )
    {
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint = 0;
        MDrv_XC_MLoad_AddCmd(pInstance, u32Cmd);
        bRet = TRUE;
    }
    else
    {
        u16DummyCmdIdx = _AlignTo(_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint+1 + 16 , 4);

        if (u16DummyCmdIdx == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint+1)
        {
            u16DummyCmdIdx = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint+1+MS_MLOAD_CMD_ALIGN;
        }

        if(u16DummyCmdIdx < _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16MaxCmdCnt)
        {
            u16QueueCmd = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint - _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint + 1;

            if(u16QueueCmd == MS_MLOAD_MAX_CMD_CNT - 1) //max cmd depth is MS_MLOAD_MAX_CMD_CNT
            {
                MDrv_XC_MLoad_AddCmd(pInstance, MS_MLOAD_NULL_CMD);
            }
            MDrv_XC_MLoad_AddCmd(pInstance, u32Cmd);

            bRet = TRUE;

        }
        else
        {
            MLDBG(printf("WPoint=%x, MaxCnt=%x, DummyIdx=%x\n"
                        , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint
                        , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16MaxCmdCnt
                        , u16DummyCmdIdx));
            bRet = FALSE;
        }
    }

    return bRet;
}

MS_BOOL MDrv_XC_MLoad_KickOff(void *pInstance)
{
    //MS_MLOAD_END_CMD use instance, does not remove this
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bRet;
    MS_U16 u16CmdCnt;
    MS_U32 u32EndCmd;
    MS_U16 u16QueueCmd, u16FireIdx;
    MS_PHY CmdBufAddr;

    MLDBG(printf("%s\n",__FUNCTION__));
    if(MDrv_XC_MLoad_Check_Done(pInstance) && _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint)
    {
        if(_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint > _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint)
        {
            u16QueueCmd = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint - _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint;

            if(u16QueueCmd > MS_MLOAD_MAX_CMD_CNT)
            {
                MLDBG_WARN(printf("Queue Too Many !!!!!!!!!!!!!!!!!\n"));
                MLDBG_WARN(printf("WPoint=%d, FirePoint=%d, RPoint=%d\n"
                                  , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint
                                  , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint
                                  , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint));
            }

            u16FireIdx = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint;
            MLDBG(printf("u16QueueCmd= %u, u16FireIdx= %u\n", u16QueueCmd, u16FireIdx));

            //check the last command to see whether it is null command
            CmdBufAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.PhyAddr + u16FireIdx * MS_MLOAD_CMD_LEN;
            u32EndCmd = MDrv_XC_MLoad_GetCmd(CmdBufAddr);
            if(u32EndCmd != MS_MLOAD_NULL_CMD)
            {
                MLDBG_WARN(printf("WPoint=%d, FirePoint=%d, RPoint=%d\n"
                           , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint
                           , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint
                           , _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint));
                MLDBG_WARN(printf("KickOff: Non Null Cmd. Trigger is not executed!!\n"));
                MS_ASSERT(0);
                bRet = FALSE;
            }
            else
            {
                u16CmdCnt = u16FireIdx - _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint + 1;
                u32EndCmd = MS_MLOAD_END_CMD(((MS_U32)u16CmdCnt));
                MLDBG(printf("CmdBufAddr = 0x%lx, u32EndCmd = 0x%lx\n", CmdBufAddr, u32EndCmd));
                MDrv_XC_MLoad_SetCmd(CmdBufAddr, u32EndCmd);
                MsOS_FlushMemory();

                CmdBufAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint * MS_MLOAD_CMD_LEN;
                MDrv_XC_MLoad_Trigger(pInstance, CmdBufAddr, u16CmdCnt);
                _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint = u16FireIdx + 1;
                bRet = TRUE;
                MLDBG(printf("after fire u16WPoint=%u u16RPoint=%u u16FPoint=%u\n",
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint,
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint,
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint));
            }
        }
        else if(_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint &&
                _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint-1)
        {
            bRet = TRUE;
        }
        else
        {
            MLDBG(printf("MenuLoad: WPoint=%u, FPoint=%u, RPoint=%u !!\n",
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint,
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint,
                    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint));
            bRet = FALSE;
        }
    }
    else
    {
        bRet = TRUE;
    }
    return bRet;
}

void MDrv_XC_MLoad_AddNull(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLDBG(printf("%s\n",__FUNCTION__));
    do
    {
        MDrv_XC_MLoad_AddCmd(pInstance, MS_MLOAD_NULL_CMD);
    }while (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint%MS_MLOAD_CMD_ALIGN != 0);
}

//-------------------------------------------------------------------------------------------------
/// Initialize the Menuload
/// @param  PhyAddr                 \b IN: the physical address for the menuload
/// @param  u32BufByteLen           \b IN: the buffer length of the menu load commands
//-------------------------------------------------------------------------------------------------
void MApi_XC_MLoad_Init_U2(void* pInstance, MS_PHY PhyAddr, MS_U32 u32BufByteLen)
{
    char word[] = {"_MLOAD_MUTEX"};
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MLDBG(printf("%s: 0x%lx, 0x%lx\n", __FUNCTION__, PhyAddr, u32BufByteLen));

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(MDrv_XC_MLoad_GetCaps(pInstance))
    {
        if (_MLOAD_MUTEX == -1)
        {
            _MLOAD_MUTEX = MsOS_CreateMutex(E_MSOS_FIFO, word, MSOS_PROCESS_SHARED);
            if (_MLOAD_MUTEX == -1)
            {
                MLDBG(printf("[MAPI MLOAD][%06d] create mutex fail\n", __LINE__));

#if SC2_ENABLE
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
                _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

                return;
            }
        }

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
        _MLOAD_ENTRY();
#endif //#if SC2_ENABLE

        memset((MS_VIRT *)MS_PA2KSEG1(PhyAddr), 0, u32BufByteLen);

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.bEnable = FALSE;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16MaxCmdCnt = (MS_U16)(u32BufByteLen / MS_MLOAD_CMD_LEN);
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.PhyAddr = PhyAddr;

        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadPhyAddr_Suspend = PhyAddr;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadBufByteLen_Suspend = u32BufByteLen;

        MDrv_XC_MLoad_Init(pInstance, PhyAddr);


#if SC2_ENABLE
        _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_MLoad_Init(MS_PHY PhyAddr, MS_U32 u32BufByteLen)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_MLOAD_INIT XCArgs;
    XCArgs.PhyAddr = PhyAddr;
    XCArgs.u32BufByteLen = u32BufByteLen;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLOAD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable the MLoad
/// @return  void
//-------------------------------------------------------------------------------------------------
void MApi_XC_MLoad_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.bEnable = bEnable;
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.bEnable = bEnable;
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_MLoad_Enable(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_MLOAD_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLOAD_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the status of MLoad
/// @return  MLOAD_TYPE
//-------------------------------------------------------------------------------------------------
MLOAD_TYPE MDrv_XC_MLoad_GetStatus(void *pInstance)
{
    MLOAD_TYPE type = E_MLOAD_UNSUPPORTED;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (MDrv_XC_MLoad_GetCaps(pInstance))
    {
        if (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.bEnable)
        {
            type = E_MLOAD_ENABLED;
        }
        else
        {
            type = E_MLOAD_DISABLED;
        }
    }

    return type;
}

MLOAD_TYPE MApi_XC_MLoad_GetStatus_U2(void* pInstance)
{
    MLOAD_TYPE eMode = E_MLOAD_UNSUPPORTED;

    eMode = MDrv_XC_MLoad_GetStatus(pInstance);
    return eMode;
}

MLOAD_TYPE MApi_XC_MLoad_GetStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_MLOAD_UNSUPPORTED;
    }

    stXC_MLOAD_GETSTATUS XCArgs;
    XCArgs.eReturnValue = E_MLOAD_UNSUPPORTED;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLOAD_GETSTATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_MLOAD_UNSUPPORTED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Fire the Menuload commands
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate)
{
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    bReturn = MDrv_XC_MLoad_Fire(pInstance, bImmediate);
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    bReturn = MDrv_XC_MLoad_Fire(pInstance, bImmediate);
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return bReturn;
}

MS_BOOL MDrv_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate)
{
    MS_U32 u32Delayms = 0;
    MS_U16 u16Boundary = 16, i = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLDBG(printf("%s, u16WPoint=%u u16RPoint=%u u16FPoint=%u\n", __FUNCTION__,
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint,
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint,
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint));

    if (MDrv_XC_MLoad_BufferEmpty(pInstance))
    {
        MLDBG(printf("%s MLoad buffer is empty, Skip menuload fire.\n", __FUNCTION__));
        return TRUE;
    }
    else
    {
        // 16 entry as boundary for menuload to prevent cmd sent unpredictable.
        for ( ; i < u16Boundary ; i++)
        {
            MDrv_XC_MLoad_AddCmd(pInstance, MS_MLOAD_NULL_CMD);
        }

        MDrv_XC_MLoad_AddNull(pInstance);

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint - 1;

        MLDBG(printf("u16WPoint=%u u16RPoint=%u u16FPoint=%u\n",
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint,
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint,
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint));

        MDrv_XC_MLoad_KickOff(pInstance);

        if(bImmediate)
        {    // we need to make sure the value is
            while((MDrv_XC_MLoad_Check_Done(pInstance) == FALSE) && u32Delayms < 600)
            {
                MsOS_DelayTask(1);
                u32Delayms++;
            }

            // Patch HW outpu Vsync plus width too short and cause MLoad missing.
            // T3 U06 will fix it.
            if(MDrv_XC_MLoad_Check_Done(pInstance) == FALSE)
            {
                MLDBG_WARN(printf("MLoad, TimeOut t=%lu [%s,%5d] u32Delayms = %d, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L) = %lx\n",MsOS_GetSystemTime(),__FUNCTION__,__LINE__,u32Delayms,SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L)));
                Hal_XC_MLoad_set_on_off(pInstance, DISABLE);
                _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16RPoint = 0;
                _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16WPoint = 0;
                _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLoadInfo.u16FPoint = 0;
                return FALSE;
            }
        }
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Write command to the Menuload buffer by WORD
/// @param  u32Addr                 \b IN: the address (sub-bank + 8-bit address)
/// @param  u16Data                 \b IN: the data
/// @param  u16Mask                 \b IN: the mask
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    MS_BOOL bReturn = FALSE;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
    bReturn = MDrv_XC_MLoad_WriteCmd(pInstance, u32Addr, u16Data, u16Mask);
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
    bReturn = MDrv_XC_MLoad_WriteCmd(pInstance, u32Addr, u16Data, u16Mask);
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return bReturn;
}

MS_BOOL MDrv_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_MLoad_Data data;
    MS_BOOL bRet;

#ifdef A3_MLG  // need to refine later, test the capibility first
    MS_U8 u8MLoadMIUSel = Hal_XC_MLoad_get_miusel();

    if (u8MLoadMIUSel != _stdrvXC_Menuload[psXCInstPri->u32DeviceID].u8XCMLoadMIUSel){
        Hal_XC_MLoad_set_miusel(pInstance, _stdrvXC_Menuload[psXCInstPri->u32DeviceID].u8XCMLoadMIUSel);
        Hal_XC_MLG_set_miusel(pInstance, _stdrvXC_Menuload[psXCInstPri->u32DeviceID].u8XCMLoadMIUSel);
        }
#endif
    data.u8Addr = (u32Addr & 0xFF) >> 1;
    data.u8Bank = ((u32Addr >> 8) & 0xFF) + _XC_Device_Offset[psXCInstPri->u32DeviceID];

    if( u16Mask == 0xFFFF )
    {
        data.u16Data = u16Data;
    }
    else
    {
        data.u16Data = (SC_R2BYTE(psXCInstPri->u32DeviceID, u32Addr) & ~u16Mask) | (u16Data & u16Mask);
    }

    if(_MDrv_XC_MLoad_WriteCmd(pInstance, data.u32Cmd))
    {
        bRet = TRUE;
    }
    else
    {
        MLDBG(printf("Fail: %04lx %04x %04x \n", u32Addr, u16Data, u16Mask));
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire_U2(void* pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    MS_BOOL bRet = TRUE;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();

#endif //#if SC2_ENABLE


    if (!MDrv_XC_MLoad_WriteCmd(pInstance, u32Addr, u16Data, u16Mask))
    {
        bRet = FALSE;
        MLDBG(printf("Fail: Cmd %4lx %x %x \n", u32Addr, u16Data, u16Mask));
    }
    else
    {
       if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
       {
            bRet = FALSE;
            MLDBG(printf("Fire Error!!!!\n "));
       }
    }

#if SC2_ENABLE
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return bRet;
}

MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_MLOAD_WRITE_CMD_AND_FIRE XCArgs;
    XCArgs.u32Addr = u32Addr;
    XCArgs.u16Data = u16Data;
    XCArgs.u16Mask = u16Mask;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLOAD_WRITE_CMD_AND_FIRE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire_U2(void* pInstance, MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt)
{
    MS_BOOL bRet = TRUE;
    MS_U16 i = 0, j = 0;

#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
#endif //#if SC2_ENABLE



    //Note: If there are some same regs in the Mload cmd queue,
    //  MLoad will use the value of the last reg address to fire. others same regs will be ignore.
    // So we handle this situation here, to combine every same reg's setting value to the same.
    for(i=0; i<u16CmdCnt; i++)
    {
        for(j=i+1; j<u16CmdCnt; j++)
        {
            if(pu32Addr[i] == pu32Addr[j])
            {
                pu16Mask[i] |= pu16Mask[j];
                pu16Data[i] |= pu16Data[j];
                pu16Mask[j] = pu16Mask[i];
                pu16Data[j] = pu16Data[i];
            }
        }
    }

    for(i=0; i<u16CmdCnt; i++)
    {

        if (!MDrv_XC_MLoad_WriteCmd(pInstance, pu32Addr[i], pu16Data[i], pu16Mask[i]))
        {
            bRet = FALSE;
            MLDBG(printf("Fail: Cmd %4lx %x %x \n", pu32Addr[i], pu16Data[i], pu16Mask[i]));
        }
    }

    if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
    {
        bRet = FALSE;
        MLDBG(printf("Fire Error!!!!\n "));
    }

#if SC2_ENABLE
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    return bRet;
}

MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire(MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_MLOAD_WRITE_CMDS_AND_FIRE XCArgs;
    XCArgs.pu32Addr = pu32Addr;
    XCArgs.pu16Data = pu16Data;
    XCArgs.pu16Mask = pu16Mask;
    XCArgs.u16CmdCnt = u16CmdCnt;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLOAD_WRITE_CMDS_AND_FIRE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

static void MDrv_XC_MLG_Init(void *pInstance, MS_PHYADDR phyAddr)
{
    MS_U32 u32StartOffset = 0;
    MS_U8 u8MIUSel = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s\n",__FUNCTION__));

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, u32StartOffset, phyAddr);
    UNUSED(u32StartOffset);
    MLG(printf("u8MIUSel = %d\n", u8MIUSel));
    if (Hal_XC_MLG_GetCaps(pInstance))
    {
        //Hal_XC_MLG_set_trigger_timing(TRIG_SRC_FE_VSYNC); //output VSync falling edge
        Hal_XC_MLG_set_trigger_timing(pInstance, TRIG_SRC_FE_VFDE); //output VDE falling edge
        Hal_XC_MLG_set_trigger_delay(pInstance, 0x01);
        Hal_XC_MLG_set_len(pInstance, MS_MLG_REG_LEN);//length of DMA request
        Hal_XC_MLG_set_miusel(pInstance, u8MIUSel);
        Hal_XC_MLoad_set_miusel(pInstance, u8MIUSel);

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].u8XCMLoadMIUSel = u8MIUSel;
    }

}

static void MDrv_XC_MLG_Trigger(void *pInstance, MS_PHY startAddr, MS_U16 u16CmdCnt)
{
 //   printf("\33[0;35m Jayce : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    MS_PHY u32StartOffset;
    MS_U8 u8MIUSel = 0;

    MLG(printf("%s 0x%lx, %u\n",__FUNCTION__, startAddr, u16CmdCnt));

    _phy_to_miu_offset(u8MIUSel, u32StartOffset, startAddr);
    UNUSED(u8MIUSel);
    Hal_XC_MLG_set_base_addr(pInstance, u32StartOffset);
    Hal_XC_MLG_set_depth(pInstance, u16CmdCnt);
    Hal_XC_MLG_set_on_off(pInstance, ENABLE);
    MsOS_DelayTaskUs(3);
}

#if 0 //munich does not support Gamma auto download
static void MDrv_XC_MLG_WriteCmd(void *pInstance, MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue)
{
   // printf("\33[0;35m Jayce! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    MS_SC_MLG_TBL *pMLG_Addr;
    MS_PHYADDR DstAddr;
    MS_U16 i,j;
    MS_U16 u16Temp_R,u16Temp_G,u16Temp_B;//, u16GammaDeltaR, u16GammaDeltaG, u16GammaDeltaB;
    MS_U16 u16CmdCount = 0;

    MS_U64 BData0 = 0;
    MS_U64 BData1 = 0;
    MS_U64 BData2 = 0;
    MS_U64 GData0 = 0;
    MS_U64 GData1 = 0;
    MS_U64 GData2_L = 0;

    MS_U64 GData2_H = 0;
    MS_U64 RData0 = 0;
    MS_U64 RData1 = 0;
    MS_U64 RData2 = 0;
    MS_U64 DEnable = 0 ;



    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s,u16Count=%u.\n",__FUNCTION__,u16Count));
    if((Hal_XC_MLG_get_status(pInstance) == 0) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint))
    {
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
    }
    pMaxGammaValue[0] = pMaxGammaValue [1] = pMaxGammaValue [2] = 0;

    DstAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint * MS_MLG_CMD_LEN;
    pMLG_Addr = (MS_SC_MLG_TBL *)MS_PA2KSEG1(DstAddr);

    if (u16Count == 1024)//For 1024 Entry Mode
        u16CmdCount = u16Count/4;
    else    //For 256 Entry Mode
        u16CmdCount = u16Count;
  //  printf("\33[0;35m Jayce! : [%s][%d],_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt=[%d] \33[m \n",__FUNCTION__,__LINE__,_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt);
    if ((_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint + u16CmdCount) <= _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt)
    {
        if (u16Count == 1024)//For 1024 Entry Mode
        {
            MLG(printf("\033[0;31m [%s][%d] 1024 entry  \033[0m\n", __FUNCTION__, __LINE__));
            MS_U16 u16Delta1, u16Delta2, u16Delta3;
            //Decode from Gamma table
            for (i = 0, j = 0; i < u16Count/4; i+=1 , j+=6)
            {
                // For index i B0 data
                u16Temp_B = (pB[j] & 0x0F) | (pB[j+1] << 4);
                MLG(printf("[%u]-B1 , u16Temp_B=0x%x.\n",i,u16Temp_B));
                BData0 = u16Temp_B;

                // For B delta
                u16Delta1 =  (((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4)) - BData0;
                u16Delta2 =  ((pB[j+3] & 0x0F) | (pB[j+4] << 4)) - (((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4));
                u16Delta3 =  (((pB[j+3] & 0xF0) >> 4) | (pB[j+5] << 4)) - ((pB[j+3] & 0x0F) | (pB[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                BData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                BData2 = ((u16Delta3 & 0x1F) << 4) | ((u16Delta2 & 0x10) >> 4);

                // For index i G0 data
                u16Temp_G = (pG[j] & 0x0F) | (pG[j+1] << 4);
                MLG(printf("[%u]-G1 , u16Temp_G=0x%x.\n",i,u16Temp_G));
                GData0 = u16Temp_G;

                // For G delta
                u16Delta1 =  (((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4)) - GData0;
                u16Delta2 =  ((pG[j+3] & 0x0F) | (pG[j+4] << 4)) - (((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4));
                u16Delta3 =  (((pG[j+3] & 0xF0) >> 4) | (pG[j+5] << 4)) - ((pG[j+3] & 0x0F) | (pG[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                GData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                GData2_L = ((u16Delta2 & 0x10) >> 4);
                GData2_H = (u16Delta3 & 0x1F);

                // For index i R0 data
                u16Temp_R = (pR[j] & 0x0F) | (pR[j+1] << 4);
                MLG(printf("[%u]-R1 , u16Temp_R=0x%x.\n",i,u16Temp_R));
                RData0 = u16Temp_R;

                // For R delta
                u16Delta1 =  (((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4)) - RData0;
                u16Delta2 =  ((pR[j+3] & 0x0F) | (pR[j+4] << 4)) - (((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4));
                u16Delta3 =  (((pR[j+3] & 0xF0) >> 4) | (pR[j+5] << 4)) - ((pR[j+3] & 0x0F) | (pR[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                RData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                RData2 = ((u16Delta3 & 0x1F) << 4) | ((u16Delta2 & 0x10) >> 4);
                DEnable = BIT(0) | BIT(1) | BIT(2);

                // =============== fill index i 's info here ============================
                //pMLG_Addr[i].REnable = pMLG_Addr[i].GEnable = pMLG_Addr[i].BEnable = TRUE;
                //pMLG_Addr[i].Dummy0 = pMLG_Addr[i].Dummy1 = pMLG_Addr[i].Dummy2 = pMLG_Addr[i].Dummy3 = 0;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }
                pMLG_Addr[i].LData =  ((GData2_L & 0x000F ) << 60) | ((GData1 & 0x0FFF) << 48 ) | ((GData0 & 0x0FFF) <<36 ) | ((BData2 & 0x0FFF) << 24) | ((BData1 & 0x0FFF) << 12)| (BData0 & 0x0FFF);
                pMLG_Addr[i].HData = ((DEnable & 0x000F) << 44) |( (RData2 & 0x0FFF) << 32) | ((RData1 & 0x0FFF) << 20) | ( (RData0 & 0x0FFF) << 8 ) | ((GData2_H & 0x0FF0 ) >>4);


            }
        }
        else    //For 256 Entry Mode
        {
        #if 0
            MLG(printf("\033[0;31m [%s][%d] 256 entry  \033[0m\n", __FUNCTION__, __LINE__));
            //Decode from Gamma table
            for (i = 0, j = 0; i < u16Count; i+=2 , j+=3)
            {
                // For index i B0/G0/R0 data
                u16Temp_B = (pB[j] & 0x0F) | (pB[j+1] << 4);
                MLG(printf("[%u]-B1 , u16Temp_B=0x%x.\n",i,u16Temp_B));
                pMLG_Addr[i].BData0 = u16Temp_B;

                u16Temp_G = (pG[j] & 0x0F) | (pG[j+1] << 4);
                MLG(printf("[%u]-G1 , u16Temp_G=0x%x.\n",i,u16Temp_G));
                pMLG_Addr[i].GData0 = u16Temp_G;

                u16Temp_R = (pR[j] & 0x0F) | (pR[j+1] << 4);
                MLG(printf("[%u]-R1 , u16Temp_R=0x%x.\n",i,u16Temp_R));
                pMLG_Addr[i].RData0 = u16Temp_R;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }

                // For index i+1  B0/G0/R0 data
                u16Temp_B = ((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4);
                MLG(printf("[%u]-B0 , B_u16Temp-0=0x%x.\n",i+1,u16Temp_B));
                pMLG_Addr[i+1].BData0 = u16Temp_B;

                u16Temp_G = ((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4);
                MLG(printf("[%u]-G0 , G_u16Temp-0=0x%x.\n",i+1,u16Temp_G));
                pMLG_Addr[i+1].GData0 = u16Temp_G;

                u16Temp_R = ((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4);
                MLG(printf("[%u]-R0 , R_u16Temp=0x%x.\n",i+1,u16Temp_R));
                pMLG_Addr[i+1].RData0 = u16Temp_R;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }

                // cal delta vaule
                u16GammaDeltaB = (pMLG_Addr[i+1].BData0 - pMLG_Addr[i].BData0) /4;
                u16GammaDeltaG = (pMLG_Addr[i+1].GData0 - pMLG_Addr[i].GData0) /4;
                u16GammaDeltaR = (pMLG_Addr[i+1].RData0 - pMLG_Addr[i].RData0) /4;

                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i].BData1 = ((u16GammaDeltaB & 0x0F) << 8) | (u16GammaDeltaB & 0x1F);
                pMLG_Addr[i].BData2 = ((u16GammaDeltaB & 0x1F) << 4) | ((u16GammaDeltaB & 0x10) >> 4);

                pMLG_Addr[i].GData1 = ((u16GammaDeltaG & 0x0F) << 8) | (u16GammaDeltaG & 0x1F);
                pMLG_Addr[i].GData2_L = ((u16GammaDeltaG & 0x10) >> 4);
                pMLG_Addr[i].GData2_H = (u16GammaDeltaG & 0x1F);

                pMLG_Addr[i].RData1 = ((u16GammaDeltaR & 0x0F) << 8) | (u16GammaDeltaR & 0x1F);
                pMLG_Addr[i].RData2 = ((u16GammaDeltaR & 0x1F) << 4) | ((u16GammaDeltaR & 0x10) >> 4);

                // =============== fill index i 's info here ============================
                pMLG_Addr[i].REnable = pMLG_Addr[i].GEnable = pMLG_Addr[i].BEnable = TRUE;
                pMLG_Addr[i].Dummy0 = pMLG_Addr[i].Dummy1 = pMLG_Addr[i].Dummy2 = pMLG_Addr[i].Dummy3 = 0;


                // For index i+2 B0/G0/R0 data, just in order to get index (i+1) ~ (i+2) 's delta
                u16Temp = (pB[j+3] & 0x0F) | (pB[j+4] << 4);
                MLG(printf("[%u]-B0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp));
                pMLG_Addr[i+2].BData0 = u16Temp;

                u16Temp = (pG[j+3] & 0x0F) | (pG[j+4] << 4);
                MLG(printf("[%u]-G0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp));
                pMLG_Addr[i+2].GData0 = u16Temp;

                u16Temp = (pR[j+3] & 0x0F) | (pR[j+4] << 4);
                MLG(printf("[%u]-R0 , G_u16Temp-0=0x%x.\n\n",i+2,u16Temp));
                pMLG_Addr[i+2].RData0 = u16Temp;

                // cal delta vaule
                u16GammaDeltaB = (pMLG_Addr[i+2].BData0 - pMLG_Addr[i+1].BData0) /4;
                u16GammaDeltaG = (pMLG_Addr[i+2].GData0 - pMLG_Addr[i+1].GData0) /4;
                u16GammaDeltaR = (pMLG_Addr[i+2].RData0 - pMLG_Addr[i+1].RData0) /4;

                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i+1].BData1 = ((u16GammaDeltaB & 0x0F) << 8) | (u16GammaDeltaB & 0x1F);
                pMLG_Addr[i+1].BData2 = ((u16GammaDeltaB & 0x1F) << 4) | ((u16GammaDeltaB & 0x10) >> 4);

                pMLG_Addr[i+1].GData1 = ((u16GammaDeltaG & 0x0F) << 8) | (u16GammaDeltaG & 0x1F);
                pMLG_Addr[i+1].GData2_L = (u16GammaDeltaG & 0x10) >> 4;
                pMLG_Addr[i+1].GData2_H = (u16GammaDeltaG & 0x1F);

                pMLG_Addr[i+1].RData1 = ((u16GammaDeltaR & 0x0F) << 8) | (u16GammaDeltaR & 0x1F);
                pMLG_Addr[i+1].RData2 = ((u16GammaDeltaR & 0x1F) << 4) | ((u16GammaDeltaR & 0x10) >> 4);

                // =============== fill index i+1 's info here ============================
                pMLG_Addr[i+1].REnable = pMLG_Addr[i+1].GEnable = pMLG_Addr[i+1].BEnable = TRUE;
                pMLG_Addr[i+1].Dummy0 = pMLG_Addr[i+1].Dummy1 = pMLG_Addr[i+1].Dummy2 = pMLG_Addr[i+1].Dummy3 = 0;

            }
            #endif
     }


        pMaxGammaValue[0] = pMaxGammaValue[0]|0xF00;
        pMaxGammaValue[1] = pMaxGammaValue[1]|0xF00;
        pMaxGammaValue[2] = pMaxGammaValue[2]|0xF00;

        MLG(printf("[MLG]Red   MaxGammaValue = 0x%x\n",pMaxGammaValue[0]));
        MLG(printf("[MLG]Green MaxGammaValue = 0x%x\n",pMaxGammaValue[1]));
        MLG(printf("[MLG]Blue  MaxGammaValue =0x%x\n",pMaxGammaValue[2]));

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint +=u16CmdCount;
    }
    else
    {
        printf("Size is not enough, Cmd Too Many !!!!!!!!!!!!!!!!!\n");
    }

    MLG(printf("stMLGInfo.u16WPoint = %d\n",_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint));
}
#endif

#ifdef A3_MLG  // need to refine later, test the capibility first
/////////////////////////////////////////////////////////////////////////////
//arrary[]=
//    0x00,0x00,0x01,0x00,0x02,0x03,0x00,0x04,0x05,  //DATA in format
//-------------------------------------------------------------------------//
// Menuload Gamma command (12 bit content)
//
// One MIU Word
// A3,A5 support this function
// [A3,A5]
// ______________________________________________________________________________
//      Byte | 0         1                2           3           4          5                      6
// [ gamma table B content]      [ gamma table G content]  [ gamma table R content]  [B enable] [G enable]  [R enable]
//            35:0                          71:36                           72:107                          144:144    145:145      146:146
//
//For example of 256 entry:
//R_in[35:0](0)        = { 12’h0, WR_R[11:0](0), 12’h0} for 256 entry
//R_in[35:0](1)        = { 12’h0, WR_R[11:0](1), WR_R[11:0](0)} for 256 entry
//R_in[35:0](2)        = { 12’h0, WR_R[11:0](2), WR_R[11:0](1)} for 256 entry
/////////////////////////////////////////////////////////////////////////////
static void MDrv_XC_MLG_WriteCmd(void *pInstance, MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue)
{
  //  printf("\33[0;35m Jayce! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    MS_SC_MLG_TBL *pMLG_Addr;
    MS_PHY DstAddr;
    MS_U16 i,j;
    MS_U16 u16Temp,u16Temp_R,u16Temp_G,u16Temp_B;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s,u16Count=%u.\n",__FUNCTION__,u16Count));
    if((Hal_XC_MLG_get_status(pInstance) == 0) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint))
    {
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
    }
    pMaxGammaValue[0] = pMaxGammaValue [1] = pMaxGammaValue [2] = 0;

    DstAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint * MS_MLG_CMD_LEN;
    pMLG_Addr = (MS_SC_MLG_TBL *)MS_PA2KSEG1(DstAddr);

    if ((_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint + u16Count) <= _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt)
    {
        pMLG_Addr[0].RData0 = 0;
        pMLG_Addr[0].GData0 = 0;
        pMLG_Addr[0].BData0 = 0;
        //Decode from Gamma table
        for (i = 0, j = 0; i < u16Count; i+=2 , j+=3)
        {
            //高位元低放置組//for 12 bit(8~11),放置到arrary[i-1];i=1
            u16Temp_B = 0;
            u16Temp_B = ((pB[j] & 0x0F) << 8) | (pB[j+1]);
            u16Temp_B = (u16Temp_B << 4);
            //printf("[MLG],[%u]-B1 , u16Temp_B=0x%x.\n",i,u16Temp_B);
            pMLG_Addr[i].BData1_L = (u16Temp_B & 0x000F);
            pMLG_Addr[i].BData1 = (u16Temp_B >> 4);;
// ______________________________________________________________________________
            u16Temp_G = 0;
            u16Temp_G = ((pG[j] & 0x0F) << 8) | (pG[j+1]);
            u16Temp_G = (u16Temp_G << 4);
            //printf("[MLG],[%u]-G1 , u16Temp_G=0x%x.\n",i,u16Temp_G);
            pMLG_Addr[i].GData1 = (u16Temp_G & 0x00FF);
            pMLG_Addr[i].GData1_H = (u16Temp_G >> 8);
// ______________________________________________________________________________
            u16Temp_R = 0;
            u16Temp_R = ((pR[j] & 0x0F) << 8) | (pR[j+1]);
            u16Temp_R = (u16Temp_R << 4);
            //printf("[MLG],[%u]-R1 , u16Temp_R=0x%x.\n",i,u16Temp_R);
            pMLG_Addr[i].RData1_L= (u16Temp_R & 0x000F);
            pMLG_Addr[i].RData1 = (u16Temp_R >> 4);
// =====================================================================
            u16Temp = 0;
            u16Temp = ((pB[j] & 0x0F) << 8) | (pB[j+1]);
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-B0 , B_u16Temp-0=0x%x.\n",i+1,u16Temp);
            pMLG_Addr[i+1].BData0 = (u16Temp & 0x00FF);
            pMLG_Addr[i+1].BData0_H = (u16Temp >> 8);
// ______________________________________________________________________________
            u16Temp = 0;
            u16Temp = ((pG[j] & 0x0F) << 8) | (pG[j+1]);
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-G0 , G_u16Temp-0=0x%x.\n",i+1,u16Temp);
            pMLG_Addr[i+1].GData0_L = (u16Temp & 0x000F);
            pMLG_Addr[i+1].GData0 = (u16Temp >> 4);
// ______________________________________________________________________________
            u16Temp = 0;
            u16Temp = ((pR[j] & 0x0F) << 8) | (pR[j+1]);
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-R0 , R_u16Temp=0x%x.\n",i+1,u16Temp);
            pMLG_Addr[i+1].RData0 = (u16Temp & 0x00FF);
            pMLG_Addr[i+1].RData0_H = (u16Temp >> 8);
// =====================================================================

            pMLG_Addr[i].REnable = pMLG_Addr[i].GEnable = pMLG_Addr[i].BEnable = TRUE;
            pMLG_Addr[i].Dummy0 = pMLG_Addr[i].Dummy1 = pMLG_Addr[i].Dummy2 = pMLG_Addr[i].Dummy3 = 0;
            pMLG_Addr[i].RData2 = pMLG_Addr[i].GData2 = pMLG_Addr[i].BData2 = 0;
            pMLG_Addr[i].RData2_H = pMLG_Addr[i].GData2_L = pMLG_Addr[i].BData2_H = 0;

            if(pMaxGammaValue[0] < (u16Temp_R>>4))
            {
                pMaxGammaValue[0] = (u16Temp_R>>4);
            }
            if(pMaxGammaValue[1] < (u16Temp_G>>4))
            {
                pMaxGammaValue[1] = (u16Temp_G>>4);
            }
            if(pMaxGammaValue[2] < (u16Temp_B>>4))
            {
                pMaxGammaValue[2] = (u16Temp_B>>4);
            }

//    0x00,0x00,0x01,0x00,0x02,0x03,0x00,0x04,0x05,  //DATA in format
            //高位元高放置組//for 12 bit(8~11),放置到arrary[i+1];i=2
            u16Temp_B= 0;
            u16Temp_B = ((pB[j] & 0xF0) << 4) | (pB[j+2]);;
            u16Temp_B = (u16Temp_B << 4);
            //printf("[MLG],[%u]-B1 , u16Temp_B =0x%x.\n",i+1,u16Temp_B);
            pMLG_Addr[i+1].BData1_L = (u16Temp_B & 0x000F);
            pMLG_Addr[i+1].BData1 = (u16Temp_B >> 4);
// ______________________________________________________________________________
            u16Temp_G = 0;
            u16Temp_G = ((pG[j] & 0xF0) << 4) | (pG[j+2]);;
            u16Temp_G = (u16Temp_G << 4);
            //printf("[MLG],[%u]-G1 , u16Temp_G = 0x%x.\n",i+1,u16Temp_G);
            pMLG_Addr[i+1].GData1 = (u16Temp_G & 0x00FF);
            pMLG_Addr[i+1].GData1_H = (u16Temp_G >> 8);
// ______________________________________________________________________________
            u16Temp_R = 0;
            u16Temp_R = ((pR[j] & 0xF0) << 4) | (pR[j+2]);;
            u16Temp_R = (u16Temp_R << 4);
            //printf("[MLG],[%u]-R1 , u16Temp_R=0x%x.\n",i+1,u16Temp_R);
            pMLG_Addr[i+1].RData1_L = (u16Temp_R & 0x000F);
            pMLG_Addr[i+1].RData1 = (u16Temp_R >> 4);
// =====================================================================
            u16Temp = 0;
            u16Temp = ((pB[j] & 0xF0) << 4) | (pB[j+2]);;
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-B0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp);
            pMLG_Addr[i+2].BData0 = (u16Temp & 0x00FF);
            pMLG_Addr[i+2].BData0_H = (u16Temp >> 8);
// ______________________________________________________________________________
            u16Temp = 0;
            u16Temp = ((pG[j] & 0xF0) << 4) | (pG[j+2]);;
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-G0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp);
            pMLG_Addr[i+2].GData0_L = (u16Temp & 0x000F);
            pMLG_Addr[i+2].GData0 = (u16Temp >> 4);
// ______________________________________________________________________________
            u16Temp = 0;
            u16Temp = ((pR[j] & 0xF0) << 4) | (pR[j+2]);;
            u16Temp = (u16Temp << 4);
            //printf("[MLG],[%u]-R0 , G_u16Temp-0=0x%x.\n\n",i+2,u16Temp);
            pMLG_Addr[i+2].RData0 = (u16Temp & 0x00FF);
            pMLG_Addr[i+2].RData0_H = (u16Temp >> 8);
// =====================================================================

            pMLG_Addr[i+1].REnable = pMLG_Addr[i+1].GEnable = pMLG_Addr[i+1].BEnable = TRUE;
            pMLG_Addr[i+1].Dummy0 = pMLG_Addr[i+1].Dummy1 =pMLG_Addr[i+1].Dummy2 =pMLG_Addr[i+1].Dummy3 =0;
            pMLG_Addr[i+1].RData2 = pMLG_Addr[i+1].GData2 = pMLG_Addr[i+1].BData2 = 0;
            pMLG_Addr[i+1].RData2_H = pMLG_Addr[i+1].GData2_L = pMLG_Addr[i+1].BData2_H = 0;

            if(pMaxGammaValue[0] < (u16Temp_R>>4))
            {
                pMaxGammaValue[0] = (u16Temp_R>>4);
            }
            if(pMaxGammaValue[1] < (u16Temp_G>>4))
            {
                pMaxGammaValue[1] = (u16Temp_G>>4);
            }
            if(pMaxGammaValue[2] < (u16Temp_B>>4))
            {
                pMaxGammaValue[2] = (u16Temp_B>>4);
            }
        }

        pMaxGammaValue[0] = pMaxGammaValue[0]|0xF00;
        pMaxGammaValue[1] = pMaxGammaValue[1]|0xF00;
        pMaxGammaValue[2] = pMaxGammaValue[2]|0xF00;

        MLG(printf("[MLG]Red   MaxGammaValue = 0x%x\n",pMaxGammaValue[0]));
        MLG(printf("[MLG]Green MaxGammaValue = 0x%x\n",pMaxGammaValue[1]));
        MLG(printf("[MLG]Blue  MaxGammaValue =0x%x\n",pMaxGammaValue[2]));

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint +=u16Count;
    }
    else
    {
        printf("Size is not enough, Cmd Too Many !!!!!!!!!!!!!!!!!\n");
    }
  //  printf("\33[0;35m Jayce : [%s][%d],Red MaxGammaValue = 0x%x \33[m \n",__FUNCTION__,__LINE__,pMaxGammaValue[0]);
  //  printf("\33[0;35m Jayce : [%s][%d],Green MaxGammaValue = 0x%x \33[m \n",__FUNCTION__,__LINE__,pMaxGammaValue[1]);
  //  printf("\33[0;35m Jayce : [%s][%d],Blue MaxGammaValue = 0x%x \33[m \n",__FUNCTION__,__LINE__,pMaxGammaValue[2]);
    MLG(printf("stMLGInfo.u16WPoint = %d\n",_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint));
}
#else
#ifdef MLG_1024
/////////////////////////////////////////////////////////////////////////////
//arrary[]=
//    0x00,0x00,0x01,0x00,0x02,0x03,0x00,0x04,0x05,  //DATA in format
//-------------------------------------------------------------------------//
// Menuload Gamma command (12 bit content)
//
// One MIU Word
// A3,A5 support this function
// [A3,A5]
// ______________________________________________________________________________
//      Byte | 0         1                2           3           4          5                      6
// [ gamma table B content]      [ gamma table G content]  [ gamma table R content]  [B enable] [G enable]  [R enable]
//            35:0                          71:36                           72:107                          144:144    145:145      146:146 ....255
//
//B0[11:0]                      : B_ref[11:0]
//{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
// ===> {B2[11:0], B1[11:0], B0[11:0]} : {3'b0, B_delta3[4:0], 3'b0, B_delta2[4:0], 3'b0, B_delta1[4:0], B_ref[11:0]}
static void MDrv_XC_MLG_WriteCmd(void *pInstance, MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue)
{
   // printf("\33[0;35m Jayce! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    MS_SC_MLG_TBL *pMLG_Addr;
    MS_PHYADDR DstAddr;
    MS_U16 i,j;
    MS_U16 u16Temp,u16Temp_R,u16Temp_G,u16Temp_B, u16GammaDeltaR, u16GammaDeltaG, u16GammaDeltaB;
    MS_U16 u16CmdCount = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s,u16Count=%u.\n",__FUNCTION__,u16Count));
    if((Hal_XC_MLG_get_status(pInstance) == 0) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint))
    {
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
    }
    pMaxGammaValue[0] = pMaxGammaValue [1] = pMaxGammaValue [2] = 0;

    DstAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint * MS_MLG_CMD_LEN;
    pMLG_Addr = (MS_SC_MLG_TBL *)MS_PA2KSEG1(DstAddr);

    if (u16Count == 1024)//For 1024 Entry Mode
        u16CmdCount = u16Count/4;
    else    //For 256 Entry Mode
        u16CmdCount = u16Count;
  //  printf("\33[0;35m Jayce! : [%s][%d],_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt=[%d] \33[m \n",__FUNCTION__,__LINE__,_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt);
    if ((_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint + u16CmdCount) <= _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt)
    {
        if (u16Count == 1024)//For 1024 Entry Mode
        {
            MLG(printf("\033[0;31m [%s][%d] 1024 entry  \033[0m\n", __FUNCTION__, __LINE__));
            MS_U16 u16Delta1, u16Delta2, u16Delta3;
            //Decode from Gamma table
            for (i = 0, j = 0; i < u16Count/4; i+=1 , j+=6)
            {
                // For index i B0 data
                u16Temp_B = (pB[j] & 0x0F) | (pB[j+1] << 4);
                MLG(printf("[%u]-B1 , u16Temp_B=0x%x.\n",i,u16Temp_B));
                pMLG_Addr[i].BData0 = u16Temp_B;

                // For B delta
                u16Delta1 =  (((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4)) - pMLG_Addr[i].BData0;
                u16Delta2 =  ((pB[j+3] & 0x0F) | (pB[j+4] << 4)) - (((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4));
                u16Delta3 =  (((pB[j+3] & 0xF0) >> 4) | (pB[j+5] << 4)) - ((pB[j+3] & 0x0F) | (pB[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i].BData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                pMLG_Addr[i].BData2 = ((u16Delta3 & 0x1F) << 4) | ((u16Delta2 & 0x10) >> 4);

                // For index i G0 data
                u16Temp_G = (pG[j] & 0x0F) | (pG[j+1] << 4);
                MLG(printf("[%u]-G1 , u16Temp_G=0x%x.\n",i,u16Temp_G));
                pMLG_Addr[i].GData0 = u16Temp_G;

                // For G delta
                u16Delta1 =  (((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4)) - pMLG_Addr[i].GData0;
                u16Delta2 =  ((pG[j+3] & 0x0F) | (pG[j+4] << 4)) - (((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4));
                u16Delta3 =  (((pG[j+3] & 0xF0) >> 4) | (pG[j+5] << 4)) - ((pG[j+3] & 0x0F) | (pG[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i].GData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                pMLG_Addr[i].GData2_L = ((u16Delta2 & 0x10) >> 4);
                pMLG_Addr[i].GData2_H = (u16Delta3 & 0x1F);

                // For index i R0 data
                u16Temp_R = (pR[j] & 0x0F) | (pR[j+1] << 4);
                MLG(printf("[%u]-R1 , u16Temp_R=0x%x.\n",i,u16Temp_R));
                pMLG_Addr[i].RData0 = u16Temp_R;

                // For R delta
                u16Delta1 =  (((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4)) - pMLG_Addr[i].RData0;
                u16Delta2 =  ((pR[j+3] & 0x0F) | (pR[j+4] << 4)) - (((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4));
                u16Delta3 =  (((pR[j+3] & 0xF0) >> 4) | (pR[j+5] << 4)) - ((pR[j+3] & 0x0F) | (pR[j+4] << 4));
                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i].RData1 = ((u16Delta2 & 0x0F) << 8) | (u16Delta1 & 0x1F);
                pMLG_Addr[i].RData2 = ((u16Delta3 & 0x1F) << 4) | ((u16Delta2 & 0x10) >> 4);

                // =============== fill index i 's info here ============================
                pMLG_Addr[i].REnable = pMLG_Addr[i].GEnable = pMLG_Addr[i].BEnable = TRUE;
                pMLG_Addr[i].Dummy0 = pMLG_Addr[i].Dummy1 = pMLG_Addr[i].Dummy2 = pMLG_Addr[i].Dummy3 = 0;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }

            }
        }
        else    //For 256 Entry Mode
        {
            MLG(printf("\033[0;31m [%s][%d] 256 entry  \033[0m\n", __FUNCTION__, __LINE__));
            //Decode from Gamma table
            for (i = 0, j = 0; i < u16Count; i+=2 , j+=3)
            {
                // For index i B0/G0/R0 data
                u16Temp_B = (pB[j] & 0x0F) | (pB[j+1] << 4);
                MLG(printf("[%u]-B1 , u16Temp_B=0x%x.\n",i,u16Temp_B));
                pMLG_Addr[i].BData0 = u16Temp_B;

                u16Temp_G = (pG[j] & 0x0F) | (pG[j+1] << 4);
                MLG(printf("[%u]-G1 , u16Temp_G=0x%x.\n",i,u16Temp_G));
                pMLG_Addr[i].GData0 = u16Temp_G;

                u16Temp_R = (pR[j] & 0x0F) | (pR[j+1] << 4);
                MLG(printf("[%u]-R1 , u16Temp_R=0x%x.\n",i,u16Temp_R));
                pMLG_Addr[i].RData0 = u16Temp_R;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }

                // For index i+1  B0/G0/R0 data
                u16Temp_B = ((pB[j] & 0xF0) >> 4) | (pB[j+2] << 4);
                MLG(printf("[%u]-B0 , B_u16Temp-0=0x%x.\n",i+1,u16Temp_B));
                pMLG_Addr[i+1].BData0 = u16Temp_B;

                u16Temp_G = ((pG[j] & 0xF0) >> 4) | (pG[j+2] << 4);
                MLG(printf("[%u]-G0 , G_u16Temp-0=0x%x.\n",i+1,u16Temp_G));
                pMLG_Addr[i+1].GData0 = u16Temp_G;

                u16Temp_R = ((pR[j] & 0xF0) >> 4) | (pR[j+2] << 4);
                MLG(printf("[%u]-R0 , R_u16Temp=0x%x.\n",i+1,u16Temp_R));
                pMLG_Addr[i+1].RData0 = u16Temp_R;

                if(pMaxGammaValue[0] < (u16Temp_R >> 4))
                {
                    pMaxGammaValue[0] = (u16Temp_R >> 4);
                }
                if(pMaxGammaValue[1] < (u16Temp_G >> 4))
                {
                    pMaxGammaValue[1] = (u16Temp_G >> 4);
                }
                if(pMaxGammaValue[2] < (u16Temp_B >> 4))
                {
                    pMaxGammaValue[2] = (u16Temp_B >> 4);
                }

                // cal delta vaule
                u16GammaDeltaB = (pMLG_Addr[i+1].BData0 - pMLG_Addr[i].BData0) /4;
                u16GammaDeltaG = (pMLG_Addr[i+1].GData0 - pMLG_Addr[i].GData0) /4;
                u16GammaDeltaR = (pMLG_Addr[i+1].RData0 - pMLG_Addr[i].RData0) /4;

                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i].BData1 = ((u16GammaDeltaB & 0x0F) << 8) | (u16GammaDeltaB & 0x1F);
                pMLG_Addr[i].BData2 = ((u16GammaDeltaB & 0x1F) << 4) | ((u16GammaDeltaB & 0x10) >> 4);

                pMLG_Addr[i].GData1 = ((u16GammaDeltaG & 0x0F) << 8) | (u16GammaDeltaG & 0x1F);
                pMLG_Addr[i].GData2_L = ((u16GammaDeltaG & 0x10) >> 4);
                pMLG_Addr[i].GData2_H = (u16GammaDeltaG & 0x1F);

                pMLG_Addr[i].RData1 = ((u16GammaDeltaR & 0x0F) << 8) | (u16GammaDeltaR & 0x1F);
                pMLG_Addr[i].RData2 = ((u16GammaDeltaR & 0x1F) << 4) | ((u16GammaDeltaR & 0x10) >> 4);

                // =============== fill index i 's info here ============================
                pMLG_Addr[i].REnable = pMLG_Addr[i].GEnable = pMLG_Addr[i].BEnable = TRUE;
                pMLG_Addr[i].Dummy0 = pMLG_Addr[i].Dummy1 = pMLG_Addr[i].Dummy2 = pMLG_Addr[i].Dummy3 = 0;


                // For index i+2 B0/G0/R0 data, just in order to get index (i+1) ~ (i+2) 's delta
                u16Temp = (pB[j+3] & 0x0F) | (pB[j+4] << 4);
                MLG(printf("[%u]-B0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp));
                pMLG_Addr[i+2].BData0 = u16Temp;

                u16Temp = (pG[j+3] & 0x0F) | (pG[j+4] << 4);
                MLG(printf("[%u]-G0 , G_u16Temp-0=0x%x.\n",i+2,u16Temp));
                pMLG_Addr[i+2].GData0 = u16Temp;

                u16Temp = (pR[j+3] & 0x0F) | (pR[j+4] << 4);
                MLG(printf("[%u]-R0 , G_u16Temp-0=0x%x.\n\n",i+2,u16Temp));
                pMLG_Addr[i+2].RData0 = u16Temp;

                // cal delta vaule
                u16GammaDeltaB = (pMLG_Addr[i+2].BData0 - pMLG_Addr[i+1].BData0) /4;
                u16GammaDeltaG = (pMLG_Addr[i+2].GData0 - pMLG_Addr[i+1].GData0) /4;
                u16GammaDeltaR = (pMLG_Addr[i+2].RData0 - pMLG_Addr[i+1].RData0) /4;

                //{B2[11:0], B1[11:0]}    : {{3'b0, B_delta3[4:0]}, {3'b0, B_delta2[4:0]}, {3'b0, B_delta1[4:0]}}
                pMLG_Addr[i+1].BData1 = ((u16GammaDeltaB & 0x0F) << 8) | (u16GammaDeltaB & 0x1F);
                pMLG_Addr[i+1].BData2 = ((u16GammaDeltaB & 0x1F) << 4) | ((u16GammaDeltaB & 0x10) >> 4);

                pMLG_Addr[i+1].GData1 = ((u16GammaDeltaG & 0x0F) << 8) | (u16GammaDeltaG & 0x1F);
                pMLG_Addr[i+1].GData2_L = (u16GammaDeltaG & 0x10) >> 4;
                pMLG_Addr[i+1].GData2_H = (u16GammaDeltaG & 0x1F);

                pMLG_Addr[i+1].RData1 = ((u16GammaDeltaR & 0x0F) << 8) | (u16GammaDeltaR & 0x1F);
                pMLG_Addr[i+1].RData2 = ((u16GammaDeltaR & 0x1F) << 4) | ((u16GammaDeltaR & 0x10) >> 4);

                // =============== fill index i+1 's info here ============================
                pMLG_Addr[i+1].REnable = pMLG_Addr[i+1].GEnable = pMLG_Addr[i+1].BEnable = TRUE;
                pMLG_Addr[i+1].Dummy0 = pMLG_Addr[i+1].Dummy1 = pMLG_Addr[i+1].Dummy2 = pMLG_Addr[i+1].Dummy3 = 0;

            }
        }


        pMaxGammaValue[0] = pMaxGammaValue[0]|0xF00;
        pMaxGammaValue[1] = pMaxGammaValue[1]|0xF00;
        pMaxGammaValue[2] = pMaxGammaValue[2]|0xF00;

        MLG(printf("[MLG]Red   MaxGammaValue = 0x%x\n",pMaxGammaValue[0]));
        MLG(printf("[MLG]Green MaxGammaValue = 0x%x\n",pMaxGammaValue[1]));
        MLG(printf("[MLG]Blue  MaxGammaValue =0x%x\n",pMaxGammaValue[2]));

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint +=u16CmdCount;
    }
    else
    {
        printf("Size is not enough, Cmd Too Many !!!!!!!!!!!!!!!!!\n");
    }

    MLG(printf("stMLGInfo.u16WPoint = %d\n",_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint));
}
#else  // MLG_256 or not support MLG case
/////////////////////////////////////////////////////////////////////////////
// Menuload Gamma command (12 bit content)
//
// One MIU Word
// T8, T11, T12 support this function
// [T8, T12]             128 Bit = 16 Bytess
// ______________________________________________________________________________
//      Byte | 0         1                2           3           4          5                      6
// [ gamma table B content]      [ gamma table G content]  [ gamma table R content]  [B enable] [G enable]  [R enable]
//            11:0                      27:16                       43:32               48:48     49:49        50:50
//
static void MDrv_XC_MLG_WriteCmd(void *pInstance, MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue)
{
    MS_SC_MLG_TBL *pMLG_Addr;
    MS_PHYADDR DstAddr;
    MS_U16 i,j;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s\n",__FUNCTION__));
    if((Hal_XC_MLG_get_status(pInstance) == 0) &&
       (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint == _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint))
    {
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
    }
    pMaxGammaValue[0] = pMaxGammaValue [1] = pMaxGammaValue [2] = 0;

    DstAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint * MS_MLG_CMD_LEN;
    pMLG_Addr = (MS_SC_MLG_TBL *)MS_PA2KSEG1(DstAddr);

    if ((_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint + u16Count) <= _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt)
    {

        for (i = 0, j = 0; i < u16Count; i+=2 , j+=3)
        {
            pMLG_Addr[i].u16R = (pR[j] & 0x0F) | (pR[j+1] << 4);
            pMLG_Addr[i].u16G = (pG[j] & 0x0F) | (pG[j+1] << 4);
            pMLG_Addr[i].u16B = (pB[j] & 0x0F) | (pB[j+1] << 4);
            pMLG_Addr[i].u16Enable = BIT(0) | BIT(1) | BIT(2);

            MLG(printf("MLG_Addr[%d].u16R = 0x%x\n",i, pMLG_Addr[i].u16R));
            MLG(printf("MLG_Addr[%d].u16G = 0x%x\n",i, pMLG_Addr[i].u16G));
            MLG(printf("MLG_Addr[%d].u16B = 0x%x\n",i, pMLG_Addr[i].u16B));
            if(pMaxGammaValue[0] < pMLG_Addr[i].u16R)
            {
                pMaxGammaValue[0] = pMLG_Addr[i].u16R;
            }
            if(pMaxGammaValue[1] < pMLG_Addr[i].u16G)
            {
                pMaxGammaValue[1] = pMLG_Addr[i].u16G;
            }
            if(pMaxGammaValue[2] < pMLG_Addr[i].u16B)
            {
                pMaxGammaValue[2] = pMLG_Addr[i].u16B;
            }

            pMLG_Addr[i+1].u16R = (pR[j] >>4) | (pR[j+2] << 4);
            pMLG_Addr[i+1].u16G = (pG[j] >>4) | (pG[j+2] << 4);
            pMLG_Addr[i+1].u16B = (pB[j] >>4) | (pB[j+2] << 4);
            pMLG_Addr[i+1].u16Enable = BIT(0) | BIT(1) | BIT(2);

            MLG(printf("MLG_Addr[%d].u16R = 0x%x\n",i+1, pMLG_Addr[i+1].u16R));
            MLG(printf("MLG_Addr[%d].u16G = 0x%x\n",i+1, pMLG_Addr[i+1].u16G));
            MLG(printf("MLG_Addr[%d].u16B = 0x%x\n",i+1, pMLG_Addr[i+1].u16B));
            if(pMaxGammaValue[0] < pMLG_Addr[i+1].u16R)
            {
                pMaxGammaValue[0] = pMLG_Addr[i+1].u16R;
            }
            if(pMaxGammaValue[1] < pMLG_Addr[i+1].u16G)
            {
                pMaxGammaValue[1] = pMLG_Addr[i+1].u16G;
            }
            if(pMaxGammaValue[2] < pMLG_Addr[i+1].u16B)
            {
                pMaxGammaValue[2] = pMLG_Addr[i+1].u16B;
            }
            MLG(printf("Red   MaxGammaValue = 0x%x\n",pMaxGammaValue[0]));
            MLG(printf("Green MaxGammaValue = 0x%x\n",pMaxGammaValue[1]));
            MLG(printf("Blue  MaxGammaValue =0x%x\n",pMaxGammaValue[2]));
        }

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint +=u16Count;
    }
    else
    {
        MLDBG(printf("Size is not enough, Cmd Too Many !!!!!!!!!!!!!!!!!\n"));
    }
    MLG(printf("stMLGInfo.u16WPoint = %d\n",_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint));
}
#endif
#endif
//-------------------------------------------------------------------------------------------------
/// Initialize the Menuload Gamma
/// @param  PhyAddr                 \b IN: the physical address for the menuload
/// @param  u32BufByteLen           \b IN: the buffer length of the menu load commands
/// @return  void
//-------------------------------------------------------------------------------------------------
void MApi_XC_MLG_Init_U2(void* pInstance, MS_PHY PhyAddr, MS_U32 u32BufByteLen)
{
  //  printf("\33[0;35m Jayce! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MLG(printf("%s: 0x%lx, 0x%lx\n", __FUNCTION__, PhyAddr, u32BufByteLen));

    if(Hal_XC_MLG_GetCaps(pInstance))
    {

        memset((void *)MS_PA2KSEG1(PhyAddr), 0, u32BufByteLen);

        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.bEnable = FALSE;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt = (MS_U16)(u32BufByteLen / MS_MLG_CMD_LEN);
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr = PhyAddr;

        MDrv_XC_MLG_Init(pInstance, PhyAddr);
        MLG(printf("Cmd max number = %d\n",_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt));
       // printf("\33[0;35m Jayce : [%s][%d],PhyAddr=[d] \33[m \n",__FUNCTION__,__LINE__,PhyAddr);
      //  printf("\33[0;35m Jayce! : [%s][%d],Cmd max number = %d \33[m \n",__FUNCTION__,__LINE__,_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16MaxCmdCnt);
    }

}

void MApi_XC_MLG_Init(MS_PHYADDR PhyAddr, MS_U32 u32BufByteLen)
{
#if 0
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_MLG_INIT XCArgs;
    XCArgs.PhyAddr = PhyAddr;
    XCArgs.u32BufByteLen = u32BufByteLen;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_MLG_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif


if (pu32XCInst == NULL)
{
    printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    return;
}

stXC_MLOAD_INIT XCArgs;
XCArgs.PhyAddr = PhyAddr;
XCArgs.u32BufByteLen = u32BufByteLen;

if(UtopiaIoctl(pu32XCInst, E_XC_CMD_MLG_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
{
    printf("Obtain XC engine fail\n");
    return;
}
else
{
    return;
}



}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable the Menuload Gamma
/// @return  void
//-------------------------------------------------------------------------------------------------
void MApi_XC_MLG_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.bEnable = bEnable;
}

void MApi_XC_MLG_Enable(MS_BOOL bEnable)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
     //   printf("\33[0;35m Jayce!!!!!!!!!!!!!!!!!!!!!!! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_MLG_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_MLG_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
   //     printf("\33[0;35m Jayce!!!!!!!!!!!!!!!!!!!!!!! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the caps of Menuload Gamma
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_MLG_GetCaps_U2(void* pInstance)
{
    return Hal_XC_MLG_GetCaps(pInstance);
}

MS_BOOL MApi_XC_MLG_GetCaps(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_MLG_GET_CAPS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_MLG_GET_CAPS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the status of Menuload Gamma
/// @return  MLG_TYPE
//-------------------------------------------------------------------------------------------------
MLG_TYPE MApi_XC_MLG_GetStatus_U2(void* pInstance)
{
    MLG_TYPE type = E_MLG_UNSUPPORTED;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (Hal_XC_MLG_GetCaps(pInstance))
    {
        if (_stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.bEnable)
        {
            type = E_MLG_ENABLED;
        }
        else
        {
            type = E_MLG_DISABLED;
        }
    }
    return type;
}

MLG_TYPE MApi_XC_MLG_GetStatus(void)
{
    if (pu32XC_Kernel_Inst == NULL)
    {
        printf("\033[1;33m[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\033[m\n",__FUNCTION__,__LINE__);
        return E_MLG_UNSUPPORTED;
    }

    stXC_MLG_GETSTATUS XCArgs;
    XCArgs.eReturnValue = E_MLG_UNSUPPORTED;

    if(UtopiaIoctl(pu32XC_Kernel_Inst, E_XC_CMD_MLG_GETSTATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_MLG_UNSUPPORTED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Fire the Menuload gamma commands
/// @return  TRUE if succeed, FALSE if failed
//-------------------------------------------------------------------------------------------------
void MApi_XC_MLG_Fire(MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue)
{
  //  printf("\33[0;35m Jayce! : [%s][%d] \33[m \n",__FUNCTION__,__LINE__);
    void* pInstance = pu32XCInst_private;
    //MS_U32 u32Delayms = 0;
    #define BUSY          1
    #define AVAILABLE     0
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _MLOAD_ENTRY(pInstance);
#else //SC2_ENABLE
    _MLOAD_ENTRY();
#endif //#if SC2_ENABLE



    #if 0
    MS_U8 u8MLoadMIUSel = Hal_XC_MLoad_get_miusel();

    // Before MLG, set miu as MLG status
    if (u8MLoadMIUSel != u8XCMLoadMIUSel)
    {
        MLG(printf("\033[0;36m [%s][%d] Before MLG, set miu as MLG status  \033[0m\n", __FUNCTION__, __LINE__));
        Hal_XC_MLoad_set_miusel(u8XCMLoadMIUSel);
    }
    #endif

    MS_U16 u16CmdCount = 0;
    if (u16Count == 1024)//For 1024 Entry Mode
        u16CmdCount = u16Count/4;
    else    //For 256 Entry Mode
        u16CmdCount = u16Count;

    //intf("[MLG]%s,Hal_XC_MLG_GetCaps()=%u,Hal_XC_MLG_get_status()=%u.\n",__FUNCTION__,Hal_XC_MLG_GetCaps(),Hal_XC_MLG_get_status());
    if(Hal_XC_MLG_GetCaps(pInstance) && (Hal_XC_MLG_get_status(pInstance) == AVAILABLE))
    {
        MS_PHYADDR CmdBufAddr;
        MDrv_XC_MLG_WriteCmd(pInstance, pR, pG, pB, u16Count, pMaxGammaValue);
        MsOS_FlushMemory();
        CmdBufAddr = _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.PhyAddr + _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint * MS_MLG_CMD_LEN;
        MDrv_XC_MLG_Trigger(pInstance, CmdBufAddr, u16CmdCount);
        _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint += u16CmdCount;

       // while((Hal_XC_MLG_get_status(pInstance) == BUSY) && u32Delayms < 600)
       // {
            MsOS_DelayTask(40);
       //     u32Delayms++;
      //  }

        // Patch HW outpu Vsync plus width too short and cause MLoad missing.
        // T3 U06 will fix it.
        //intf("[MLG]%s,Hal_XC_MLG_get_status()=%u.\n",__FUNCTION__,Hal_XC_MLG_get_status());
        if(Hal_XC_MLG_get_status(pInstance) == BUSY)
        {
           //MLDBG_WARN(printf("[Gamma]MLoad, TimeOut\n"));
            Hal_XC_MLG_set_on_off(pInstance, DISABLE);
            // add reset??
            //MDrv_XC_SoftwareReset(REST_CLK, MAIN_WINDOW);
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16RPoint = 0;
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16WPoint = 0;
            _stdrvXC_Menuload[psXCInstPri->u32DeviceID].stMLGInfo.u16FPoint = 0;
        }
    }

    #if 0
    // after MLG, set miu as original status
    if (u8MLoadMIUSel != u8XCMLoadMIUSel)
    {
        MLG(printf("\033[0;36m [%s][%d] after MLG, set miu as original status   \033[0m\n", __FUNCTION__, __LINE__));
        Hal_XC_MLoad_set_miusel(u8MLoadMIUSel);
    }
    #endif


#if SC2_ENABLE
    _MLOAD_RETURN(pInstance);
#else //SC2_ENABLE
    _MLOAD_RETURN();
#endif //#if SC2_ENABLE
}


//___|T_________________........__|T____ VSync
//__________|T__________________         ATP(refer the size befor memory to cal the pip sub and main length)
//_________________|T___________         Disp

//Generate TRAIN_TRIG_P from delayed line of Vsync(Setting the delay line for Auto tune area)(monaco ip DI point add trig engine)
//Generate DISP_TRIG_P from delayed line of Vsync(Setting the delay line for Display area)(monaco ip DI point add trig engine)
MS_BOOL MDrv_XC_MLoad_set_IP_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1A_L, u16train, 0x0FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1B_L, u16disp,  0x0FFF);
#endif
    return TRUE;
}

//Get the delay line for Auto tune area
//Get the delay line for Display area
MS_BOOL MDrv_XC_MLoad_get_IP_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    *pu16Train = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1A_L, 0x0FFF);
    *pu16Disp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1B_L, 0x0FFF);
#endif
    return TRUE;
}

void MDrv_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync)
{
    Hal_XC_MLoad_set_trigger_sync(pInstance,eTriggerSync);
}
