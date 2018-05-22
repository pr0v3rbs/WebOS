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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvRASP.c
/// @brief  RASP Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"
#include "regRASP.h"
#include "halRASP.h"

#ifdef HW_PVR_ENABLE
    #include "halPVR.h"
#endif

#include "drvRASP.h"
#include "drvMMIO.h"
#include "asmCPU.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

//--------------------------------------------------------------------------------------------------
//  Debug Macro
//--------------------------------------------------------------------------------------------------
#define MS_DEBUG 1
#if MS_DEBUG
#define RASP_DBG(_fmt,_args...)             {printf("[%s][%d]",__FUNCTION__,__LINE__); printf(_fmt,##_args);}
#else
#define RASP_DBG(_args)                     { }
#endif

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

// Safe Mode Configuration
// ============================================
//              ISR_SAFE            TASK_SAFE
//  ISR_SAFE    Int,Reg             Int,Reg,Task
//  TASK_SAFE   Int,Reg,Task        Mutex,Reg,Task
//
#define _RASP_SetEvent(flag)                            MsOS_SetEvent(_s32RASPEventId, (flag))
#define _RASP_SetSecRdyId(eng, flag)                    MsOS_SetEvent(_s32RASPSecRdyId[(eng)], (flag))
#define _RASP_SetSecOvfId(eng, flag)                    MsOS_SetEvent(_s32RASPSecOvfId[(eng)], (flag))

#define _RASP_GetEvent(events, flag, opt, timeout)      MsOS_WaitEvent(_s32RASPEventId, (flag), &(events), (opt), (timeout))
#define _RASP_GetSecRdyId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecRdyId[(eng)], (flag), &(events), (opt), 0)
#define _RASP_GetSecOvfId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecOvfId[(eng)], (flag), &(events), (opt), 0)

#define _RASP_SetSecCrcId(eng, flag)                    MsOS_SetEvent(_s32RASPSecCrcId[(eng)], (flag))
#define _RASP_GetSecCrcId(eng, flag, events, opt)       MsOS_WaitEvent(_s32RASPSecCrcId[(eng)], (flag), &(events), (opt), 0)

#define _RASP_SetFltDisableId(eng, flag)                MsOS_SetEvent(_s32RASPFltDisableId[(eng)], (flag))
#define _RASP_GetFltDisableId(eng, flag, events, opt)   MsOS_WaitEvent(_s32RASPFltDisableId[(eng)], (flag), &(events), (opt), 0)

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------

#define _RASP_ENTRY()       if (!MsOS_ObtainMutex(_prasp_res->_s32RASPMutexId, MSOS_WAIT_FOREVER))      \
                            {                                                               \
                                _prasp_res->_u32LastErr = DRVRASP_FUNC_ERROR;                           \
                                return DRVRASP_FUNC_ERROR;                                  \
                            }                                                               \
                            if (_prasp_res->_bSuspend)                                                  \
                            {                                                               \
                                _RASP_RETURN(DRVRASP_FAIL);                                 \
                            }

#define _RASP_RETURN(_ret)  if(MsOS_ReleaseMutex(_prasp_res->_s32RASPMutexId)) {return _ret;}           \
                            else                                                            \
                            {                                                               \
                                _prasp_res->_u32LastErr = DRVRASP_FUNC_ERROR;                           \
                                return DRVRASP_FUNC_ERROR;                                  \
                            }

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

typedef struct _DrvRASP_State
{
    MS_U32          _RASP_init;
    DrvRASP_FltState _RASP_FltState;
} DrvRASP_State;

typedef struct _RASP_RESOURCE_PRIVATE
{
    MS_S32                      _s32RASPMutexId;
    MS_U32                      _u32LastErr;
    MS_BOOL                     _bSuspend;

    DrvRASP_State               _RASPState[RASP_NUM][RASP_PIDFLT_NUM_ALL];

    #ifdef HW_PVR_ENABLE
    DrvRASP_State               _PVRState[PVR_NUM][PVR_PIDFLT_NUM_ALL];
    DrvRASP_Event               _PVREvtNotify[PVR_NUM];
    P_DrvRASP_EvtCallback       _PVRCallback[PVR_NUM];
    #endif
    DrvRASP_Event               _RASPEvtNotify[RASP_NUM];
    P_DrvRASP_EvtCallback       _RASPCallback[RASP_NUM];

    MS_BOOL                     _bRASPInitReady;
}RASP_RESOURCE_PRIVATE;


//static MSIF_Version _drv_RASP_version = {
//    .DDI = { RASP_DRV_VERSION, },
//};

#ifndef RASP_UTOPIA_20
static RASP_RESOURCE_PRIVATE _rasp_res = {
    ._s32RASPMutexId                        = -1,
    ._u32LastErr                            = DRVRASP_OK,
    ._bSuspend                              = FALSE,
    ._bRASPInitReady                        = FALSE,
};

static RASP_RESOURCE_PRIVATE*    _prasp_res = &_rasp_res;
#else
static RASP_RESOURCE_PRIVATE*    _prasp_res = NULL;
#endif

#if (RASP_NUM == 0)
MS_U32                              _u32RASP0IntNum = E_INT_IRQ_FIQ_NONE;
MS_U32                              _u32RASP1IntNum = E_INT_IRQ_FIQ_NONE;
#elif (RASP_NUM == 1)
MS_U32                              _u32RASP0IntNum = E_INT_IRQ_RASP;
MS_U32                              _u32RASP1IntNum = E_INT_IRQ_FIQ_NONE;
#elif (RASP_NUM == 2)
MS_U32                              _u32RASP0IntNum = E_INT_IRQ_RASP;
MS_U32                              _u32RASP1IntNum = E_INT_IRQ_RASP1;
#else
    #error "Plz extent RASP interrupt num!!"
#endif
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

static MS_BOOL _RASP_InitResource(RASP_RESOURCE_PRIVATE* presource)
{
    if(presource == NULL)
        return FALSE;

    presource->_s32RASPMutexId = -1;
    presource->_bSuspend = FALSE;
    presource->_bRASPInitReady = FALSE;
    presource->_u32LastErr = DRVRASP_OK;

    return TRUE;
}

static void _RASP_SwStateReset(void)
{
    MS_U32 _u32RASPnum = 0;
    MS_U32 i = 0;
    for (_u32RASPnum = 0 ; _u32RASPnum < RASP_NUM ; _u32RASPnum++)
    {
        for (i = RASP_PIDFLT_START_NUM; i < RASP_PIDFLT_END_NUM; i++)
        {
            _prasp_res->_RASPState[_u32RASPnum][i]._RASP_init = 0;
            _prasp_res->_RASPState[_u32RASPnum][i]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
        }
    }
#ifdef HW_PVR_ENABLE
    for (_u32RASPnum = RASP_NUM ; _u32RASPnum < (RASP_NUM + PVR_NUM) ; _u32RASPnum++)
    {
        for (i = PVR_PIDFLT_START_NUM; i < PVR_PIDFLT_END_NUM; i++)
        {
            _prasp_res->_PVRState[_u32RASPnum - RASP_NUM][i]._RASP_init = 0;
            _prasp_res->_PVRState[_u32RASPnum - RASP_NUM][i]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
        }
    }
#endif
}

static void _RASP_RegStateReset(void)
{
    MS_U16 _u16fltnum = 0;
    MS_U32 _u32RASPnum = 0;
    for (_u32RASPnum = 0 ; _u32RASPnum < RASP_NUM ; _u32RASPnum++)
    {
        HAL_RASP_SetStr2Miu_StartAddr(_u32RASPnum, (MS_U32)NULL, (MS_U32)NULL);
        HAL_RASP_SetStr2Miu_MidAddr(_u32RASPnum, (MS_U32)NULL, (MS_U32)NULL);
        HAL_RASP_SetStr2Miu_EndAddr(_u32RASPnum, (MS_U32)NULL, (MS_U32)NULL);

        for (_u16fltnum = 0 ; _u16fltnum < RASP_PIDFLT_NUM ; _u16fltnum++)
        {
            HAL_RASP_SetPidflt(_u32RASPnum, _u16fltnum, RASP_PIDFLT_DEF);
        }

        for (_u16fltnum = 0 ; _u16fltnum < RASP_EVENT_NUM ; _u16fltnum++)
        {
            HAL_RASP_SetEventMask(_u32RASPnum, _u16fltnum, ~RASP_BYPASS_MASK);
            HAL_RASP_SetPayloadMask(_u32RASPnum, _u16fltnum, ~RASP_BYPASS_MASK);
        }
    }
#ifdef HW_PVR_ENABLE
    for (_u32RASPnum = RASP_NUM ; _u32RASPnum < (RASP_NUM + PVR_NUM) ; _u32RASPnum++)
    {
        HAL_PVR_SetStr2Miu_StartAddr(_u32RASPnum - RASP_NUM, (MS_U32)NULL, (MS_U32)NULL);
        HAL_PVR_SetStr2Miu_MidAddr(_u32RASPnum - RASP_NUM, (MS_U32)NULL, (MS_U32)NULL);
        HAL_PVR_SetStr2Miu_EndAddr(_u32RASPnum - RASP_NUM, (MS_U32)NULL, (MS_U32)NULL);

        for (_u16fltnum = 0 ; _u16fltnum < PVR_PIDFLT_NUM ; _u16fltnum++)
        {
            HAL_PVR_SetPidflt(_u32RASPnum - RASP_NUM, _u16fltnum, PVR_PIDFLT_DEF);
        }
    }
#endif
}

#if 0 //PVR didn't used callback in AP layer
//static void _RASP_Isr_ProcSelf(OSAL_RASP_ProcTypeParam* pData);
//static void _RASP_Isr_ProcPoll(OSAL_RASP_ProcTypeParam* pData, MS_U32* pu32Evt);
//static void _RASP_Isr_Proc_Task(OSAL_RASP_ProcTypeParam* pDummy, MS_U32* pu32Dummy);
#ifdef OS_NOS
void _RASP_Isr(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data)
#else
static void _RASP_Isr(void)
#endif
{
}
#endif

static RASP_Result _RASP_Init(void)
{
    MS_U8 u8ii;
    MS_U32 u32Bank, u32BankSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_RASP))
    {
        RASP_DBG("MDrv_MMIO_GetBASE fail\n");
        MS_ASSERT(0);
    }

    HAL_RASP_SetBank(u32Bank);

    for(u8ii = 0; u8ii < RASP_NUM; u8ii++)
    {
        _prasp_res->_RASPEvtNotify[u8ii] = E_DRVRASP_EVENT_DATA_INIT;
        _prasp_res->_RASPCallback[u8ii] = NULL;

        HAL_RASP_PowerCtrl(u8ii, ENABLE);
        HAL_RASP_Init(u8ii);
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
        HAL_RASP_INT_Disable(u8ii, RASP_INT_ALL);// Diable interrupt
        HAL_RASP_INT_ClrHW(u8ii, RASP_INT_ALL);
        HAL_RASP_SetCallbackSize(u8ii, 0);//the default value is not 0, clear to 0
#endif
    }

#ifdef HW_PVR_ENABLE
    HAL_PVR_SetBank(u32Bank);

    for(u8ii = RASP_NUM; u8ii < (RASP_NUM + PVR_NUM); u8ii++)
    {
        HAL_PVR_Init(u8ii - RASP_NUM);
    }
#endif

    _RASP_RegStateReset();
    _RASP_SwStateReset();

    if (-1 == _prasp_res->_s32RASPMutexId)
    {
        _prasp_res->_s32RASPMutexId = MsOS_CreateMutex(E_MSOS_FIFO , "RASP_Mutex" , MSOS_PROCESS_SHARED);
    }
    else
    {
        RASP_DBG("Create _s32RASPMutexId more than once\n");
    }

    return DRVRASP_OK;
}

static void _RASP_Close(void)
{
    MS_U8 u8ii;

    for(u8ii = 0; u8ii < RASP_NUM; u8ii++)
    {
        HAL_RASP_Exit(u8ii);
    }

#ifdef HW_PVR_ENABLE
    for(u8ii = RASP_NUM; u8ii < (RASP_NUM + PVR_NUM); u8ii++)
    {
        HAL_PVR_Exit(u8ii - RASP_NUM);
    }
#endif
}

// General API
//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
RASP_Result MDrv_RASP_InitLibResource(void* pResMemAddr)
{
    if(pResMemAddr == 0)
        return DRVRASP_FAIL;

    _prasp_res = (RASP_RESOURCE_PRIVATE*)pResMemAddr;

    if(_RASP_InitResource(_prasp_res) == FALSE)
    {
        _prasp_res->_u32LastErr = DRVRASP_FAIL;
    }
    else
        _prasp_res->_u32LastErr = DRVRASP_OK;

    return (_prasp_res->_u32LastErr);
}

//--------------------------------------------------------------------------------------------------
/// Initialize RASP driver and RASP engine
/// @return RASP_Result
/// @note
/// It should be called before calling any other RASP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//--------------------------------------------------------------------------------------------------
RASP_Result MDrv_RASP_Init(void)
{
    RASP_Result bRet;

    bRet = _RASP_Init();

    _RASP_ENTRY();
    _prasp_res->_bRASPInitReady = TRUE;
    _RASP_RETURN(bRet);
}

//--------------------------------------------------------------------------------------------------
/// Poweroff RASP driver and RASP engine
/// @return RASP_Result
/// @note
//--------------------------------------------------------------------------------------------------
RASP_Result MDrv_RASP_Reset(void)
{
    _RASP_ENTRY();
    _RASP_SwStateReset();
    _RASP_RegStateReset();
    _RASP_RETURN(DRVRASP_OK);
}

//--------------------------------------------------------------------------------------------------
/// Close RASP driver and reset software/register state
/// @return RASP_Result
/// @note
//--------------------------------------------------------------------------------------------------
RASP_Result MDrv_RASP_Exit(void)
{
    if (_prasp_res->_s32RASPMutexId != -1)
    {
        _RASP_ENTRY();
    }
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
    MS_U8 u8ii;
    for(u8ii = 0; u8ii < RASP_NUM; u8ii++)// Diable interrupt
        HAL_RASP_INT_Disable(u8ii, RASP_INT_ALL);
#endif
    if(_prasp_res->_bRASPInitReady == TRUE)
    {
        _RASP_SwStateReset();
        _RASP_RegStateReset();
        _RASP_Close();
        _prasp_res->_bRASPInitReady = FALSE;
    }

    if (_prasp_res->_s32RASPMutexId != -1)
    {
        MsOS_ReleaseMutex(_prasp_res->_s32RASPMutexId);
        MsOS_DeleteMutex(_prasp_res->_s32RASPMutexId);
        _prasp_res->_s32RASPMutexId = -1;
    }

    return DRVRASP_OK;
}

RASP_Result MDrv_RASP_PVR_SetBuffer(MS_U32 u32RASPEng, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetStr2Miu_StartAddr(u32RASPEng, u32BufStart0, u32BufStart1);
        HAL_RASP_SetStr2Miu_MidAddr(u32RASPEng, u32BufStart0 + u32BufSize0 , u32BufStart1 + u32BufSize1 );
        HAL_RASP_SetStr2Miu_EndAddr(u32RASPEng, u32BufStart0 + u32BufSize0 , u32BufStart1 + u32BufSize1 );
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_SetStr2Miu_StartAddr(u32RASPEng - RASP_NUM, u32BufStart0, u32BufStart1);
        HAL_PVR_SetStr2Miu_MidAddr(u32RASPEng - RASP_NUM, u32BufStart0 + u32BufSize0 , u32BufStart1 + u32BufSize1 );
        HAL_PVR_SetStr2Miu_EndAddr(u32RASPEng - RASP_NUM, u32BufStart0 + u32BufSize0 , u32BufStart1 + u32BufSize1 );
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_Start(MS_U32 u32RASPEng, DrvRASP_RecMode eRecMode, MS_BOOL bStart)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        if (!bStart)
        {
            //_RASP_RegStateReset();
            HAL_RASP_Stop(u32RASPEng);
        }
        else
        {
            switch (eRecMode)
            {
                case E_DRVRASP_REC_MODE_PID:
                {
                    HAL_RASP_Rec_PID(u32RASPEng, TRUE);
                    break;
                }
                case E_DRVRASP_REC_MODE_ALL:
                {
                    HAL_RASP_Rec_PID(u32RASPEng, FALSE);
                    break;
                }
                default :
                    break;
            }
            HAL_RASP_Start(u32RASPEng);
        }
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        if (!bStart)
        {
            //_RASP_RegStateReset();
            HAL_PVR_Stop(u32RASPEng - RASP_NUM);
        }
        else
        {
            switch (eRecMode)
            {
                case E_DRVRASP_REC_MODE_PID:
                {
                    HAL_PVR_Rec_PID(u32RASPEng - RASP_NUM, TRUE);
                    break;
                }
                case E_DRVRASP_REC_MODE_ALL:
                {
                    HAL_PVR_Rec_PID(u32RASPEng - RASP_NUM, FALSE);
                    break;
                }
                default :
                    break;
            }
            HAL_PVR_Start(u32RASPEng - RASP_NUM);
        }
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}


RASP_Result MDrv_RASP_PVR_Pause(MS_U32 u32RASPEng, MS_BOOL bPause)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM )
    {
        HAL_RASP_Pause(u32RASPEng,bPause);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_Pause(u32RASPEng - RASP_NUM, bPause);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}


RASP_Result MDrv_RASP_PVR_GetWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
        *pu32WriteAddr = HAL_RASP_GetWritePtr(u32RASPEng);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        *pu32WriteAddr = HAL_PVR_GetWritePtr(u32RASPEng - RASP_NUM);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_Notify(MS_U32 u32RASPEng, DrvRASP_Event eEvents, P_DrvRASP_EvtCallback pfCallback)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM)
    {
        if (pfCallback)
        {
            _prasp_res->_RASPEvtNotify[u32RASPEng] = eEvents;
            _prasp_res->_RASPCallback[u32RASPEng] = pfCallback;
        }
        else
        {
            _prasp_res->_RASPEvtNotify[u32RASPEng] = E_DRVRASP_EVENT_DATA_INIT;
            _prasp_res->_RASPCallback[u32RASPEng] = NULL;
        }
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        //to-do
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_SetPacketMode(MS_U32 u32RASPEng, DrvRASP_RecType eRecType,MS_BOOL bSet)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
        switch (eRecType)
        {
            case STR2MIU:
                HAL_RASP_SetStrPacketMode(u32RASPEng, bSet);
                break;
            default:
                break;
        }
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        switch (eRecType)
        {
            case STR2MIU:
                HAL_PVR_SetStrPacketMode(u32RASPEng - RASP_NUM, bSet);
                break;
            default:
                break;
        }
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_TimeStampSetRecordStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
        HAL_RASP_SetPVRTimeStamp(u32RASPEng, u32Stamp);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_SetPVRTimeStamp(u32RASPEng - RASP_NUM, u32Stamp);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_TimeStampGetRecordStamp(MS_U32 u32RASPEng, MS_U32* u32Stamp)
{
    if(u32RASPEng < RASP_NUM )
    {
        *u32Stamp = HAL_RASP_GetPVRTimeStamp(u32RASPEng);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        *u32Stamp = HAL_PVR_GetPVRTimeStamp(u32RASPEng - RASP_NUM);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    return DRVRASP_OK;
}

RASP_Result MDrv_RASP_PVR_TimeStampSelRecordStampSrc(MS_U32 u32RASPEng, MS_BOOL bLocal)
{
#ifdef FQ_ENABLE
    if(u32RASPEng < RASP_NUM )
    {
        HAL_RASP_SelPVRTimeStampSrc(u32RASPEng, bLocal);
    }
    #ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        RASP_DBG("RASP Engine %u does not support  MDrv_RASP_PVR_TimeStampSetRecordStamp()\n", (unsigned int)u32RASPEng);
    }
    #endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
#endif

    return DRVRASP_OK;
}

RASP_Result MDrv_RASP_PVR_SetTSIF(MS_U32 u32RASPEng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
        HAL_RASP_SetTSIF(u32RASPEng, bPara, bExtSync, bDataSWP);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_SetTSIF(u32RASPEng - RASP_NUM, bPara, bExtSync, bDataSWP);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
        _RASP_RETURN(DRVRASP_FAIL);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_GetTSIFStatus(MS_U32 u32RASPEng, MS_BOOL* pbExtSyc, MS_BOOL* pbParl, MS_BOOL *pbDataSWP)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
        //we mark this line since RASP data can be get from TSIF
//        HAL_RASP_GetTSIF(u32RASPEng, pbParl, pbExtSyc, pbDataSWP);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_GetTSIF(u32RASPEng - RASP_NUM, pbParl, pbExtSyc, pbDataSWP);
    }
#endif
    else
    {
        RASP_DBG("PVR engine number %u is illegal\n", (unsigned int)u32RASPEng);
        _RASP_RETURN(DRVRASP_FAIL);
    }
    _RASP_RETURN(DRVRASP_OK);
}




RASP_Result MDrv_RASP_PVR_AllocFlt(MS_U32 u32RASPEng, MS_U32 *pu32PidFltId)
{
    MS_U32 i = 0;
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM )
    {
        for (i = RASP_PIDFLT_START_NUM ; i < RASP_PIDFLT_END_NUM ; i++)
        {
            if (_prasp_res->_RASPState[u32RASPEng][i]._RASP_FltState == E_DRVRASP_FLT_STATE_FREE)
            {
                _prasp_res->_RASPState[u32RASPEng][i]._RASP_FltState = E_DRVRASP_FLT_STATE_ALLOC;
                *pu32PidFltId = i;
                break;
            }
        }

        if (i == RASP_PIDFLT_END_NUM)
        {
            //RASP_ASSERT(0, OSAL_TSP_Print("[TSP_ERROR][%06d] No free fitler found 0x%02lx\n", __LINE__, i));
            _RASP_RETURN(DRVRASP_FAIL);
        }
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        for (i = PVR_PIDFLT_START_NUM ; i < PVR_PIDFLT_END_NUM ; i++)
        {
            if (_prasp_res->_PVRState[u32RASPEng - RASP_NUM][i]._RASP_FltState == E_DRVRASP_FLT_STATE_FREE)
            {
                _prasp_res->_PVRState[u32RASPEng - RASP_NUM][i]._RASP_FltState = E_DRVRASP_FLT_STATE_ALLOC;
                *pu32PidFltId = i;
                break;
            }
        }

        if (i == PVR_PIDFLT_END_NUM)
        {
            //RASP_ASSERT(0, OSAL_TSP_Print("[TSP_ERROR][%06d] No free fitler found 0x%02lx\n", __LINE__, i));
            _RASP_RETURN(DRVRASP_FAIL);
        }
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_SetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetPidflt(u32RASPEng, u16Fltid, u16Pid);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_SetPidflt(u32RASPEng - RASP_NUM, u16Fltid, u16Pid);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_GetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 *pu16Pid)
{
    _RASP_ENTRY();


    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_GetPidflt(u32RASPEng, u16Fltid, pu16Pid);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_GetPidflt(u32RASPEng - RASP_NUM, u16Fltid, pu16Pid);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}




RASP_Result MDrv_RASP_PVR_ReleaseFlt(MS_U32 u32RASPEng, MS_U16 u32FltId)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        _prasp_res->_RASPState[u32RASPEng][u32FltId]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
        HAL_RASP_SetPidflt(u32RASPEng, u32FltId, DRVRASP_PID_NULL);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        _prasp_res->_PVRState[u32RASPEng - RASP_NUM][u32FltId]._RASP_FltState = E_DRVRASP_FLT_STATE_FREE;
        HAL_PVR_SetPidflt(u32RASPEng - RASP_NUM, u32FltId, DRVRASP_PID_NULL);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_FileinEnable(MS_U32 u32RASPEng, MS_BOOL bEn)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetFileinStart(u32RASPEng, bEn);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        RASP_DBG("RASP Engine %u does not support  MDrv_RASP_PVR_FileinEnable()\n", (unsigned int)u32RASPEng);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_GetFileinEnable(MS_U32 u32RASPEng, MS_BOOL* bEn)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        *bEn = HAL_RASP_GetFileinStart(u32RASPEng);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        RASP_DBG("RASP Engine %u does not support  MDrv_RASP_PVR_FileinEnable()\n", (unsigned int)u32RASPEng);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_GetCap(DrvRASP_Cap eCap, void* pOutput)
{
	*((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case E_DRVRASP_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = sizeof(RASP_RESOURCE_PRIVATE);
            return DRVRASP_OK;
        case E_DRVRASP_CAP_FILTER_NUM:
        case E_DRVRASP_CAP_FILTER_PVR_NUM:
            *((MS_U32*)pOutput) = RASP_PIDFLT_NUM_ALL;
            return DRVRASP_OK;
        case E_DRVRASP_CAP_PVR_ALIGN:
            *((MS_U32*)pOutput) = 16;
            return DRVRASP_OK;
        case E_DRVRASP_CAP_RASP_NUM:
            *((MS_U32*)pOutput) = RASP_NUM;
            return DRVRASP_OK;
        default:
            return DRVRASP_INVALID_PARAM;
    }
}


RASP_Result MDrv_RASP_SetEvent(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Event)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetEventMask(u32RASPEng, u16Flt, u32Event);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        RASP_DBG("RASP Engine %u does not support  MDrv_RASP_SetEvent()\n", (unsigned int)u32RASPEng);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_SetPayload(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Payload)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetPayloadMask(u32RASPEng, u16Flt, u32Payload);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        RASP_DBG("RASP Engine %u does not support  MDrv_RASP_SetPayload()\n", (unsigned int)u32RASPEng);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn)
{
    _RASP_ENTRY();

    if(u32RASPEng < RASP_NUM)
    {
        HAL_RASP_SetDataSwap(u32RASPEng, bEn);
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        HAL_PVR_SetDataSwap(u32RASPEng - RASP_NUM, bEn);
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_RASP_PVR_CallbackSize(MS_U32 u32RASPEng, MS_U32* pu32CallbackSize, MS_BOOL bSet)
{
    _RASP_ENTRY();
    if(u32RASPEng < RASP_NUM )
    {
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
        if(bSet)
        {
            HAL_RASP_SetCallbackSize(u32RASPEng, *pu32CallbackSize);
            if(*pu32CallbackSize < CALLBACK_SIZE_MIN)
            {
                HAL_RASP_INT_Disable(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
            }
            else
            {
                if(_prasp_res->_RASPCallback[u32RASPEng])
                    HAL_RASP_INT_Enable(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
            }
        }
        else
            *pu32CallbackSize = HAL_RASP_GetCallbackSize(u32RASPEng);
#endif
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        //to-do
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }

    _RASP_RETURN(DRVRASP_OK);
}

RASP_Result MDrv_PROC_RASP_PVR_SizeMet(MS_U32 u32RASPEng)
{
    static DrvRASP_Msg stMsg;
    if(u32RASPEng < RASP_NUM )
    {
        if(_prasp_res->_RASPEvtNotify[u32RASPEng] && _prasp_res->_RASPCallback[u32RASPEng])
        {
            if(_prasp_res->_RASPEvtNotify[u32RASPEng] & E_DRVRASP_EVENT_CALLBACK_SIZE_MET)
            {
                stMsg.u8PVREngNum = u32RASPEng;
                _prasp_res->_RASPCallback[u32RASPEng](E_DRVRASP_EVENT_CALLBACK_SIZE_MET, &stMsg);
            }
        }
    }
#ifdef HW_PVR_ENABLE
    else if(u32RASPEng < (RASP_NUM + PVR_NUM))
    {
        //to-do
    }
#endif
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
    return DRVRASP_OK;
}

RASP_Result MDrv_RASP_CallbackIntCheck(MS_U32 u32RASPEng, MS_BOOL* bInterrupted)
{//this func may be used in interrupt context , so watch out!!! Here's no synchronization mechanism.
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
    if(!bInterrupted)
        return DRVRASP_FAIL;

    if(u32RASPEng < RASP_NUM )
    {
        if(HAL_RASP_INT_GetHW(u32RASPEng) & RASP_INT_CALLBACK_BY_SIZE)
            *bInterrupted = TRUE;
        else
            *bInterrupted = FALSE;
    }
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
#endif
    return DRVRASP_OK;
}

RASP_Result MDrv_RASP_CallbackIntClr(MS_U32 u32RASPEng)
{//this func may be used in interrupt context , so watch out!!! Here's no synchronization mechanism.
#ifdef PVR_CALLBACK_BY_SIZE_ENABLE
    if(u32RASPEng < RASP_NUM )
        HAL_RASP_INT_ClrHW(u32RASPEng, RASP_INT_CALLBACK_BY_SIZE);
    else
    {
        RASP_DBG("RASP engine number %u is illegal\n", (unsigned int)u32RASPEng);
    }
#endif
    return DRVRASP_OK;
}

