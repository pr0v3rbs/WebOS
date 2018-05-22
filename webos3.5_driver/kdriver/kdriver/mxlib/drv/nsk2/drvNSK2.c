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
///////////////////////////////////////////////////////////////////////////////
#define _DRVNSK2_C

////////////////////////////////////////////////////////////////////////////////
/// file drvNSK2.c
/// @author MStar Semiconductor Inc.
/// @brief
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"

#include "drvNSK2.h"
#include "halNSK2.h"
#include "drvNSK2Type.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static MS_U32 _g32NSK2DbgLv = NSK2_DBGLV_DEBUG;

#define DRVNSK2_DBG(lv, x, args...)   if (lv <= _g32NSK2DbgLv ) \
                                        {printf(x, ##args);}

#define NSK2_TASK_STACK_SIZE        16*1024

#define NDS_NSK2_IRQ                E_INT_IRQ_CA_NSK_INT//E_INT_IRQ_CA_SVP

#define NSK2_INT_EVENT_0            0x00000001
#define NSK2_INT_GROUP              0x0000FFFF

////////////////////////////////////////////////////////////////////////////////
// External funciton
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local Variables
////////////////////////////////////////////////////////////////////////////////
static MS_U32  u32NSK2_Stack[NSK2_TASK_STACK_SIZE/sizeof(MS_U32)];
static MS_S32  s32NSK2_TaskId = -1;
static MS_S32  s32NSK2_EventId = -1;


static NSK2_IntNotify _gNs2IntNtf;


////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////


static void _NSK2_Isr(void)
{
    //MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    MsOS_SetEvent(s32NSK2_EventId, NSK2_INT_EVENT_0);
}


static void _NSK2_Isr_Task(void)
{
    MS_U32              u32Reg;
    MS_U32              u32Events = 0;

    while(1)
    {
        MsOS_WaitEvent(s32NSK2_EventId, NSK2_INT_GROUP, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        //MsOS_DisableInterrupt(NDS_NSK_IRQ);
        DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"s32NSK2_EventId = %ld\n",s32NSK2_EventId);
        if (u32Events & NSK2_INT_EVENT_0)
        {
            u32Reg = HAL_NSK2_GetIntStatus();

            HAL_NSK2_ClearInt(u32Reg);
	    if(_gNs2IntNtf != NULL)
	    {
            	_gNs2IntNtf(u32Reg, 0);
	    }

        }
        else
        {
            u32Reg = HAL_NSK2_GetIntStatus();

            HAL_NSK2_ClearInt(u32Reg);
        }
        MsOS_EnableInterrupt(NDS_NSK2_IRQ);

        MsOS_DelayTask(1);
    }
}



////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////


MS_U32 MDrv_NSK2_Init(void)
{
    //init clock, register bank
    MS_U32 u32Base, u32BankSize;      // Non-PM bank

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"enter %s \n", __FUNCTION__);

    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32BankSize, MS_MODULE_HW))
    {
        DRVNSK2_DBG(NSK2_DBGLV_EMERG,"%s, %d MDrv_MMIO_GetBASE (NonPM base)fail\n", __FUNCTION__,__LINE__);
        MS_ASSERT(0);
    }

    HAL_NSK2_SetBase(u32Base); //set non pm bank
    HAL_NSK2_Init();

    //HAL_NSK2_SWReset();
    s32NSK2_EventId = MsOS_CreateEventGroup("NDS_NSK2_Event");
    if (s32NSK2_EventId < 0)
    {
        return FALSE;
    }

    s32NSK2_TaskId = MsOS_CreateTask((TaskEntry)_NSK2_Isr_Task,
                                    (MS_U32)NULL,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    u32NSK2_Stack,
                                    NSK2_TASK_STACK_SIZE,
                                    "NDS_NSK2_Task");
    if (s32NSK2_TaskId < 0)
    {
        MsOS_DeleteEventGroup(s32NSK2_EventId);
        return FALSE;
    }

    MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    MsOS_DetachInterrupt(NDS_NSK2_IRQ);
    MsOS_AttachInterrupt(NDS_NSK2_IRQ, (InterruptCb)_NSK2_Isr);
    MsOS_EnableInterrupt(NDS_NSK2_IRQ);


    return TRUE;
}


MS_U32 MDrv_NSK2_OTPCheck(MS_U8 *pSecret,  MS_U32 CheckSum,
                          MS_U32 KeyValid, MS_U32 SecMode  )
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return TRUE;
}


MS_U32 MDrv_NSK2_ColdReset(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_ColdReset();
    return TRUE;
}

MS_U32 MDrv_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                         MS_U32 Mask, MS_U32 ExpectResult)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_Compare(StartAddr, CompareLens, CompareSim, Mask,  ExpectResult);
}

MS_U32 MDrv_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                            MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareMem(reserved, StartAddr, CompareLens, CompareSim, ExpectResult, pGolden);
}

MS_U32 MDrv_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens, void *pWriteData)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteMem(reserved,StartAddr,WriteLens,pWriteData);
    return TRUE;
}


MS_U32 MDrv_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteSFR(StartAddr, Data);
    return TRUE;
}

MS_U32 MDrv_NSK2_CompareIRQ(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 reserved_3)
{
    return TRUE;
}


MS_U32 MDrv_NSK2_TestIncomplete(void)
{
    return TRUE;
}


MS_U32 MDrv_NSK2_BasicInitializationComplete(void)
{
    //the chip should manager internal variables NSKactive and ConcurrencyMode
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_NSKBasicInitializationComplete();
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteESA(MS_U16 PID, MS_U8 ESASelect, MS_U8 ESASubSelect)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_WriteESA(PID, ESASelect, ESASubSelect);
    return TRUE;

}

MS_U32 MDrv_NSK2_WriteTransportKey(MS_U16 PID, MS_U8 SCB, MS_U8 ForceSCB, void *pLabel)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    MS_U8 NewSCB, NewForceSCB;

    //scb and ForceSCB needs to transfer....
    NewSCB = (SCB & 0x1) + ((SCB>>4)<<1);
    NewForceSCB = (ForceSCB & 0x1) + ((ForceSCB>>4)<<1);
	//_ForceSCB = NewSCB;
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"SCB = %x, NewSCB = %x, ForceSCB = %x, NewForceSCB = %x\n",SCB,NewSCB,ForceSCB,NewForceSCB);

    HAL_NSK2_WriteTransportKey(PID,NewSCB,NewForceSCB,pLabel);
    return TRUE;
}


MS_U32 MDrv_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareKTE(reserved_1, reserved_2, pLabel);
}


MS_U32 MDrv_NSK2_CompareKTvalid(MS_U32 reserved_1, MS_U32 reserved_2, MS_U8 KTE_Mode, MS_U16 PID, MS_U8 SCB, MS_U8 Expected)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    MS_U8 NewSCB;

    //scb needs to transfer....
    NewSCB = (SCB & 0x1) + ((SCB>>4)<<1);

    return HAL_NSK2_CompareKTvalid( reserved_1, reserved_2, KTE_Mode, PID, NewSCB, Expected);
}

MS_U32 MDrv_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    return HAL_NSK2_CompareOut( reserved_1, reserved_2, HighDWord, LowDWord);
}

MS_U32 MDrv_NSK2_SetRNG(MS_U32 reserved_1, MS_U16 RNG_Value)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    HAL_NSK2_SetRNG( reserved_1, RNG_Value);
    return TRUE;
}


MS_U32 MDrv_NSK2_DriveKteAck(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    HAL_NSK2_DriveKteAck();
    return TRUE;
}



MS_U32 MDrv_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, SubAlgo = %x \n", __FUNCTION__,SubAlgo);

    //can't find the algorithm meaning
    HAL_NSK2_WriteM2MKey(pIV,SubAlgo);
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteSCPUKey(void)
{
    HAL_NSK2_WriteSCPUKey();
	return TRUE;

}

MS_U32 MDrv_NSK2_AssociatePID(MS_U32 PrimaryPID, MS_U32 SecondaryPID, MS_U32 CCmode)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, PrimaryPID = %lx, SecondaryPID = %lx, CCmode = %lx \n", __FUNCTION__,PrimaryPID, SecondaryPID, CCmode);

	HAL_NSK2_AssociatePID(PrimaryPID, SecondaryPID, CCmode);

    return TRUE;
}


MS_U32 MDrv_NSK2_FillJTagPswd(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_SetJTagPswd();
    return TRUE;
}

MS_U32 MDrv_NSK2_CheckPubOTPConfig(void *pCheck)
{

    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_CheckPubOTPConfig(pCheck);
    return TRUE;
}

MS_U32 MDrv_NSK2_UnlockOTPCtrl(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_UnlockOTPCtrl();
    return TRUE;
}


MS_U32 MDrv_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData32(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData32(addr_offset, data_size, data);
}


MS_U32 MDrv_NSK2_SetIntNotify(NSK2_IntNotify IntNotify)
{
    _gNs2IntNtf = IntNotify;
    return TRUE;
}


MS_U32 MDrv_NSK2_ExcuteCmd(MS_U32 command, MS_U32 control,
                           MS_U32 register_offset, MS_U32 data_size,
                           const MS_U8    *data)
{
    MS_U32 u32Status;
    HAL_NSK2_WriteControl(control);

    if(data_size != 0)
    {
        DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s data_size = %lx\n", __FUNCTION__,data_size);
        u32Status = HAL_NSK2_WriteData8(register_offset, data_size, ( MS_U8 *)data);
        if(TRUE != u32Status)
        {
            DRVNSK2_DBG(NSK2_DBGLV_ERR,"%s, write excute cmd error", __FUNCTION__);
        }
    }

    HAL_NSK2_WriteCommand(command);
    return TRUE;
}

MS_U32 MDrv_NSK2_SetPidNo(MS_U8 u8PidNo)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s u8PidNo = %x\n", __FUNCTION__,u8PidNo);
    HAL_NSK2_SetPidNo(u8PidNo);
    return TRUE;
}

MS_U32 MDrv_NSK2_SetSecondaryPidNo(MS_U8 u8PidNo)
{

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s u8PidNo = %x\n", __FUNCTION__,u8PidNo);
    HAL_NSK2_SetSecondaryPidNo(u8PidNo);
    return TRUE;
}

MS_U32 MDrv_NSK2_SetPid_PidNo(MS_U16 Pid, MS_U8 u8PidNo, MS_U8 SrcType)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, Pid = %x, u8PidNo = %x, SrcType = %x\n", __FUNCTION__,Pid, u8PidNo, SrcType);
    HAL_NSK2_SetPid_PidNo(Pid, u8PidNo, SrcType);
    return TRUE;
}

MS_U32 MDrv_NSK2_GetPid_PidNo(MS_U16 Pid)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s, Pid = %x PidNo = %x\n", __FUNCTION__,Pid, HAL_NSK2_GetPidNo(Pid));
    return HAL_NSK2_GetPidNo(Pid);
}

MS_U32 MDrv_NSK2_GetMaxXConn(void)
{
    return HAL_NSK2_GetMaxXConn();
}

MS_U32 MDrv_NSK2_ReadSwitch(MS_U32 SCB, MS_U32 pid_no)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, SCB = %lx, pid_no = %lx\n", __FUNCTION__, SCB, pid_no);
    HAL_NSK2_ReadSwitch(SCB, pid_no);
    return TRUE;
}

MS_U32 MDrv_NSK2_ReadKTEValid(void)
{
    HAL_NSK2_ReadKTEValid();
    return TRUE;
}

void MDrv_NSK2_AllTSPPidFilter(void)
{
    HAL_NSK2_AllTSPPidFilter();
}

void MDrv_NSK2_ChangePidFilter(MS_U32 pid_no, MS_U32 Data)
{
    HAL_NSK2_ChangePidFilter(pid_no, Data);
}

void MDrv_NSK2_EnableKTEValidPatch(void)
{
    HAL_NSK2_EnableKTEValidPatch();
}

MS_U32 MDrv_NSK2_GetOTPValue(MS_U32 u32Offset)
{
    return HAL_NSK2_GetOTPValue(u32Offset);
}

MS_U32 MDrv_NSK2_CMChannelNum(void)
{
    return HAL_NSK2_CMChannelNum();
}

void MDrv_NSK2_DumpPktView(MS_U32 u32Eng , MS_U32 u32FltId, MS_BOOL reset)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s, u32FltId = %lx, reset = %d\n", __FUNCTION__, u32FltId, reset);
    HAL_NSK2_DumpPktView(u32Eng , u32FltId,  reset);
}

MS_U32 MDrv_NSK2_GetOTPFieldValue(MS_U8 u8Tag, MS_U8 *pLength, MS_U32 *pRetValue)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, u8Tag = %x\n", __FUNCTION__, u8Tag);

    return HAL_NSK2_GetOTPFieldValue(u8Tag, pLength, pRetValue);
}

void MDrv_NSK2_SetDbgLevel(MS_U32 u32Level)
{
    _g32NSK2DbgLv = u32Level;
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s level: %lx\n", __FUNCTION__, u32Level);
    HAL_NSK2_SetDbgLevel(u32Level);
}

void MDrv_NSK2_SetPollingCnt(MS_U32 u32Cnt)
{
    HAL_NSK2_SetPollingCnt(u32Cnt);
}


#if 0
void MDrv_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode)
{
	HAL_NSK2_BurstLen(u32PVREng, u32BurstMode);
}
#endif

void MDrv_NSK2_RunFree(MS_BOOL bRunFree)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, bRunFree = %x\n", __FUNCTION__, bRunFree);

	HAL_NSK2_RunFree(bRunFree);
}

void MDrv_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 TenSecond)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, HaltClk = %d, TenSecond = %lx \n", __FUNCTION__, HaltClk, TenSecond);

	HAL_NSK2_PushSlowClock(HaltClk, TenSecond);
}


#if 0
void MDrv_NSK2_CCDrop(MS_U32 u32FltId, MS_BOOL bEnable)
{
	// TODO: Check how to get pktDmxId, efang
    MS_U32 pktDmxId  = 0;// = HAL_TSP_FilePath2Tsif(0);

	HAL_DSCMB_Get_TsidInput(u32FltId, &pktDmxId);

	HAL_TSP_PktDmx_CCDrop(pktDmxId, bEnable);
    printf("%s, pktDmxId = %x, bEnable = %d\n", __FUNCTION__, pktDmxId, bEnable);
}

void MDrv_NSK2_SPD_BypassEnable(MS_BOOL bByPassEn)
{
	HAL_TSP_SPD_Bypass_En(bByPassEn);
}

void MDrv_NSK2_SPD_Reset(MS_U32 u32FltId)
{
    MS_U32 pktDmxId  = 0;
	HAL_DSCMB_Get_TsidInput(u32FltId, &pktDmxId);
	HAL_TSP_TSIF_SPD_Reset(pktDmxId);
    printf("%s, pktDmxId = %x, u32FltId = %d\n", __FUNCTION__, pktDmxId, u32FltId);
}
#endif
