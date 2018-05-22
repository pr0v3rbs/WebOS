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
/// file    drvTEMP.c
/// @brief  SmartCard Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(OS_ECOS) || defined(OS_LINUX))


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvTEMP.h"

// Internal Definition
#include "temp_msos.h"
#include "halTEMP.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//#define OS_TEMP_TASK_STACK_SIZE     512


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_S32                              os_TEMP_MutexID = -1;


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32                       _os_TEMP_TaskStack[OS_TEMP_TASK_STACK_SIZE]; //TODO smaller stack
static MS_S32                       _os_TEMP_TaskID = -1;
static MS_S32                       _os_TEMP_EventID = -1;
static P_SC_ISR_Proc                _os_TEMP_ISR_Proc;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void OS_TEMP_SetEvent(MS_U32 u32Events)
{
    MsOS_SetEvent(_os_TEMP_EventID, u32Events);
}


MS_U32 OS_TEMP_WaitEvent(void)
{
    MS_U32              u32Events;

    MsOS_WaitEvent(_os_TEMPEventID, OS_TEMP_EVENT_MASK | OS_TEMP_EVENT_MASK2, &u32Events, E_OR, MSOS_WAIT_FOREVER);

    return u32Events;
}


void OS_TEMP_ClearEvent(MS_U32 u32Events)
{
    MsOS_ClearEvent(_os_TEMP_EventID, u32Events);
}

static void _OS_TEMP_Task(void)
{
    while(1)
    {
        MsOS_WaitEvent(_os_TEMP_EventID, TEMP_EVENT, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_TEMP_Task_Proc();
    }
}


static void _OS_TEMP_ISR(void)
{
    _os_TEMP_ISR_Proc();

    // IRQ handled
    MsOS_EnableInterrupt(TEMP_IRQ);
}


MS_BOOL OS_TEMP_ISR_Enable(P_TEMP_ISR_Proc pfISRProc)
{
    _os_TEMP_ISR_Proc = pfISRProc;
    MsOS_AttachInterrupt(TEMP_IRQ, _OS_TEMP_ISR);
    MsOS_EnableInterrupt(TEMP_IRQ);

    return TRUE;
}


void OS_TEMP_ISR_Disable(void)
{
    MsOS_DisableInterrupt(TEMP_IRQ);
    MsOS_DetachInterrupt (TEMP_IRQ);
    _os_TEMP_ISR_Proc = NULL;
}


MS_BOOL OS_TEMP_Init(void)
{
    MsOS_DisableInterrupt(TEMP_IRQ);
    MsOS_DetachInterrupt (TEMP_IRQ);

    // Initialize inter-process variable --------------------------------------
    if (_os_TEMP_TaskID < 0)
    {
        _os_TEMP_TaskID = MsOS_CreateTask((TaskEntry)_OS_TEMP_Task,
                                          NULL,
                                          E_TASK_PRI_SYS,
                                          TRUE,
                                          _os_TEMP_TaskStack,
                                          OS_TEMP_TASK_STACK_SIZE*4,
                                          "OS_TEMP_Task");
        if (_os_TEMP_TaskID < 0)
        {
            return FALSE;
        }

        _os_TEMP_EventID = MsOS_CreateEventGroup("OS_TEMP_Event");
        if (_os_TEMP_EventID < 0)
        {
            MsOS_DeleteTask(_os_TEMP_TaskID);
            _os_TEMP_TaskID = -1;
            return FALSE;
        }

        os_TEMP_MutexID = MsOS_CreateMutex(E_MSOS_FIFO, "OS_TEMP_Mutex");
        if (os_TEMP_MutexID < 0)
        {
            MsOS_DeleteEventGroup(_os_TEMP_EventID);
	        _os_TEMP_EventID = -1;
            MsOS_DeleteTask(_os_TEMP_TaskID);
            _os_TEMP_TaskID = -1;
            return FALSE;
        }

    }
    return TRUE;
}

MS_BOOL OS_TEMP_Exit(void)
{
    if (_os_TEMP_TaskID < 0)
    {
        return FALSE;
    }

    MsOS_DeleteMutex(os_TEMP_MutexID);
	os_TEMP_MutexID = -1;

    MsOS_DeleteEventGroup(_os_TEMP_EventID);
	_os_TEMP_EventID = -1;

    MsOS_DeleteTask(_os_TEMP_TaskID);
	_os_TEMP_TaskID = -1;

    return TRUE;
}


#endif //OS_ECOS
