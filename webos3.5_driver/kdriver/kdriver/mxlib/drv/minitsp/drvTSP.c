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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP.c
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvTSP.h"

// Internal Definition
#include "asmCPU.h"
#include "osalTSP.h"
#include "regTSP.h"
#include "halTSP.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_firmware_io.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define _TSP_SetEvent(flag)                             OSAL_TSP_EventSet(_s32TspEventId, (flag))
#define _TSP_SetSecRdyId(eng, flag)                     OSAL_TSP_EventSet(_s32TspSecRdyId[(eng)], (flag))
#define _TSP_SetSecOvfId(eng, flag)                     OSAL_TSP_EventSet(_s32TspSecOvfId[(eng)], (flag))

#define _TSP_GetEvent(events, flag, opt, timeout)       OSAL_TSP_EventGet(_s32TspEventId, (flag), &(events), (opt), (timeout))
#define _TSP_GetSecRdyId(eng, flag, events, opt)        OSAL_TSP_EventGet(_s32TspSecRdyId[(eng)], (flag), &(events), (opt), 0)
#define _TSP_GetSecOvfId(eng, flag, events, opt)        OSAL_TSP_EventGet(_s32TspSecOvfId[(eng)], (flag), &(events), (opt), 0)



#define _TSP_SetSecCrcId(eng, flag)                     OSAL_TSP_EventSet(_s32TspSecCrcId[(eng)], (flag))
#define _TSP_GetSecCrcId(eng, flag, events, opt)        OSAL_TSP_EventGet(_s32TspSecCrcId[(eng)], (flag), &(events), (opt), 0)

#define _TSP_SetFltDisableId(eng, flag)                 OSAL_TSP_EventSet(_s32TspFltDisableId[(eng)], (flag))
#define _TSP_GetFltDisableId(eng, flag, events, opt)    OSAL_TSP_EventGet(_s32TspFltDisableId[(eng)], (flag), &(events), (opt), 0)


#define _TSP_ENTRY()                                    if (!OSAL_TSP_MutexObtain(_s32TspMutexId, OSAL_TSP_MUTEX_TIMEOUT))      \
                                                        {\
                                                            printf("can't obtain mutex\n");                                                      \
                                                            _u32LastErr = DRVTSP_FUNC_ERROR;                                    \
                                                            return DRVTSP_FUNC_ERROR;                                           \
                                                        }                                                                       \
                                                        if (_bSuspend)                                                          \
                                                        {                                                                       \
                                                            printf("suspend\n"); \
                                                            _TSP_RETURN(DRVTSP_FAIL);                                           \
                                                        }                                                                       \
                                                        _TSP_IND_LOCK();

#define _TSP_RETURN(_ret)                               _TSP_IND_UNLOCK();                                                      \
                                                        OSAL_TSP_MutexRelease(_s32TspMutexId);                                  \
                                                        if (_ret != DRVTSP_OK) { _u32LastErr = _ret; }                          \
                                                        return _ret; // Should be optimized by compiler

#define _TSP_IND_LOCK()                                 OSAL_TSP_MutexObtain(_s32IndMutexId, OSAL_TSP_MUTEX_TIMEOUT)
#define _TSP_IND_UNLOCK()                               OSAL_TSP_MutexRelease(_s32IndMutexId)

#define _TSP_EXT_LOCK()                                 OSAL_TSP_MutexObtain(_s32ExtMutexId, OSAL_TSP_MUTEX_TIMEOUT)
#define _TSP_EXT_UNLOCK()                               OSAL_TSP_MutexRelease(_s32ExtMutexId)

#define _TSP_SECFLT_LOCK(eng, id)						OSAL_TSP_MutexObtain(_TspState[eng].s32FltMutexId[id], OSAL_TSP_MUTEX_TIMEOUT)
#define _TSP_SECFLT_UNLOCK(eng, id)						OSAL_TSP_MutexRelease(_TspState[eng].s32FltMutexId[id])


#define TSP_ASSERT(_bool, _f)                           if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L1)&&(!(_bool))) { (_f); return DRVTSP_FAIL;} \
                                                        else if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L2)&&(!(_bool))) { (_f); while(1);} \
                                                        else {while(0);}

#define TSP_ASSERT2(_bool, _f)                          if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L1)&&(!(_bool))) { (_f); _TSP_RETURN(DRVTSP_FAIL);} \
                                                        else if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L2)&&(!(_bool))) { (_f); while(1);} \
                                                        else {while(0);}

#define TSP_DBG(_f, _a...)                              { if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L1)||(_u32TSPDGBLevel == E_DRVTSP_DBG_L2)) { OSAL_TSP_Print("[TSP_DBG]"); OSAL_TSP_Print(_f, ##_a); } }
#define DRVTSP_MAP_NULL                                 0xFFFFFFFF


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _DrvTSP_State
{
    DrvTSP_SW_Status                TSPStatus;
    DrvTSP_FltState                 FltState[TSP_FLT_NUM_ALL];
    DrvTSP_FltType                  FltType[TSP_FLT_NUM_ALL];
    MS_U32                          FltSource[TSP_FLT_NUM_ALL];
    //DrvTSP_FltState                 SecFltState[TSP_SECFLT_NUM];
    DrvTSP_Event                    FltEvtNotify[TSP_FLT_NUM_ALL];
    P_DrvTSP_EvtCallback            FltCallback[TSP_FLT_NUM_ALL];
    DrvTSP_FltState                 SecBufState[TSP_FLT_NUM_ALL];
    DrvTSP_FltMode                  FltMode[TSP_FLT_NUM_ALL];
    MS_S32                          s32FltMutexId[TSP_FLT_NUM_ALL];
} DrvTSP_State;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static MS_U8 _db_flag = 1;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 0
static unsigned char                _tsp_dat[] = // __attribute__((section(".tspdat"))) =
{
    #include "fwTSP.dat"
};
#endif

static MSIF_Version _drv_tsp_version = {
    .DDI = { TSP_DRV_VERSION, },
};

static MS_S32                       _s32TspMutexId = -1;
static MS_S32                       _s32IndMutexId = -1;
static MS_S32                       _s32ExtMutexId = -1;
static MS_S32                       _s32TspEventId = -1;

static MS_S32                       _s32TspSecRdyId[TSP_ENGINE_NUM] = { -1 };
static MS_S32                       _s32TspSecOvfId[TSP_ENGINE_NUM] = { -1 };
static MS_S32                       _s32TspSecCrcId[TSP_ENGINE_NUM] = { -1 };
static MS_S32                       _s32TspFltDisableId[TSP_ENGINE_NUM] = { -1 };
static MS_S32                       _s32TspTaskId = -1;
static MS_U32                       _u32LastErr;
static MS_BOOL                      _bSuspend = FALSE;
static DrvTSP_State                 _TspState[TSP_ENGINE_NUM];
static DrvTSP_Event                 _CmdEvtNotify = 0;
static DrvTSP_Event                 _RecEvtNotify = 0;

static P_DrvTSP_EvtCallback         _CmdCallback = NULL;
static P_DrvTSP_EvtCallback         _RecCallback = NULL;

static MS_BOOL                      _bSWInitReady = FALSE;
static MS_U32                       _u32DbgIntrCount = 0;
MS_U32                              _u32IntNum = E_INT_IRQ_TSP2HK;
MS_S32                              _s32EventFree0 = -1;
MS_S32                              _s32EventFree1 = -1;
MS_S32                              _s32EventFreeDefer0 = -1;
MS_S32                              _s32EventFreeDefer1 = -1;
MS_U32 _u32TaskLine = 0;

DrvTSP_Path eAudioPath = E_DRVTSP_PATH_SECOND;
DrvTSP_Path eVideoPath = E_DRVTSP_PATH_MAIN;

static DrvTSP_DbgLevel              _u32TSPDGBLevel = E_DRVTSP_DBG_L1;

#ifdef MSOS_TYPE_LINUX_KERNEL
wait_queue_head_t                   tsp_wq_head[TSP_FLT_NUM_ALL];
#endif // MSOS_TYPE_LINUX_KERNEL


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#ifdef T4_SWPATCH
static inline MS_U32 _TSP_RingBuffer_DataSize(MS_U8* pu8Read, MS_U8* pu8Write, MS_U8* pu8BufStart, MS_U8* pu8BufEnd)
{
    return (pu8Write >= pu8Read) ? (MS_U32)(pu8Write- pu8Read): ((pu8BufEnd- pu8BufStart)- (pu8Read- pu8Write));
}
#endif


static MS_U32 _TSP_GetTrailOne64(MS_U64 u64Flags)
{
    MS_U32 u32Temp = (MS_U32)(u64Flags & 0xFFFFFFFF);

    if (u32Temp)
        return MAsm_CPU_GetTrailOne(u32Temp);
    return (32 + MAsm_CPU_GetTrailOne((MS_U32)(u64Flags>> 32)));
}


static void _TSP_CmdQ_Init(void)
{
    while (HAL_TSP_CmdQ_CmdCount(TSP_PATH_MAIN) ||
            HAL_TSP_CmdQ_CmdCount(TSP_PATH_SECOND)); // wait command finish
}

static void _TSP_SwStateReset(MS_BOOL bInited)
{
    MS_U32 u32EngId, u32FilterId;
    MS_U8  mutexname[20] = { 0 };
    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_FLT_NUM_ALL; u32FilterId++)
        {
            _TspState[u32EngId].FltState[u32FilterId]        = E_DRVTSP_FLT_STATE_FREE;
            _TspState[u32EngId].FltType[u32FilterId]         = E_DRVTSP_FLT_TYPE_VIDEO;
            //_TspState[u32EngId].SecFltState[u32FilterId]     = E_DRVTSP_FLT_STATE_FREE;
            _TspState[u32EngId].SecBufState[u32FilterId]     = E_DRVTSP_FLT_STATE_FREE;
            _TspState[u32EngId].FltMode[u32FilterId]         = E_DRVTSP_FLT_MODE_CONTI;
            _TspState[u32EngId].FltEvtNotify[u32FilterId]    = E_DRVTSP_EVENT_DATA_INIT;
            _TspState[u32EngId].FltCallback[u32FilterId]     = NULL;
            if (bInited == TRUE)
            {
                if (_TspState[u32EngId].s32FltMutexId[u32FilterId] != -1)
                {
                    MsOS_ReleaseMutex(_TspState[u32EngId].s32FltMutexId[u32FilterId]);
                    MsOS_DeleteMutex(_TspState[u32EngId].s32FltMutexId[u32FilterId]);
                    _TspState[u32EngId].s32FltMutexId[u32FilterId] = -1;
                }
            }
            else
            {
                snprintf((char*)mutexname, 20, "FltMutex_%d", (int)u32FilterId);
                _TspState[u32EngId].s32FltMutexId[u32FilterId] = OSAL_TSP_MutexCreate(mutexname);
            }
        }
    }
}


static void _TSP_SW_Exit(void)
{
    MS_U32 u32Timer;

    _TSP_EXT_LOCK();
    _db_flag = 0;
    _TSP_EXT_UNLOCK();

    u32Timer = MsOS_GetSystemTime();
    // check _s32TspTaskId for NOS case
    while(_s32TspTaskId > 0)
    {
        _TSP_SetEvent(TSP_TASK_EVENT_FLT_FREE);
        _TSP_EXT_LOCK();
        if (_db_flag == 1)
        {
            _TSP_EXT_UNLOCK();
            break;
        }
        _TSP_EXT_UNLOCK();

        if (MsOS_GetSystemTime()-u32Timer > 1000)
        {
            TSP_DBG("[%s][%d] TSP task exit timeout!!!!!!\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    if (_s32EventFree0 != -1)
    {
        OSAL_TSP_EventDelete(_s32EventFree0);
    }
    if (_s32EventFree1 != -1)
    {
        OSAL_TSP_EventDelete(_s32EventFree1);
    }
    if (_s32EventFreeDefer0 != -1)
    {
        OSAL_TSP_EventDelete(_s32EventFreeDefer0);
    }
    if (_s32EventFreeDefer1 != -1)
    {
        OSAL_TSP_EventDelete(_s32EventFreeDefer1);
    }
    if (_s32TspMutexId != -1)
    {
        MsOS_ReleaseMutex(_s32TspMutexId);
        MsOS_DeleteMutex(_s32TspMutexId);
    }
    if (_s32IndMutexId != -1)
    {
        MsOS_ReleaseMutex(_s32IndMutexId);
        MsOS_DeleteMutex(_s32IndMutexId);
    }
    if (_s32ExtMutexId != -1)
    {
        MsOS_ReleaseMutex(_s32ExtMutexId);
        MsOS_DeleteMutex(_s32ExtMutexId);
    }
    if (_s32TspEventId != -1)
    {
        OSAL_TSP_EventDelete(_s32TspEventId);
    }
    if (_s32TspSecRdyId[0] != -1)
    {
        OSAL_TSP_EventDelete(_s32TspSecRdyId[0]);
    }
    if (_s32TspSecOvfId[0] != -1)
    {
        OSAL_TSP_EventDelete(_s32TspSecOvfId[0]);
    }
    if (_s32TspSecCrcId[0] != -1)
    {
        OSAL_TSP_EventDelete(_s32TspSecCrcId[0]);
    }
    if (_s32TspFltDisableId[0] != -1)
    {
        OSAL_TSP_EventDelete(_s32TspFltDisableId[0]);
    }

    _s32TspTaskId           = -1;
    _s32EventFree0          = -1;
    _s32EventFree1          = -1;
    _s32EventFreeDefer0     = -1;
    _s32EventFreeDefer1     = -1;
    _s32TspMutexId          = -1;
    _s32IndMutexId          = -1;
    _s32TspEventId          = -1;
    _s32TspSecRdyId[0]      = -1;
    _s32TspSecOvfId[0]      = -1;
    _s32TspSecCrcId[0]      = -1;
    _s32TspFltDisableId[0]  = -1;
    _TspState[0].TSPStatus  = E_DRVTSP_SW_EXIT;
    _bSWInitReady           = FALSE;
}


static void _TSP_Close(void)
{
    HAL_TSP_Int_Disable(TSP_INT_TRIGGER_MASK);
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();
    HAL_TSP_WbDmaEnable(TSP_PATH_MAIN, FALSE);
    HAL_TSP_WbDmaEnable(TSP_PATH_SECOND, FALSE);
    HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);
    _TSP_SW_Exit();
    HAL_TSP_PowerCtrl(DISABLE);
// @FIXME: check root cause of APP does not release lock
// Check all possibilities of deadlock
// 1. Both coprocess flow
// 2. Any unfinshed and unstable status of like unfinished job (ISR,TASK,AP) after CLOSE
}


static void _TSP_RegStateStore(void)
{
    HAL_TSP_Int_Disable(TSP_INT_TRIGGER_MASK);
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();

    HAL_TSP_SaveFltState();
}


static void _TSP_RegStateRestore(void)
{
    HAL_TSP_RestoreFltState();
}

static void _TSP_PidFlt_Init(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    REG_PidFlt *pPidFilter = NULL;
    if(u32PidFltId == _PCRFLT_)
    {
        pPidFilter = _PFLT_PCR;
    }
    else
    {
        pPidFilter = &(_TspPid[u32EngId].Flt[u32PidFltId]);
    }

    HAL_TSP_PidFlt_SetPid(pPidFilter, DRVTSP_PID_NULL);
    HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_EN_NONE);
    //_TspState[u32EngId].FltMap[u32PidFltId] = DRVTSP_MAP_NULL;
    _TspState[u32EngId].FltEvtNotify[u32PidFltId] = E_DRVTSP_EVENT_DATA_INIT;
    _TspState[u32EngId].FltCallback[u32PidFltId]  = NULL;
}

static void _TSP_PidFlt_Disable(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    REG_PidFlt *pPidFilter = NULL;

    if(_PCRFLT_ == u32PidFltId)
    {
        pPidFilter = _PFLT_PCR;
    }
    else
    {
        pPidFilter = &(_TspPid[u32EngId].Flt[u32PidFltId]);
    }

    // PVR don't need reset flter output
    if (E_DRVTSP_FLT_TYPE_PVR!= _TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_EN_NONE);
    }

    if (E_DRVTSP_FLT_TYPE_PES ==_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PVR_Enable(FALSE);
    }
    //HAL_TSP_PidFlt_SelFltSource(pPidFilter, TSP_PIDFLT_EN_NONE);
    //HAL_TSP_SecFlt_SetRmnCount(&(_TspSec[u32EngId].Flt[u32PidFltId]), 0);

    _TspState[u32EngId].FltState[u32PidFltId] &= (DrvTSP_FltState)(~E_DRVTSP_FLT_STATE_ENABLE);

}

static MS_BOOL _TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId)
{
    MS_U32  i;
    MS_U32  pid_idx_start = 0;
    MS_U32  pid_idx_end = 0;

    *pu32PidFltId = 0xFFFFFFFF;

    if (E_DRVTSP_FLT_TYPE_VIDEO  == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        pid_idx_start = (eVideoPath == E_DRVTSP_PATH_MAIN)?0:TSP_PIDFLT_NUM;
        pid_idx_end = (eVideoPath == E_DRVTSP_PATH_MAIN)?TSP_PIDFLT_NUM:TSP_PIDFLT_NUM_ALL;
    }
    if (E_DRVTSP_FLT_TYPE_AUDIO  == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        pid_idx_start = (eAudioPath == E_DRVTSP_PATH_MAIN)?0:TSP_PIDFLT_NUM;
        pid_idx_end = (eAudioPath == E_DRVTSP_PATH_MAIN)?TSP_PIDFLT_NUM:TSP_PIDFLT_NUM_ALL;

    }

    if (E_DRVTSP_FLT_TYPE_PES == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        pid_idx_start = (_RECORD_PATH == E_DRVTSP_PATH_MAIN)?0:TSP_PIDFLT_NUM;
        pid_idx_end = (_RECORD_PATH == E_DRVTSP_PATH_MAIN)?TSP_PIDFLT_NUM:TSP_PIDFLT_NUM_ALL;
    }

    if (E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        pid_idx_start = _PCRFLT_;
        pid_idx_end = _PCRFLT_ + 1;
    }

    for(i = pid_idx_start; i< pid_idx_end; i++)
    {
        if (_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            break;
        }
    }

    if (i == pid_idx_end)
    {
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] No free fitler found 0x%02lx\n", __LINE__, i));
        return FALSE;
    }

    _TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_ALLOC;
    _TspState[u32EngId].FltType[i] = (eFilterType&(DrvTSP_FltType)(~E_DRVTSP_FLT_MASK));
    _TspState[u32EngId].FltSource[i] = E_DRVTSP_FLT_SOURCE_TYPE_FILE;

    _TSP_PidFlt_Init(u32EngId, i);


    *pu32PidFltId = i;

    return TRUE;
}

TSP_Result _TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bResetState)
{
    if ( (u32EngId    >= TSP_ENGINE_NUM) ||
         (u32PidFltId >= TSP_FLT_NUM_ALL) ||
         (_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_FREE) )
    {
        //TSP_DBG("[TSP_ERROR][%06d] (EngId, PidFltId, FilterState)= (%ld %ld %d)\n", __LINE__, u32EngId, u32PidFltId, _TspState[u32EngId].FltState[u32PidFltId]);
        TSP_DBG("[TSP_ERROR][%06d] Bad free parmeter\n",__LINE__);
        return (TSP_Result)FALSE;
    }

    _TSP_PidFlt_Disable(u32EngId, u32PidFltId);

    // Release Filter
    _TSP_PidFlt_Init(u32EngId, u32PidFltId);
    if (bResetState)
    {
        _TspState[u32EngId].FltState[u32PidFltId] = E_DRVTSP_FLT_STATE_FREE;
    }

    // Reset call back function without PVR
    //if (_TspState[u32EngId].FltType[u32PidFltId] == E_DRVTSP_FLT_TYPE_PVR)
    if (u32PidFltId < TSP_PIDFLT_NUM)
    {
        _TspState[u32EngId].FltCallback[u32PidFltId] = NULL;
        _TspState[u32EngId].FltMode[u32PidFltId] = E_DRVTSP_FLT_MODE_CONTI;
    }
    return (TSP_Result)TRUE;
}

static MS_BOOL _TSP_PidFlt_StateGet(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState)
{
    REG_PidFlt *pPidFilter;

    // @FIXME: Richard: Don't plan to have section filter in PVR pid filter at this stage.
    //              Therefore, don't need to check PVR pid filter
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT(TSP_PIDFLT_NUM> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFlt Id %ld\n", __LINE__, u32PidFltId));
    TSP_ASSERT(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if(_PCRFLT_ == u32PidFltId)
    {
        pPidFilter = _PFLT_PCR;
    }
    else
    {
        pPidFilter= &(_TspPid[u32EngId].Flt[u32PidFltId]);
    }
    *peState=   _TspState[u32EngId].FltState[u32PidFltId];
/*
    if (HAL_TSP_Scmb_Status(TRUE, u32PidFltId) || HAL_TSP_Scmb_Status(FALSE, u32PidFltId))
    {
        *peState |= E_DRVTSP_FLT_STATE_SCRAMBLED;
    }

    switch (_TspState[u32EngId].FltType[u32PidFltId])
    {
    case E_DRVTSP_FLT_TYPE_VIDEO:
    case E_DRVTSP_FLT_TYPE_AUDIO:
    case E_DRVTSP_FLT_TYPE_AUDIO2:
        break;
    case E_DRVTSP_FLT_TYPE_SECTION:
    case E_DRVTSP_FLT_TYPE_PES:
    case E_DRVTSP_FLT_TYPE_PACKET:
    case E_DRVTSP_FLT_TYPE_TELETEXT:
        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);
        if (HAL_TSP_SecBuf_GetBufRead(&(_TspSec[u32EngId].Flt[u32SecFltId])) != HAL_TSP_SecBuf_GetBufWrite(&(_TspSec[u32EngId].Flt[u32SecFltId])))
        {
            *peState |= E_DRVTSP_FLT_STATE_STREAM_AVAIL;
        }
        break;
    case E_DRVTSP_FLT_TYPE_PCR:
    default:
        break;
    }
*/
    return TRUE;
}


static void _TSP_RegStateReset(void)
{
    MS_U32 u32EngId;
    MS_U32 u32FilterId;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_FLT_NUM_ALL; u32FilterId++)
        {
            _TSP_PidFlt_Init(u32EngId, u32FilterId);
        }
/*
        for (u32FilterId = 0; u32FilterId < TSP_SECFLT_NUM; u32FilterId++)
        {
            _TSP_SecBuf_Init(u32EngId, u32FilterId);
            HAL_TSP_SecBuf_SetBuffer(&(_TspSec[u32EngId].Flt[u32FilterId]), 0, 0);
            _TSP_SecFlt_Init(u32EngId, u32FilterId);
        }
*/
    }
}


static MS_BOOL _TSP_IsPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_S32 s32Event;
    MS_U32 u32Flag = 0;

    if (u32PidFltId>= 32)
    {
        s32Event = _s32EventFreeDefer1;
        u32PidFltId -= 32;
    }
    else
    {
        s32Event = _s32EventFreeDefer0;
    }
    OSAL_TSP_EventGet(s32Event, 1<< u32PidFltId, &u32Flag, E_AND, 0);
    return (u32Flag & (1<< u32PidFltId)) ? TRUE: FALSE;
}

static void _TSP_SetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_S32 s32Event;

    if (u32PidFltId>= 32)
    {
        s32Event = _s32EventFreeDefer1;
        u32PidFltId -= 32;
    }
    else
    {
        s32Event = _s32EventFreeDefer0;
    }
    OSAL_TSP_EventSet(s32Event, 1<< u32PidFltId);
}

static void _TSP_ResetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_S32 s32Event;
    MS_U32 u32Flag = 0;

    if (u32PidFltId>= 32)
    {
        s32Event = _s32EventFreeDefer1;
        u32PidFltId -= 32;
    }
    else
    {
        s32Event = _s32EventFreeDefer0;
    }
    OSAL_TSP_EventGet(s32Event, 1<< u32PidFltId, &u32Flag, E_OR_CLEAR, 0);
}

static MS_BOOL _TSP_IsAskPidFltFree(MS_U32 u32PidFltId)
{
    MS_S32 s32Event;
    MS_U32 u32Flag = 0;

    if (u32PidFltId>= 32)
    {
        s32Event = _s32EventFree1;
        u32PidFltId -= 32;
    }
    else
    {
        s32Event = _s32EventFree0;
    }
    OSAL_TSP_EventGet(s32Event, 1<< u32PidFltId, &u32Flag, E_AND, 0);
    return (u32Flag & (1<< u32PidFltId)) ? TRUE: FALSE;
}

static void _TSP_AskPidFltFree(MS_U32 u32PidFltId)
{
    MS_S32 s32Event;

    if (u32PidFltId >= 32)
    {
        s32Event = _s32EventFree1;
        u32PidFltId -= 32;
    }
    else
    {
        s32Event = _s32EventFree0;
    }
    OSAL_TSP_EventSet(s32Event, 1<< u32PidFltId);
}

static MS_U64 _TSP_GetAskPidFltFreeList(void)
{
    MS_U32 u32Free0, u32Free1;

    OSAL_TSP_EventGet(_s32EventFree0, 0xFFFFFFFF, &u32Free0, E_OR, 0);
    OSAL_TSP_EventGet(_s32EventFree1, 0xFFFFFFFF, &u32Free1, E_OR, 0);
    return (((MS_U64)u32Free1)<< 32) | (MS_U64)u32Free0;
}

static void _TSP_ClrAskPidFltFreeList(MS_U64 u64Mask)
{
    MS_U32 u32Free0 = (MS_U32)(u64Mask & 0xFFFFFFFF);
    MS_U32 u32Free1 = (MS_U32)((u64Mask >> 32) & 0xFFFFFFFF);
    MS_U32 u32Dummy;

    OSAL_TSP_EventGet(_s32EventFree0, u32Free0, &u32Dummy, E_OR_CLEAR, 0);

#if 0
    if (u32Free0 != u32Dummy)
    {
//        TSP_DBG("[UNKNOWN][%s][%d] strange 1 error, 0x%08x , 0x%08x\n", __FUNCTION__, __LINE__, (int)u32Free0, (int)u32Dummy);
    }
#endif

    OSAL_TSP_EventGet(_s32EventFree1, u32Free1, &u32Dummy, E_OR_CLEAR, 0);
#if 0
    if (u32Free1 != u32Dummy)
    {
//        TSP_DBG("[UNKNOWN][%s][%d] strange 1 error, 0x%08x , 0x%08x\n", __FUNCTION__, __LINE__, (int)u32Free1, (int)u32Dummy);
    }
#endif
}


static MS_BOOL _TSP_Flt_StateGet(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState)
{
    DrvTSP_FltState     PidFltState;

    if(TSP_FLT_NUM_ALL <= u32FltId)
        return FALSE;

    if (!_TSP_PidFlt_StateGet(u32EngId, u32FltId, &PidFltState))
    {
         return FALSE;
    }

    *peState=   PidFltState;

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
// Interrupt service routine of TSP
// Arguments:   None
// Return:      None
//--------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _TSP_Isr(int irq,void *dev_id)
#else
static void _TSP_Isr(void)
#endif
{
    MS_U32                 u32HwInt;
    MS_U32                 u32Isrhandled = 0;
    OSAL_TSP_IntDisable();
    _u32DbgIntrCount++;
    //HAL_TSP_ISR_SAVE_ALL();

    u32HwInt = HAL_TSP_HW_INT_STATUS();
    if (u32HwInt)
    {
        //u32EngId    = (u32SwInt & TSP_SWINT_INFO_ENG_MASK) >> TSP_SWINT_INFO_ENG_SHFT;
        //u32IntStat  = (u32SwInt & TSP_SWINT_STATUS_CMD_MASK) >> TSP_SWINT_STATUS_CMD_SHFT;
        //u32SecFltId = (u32SwInt & TSP_SWINT_INFO_SEC_MASK) >> TSP_SWINT_INFO_SEC_SHFT;
        //pSecFilter  = &(_TspSec[u32EngId].Flt[u32SecFltId]);
        if(u32HwInt & TSP_INT_PCR_UPDATE_DONE)
        {
            u32Isrhandled = TSP_INT_PCR_UPDATE_DONE;
            HAL_TSP_SYNTH_Adjust();
        }
        else if(u32HwInt & TSP_INT_FILEIN_READ_DONE_P1)
        {
            //printf("main path file in done\n");
            u32Isrhandled = TSP_INT_FILEIN_READ_DONE_P1;
            if(E_DRVTSP_EVENT_CB_POLL == (_CmdEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_FILE_DONE_P1_POLL);
            }
            else if(E_DRVTSP_EVENT_CB_SELF == (_CmdEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_FILE_DONE_P1_SELF);
            }
        }
        else if(u32HwInt & TSP_INT_FILEIN_READ_DONE_P2)
        {
            u32Isrhandled = TSP_INT_FILEIN_READ_DONE_P2;
            if(E_DRVTSP_EVENT_CB_POLL == (_CmdEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_FILE_DONE_P2_POLL);
            }
            else if(E_DRVTSP_EVENT_CB_SELF == (_CmdEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_FILE_DONE_P2_SELF);
            }
        }
        else if(u32HwInt & TSP_INT_MUI_MEET_TAIL)
        {
            u32Isrhandled = TSP_INT_MUI_MEET_TAIL;
            if(E_DRVTSP_EVENT_CB_POLL == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_RECORD_RDY_POLL);
            }
            else if(E_DRVTSP_EVENT_CB_SELF == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_RECORD_RDY_SELF);
            }
        }
        else if(u32HwInt & TSP_INT_MIU_MEET_THRESHOLD)
        {
            u32Isrhandled = TSP_INT_MIU_MEET_THRESHOLD;
            if(E_DRVTSP_EVENT_CB_POLL == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_MEET_THRESHOLD_POLL);
            }
            else if(E_DRVTSP_EVENT_CB_SELF == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_MEET_THRESHOLD_SELF);
            }
        }
        else if(u32HwInt & TSP_INT_MIU_MEET_MIDDLE)
        {
            u32Isrhandled = TSP_INT_MIU_MEET_MIDDLE;
            if(E_DRVTSP_EVENT_CB_POLL == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_MEET_MIDDLE_POLL);
            }
            else if(E_DRVTSP_EVENT_CB_SELF == (_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK))
            {
               _TSP_SetEvent(TSP_TASK_EVENT_MEET_MIDDLE_SELF);
            }
        }
        else
        {
            //[ISR print]
            u32Isrhandled = u32HwInt;
            TSP_DBG("[%s] unknown TSP interrupt 0x%08x\n", __FUNCTION__, (unsigned int)u32HwInt);
        }
    }

    HAL_TSP_Int_ClearHw(u32Isrhandled);

    //HAL_TSP_ISR_RESTORE_ALL();
#ifdef MSOS_TYPE_LINUX_KERNEL
    return IRQ_HANDLED;
#else
    OSAL_TSP_IntEnable();
#endif
}

TSP_Result MDRV_TSP_FAIL_NOT_SUPPORTED(void)
{
    //TSP_DBG("not supported function\n");
    return DRVTSP_NOT_SUPPORTED;
}
TSP_Result MDRV_TSP_OK_NOT_SUPPORTED(void)
{
    return DRVTSP_OK;
}
#if 0
void MDrv_TSP_SecFlt_Update(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    _TSP_SetSecRdyId(u32EngId, (1 << u32SecFltId));
}

static MS_BOOL _TSP_Isr_Proc_SecRdyCrc(MS_U32 u32EngId, MS_U32 u32FltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
//    MS_BOOL bSecCrc;
    MS_BOOL bData;
    MS_U32 u32Event;
    MS_U32 u32Events;
    MS_U32 u32EvtNotify;
    P_DrvTSP_EvtCallback pfEvtCallback = NULL;
    static DrvTSP_Msg msg;
    MS_U32 u32Read;

    //u32EvtNotify  = _TspState[u32EngId].SecFltEvtNotify[u32FltId];
    //pfEvtCallback = _TspState[u32EngId].SecFltCallback[u32FltId];

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }

    if (!(_TspState[u32EngId].FltState[u32FltId] & E_DRVTSP_FLT_STATE_ENABLE))
    {
        // TSP_DBG("[%s][%d] %d 0x%08x\n", __FUNCTION__, __LINE__,
        //(int)u32FltId, (int)_TspState[u32EngId].FltState[u32FltId]);
        goto jump_bad_state;
    }

    //bSecCrc = (_TSP_GetSecCrcId(u32EngId, (1<< u32FltId), u32Event, OSAL_EVENT_OR_CLEAR))? TRUE: FALSE;
    _TSP_IND_LOCK();

#if 0
    if (_TSP_GetFltDisableId(u32EngId, (1 << u32FltId), u32Event, OSAL_EVENT_OR_CLEAR))
    {
    	//No need to disable filter here or Overflow callback will miss. fannie, 2010/03/25
        _TSP_PidFlt_Disable(u32EngId, u32FltId);  // critical section
        //_TSP_GetSecRdyId(u32EngId, (1<< u32FltId), u32Event, OSAL_EVENT_OR_CLEAR);
        //_TSP_GetSecOvfId(u32EngId, (1<< u32FltId), u32Event, OSAL_EVENT_OR_CLEAR);
    }
#endif

    u32Read = HAL_TSP_SecBuf_GetBufRead(&(_TspSec[u32EngId].Flt[u32FltId]));
    bData = (u32Read != HAL_TSP_SecBuf_GetBufWrite(&(_TspSec[u32EngId].Flt[u32FltId])))? TRUE: FALSE; // critical section
    _TSP_IND_UNLOCK();

#ifdef T4_SWPATCH
    MS_PHYADDR PHYStart, PHYEnd, PHYWrite, PHYRead;
    MS_U32 u32DataLen;
    MS_U32 u32RBLen;
    MS_U8*      pu8Start;
    MS_U8*      pu8End;
    MS_U8*      pu8Read;
    MS_U8*      pu8Write;

    PHYStart = HAL_TSP_SecBuf_GetBufStart(&(_TspSec[u32EngId].Flt[u32FltId]));
    PHYEnd = HAL_TSP_SecBuf_GetBufEnd(&(_TspSec[u32EngId].Flt[u32FltId]));
    PHYRead = HAL_TSP_SecBuf_GetBufRead(&(_TspSec[u32EngId].Flt[u32FltId]));
    PHYWrite = HAL_TSP_SecBuf_GetBufWrite(&(_TspSec[u32EngId].Flt[u32FltId]));

    pu8Start = (MS_U8 *)PHYStart;
    pu8End = (MS_U8 *)PHYEnd;
    pu8Read = (MS_U8 *)PHYRead;
    pu8Write = (MS_U8 *)PHYWrite;

    pu8Read  =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Read);
    pu8Write =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Write);
    pu8Start =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Start);
    pu8End   =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8End);

    u32RBLen = _TSP_RingBuffer_DataSize(pu8Read, pu8Write, pu8Start, pu8End);
    // Check not enought section data
    if ((pu8Read + 3) <= pu8End)
    {
        u32DataLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        u32DataLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
    }
    else // ((uReadAddr+1) == uEndAddr)
    {
        u32DataLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
    }
    u32DataLen +=        3;

    if (bData && ((u32RBLen>8)&((u32DataLen<=u32RBLen))))
#else
    if (bData)
#endif
    {
        if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
        {
            *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_READY;
        }
        else
        {
        	_TSP_SECFLT_LOCK(u32EngId, u32FltId);
			u32EvtNotify  = _TspState[u32EngId].SecFltEvtNotify[u32FltId];
			pfEvtCallback = _TspState[u32EngId].SecFltCallback[u32FltId];
            if ((u32EvtNotify & E_DRVTSP_EVENT_DATA_READY) && pfEvtCallback)
            {
                msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32FltId <<MSG_FLTINFO_SEC_ID_SHFT);
                _u32TaskLine = __LINE__;
                _TSP_SECFLT_UNLOCK(u32EngId, u32FltId);
                pfEvtCallback(E_DRVTSP_EVENT_DATA_READY, &msg);
                _u32TaskLine = __LINE__;

#ifndef T4_SWPATCH
                _TSP_IND_LOCK();
                if (u32Read == HAL_TSP_SecBuf_GetBufRead(&(_TspSec[u32EngId].Flt[u32FltId])))
                {
                    _TSP_GetSecRdyId(u32EngId, (1<< u32FltId), u32Event, OSAL_EVENT_OR_CLEAR);
                }
                _TSP_IND_UNLOCK();
#endif
            }
            else
                _TSP_SECFLT_UNLOCK(u32EngId, u32FltId);
        }
    }
    else
    {
        //_TSP_GetSecRdyId(u32EngId, (1<< u32FltId), u32Event, OSAL_EVENT_OR_CLEAR);
    }

    if (bData)
    {
        return TRUE;
    }
jump_bad_state:
    return FALSE;
}
#endif // #if 0

static MS_BOOL _TSP_Isr_Proc_FileIn(MS_U32 u32CbOpt, MS_U32 u32Events)
{
    static DrvTSP_Msg   msg;

    if ((_RecEvtNotify & E_DRVTSP_EVENT_CB_MASK) != u32CbOpt)
    {
        return FALSE;
    }

    if (_RecEvtNotify && _RecCallback)
    {
        switch (u32Events)
        {
            case TSP_TASK_EVENT_MEET_THRESHOLD_POLL:
            case TSP_TASK_EVENT_MEET_THRESHOLD_SELF:
                if (HAS_FLAG(_RecEvtNotify, E_DRVTSP_EVENT_RECBUF_MEET_THRESHOLD))
                {
                    //msg.PvrBufId = (0x0 << MSG_PVRBUF_ID_SHFT);
                    _RecCallback(E_DRVTSP_EVENT_RECBUF_MEET_THRESHOLD, &msg);
                }
                break;
            case TSP_TASK_EVENT_MEET_MIDDLE_POLL:
            case TSP_TASK_EVENT_MEET_MIDDLE_SELF:
                if (HAS_FLAG(_RecEvtNotify, E_DRVTSP_EVENT_RECBUF_MEET_MIDDLE))
                {
                    //msg.PvrBufId = (0x1 << MSG_PVRBUF_ID_SHFT);
                    _RecCallback(E_DRVTSP_EVENT_RECBUF_MEET_MIDDLE, &msg);
                }
                break;
        }
    }
    return TRUE;
}

#if 0
static MS_BOOL _TSP_Isr_Proc_SecOvf(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
    static DrvTSP_Msg   msg;
    MS_U32                  u32EvtNotify;
    P_DrvTSP_EvtCallback    pfEvtCallback = NULL;
    MS_U32 u32PidFltId;
    DrvTSP_FltState         FltState;

    //u32EvtNotify  = _TspState[u32EngId].SecFltEvtNotify[u32SecFltId];
    //pfEvtCallback = _TspState[u32EngId].SecFltCallback[u32SecFltId];

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

	//if polling mode, evets have cleared already, not need to clear again, fannie.tsai, 2010/04/22
    /*if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
    	if (!_TSP_GetEvent(u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0))
    	{
        	return FALSE;
        }
    }*/

    FltState = _TspState[u32EngId].FltState[u32SecFltId];

    //_TSP_GetSecOvfId(u32EngId, (1<< u32SecFltId), u32Events, OSAL_EVENT_OR_CLEAR);
    //_TSP_GetSecRdyId(u32EngId, (1<< u32SecFltId), u32Events, OSAL_EVENT_OR_CLEAR);

    // disable overflow filter
    _TSP_IND_LOCK();
    for (u32PidFltId = 0; u32PidFltId < TSP_PIDFLT_NUM; u32PidFltId++)
    {
        REG_PidFlt *pPidFilter = &_TspPid[u32EngId].Flt[u32PidFltId];
        if ( (HAL_TSP_PidFlt_GetSecFlt(pPidFilter) == u32SecFltId) &&
             (HAL_TSP_PidFlt_GetFltOutput(pPidFilter) & TSP_PIDFLT_OUT_SECFLT) )   // ?????? OK  // // critical section
        {
            _TSP_PidFlt_Disable(u32EngId, u32PidFltId);  // ???? move out --> OK  // // critical section
            break;
        }
    }
    _TSP_IND_UNLOCK();
    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        *pu32Evt = (MS_U32)E_DRVTSP_EVENT_BUF_OVERFLOW;
    }
    else
    {
    	_TSP_SECFLT_LOCK(u32EngId, u32SecFltId);
    	u32EvtNotify  = _TspState[u32EngId].SecFltEvtNotify[u32SecFltId];
	    pfEvtCallback = _TspState[u32EngId].SecFltCallback[u32SecFltId];
        if (!(FltState & E_DRVTSP_FLT_STATE_ENABLE))
        {
            //goto jump_bad_state;
            _TSP_SECFLT_UNLOCK(u32EngId, u32SecFltId);
			return TRUE;
        }

        if ((u32EvtNotify & E_DRVTSP_EVENT_BUF_OVERFLOW) && pfEvtCallback)
        {
            msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32SecFltId <<MSG_FLTINFO_SEC_ID_SHFT);
            _TSP_SECFLT_UNLOCK(u32EngId, u32PidFltId);
            _u32TaskLine = __LINE__;
            pfEvtCallback(E_DRVTSP_EVENT_BUF_OVERFLOW, &msg);
            _u32TaskLine = __LINE__;
        }
        else
		    _TSP_SECFLT_UNLOCK(u32EngId, u32SecFltId);
    }
//jump_bad_state:
    return TRUE;
}
#endif // #if 0

static void _TSP_Isr_ProcSelf(OSAL_TSP_ProcTypeParam* pData)
{
    MS_U32              u32Events = 0, u32SecRdy = 0, u32SecOvf = 0;//, u32SecFree = 0;

    MS_U32              u32Flag = 0;
    MS_U32              u32FltId = pData->u32FltId;
    TSP_Result*         pRet = (TSP_Result*)pData->pRet;
    MS_U32              u32PidFltId;
    MS_U64              u64Mask, u64AskFreeFltList = 0;

    u32Flag = u32FltId;

    // disable interrupt to prevent from accessing event in ISR at the same time
    // @ FIXME: should apply to every OS?
    OSAL_TSP_IntDisable();
    _TSP_GetEvent(u32Events, TSP_TASK_EVENT_GROUP_SELF | TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);
    OSAL_TSP_IntEnable();

    // _TSP_GetEvent(u32Events, TSP_TASK_EVENT_GROUP_SELF | TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR, 20);
    _u32TaskLine = __LINE__;

    *pRet = DRVTSP_FAIL;
    /*
    if (u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_POLL))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_SELF);
    }
    */
    if (u32Events & TSP_TASK_EVENT_MEET_THRESHOLD_SELF)
    {
        *pRet = (_TSP_Isr_Proc_FileIn(E_DRVTSP_EVENT_CB_SELF, TSP_TASK_EVENT_MEET_THRESHOLD_SELF))?DRVTSP_OK:DRVTSP_FAIL;
    }

    if(u32Events & TSP_TASK_EVENT_MEET_MIDDLE_SELF)
    {
        *pRet = (_TSP_Isr_Proc_FileIn(E_DRVTSP_EVENT_CB_SELF, TSP_TASK_EVENT_MEET_MIDDLE_SELF))?DRVTSP_OK:DRVTSP_FAIL;
    }

/*
    if (u32Events & TSP_TASK_EVENT_SECTION_SELF)
    {
        for (i = 0; i < TSP_ENGINE_NUM; i++)
        {
            MS_U32                  u32SecFltId;

            _TSP_GetSecOvfId(i, u32Flag, u32SecOvf, OSAL_EVENT_OR_CLEAR);
            _TSP_GetSecRdyId(i, u32Flag, u32SecRdy, OSAL_EVENT_OR_CLEAR);
#if 0
            if (u32SecOvf)
            {
                // TSP_DBG("[%s][%d] ==========================> overflow flag 0x%08x\n", __FUNCTION__, __LINE__, (int)u32SecOvf);
                // TSP_DBG("[%s][%d] ==========================> ready 1  flag 0x%08x\n", __FUNCTION__, __LINE__, (int)u32SecRdy);
            }
#endif

			while (u32SecRdy)
            {
                u32SecFltId = MAsm_CPU_GetTrailOne(u32SecRdy);  //[TODO] balance every filter priority
                                                                //[NOTE] it may be already balance for long term running
                *pRet = (_TSP_Isr_Proc_SecRdyCrc(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
				//write(2,"---------u32SecRdy while-loop\n",31);
                u32SecRdy &= ~(1 << u32SecFltId);
            }

            while (u32SecOvf)
            {
                u32SecFltId = MAsm_CPU_GetTrailOne(u32SecOvf);  //[TODO] balance every filter priority
                                                                //[NOTE] it may be already balance for long term running
                *pRet = (_TSP_Isr_Proc_SecOvf(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                u32SecOvf &= ~(1 << u32SecFltId);
            }
        }
    }
*/

    if (u32Events & TSP_TASK_EVENT_FLT_FREE)
    {
    u64Mask = u64AskFreeFltList = _TSP_GetAskPidFltFreeList();
    _u32TaskLine = __LINE__;
    if (u64Mask)
    {
        _TSP_IND_LOCK();
    }
    while (u64AskFreeFltList)
    {
        _u32TaskLine = __LINE__;
        u32PidFltId = _TSP_GetTrailOne64(u64AskFreeFltList);
        if (_TspState[0].FltState[u32PidFltId] != E_DRVTSP_FLT_STATE_FREE)
        {
            /*
            if (E_DRVTSP_FLT_TYPE_PCR == _TspState[0].FltType[u32PidFltId])
            {
                MS_U32 u32SecFltId;
                u32SecFltId=    HAL_TSP_PidFlt_GetSecFlt(&(_TspPid[0].Flt[u32PidFltId]));
                MS_ASSERT(u32SecFltId == u32PidFltId);
                _TSP_SecFlt_Free(0, u32SecFltId, FALSE);
            }
            */
            _TSP_PidFlt_Free(0, u32PidFltId, FALSE);
            _TSP_GetSecRdyId(0, (1<< u32PidFltId), u32SecRdy, OSAL_EVENT_OR_CLEAR);
            _TSP_GetSecOvfId(0, (1<< u32PidFltId), u32SecOvf, OSAL_EVENT_OR_CLEAR);
        }
        u64AskFreeFltList &= (~(((MS_U64)0x1)<<(u32PidFltId)));
    }
    if (u64Mask)
    {
        _TSP_IND_UNLOCK();
    }
    if (u64Mask)
    {
        _TSP_ClrAskPidFltFreeList(u64Mask);
        /*
        u64AskFreeFltList = _TSP_GetAskPidFltFreeList();
        if (!u64AskFreeFltList)
        {
                    //_TSP_GetEvent(u32Events, TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR_CLEAR, 0);
        }
        */
            _u32TaskLine = __LINE__;
        }
        _u32TaskLine = __LINE__;
    }
/*
    _TSP_GetSecRdyId(0, u32Flag, u32SecRdy, OSAL_EVENT_OR);
    _TSP_GetSecOvfId(0, u32Flag, u32SecOvf, OSAL_EVENT_OR);
    if (u32SecRdy || u32SecOvf)
    {
        _TSP_SetEvent(TSP_TASK_EVENT_SECTION_SELF);
    }
    else
    {
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_SECTION_SELF, OSAL_EVENT_OR_CLEAR, 0);
    }
*/
}

static void _TSP_Isr_ProcPoll(OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt)
{
    MS_U32              u32Events = 0;
    MS_U32              u32Flag = 0;
    MS_U32              u32FltId = pData->u32FltId;
    TSP_Result*         pRet = (TSP_Result*)pData->pRet;

    u32Flag = (1<< u32FltId);

    // disable interrupt to prevent from accessing event in ISR at the same time
    // @ FIXME: should apply to every OS?
    OSAL_TSP_IntDisable();
    _TSP_GetEvent(u32Events, TSP_TASK_EVENT_GROUP_POLL, OSAL_EVENT_OR_CLEAR, 0);
    OSAL_TSP_IntEnable();

    *pRet = DRVTSP_FAIL;
    /*
    if (u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_POLL))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_POLL);
    }
    */
    if (u32Events & TSP_TASK_EVENT_MEET_THRESHOLD_POLL)
    {
        *pu32Evt = TSP_TASK_EVENT_MEET_THRESHOLD_POLL;
        *pRet = (_TSP_Isr_Proc_FileIn(E_DRVTSP_EVENT_CB_POLL, TSP_TASK_EVENT_MEET_THRESHOLD_POLL))?DRVTSP_OK:DRVTSP_FAIL;
    }

    if(u32Events & TSP_TASK_EVENT_MEET_MIDDLE_POLL)
    {
        *pu32Evt = TSP_TASK_EVENT_MEET_MIDDLE_POLL;
        *pRet = (_TSP_Isr_Proc_FileIn(E_DRVTSP_EVENT_CB_POLL, TSP_TASK_EVENT_MEET_MIDDLE_POLL))?DRVTSP_OK:DRVTSP_FAIL;
    }
/*
    for (i = 0; i < TSP_ENGINE_NUM; i++)
    {
        MS_U32                  u32SecFltId;

        REG_PidFlt *pPidFilter = &_TspPid[i].Flt[u32FltId];
        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);
        u32Flag = (1<< u32SecFltId);

        #if 0
        MS_U32                  j = 0;
        MS_U32 u32Dummy;

        for (j = 0 ; j < TSP_PIDFLT_NUM ; j++)
        {
            if (_TSP_GetFltDisableId(i, (1 << j), u32Dummy, OSAL_EVENT_OR_CLEAR))
            {
                _TSP_PidFlt_Disable(i, j);
            }
        }
        #endif

        *pRet = (_TSP_Isr_Proc_SecRdyCrc(i, u32SecFltId, E_DRVTSP_EVENT_CB_POLL, pu32Evt))? DRVTSP_OK: DRVTSP_FAIL;
        if (DRVTSP_FAIL == *pRet)
        {
            if (_TSP_GetSecOvfId(i, u32Flag, u32SecOvf, OSAL_EVENT_OR))
            {
                *pRet = (_TSP_Isr_Proc_SecOvf(i, u32SecFltId, E_DRVTSP_EVENT_CB_POLL, pu32Evt))? DRVTSP_OK: DRVTSP_FAIL;
            }
        }
    }
*/
}


static void _TSP_Isr_Proc_Task(OSAL_TSP_ProcTypeParam* pDummy, MS_U32* pu32Dummy)
{
    TSP_Result          Ret;
    OSAL_TSP_ProcTypeParam Data;

    Data.u32FltId = 0xFFFFFFFF;
    Data.pRet = (void*)&Ret;
    Data.bInfiniteWait = TRUE;
    Data.bFromTask = TRUE;

    while (1)
    {
        _TSP_EXT_LOCK();
        if (_db_flag == 0)
        {
            _db_flag = 1;
            _TSP_EXT_UNLOCK();
            break;
        }
        _TSP_EXT_UNLOCK();
        _TSP_Isr_ProcSelf(&Data);
    } // Task while loop
}

TSP_Result _TSP_Init(MS_BOOL bResetSwState, MS_BOOL bRestoreRegState)
{
    MS_U32 u32Bank, u32BankSize;
    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_TSP))
    {
        TSP_DBG("MDrv_MMIO_GetBASE fail\n");
        MS_ASSERT(0);
    }

    HAL_TSP_SetBank(u32Bank);
    HAL_TSP_PowerCtrl(ENABLE);
    HAL_TSP_Reset(0);
    // Diable interrupt
    HAL_TSP_Int_Disable(TSP_INT_TRIGGER_MASK);

    // IntDisable
    // IntDetach
    OSAL_TSP_IntDisable();
#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntDetach();
#endif

    //HAL_TSP_Stc_ctrl();
    // Disable CPU
    HAL_TSP_SetCtrlMode(0, 0x0, TSP_IF_NUM);  // Disable TSP0(Reset CPU), Software_Reset

    _TSP_CmdQ_Init();
    HAL_TSP_WbDmaEnable(TSP_PATH_MAIN, TRUE);
    HAL_TSP_WbDmaEnable(TSP_PATH_SECOND, TRUE);
    //HAL_TSP_Scmb_Detect(TRUE);

    if (bResetSwState)
    {
        _TSP_SwStateReset(FALSE);
    }


    if (bRestoreRegState)
    {
        _TSP_RegStateRestore();
    }
    else
    {
        _TSP_RegStateReset();
    }


    //TSP_DBG("[%s][%d]Reset CMDQ\n",__FUNCTION__,__LINE__);
    if(!HAL_TSP_CmdQ_Reset(TSP_PATH_MAIN) ||
        !HAL_TSP_CmdQ_Reset(TSP_PATH_SECOND))
        return DRVTSP_FAIL;

    // Synthesizer Tuning
    // For difference : 100 - 4000 STC
    // Step: 10 STC
    // HAL_TSP_SetFwMsg(0x03A4); // Catch up n*10 STC in 1/20 sec

    HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU

    //TSP_DBG ("**************************DDD TSP protect miu00000000000000000\n");
    // XXX: Jericho: temp solution to prevent TSP write memory
    //HAL_TSP_OrzWriteProtect_Enable(TRUE, 0, 0);
    HAL_Reset_WB();
    HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU

    HAL_TSP_Int_ClearHw(TSP_INT_TRIGGER_MASK);

    // Implicity MDrv_TSP_Reset()
    // Set TsDma delay, //jyliu.tsp, how come the value?
    HAL_TSP_TsDma_SetDelay(TSP_PATH_MAIN, 0x0A);
    HAL_TSP_TsDma_SetDelay(TSP_PATH_SECOND, 0x0A);

    // for backward compatible, set both video and audio to main path
    eAudioPath = eVideoPath = E_DRVTSP_PATH_MAIN;
    HAL_TSP_SetPath(TSP_PATH_MAIN, TSP_PATH_MAIN);

    // STC control by TSP need indirect R/W enable
    HAL_TSP_Stc_ctrl();

    OSAL_TSP_IntAttach(_TSP_Isr);
#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntEnable();
#endif

    HAL_TSP_Int_Enable( TSP_INT_MUI_MEET_TAIL       |
                        TSP_INT_MIU_MEET_THRESHOLD  |
                        TSP_INT_MIU_MEET_MIDDLE     |
                        TSP_INT_PROTECT_ZONE_EXCEED |
                        TSP_INT_PCR_UPDATE_DONE     |
                        TSP_INT_FILEIN_READ_DONE_P1 |
                        TSP_INT_FILEIN_READ_DONE_P2);                                     // Enable TSP hardware interrupt

    if (_bSWInitReady)
    {
        return DRVTSP_OK;
    }

    _CmdEvtNotify = E_DRVTSP_EVENT_DATA_INIT;
    _CmdCallback = NULL;
    _RecEvtNotify = E_DRVTSP_EVENT_DATA_INIT;
    _RecCallback = NULL;

    _s32EventFree1 = OSAL_TSP_EventCreate((MS_U8*)"FreeList1");
    _s32EventFree0 = OSAL_TSP_EventCreate((MS_U8*)"FreeList0");
    _s32EventFreeDefer1 = OSAL_TSP_EventCreate((MS_U8*)"FreeListDefer1");
    _s32EventFreeDefer0 = OSAL_TSP_EventCreate((MS_U8*)"FreeListDefer0");
    _s32TspMutexId = OSAL_TSP_MutexCreate((MS_U8*)"TSP_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _s32IndMutexId = OSAL_TSP_MutexCreate((MS_U8*)"TSP_Ind_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _s32ExtMutexId = OSAL_TSP_MutexCreate((MS_U8*)"TSP_Ext_Mutex");
    _s32TspEventId = OSAL_TSP_EventCreate((MS_U8*)"TSP_Event"); // MsOS_CreateEventGroup("TSP_Event");
    _s32TspSecRdyId[0] = OSAL_TSP_EventCreate((MS_U8*)"TSP_SecRdy");
    _s32TspSecOvfId[0] = OSAL_TSP_EventCreate((MS_U8*)"TSP_SecOvf");
    _s32TspSecCrcId[0] = OSAL_TSP_EventCreate((MS_U8*)"TSP_SecCrc");
    _s32TspFltDisableId[0] = OSAL_TSP_EventCreate((MS_U8*)"TSP_FltDisable");
    _s32TspTaskId = OSAL_TSP_TaskCreate(_TSP_Isr_Proc_Task, (MS_U8*)"TSP_ISR_Task");
    _bSWInitReady = TRUE;
    _TspState[0].TSPStatus = E_DRVTSP_SW_INIT;
    _u32LastErr = DRVTSP_OK;

#ifdef MSOS_TYPE_LINUX_KERNEL
    int i;
    for (i= 0; i< TSP_FLT_NUM_ALL; i++)
    {
        init_waitqueue_head(&tsp_wq_head[i]);
    }
#endif
    return (TSP_Result)_u32LastErr;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Poweroff TSP driver and TSP engine
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PowerOff(void)
{
    _TSP_ENTRY();
    _TSP_Close();
    _TspState[0].TSPStatus = E_DRVTSP_SW_POWEROFF;
    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Relsase TSP indirect register read/write semaphone
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_HW_Lock_Release(void)
{
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Close TSP driver and reset software/register state
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Exit(void)
{
    MS_U8 i;

    _TSP_ENTRY();
    for (i = 0; i < TSP_ENGINE_NUM; i++)
    {
        _TspState[i].TSPStatus = E_DRVTSP_SW_EXIT;
    }
    _TSP_RegStateReset();
    _TSP_SwStateReset(HAL_TSP_Alive());
    _TSP_Close();

    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Suspend TSP driver
/// @return TSP_Result
/// @note
/// Save TSP driver states to DRAM
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Suspend(void)
{
    _TSP_ENTRY();
    _TspState[0].TSPStatus = E_DRVTSP_SW_SUSPEND;
    _bSuspend = TRUE;
    _TSP_RegStateStore();
    _TSP_Close();
    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Resume TSP driver
/// @return TSP_Result
/// @note
/// Restore TSP driver states from DRAM
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Resume(MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
    // Reset TSP register
    HAL_ResetAll();
    MsOS_DelayTask(500);

    TSP_Result ret;
    _bSuspend = FALSE;
    _TSP_ENTRY();
    _TspState[0].TSPStatus = E_DRVTSP_SW_RESUME;
    ret = _TSP_Init(FALSE, TRUE);
    _TSP_RETURN(ret);
}


// Misc API
//-------------------------------------------------------------------------------------------------
/// Set TSP engine operating mode
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eCtrlMode               \b IN: TSP engine operating mode
/// @return TSP_Result
/// @note
/// Only one engine can be set to use memory stream input mode, and all kinds
/// of input are mutually exclusive for each engine.\n
/// Only one engine can output to MAD. The TSP engine which is not selected to
/// MAD can not filter any audio stream, it will block the stream.
/// @sa MDrv_TSP_PidFlt_Alloc, MDrv_TSP_PidFlt_Enable, E_DRVTSP_FLT_TYPE_AUDIO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, DrvTSP_CtrlMode eCtrlMode)      // Set TSP internal operate
{
    _TSP_ENTRY();

    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    if ((HAL_TSP_GetCtrlMode(u32EngId) & TSP_CTRL_TSFILE_EN)||
        (eCtrlMode == E_DRVTSP_CTRL_MODE_MEM_AUD)        ||
        (eCtrlMode == E_DRVTSP_CTRL_MODE_VID)            ||
        (eCtrlMode == E_DRVTSP_CTRL_MODE_MEM))
    {
        if (!HAL_TSP_CmdQ_IsEmpty())
        {
            _TSP_RETURN(DRVTSP_FAIL);
        }
        HAL_TSP_CmdQ_TsDma_Reset();
    }

    switch (eCtrlMode)
    {
    case E_DRVTSP_CTRL_MODE_TS0_AUD:
        HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS0:
        HAL_TSP_PS_Path_Disable();
//        HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE); //|TSP_CA_AVPAUSE);
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN, 0);
        break;
    case E_DRVTSP_CTRL_MODE_TS1_AUD:
//        HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS1:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN, 1);
        break;
    case E_DRVTSP_CTRL_MODE_MEM_AUD:
        HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_MEM_PVR:
    case E_DRVTSP_CTRL_MODE_MEM:
        HAL_TSP_PS_Path_Disable();
//        HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_FILEIN|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_AVPAUSE);
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_TSFILE_EN, TSP_IF_NUM);
        break;
    case E_DRVTSP_CTRL_MODE_VID:
//        HAL_TSP_CSA_Set_ScrmPath(0);
        break;
    case E_DRVTSP_CTRL_MODE_PVR_TS0:;
        break;
#ifdef TS2_IF_SUPPORT
	case E_DRVTSP_CTRL_MODE_TS2_AUD:
        HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS2:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN, 2);
        break;
#endif
    default:
        _TSP_RETURN(DRVTSP_FAIL);
        break;
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo or not
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_AVFifo_Reset(DrvTSP_FltType eFilterType, MS_BOOL bFlush)
{
    _TSP_ENTRY();
    switch (eFilterType)
    {
    case E_DRVTSP_FLT_TYPE_VIDEO:
        HAL_TSP_Flush_AV_FIFO(0, bFlush);
        break;
    case E_DRVTSP_FLT_TYPE_AUDIO:
        HAL_TSP_Flush_AV_FIFO(1, bFlush);
        break;
    default:
        _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_AVFifo_Status(DrvTSP_FltType eFilterType, MS_U32 *u32FifoLevel)
{
    _TSP_ENTRY();
    switch (eFilterType)
    {
    case E_DRVTSP_FLT_TYPE_VIDEO:
        *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(0);
        break;
    case E_DRVTSP_FLT_TYPE_AUDIO:
        *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(1);
        break;
    default:
        _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Last General Error Return Code
/// @return TSP_Result
/// @note
/// Last error return code is reset after calling this function.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetLastErr(void)
{
    TSP_Result ret;
    ret = (TSP_Result) _u32LastErr;
    _u32LastErr = DRVTSP_OK;
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Get TSP Flt states
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  peState                 \b OUT: Flter status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_GetState(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_PIDFLT_NUM> u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %ld\n", __LINE__, u32FltId));
    if (_TSP_Flt_StateGet(0, u32FltId, peState))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
// Ask TSP to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Proc(MS_U32 u32EngId, MS_U32 u32FltId, TSP_Result* pRet, DrvTSP_Event* pEvt)                        // for non-OS TSP scheduling
{
    OSAL_TSP_ProcTypeParam Data;
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_PIDFLT_NUM > u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %ld\n", __LINE__, u32FltId));
    if (_TspState[u32EngId].FltState[u32FltId] == E_DRVTSP_FLT_STATE_NA)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    Data.u32FltId = u32FltId;
    Data.pRet = (void*)pRet;
    Data.bInfiniteWait = FALSE;
    Data.bFromTask = FALSE;

    _TSP_IND_UNLOCK();
    OSAL_TSP_Proc(_TSP_Isr_ProcPoll, &Data, (MS_U32*)pEvt);
    _TSP_IND_LOCK();

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC)
{
    TSP_ASSERT((pu32STC_32 && pu32STC), TSP_DBG("[TSP_ERROR][%06d] null pointer\n", __LINE__));

    _TSP_ENTRY();
    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    *pu32STC = HAL_TSP_GetSTC(u32EngId);
    *pu32STC_32 = HAL_TSP_GetSTC_32(u32EngId);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get PCR of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32Pcr_32              \b OUT: pointer of STC[32] return
/// @param  pu32Pcr                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr)
{
    TSP_ASSERT((pu32Pcr_32 && pu32Pcr), TSP_DBG("[TSP_ERROR][%06d] null pointer\n", __LINE__));

    _TSP_ENTRY();
    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    if (HAL_TSP_GetPcr(u32EngId, pu32Pcr_32, pu32Pcr))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_FAIL);
}


// PIDFlt API
//-------------------------------------------------------------------------------------------------
/// Allocate a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: type of PID filter to be allocated
/// @param  pu32PidFltId            \b OUT: pointer of PID filter id return
/// @return TSP_Result
/// @note
/// These filter types have to select a section filter\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION\n
/// @ref E_DRVTSP_FLT_TYPE_PCR\n
/// @ref E_DRVTSP_FLT_TYPE_PES\n
/// @note
/// These filter types also have to setup section buffer for data output\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION\n
/// @ref E_DRVTSP_FLT_TYPE_PES\n
/// @sa MDrv_TSP_PidFlt_SelSecFlt, MDrv_TSP_SecFlt_SetBuffer
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId)
{
    DrvTSP_FltType eTypeOnly = eFilterType & ~(E_DRVTSP_FLT_MASK);
    TSP_ASSERT(pu32PidFltId, TSP_DBG("[TSP_ERROR][%06d] null pointer\n", __LINE__));
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    //printf("[%s] eFilterType = %lu\n", __FUNCTION__, (MS_U32)eFilterType);
    if((eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK) != E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] File in mode only\n", __LINE__));
        printf("return 1\n");
        return DRVTSP_FAIL;
    }
    if( (E_DRVTSP_FLT_TYPE_AUDIO2 == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_PVR == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_SECTION == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_PACKET == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_TELETEXT == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_EMM == eTypeOnly) ||
        (E_DRVTSP_FLT_TYPE_ECM == eTypeOnly))
    {
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] not supported filter type %08ld\n", __LINE__, eFilterType));
        return DRVTSP_FAIL;
    }
    _TSP_ENTRY();

    *pu32PidFltId=      0xFFFFFFFF;
    if (!_TSP_PidFlt_Alloc(u32EngId, eFilterType, pu32PidFltId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate Pid Filter fail\n", __LINE__));
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _TSP_PidFlt_Init(u32EngId, *pu32PidFltId);
    //_TspState[u32EndId].FltMap[*pu32PidFltId] = *pu32PidFltId;

    // clear all event while filter allocated
    MS_U32 u32Dummy;
    _TSP_GetSecOvfId    (u32EngId, (1 << *pu32PidFltId), u32Dummy, OSAL_EVENT_OR_CLEAR);
    _TSP_GetSecCrcId    (u32EngId, (1 << *pu32PidFltId), u32Dummy, OSAL_EVENT_OR_CLEAR);
    _TSP_GetSecRdyId    (u32EngId, (1 << *pu32PidFltId), u32Dummy, OSAL_EVENT_OR_CLEAR);
    _TSP_GetFltDisableId(u32EngId, (1 << *pu32PidFltId), u32Dummy, OSAL_EVENT_OR_CLEAR);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Free a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    DrvTSP_FltType eFilterType;
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT(TSP_FLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));
    TSP_ASSERT(E_DRVTSP_FLT_STATE_FREE != _TspState[u32EngId].FltState[u32PidFltId],
                TSP_DBG("[TSP_ERROR][%06d] PidFlt has already free\n", __LINE__));
    _TSP_ENTRY();

    _TSP_IND_UNLOCK();

    if ((_TspState[u32EngId].FltEvtNotify[u32PidFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_POLL)
    {
        eFilterType = _TspState[u32EngId].FltType[u32PidFltId];
        if ((E_DRVTSP_FLT_TYPE_PCR == eFilterType))
        {
            //_TspState[u32EngId].SecFltState[u32PidFltId] = E_DRVTSP_FLT_STATE_FREE;
            _TspState[u32EngId].FltEvtNotify[u32PidFltId] = E_DRVTSP_EVENT_DATA_INIT;
        }
        _TSP_PidFlt_Free(u32EngId, u32PidFltId, TRUE);
    }
    else if ((_TspState[u32EngId].FltEvtNotify[u32PidFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
    {
        if (E_DRVTSP_FLT_STATE_FREE == _TspState[u32EngId].FltState[u32PidFltId])
        {
            _TSP_IND_LOCK();
            _TSP_RETURN(DRVTSP_OK);
        }

        if (_TSP_IsPidFltFreeDefer(u32PidFltId))
        {
            if (!_TSP_IsAskPidFltFree(u32PidFltId))
            {
                _TSP_ResetPidFltFreeDefer(u32PidFltId);
                goto goto_free_ok;
            }
        }

        if (!_TSP_IsAskPidFltFree(u32PidFltId))
        {
            _TSP_AskPidFltFree(u32PidFltId);
        }
        _TSP_SetEvent(TSP_TASK_EVENT_FLT_FREE);
        // TSP_DBG("[%s][%d] Wait filter free %d\n", __FUNCTION__, __LINE__, (int)u32PidFltId);
        MsOS_DelayTask(4);
        if (_TSP_IsAskPidFltFree(u32PidFltId))
        {
            if (E_DRVTSP_FLT_TYPE_PVR == _TspState[u32EngId].FltType[u32PidFltId])
            {
                goto goto_free_ok;
            }
            _TSP_SetPidFltFreeDefer(u32PidFltId);
            TSP_DBG("[%s][%d] %d 0x%08x\n", __FUNCTION__, __LINE__, (int)u32PidFltId, (int)_TspState[u32EngId].FltState[u32PidFltId]);
            _TSP_IND_LOCK();
            _TSP_RETURN(DRVTSP_FAIL);
        }
goto_free_ok:
        _TspState[u32EngId].FltState[u32PidFltId] = E_DRVTSP_FLT_STATE_FREE;

    }
    _TSP_IND_LOCK();

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  u32PID                  \b IN: PID value
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32PID)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

    if(_PCRFLT_ == u32PidFltId)
    {
        HAL_TSP_PidFlt_SetPid(_PFLT_PCR, u32PID);
    }
    else
    {
        HAL_TSP_PidFlt_SetPid(&(_TspPid[u32EngId].Flt[u32PidFltId]), u32PID);
    }
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  pu32PID                 \b OUT: the output PID
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_GetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32* pu32PID)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));
    //TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));
    if(_PCRFLT_ == u32PidFltId)
    {
        *pu32PID = HAL_TSP_PidFlt_GetPid(_PFLT_PCR);
    }
    else
    {
        *pu32PID = HAL_TSP_PidFlt_GetPid(&(_TspPid[u32EngId].Flt[u32PidFltId]));
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of filter to be enable
/// @param  bEnable                 \b IN: TRUE(enable), FALSE(disable)
/// @return TSP_Result
/// @note
/// When PID filter enable, the section buffer pointer will be reset to buffer start address,
/// overflow condition will be resolved if exist.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Enable(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bEnable)
{
    REG_PidFlt*         pPidFilter= NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

    if(_PCRFLT_ == u32PidFltId)
    {
        pPidFilter=         _PFLT_PCR;
    }
    else
    {
        pPidFilter=         &(_TspPid[u32EngId].Flt[u32PidFltId]);
    }

    if (!bEnable)
    {
        if (!HAS_FLAG(_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
        {
             TSP_DBG("[TSP_WARNNING][%06d] disable an inactive PidFlt %ld\n", __LINE__, u32PidFltId);
            _TSP_RETURN(DRVTSP_OK);
        }
        _TSP_PidFlt_Disable(u32EngId, u32PidFltId);
        _TSP_RETURN(DRVTSP_OK);
    }

    if (HAS_FLAG(_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
    {
        // TSP_DBG("[TSP_WARNNING][%06d] Enable an active PidFlt %d\n", __LINE__, u32PidFltId);
        _TSP_RETURN(DRVTSP_OK);
    }

    if (E_DRVTSP_FLT_TYPE_VIDEO == _TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_EN_VIDEO);
    }
    else if (E_DRVTSP_FLT_TYPE_AUDIO == _TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_EN_AUDIO);
    }
    else if(E_DRVTSP_FLT_TYPE_PCR == _TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PCR_FLT_EN_PCR);
    }
    else if(E_DRVTSP_FLT_TYPE_PES == _TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_EN_RECORD);
        HAL_TSP_PVR_Enable(TRUE);
    }
    else
    {
        TSP_DBG("[TSP_ERROR] Invalid filter type %ld\n", _TspState[u32EngId].FltType[u32PidFltId]);
    }
    /*
    else
    {
        REG_SecFlt*     pSecFilter = NULL;
        MS_U32          u32SecFltId;

        REG_SecFlt*     pSecBufFilter = NULL;
        MS_U32          u32SecBufId;
        TSP_ASSERT2(TSP_PIDFLT_NUM> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));

        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);
        TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] SecFltId= %ld\n", __LINE__, u32SecFltId));
        pSecFilter=     &(_TspSec[u32EngId].Flt[u32SecFltId]);
        u32SecBufId = HAL_TSP_SecFlt_GetSecBuf(pSecFilter);

        switch (_TspState[u32EngId].FltType[u32PidFltId])
        {
        case E_DRVTSP_FLT_TYPE_SECTION:
        case E_DRVTSP_FLT_TYPE_PES:
        case E_DRVTSP_FLT_TYPE_PACKET:
        case E_DRVTSP_FLT_TYPE_TELETEXT:
            TSP_ASSERT2(TSP_SECBUF_NUM> u32SecBufId, TSP_DBG("[TSP_ERROR][%06d] SecBufId= %ld\n", __LINE__, u32SecBufId));
            pSecBufFilter=  &(_TspSec[u32EngId].Flt[u32SecBufId]);
            HAL_TSP_SecBuf_ResetBuffer(pSecBufFilter);
            HAL_TSP_SecFlt_ResetState(pSecFilter);
            HAL_TSP_SecFlt_SetRmnCount(pSecFilter, 0);
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT);
            break;
        case E_DRVTSP_FLT_TYPE_EMM:
        case E_DRVTSP_FLT_TYPE_ECM:
            HAL_TSP_SecFlt_ResetState(pSecFilter);
            HAL_TSP_SecFlt_SetRmnCount(pSecFilter, 0);
            HAL_TSP_SecBuf_ResetBuffer(pSecFilter);
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT);
            break;
        case E_DRVTSP_FLT_TYPE_PCR:
            HAL_TSP_SecFlt_PcrReset(pSecFilter);
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT | TSP_PIDFLT_OUT_SECFLT_AF);
            break;
        default:
            break;
        }
    }
    */


    _TspState[u32EngId].FltState[u32PidFltId]|= E_DRVTSP_FLT_STATE_ENABLE;
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current PID filter status
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter
/// @param  peState                 \b OUT: current ORed state flag of PID filter\n
///                                         E_DRVTSP_FLT_STATE_ALLOC\n
///                                         E_DRVTSP_FLT_STATE_ENABLE\n
///                                         E_DRVTSP_FLT_STATE_SCRAMBLED ([VENUS]: TS level scramble status)
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_GetState(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %ld\n", __LINE__, u32PidFltId));
    if (_TSP_PidFlt_StateGet(u32EngId, u32PidFltId, peState))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

// SecFlt API
// for backward compatible
//-------------------------------------------------------------------------------------------------
/// Set buffer start address and buffer size to section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be set
/// @param  u32StartAddr            \b IN: start address of section buffer
/// @param  u32BufSize              \b IN: size of section buffer
/// @return TSP_Result
/// @note
/// Buffer start address and buffer size should be 128-bit (16-byte) aligned.\n
/// @sa MDrv_TSP_PidFlt_Alloc
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetBuffer(MS_U32 u32EngId, MS_U32 u32FltId, MS_PHYADDR u32StartAddr, MS_U32 u32BufSize)
{
    // buffer address should be 8 - byte alignment
    TSP_ASSERT(!((u32StartAddr & 0x7) || ((u32StartAddr + u32BufSize) & 0x7)),
            TSP_DBG("[TSP_ERROR][%06d] Illegal start address\n", __LINE__));
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));

    HAL_TSP_PVR_SetBuffer(u32StartAddr, u32BufSize);

    // flush PVR buffer
    HAL_TSP_PVR_WaitFlush();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Set section filtering mode
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: section filter of TSP to be free
// @param  eSecFltMode             \b IN: continue/one-shot mode of section filter
// @return TSP_Result
// @attention
// One-shot filter has the disadvantage of interrupt lost becuase it stops filter, a timeout
// to check filter status is better for usage.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetMode(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltMode eSecFltMode)
{
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT(TSP_FLT_NUM_ALL> u32SecFltId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));

    _TspState[u32EngId].FltMode[u32SecFltId] = eSecFltMode;
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for specified section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_DRVTSP_EVENT_DATA_READY\n
///                                        @ref E_DRVTSP_EVENT_BUF_OVERFLOW
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// This function register a callback function for a section filter to TSP.
/// TSP calls callback function each time when data is ready in section buffer.\n
/// Data ready of section filter:\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION : a section ready\n
/// @ref E_DRVTSP_FLT_TYPE_PES : PES packet ready or received data over than request size.
/// @sa MDrv_TSP_SecFlt_SetReqCount
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Notify(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32SecFltId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));

	if ((eEvents & (E_DRVTSP_EVENT_DATA_READY   |
                    E_DRVTSP_EVENT_BUF_OVERFLOW  )) == 0)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _TspState[u32EngId].FltEvtNotify[u32SecFltId] = eEvents;
    _TspState[u32EngId].FltCallback[u32SecFltId]  = pfCallback;

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Get current section filter status
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: index of section filter
// @param  peState                 \b OUT: current ORed state flag of section filter
//                                         E_DRVTSP_FLT_STATE_ALLOC\n
//                                         E_DRVTSP_FLT_STATE_ENABLE\n
//                                         E_DRVTSP_FLT_STATE_OVERFLOW
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetState(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));
    TSP_ASSERT2(peState, OSAL_TSP_Print("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *peState = _TspState[u32EngId].FltState[u32SecFltId];
     _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Free a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: section filter of TSP to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Free(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));
    TSP_ASSERT2((E_DRVTSP_FLT_STATE_ALLOC == _TspState[u32EngId].SecBufState[u32SecFltId]),
    TSP_DBG("[TSP_ERROR][%06d] Free a non-allocated filter %ld\n", __LINE__, u32SecFltId));

    _TspState[u32EngId].SecBufState[u32SecFltId] = E_DRVTSP_FLT_STATE_FREE;
    MDrv_TSP_SecFlt_SetBuffer(0, u32SecFltId, 0xFFFFFFF8, 0x0);
    MDrv_TSP_SetThreshold(FALSE, 0x0);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32SecFltId            \b OUT: pointer of section filter id return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Alloc(MS_U32 u32EngId, MS_U32 *pu32SecFltId)
{
    MS_U32  i=0;
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM > u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM > *pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, *pu32SecFltId));

    for(i=0;i<TSP_FLT_NUM_ALL;i++)
    {
        if(E_DRVTSP_FLT_STATE_FREE != _TspState[u32EngId].SecBufState[i])
            break;
    }

    if(TSP_FLT_NUM_ALL != i)
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] no free section buffer\n", __LINE__));
        _TSP_RETURN(DRVTSP_FAIL);
    }

    MDrv_TSP_SecFlt_SetBuffer(0, *pu32SecFltId, 0xFFFFFFF8, 0x0);
    _TspState[u32EngId].SecBufState[*pu32SecFltId] = E_DRVTSP_FLT_STATE_ALLOC;
    _TSP_RETURN(DRVTSP_OK);
}
void MDrv_TSP_SecFlt_Update(MS_U32 u32EngId, MS_U32 u32SecFltId)
{

}
//-------------------------------------------------------------------------------------------------
/// Get current section data write address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32WriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHYADDR *pu32WriteAddr)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_FLT_NUM_ALL> u32SecFltId, OSAL_TSP_Print("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));
    TSP_ASSERT2(pu32WriteAddr, OSAL_TSP_Print("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _TspState[u32EngId].FltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    *pu32WriteAddr= (MS_PHYADDR)HAL_TSP_PVR_GetBufWrite();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get buffer start/end address
/// @param  pu32BufStart           \b IN: pointer to address of buffer start
/// @param  pu32BufEnd             \b IN: pointer to address of buffer end
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetBuffer(MS_PHYADDR *phyBufStart, MS_PHYADDR *phyBufEnd)
{
    _TSP_ENTRY();
    HAL_TSP_PVR_GetBuffer(phyBufStart, phyBufEnd);
    _TSP_RETURN(DRVTSP_OK);
}
TSP_Result MDrv_TSP_SetThreshold(MS_BOOL bEnable, MS_PHYADDR phyThreshold)
{
    _TSP_ENTRY();
    if(bEnable)
    {
        HAL_TSP_SetThreshold(phyThreshold);
        HAL_TSP_Int_Enable(TSP_INT_MIU_MEET_THRESHOLD);
    }
    else
    {
        HAL_TSP_SetThreshold(0x0);
        HAL_TSP_Int_Disable(TSP_INT_MIU_MEET_THRESHOLD);
    }
    _TSP_RETURN(DRVTSP_OK);
}
TSP_Result MDrv_TSP_SetMiddle(MS_BOOL bEnable, MS_PHYADDR phyMiddle)
{
    _TSP_ENTRY();
    if(bEnable)
    {
        HAL_TSP_SetMiddle(phyMiddle);
        HAL_TSP_Int_Enable(TSP_INT_MIU_MEET_MIDDLE);
    }
    else
    {
        HAL_TSP_SetMiddle(0x0);
        HAL_TSP_Int_Disable(TSP_INT_MIU_MEET_MIDDLE);
    }
    _TSP_RETURN(DRVTSP_OK);
}
// File API
//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream start address
/// @param  path                    \b IN: path select
/// @param  u32StreamAddr           \b IN: pointer of transport stream in memory
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetAddr(DrvTSP_FileinMode eFileinMode, MS_U32 u32StreamAddr)
{
    TSP_Path path = TSP_PATH_MAIN;
    switch (eFileinMode)
    {
        case E_DRVTSP_FILEIN_MODE_ENG0_VPES:
        case E_DRVTSP_FILEIN_MODE_ENG0_VIDEO_TS:
            path = (TSP_Path)eVideoPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_APES:
        case E_DRVTSP_FILEIN_MODE_ENG0_AUDIO_TS:
            path = (TSP_Path)eAudioPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_TS:
            path = _RECORD_PATH;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_A2PES:
            TSP_DBG("unsupported File in mode E_DRVTSP_FILEIN_MODE_ENG0_A2PES\n");
            return DRVTSP_FAIL;
        default:
            TSP_DBG("unknown File in mode %ul\n", eFileinMode);
            return DRVTSP_FAIL;
    }
    _TSP_ENTRY();
    HAL_TSP_CmdQ_TsDma_SetAddr(path, u32StreamAddr);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream data size
/// @param  path                    \b IN: path select
/// @param  u32StreamSize           \b IN: size of transport stream data to be copied
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetSize(DrvTSP_FileinMode eFileinMode, MS_U32 u32StreamSize)
{
    TSP_Path path = TSP_PATH_MAIN;
    if( // t2 HW limite, CMQ had some problem while bufsize < 16
        (u32StreamSize < 8))
    return DRVTSP_FAIL;

    switch (eFileinMode)
    {
        case E_DRVTSP_FILEIN_MODE_ENG0_VPES:
        case E_DRVTSP_FILEIN_MODE_ENG0_VIDEO_TS:
            path = (TSP_Path)eVideoPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_APES:
        case E_DRVTSP_FILEIN_MODE_ENG0_AUDIO_TS:
            path = (TSP_Path)eAudioPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_TS:
            path = _RECORD_PATH;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_A2PES:
            TSP_DBG("unsupported File in mode E_DRVTSP_FILEIN_MODE_ENG0_A2PES\n");
            return DRVTSP_FAIL;
        default:
            TSP_DBG("unknown File in mode %ul\n", eFileinMode);
            return DRVTSP_FAIL;
    }

    _TSP_ENTRY();
    if (HAL_TSP_CmdQ_TsDma_SetSize((TSP_Path)path, u32StreamSize))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}


//-------------------------------------------------------------------------------------------------
//[Reserved]
// Memory to TSP stream command : Update Stream STC
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32STC_32               \b IN: STC[32] for next input stream
// @param  u32STC                  \b IN: STC[31:0] for next input stream
// @return TSP_Result
// @note M2T Command Size: 3
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC)
{
    _TSP_ENTRY();
    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    //[HW TODO][HW LIMIT]
    // STC register mapping is different between MCU and CPU.
    //printf("[%s] u32STC = %ld, u32STC_32 = %ld\n", __FUNCTION__, (unsigned int)u32STC, (unsigned int)u32STC_32);
    HAL_TSP_SetSTC(u32EngId, u32STC);
    HAL_TSP_SetSTC_32(u32EngId, u32STC_32);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Start stream input
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
/// @note Filein Command Size: 1
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Start(DrvTSP_FileinMode eFileinMode)
{
    TSP_Path path = TSP_PATH_MAIN;
    Pid_Bypass eByPass = NO_BYPASSS;

    switch (eFileinMode)
    {
        case E_DRVTSP_FILEIN_MODE_ENG0_VPES:
            eByPass = VIDEO_BYPASS;
            path = (TSP_Path)eVideoPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_VIDEO_TS:
            path = (TSP_Path)eVideoPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_APES:
            eByPass = AUDIO_BYPASS;
            path = (TSP_Path)eAudioPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_AUDIO_TS:
            path = (TSP_Path)eAudioPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_TS:
            path = _RECORD_PATH;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_A2PES:
            TSP_DBG("unsupported File in mode E_DRVTSP_FILEIN_MODE_ENG0_A2PES\n");
            return DRVTSP_FAIL;
        default:
            TSP_DBG("unknown File in mode %ul\n", eFileinMode);
            return DRVTSP_FAIL;
    }
    _TSP_ENTRY();
    // @FIXME: Good to assume enum and register definition identical?
    //if (E_DRVTSP_FILEIN_MODE_ENG0_TS != eFileinMode)
    HAL_TSP_CmdQ_TsDma_Start(path, eByPass);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current file in state
/// @return DrvTSP_M2tStatus
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_GetState(DrvTSP_FileinMode eFileinMode, DrvTSP_FileinState* peFileinState)
{
    MS_U32 state, cmdcnt;
    TSP_Path path = TSP_PATH_MAIN;
    switch (eFileinMode)
    {
        case E_DRVTSP_FILEIN_MODE_ENG0_VPES:
        case E_DRVTSP_FILEIN_MODE_ENG0_VIDEO_TS:
            path = (TSP_Path)eVideoPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_APES:
        case E_DRVTSP_FILEIN_MODE_ENG0_A2PES:
        case E_DRVTSP_FILEIN_MODE_ENG0_AUDIO_TS:
            path = (TSP_Path)eAudioPath;
            break;
        case E_DRVTSP_FILEIN_MODE_ENG0_TS:
            path = _RECORD_PATH;
            break;
    }
    _TSP_ENTRY();

    state = HAL_TSP_CmdQ_TsDma_GetState(path);
    cmdcnt = HAL_TSP_CmdQ_CmdCount(path);

    if ((state & TSP_TSDMA_CTRL_START) || cmdcnt)
    {
        *peFileinState = E_DRVTSP_FILEIN_STATE_BUSY;
    }
    else
    {
        *peFileinState = E_DRVTSP_FILEIN_STATE_IDLE;
    }
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set Memory to TSP Stream Input Rate
/// @param  u32Div2                 \b IN: Divider of M2T stream input rate ([1 .. 31], default 10)
/// @return TSP_Result
/// @note
/// <b>input_rate = stream_rate / (u32Div2 * 2)</b>\n
/// @note
/// It's not recommend to change input rate at run-time, because it conflict with
/// the internal stream synchornization mechanism.
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetRate(MS_U32 u32Div2)
{
    if(u32Div2 > 256)
        return DRVTSP_FAIL;

    _TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.

    OSAL_TSP_IntDisable();
    // set main and second path at the same time
    HAL_TSP_TsDma_SetDelay(TSP_PATH_MAIN, u32Div2);
    HAL_TSP_TsDma_SetDelay(TSP_PATH_SECOND, u32Div2);
    OSAL_TSP_IntEnable();

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set packet size to TSP file
/// @param  PacketMode                 \b IN: Mode of TSP file packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetPacketMode(DrvTSP_PacketMode PKT_Mode)
{
    _TSP_ENTRY();

    switch (PKT_Mode)
    {
        case E_DRVTSP_PKTMODE_188:
            HAL_TSP_SetPKTSize(0xBB);
            break;
        case E_DRVTSP_PKTMODE_192:
            HAL_TSP_SetPKTSize(0xBF);
            break;
        case E_DRVTSP_PKTMODE_204:
            HAL_TSP_SetPKTSize(0xCB);
            break;
        default:
            break;
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the number of empty slot of M2T command
/// @param  pu32EmptySlot           \b OUT: pointer of empty slot count return
/// @return TSP_Result
/// @note
/// M2T is a command queue command, it can be queued by TSP when another M2T command
/// is executing by TSP. The queued commands will be executed by order (FIFO)
/// when previous M2T command execution is finished.
/// @note
/// User should call GetM2TSlot to make sure there is enough empty M2T command slot
/// before sending any M2T command. (Each command has different command size)
/// @sa MDrv_TSP_M2T_SetAddr, MDrv_TSP_M2T_SetSize, MDrv_TSP_M2T_Start,
//[Reserved]    MDrv_TSP_M2T_SetSTC
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_CMDQ_GetSlot(DrvTSP_Path path, MS_U32 *pu32EmptySlot)
{
    _TSP_ENTRY();
    // use SLOT - current maximum command of two TSP path
    *pu32EmptySlot = HAL_TSP_CmdQ_EmptyCount((TSP_Path)path);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset command queue
/// @return TSP_Result
/// @note
/// SW patch--wait command queue empty
/// wait HW ECO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_CMDQ_Reset(DrvTSP_Path path)
{
    MS_BOOL bRet;
    TSP_ASSERT((E_DRVTSP_PATH_MAIN == path) || (E_DRVTSP_PATH_SECOND == path),
            TSP_DBG("[TSP_ERROR][%06d] incorrec path 0x%x\n", __LINE__, (int)path));
    _TSP_ENTRY();
    bRet = HAL_TSP_CmdQ_Reset((TSP_Path)path);
    if (TRUE == bRet)
    {
         _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Enable timestamp block scheme
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable)
{
	_TSP_ENTRY();
	HAL_TSP_FileIn_192BlockScheme_En(bEnable);
	_TSP_RETURN(DRVTSP_OK);
}

// Legacy
//-------------------------------------------------------------------------------------------------
/// Get Scmb states
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pScmbLevel              \b IN: TS interface setting
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
/*
TSP_Result MDrv_TSP_Scmb_Status(MS_U32 u32EngId, DrvTSP_Scmb_Level* pScmbLevel)
{
    _TSP_ENTRY();
    *pScmbLevel=        E_DRVTSP_SCMB_NONE;

    if (HAL_TSP_Scmb_Status(TRUE, 0xFFFFFFFF) || HAL_TSP_Scmb_Status(FALSE, 0xFFFFFFFF))
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
    _TSP_RETURN(DRVTSP_OK);
}
*/

TSP_Result MDrv_TSP_GetCap(DrvTSP_Cap eCap, void* pOutput)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Cap = 0xFFFFFFFF;

    _TSP_ENTRY();
    switch (eCap)
    {
        case E_DRVTSP_CAP_FILTER_NUM:
            u32Cap = HAL_TSP_CAP_FILTER_NUM;
            break;
        case E_DRVTSP_CAP_FILTER_PVR_NUM:
            u32Cap = HAL_TSP_CAP_FILTER_PVR_NUM;
            break;
        case E_DRVTSP_CAP_PVR_ALIGN:
            u32Cap = HAL_TSP_CAP_PVR_ALIGN;
            break;
        default:
            break;
    }

    if (0xFFFFFFFF!= u32Cap)
    {
        bRet = HAL_TSP_GetCap(u32Cap, pOutput);
    }
    if (bRet)
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

TSP_Result MDrv_TSP_GetStatus(DrvTSP_HW_Status *HW_Status, DrvTSP_SW_Status *SW_Status)
{
    _TSP_ENTRY();
    if (HAL_TSP_Alive())
    {
        *HW_Status = E_DRVTSP_HW_ALIVE;
    }
    else
    {
        *HW_Status = E_DRVTSP_HW_DEALIVE;
    }
    *SW_Status = _TspState[0].TSPStatus;

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_SetDbgLevel(DrvTSP_DbgLevel DbgLevel)
{
    // caution : when setting debug level to E_DRVTSP_DBG_Release, MACRO TSP_ASSERT will not return
    _u32TSPDGBLevel = DbgLevel;
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSP driver version
/// @param <IN>        \b const MS_U8 **pVerString :
/// @param <RET>       \b   : when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVTSP_FAIL;
    }

    *ppVersion = &_drv_tsp_version;

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId)
{
    _TSP_ENTRY();
    if (HAL_TSP_Alive())
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}


// Following are 2 private function used to solve annoying interrupt thread disappearing problem when fork.
// Should not be exposed to general users.
MS_BOOL MDrv_TSP_CloseInterrupt(void)
{
    // IntDisable
    // IntDetach
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();

    return TRUE;

}

//-------------------------------------------------------------------------------------------------
/// Initialize TSP driver and TSP engine
/// @return TSP_Result
/// @note
/// It should be called before calling any other TSP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Init(MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
    TSP_DBG ("******************start TSP init*********************************\n");
    TSP_Result bRet;

    // for FPGA verify code, unknown reason that _bSuspend will default to TRUE
    _bSuspend = FALSE;
    // yiikai test end

    if (DRVTSP_OK != (bRet = _TSP_Init(TRUE, FALSE)))
    {
        return bRet;
    }
    TSP_DBG ("**********************End TSP init*****************************\n");
    return DRVTSP_OK;
}


TSP_Result MDrv_TSP_Reset(void)
{
    _TSP_RegStateReset();
    HAL_TSP_Reset(0);
    return DRVTSP_OK;
}

MS_BOOL MDrv_TSP_OpenInterrupt(void)
{
    OSAL_TSP_IntAttach(_TSP_Isr);
    OSAL_TSP_IntEnable();
    return TRUE;
}

TSP_Result MDrv_TSP_BurstLen(MS_U32 u32Len)
{
    TSP_ASSERT(!(u32Len & 0x3), TSP_DBG("[TSP_ERROR][%06d] incorrect burst length parameter\n", __LINE__));

    _TSP_ENTRY();
    HAL_TSP_SetDMABurstLen(u32Len);
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen)
{
    _TSP_ENTRY();
    *u32TSLen = HAL_TSP_GetFileInTimeStamp();
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_GetDbgPortInfo(MS_U32 u32DbgSel, MS_U32* u32DbgInfo)
{
    _TSP_ENTRY();
    *u32DbgInfo = HAL_TSP_GetDBGPortInfo(u32DbgSel);
    _TSP_RETURN(DRVTSP_OK);
}


TSP_Result MDrv_TSP_Get_Intr_Count(MS_U32* pu32Count)
{
    *pu32Count = _u32DbgIntrCount;
    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_SetFwDBGParam(MS_PHYADDR phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord)
{
    TSP_DBG("[%s] not supported\n", __FUNCTION__);
    return DRVTSP_FAIL;
}
//--------------------------------------------------------------------------------------------------
/// Get command queue fifo level
/// @param  pu8FifoLevel      \b OUT: fifo level, 0~3
/// @return TSP_Result
/// @note
//--------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel)
{
    _TSP_ENTRY();

	*pu8FifoLevel = HAL_TSP_Get_CmdQFifoLevel();

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Initial setting of audio path and video path in multiple path TSP
/// @param <IN>        \b DrvTSP_Path AudPath
/// @param <IN>        \b DrvTSP_Path VidPath
/// @param <RET>       \b   : DRVTSP_OK
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetPath(DrvTSP_Path AudPath, DrvTSP_Path VidPath)
{
    _TSP_ENTRY();
    eAudioPath = AudPath;
    eVideoPath = VidPath;
    if(HAL_TSP_SetPath((TSP_Path)AudPath, (TSP_Path)VidPath) == TRUE)
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

TSP_Result MDrv_TSP_GetPath(DrvTSP_Path* pAudPath, DrvTSP_Path* pVidPath)
{
    _TSP_ENTRY();
    *pAudPath = eAudioPath;
    *pVidPath = eVideoPath;
    _TSP_RETURN(DRVTSP_OK);
}


#if 0
TSP_Result MDrv_TSP_ResetFileinTimestamp(void)
{
    HAL_TSP_ResetTimeStamp();
    return DRVTSP_OK;
}
#endif


TSP_Result MDrv_TSP_PVR_Notify(DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback)
{
    TSP_DBG("[%s] not supported\n", __FUNCTION__);
    return DRVTSP_FAIL;
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets 
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    _TSP_ENTRY();
	HAL_TSP_RemoveDupAVPkt(bEnable);
	_TSP_RETURN(DRVTSP_OK);
}


