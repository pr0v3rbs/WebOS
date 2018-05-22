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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
/// file    drvTSP2.c
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "tsp2_msos.h"
#include "tsp2_nos.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#else
#include "string.h"
#endif

#if defined(MSOS_TYPE_LINUX)
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_firmware_io.h"
#endif

#include "drvMMIO.h"
#include "drvRASP.h"
#include "drvTSP2.h"

// Internal Definition
#include "asmCPU.h"
#include "regTSP.h"
#include "halTSP.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SWITCH_TSP_UART                             0

#define __LEGACY__                                  1 // only for development

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


#define FWTSP_CHECK_CODE        "<FWTSP>"

// Blocking events
#define TSP_SetEvent(flag)                          TSP_OS_EventSet(_s32EventId, (flag))
#define TSP_GetEvent(events, flag, opt, timeout)    TSP_OS_EventGet(_s32EventId, (flag), (events), (opt), (timeout))

// Filter event flags
#define TSP_FLAG_SetSecRdy(fltid)                   TSP_OS_EventSet(_s32SecRdyFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetSecRdy(fltid, events, opt)      TSP_OS_EventGet(_s32SecRdyFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define TSP_FLAG_ModeSecRdy(idx, mask,events, opt)  TSP_OS_EventGet(_s32SecRdyFlags[idx], mask, (events), (opt), 0)

#define TSP_FLAG_SetCrcErr(fltid)                   TSP_OS_EventSet(_s32CrcErrFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetCrcErr(fltid, events, opt)      TSP_OS_EventGet(_s32CrcErrFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define TSP_FLAG_ModeCrcErr(idx,mask ,events, opt)  TSP_OS_EventGet(_s32CrcErrFlags[idx], mask, (events), (opt), 0)

#define TSP_FLAG_SetSecOvf(fltid)                   TSP_OS_EventSet(_s32SecOvfFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetSecOvf(fltid, events, opt)      TSP_OS_EventGet(_s32SecOvfFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define TSP_FLAG_ModeSecOvf(idx,mask ,events, opt)  TSP_OS_EventGet(_s32SecOvfFlags[idx], mask, (events), (opt), 0)

#define TSP_FLAG_SetFltDis(fltid)                   TSP_OS_EventSet(_s32FltDisFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetFltDis(fltid, events, opt)      TSP_OS_EventGet(_s32FltDisFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define TSP_FLAG_ModeFltDis(idx, mask,events, opt)  TSP_OS_EventGet(_s32FltDisFlags[idx], mask, (events), (opt), 0)

#define TSP_FLAG_FREE_TIMEOUT                       8
#define TSP_FLAG_SetFree(fltid)                     TSP_OS_EventSet(_s32FreeFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetFree(fltid, events, opt)        TSP_OS_EventGet(_s32FreeFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define TSP_FLAG_ModeFree(idx, mask,events, opt)     TSP_OS_EventGet(_s32FreeFlags[idx], mask, (events), (opt), 0)
#define TSP_FLAG_SetDone(fltid)                     TSP_OS_EventSet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)))
#define TSP_FLAG_GetDone(fltid, events, opt)        TSP_OS_EventGet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), TSP_FLAG_FREE_TIMEOUT)

#define TSP_FLAG_SetModeFlags(fltid)                { _s32ModeFlags[fltid>>5] |= (1<<(fltid&0x1F)) ;}
#define TSP_FLAG_GetModeFlags(idx ,flags)           {  flags = _s32ModeFlags[idx] ; }
#define TSP_FLAG_ClrModeFlags(fltid)                { _s32ModeFlags[fltid>>5] &= (~(1<<(fltid&0x1F))); }

#define TSP_FLAG_SetOvfRst(fltid)                   (_s32OvfRstFlags[fltid>>5] |= (1<<(fltid&0x1F)))
#define TSP_FLAG_GetOvfRst(fltid)                   ((_s32OvfRstFlags[fltid>>5] & (1<<(fltid&0x1F)))?TRUE:FALSE )
#define TSP_FLAG_ClrOvfRst(fltid)                   (_s32OvfRstFlags[fltid>>5] &= (~(1<<(fltid&0x1F))))

// for mutiple process using, record which filter is enabled by this process
#define _TSP_SetSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemoveSecNotifyId(fltid)                       (_u32SecNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

// for mutiple process using, record which pvr engine is enabled by this process
#define _TSP_SetPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemovePvrNotifyId(fltid)                       (_u32PvrNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

// for mutiple process using, record which hw pcr is enabled by this process
#define _TSP_SetHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemoveHwPcrNotifyId(fltid)                     (_u32HwPcrNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

#define TSP_ENTRY()                                 if (!TSP_OS_MutexObtain(_ptsp_res->_s32MutexId, TSP_OS_MUTEX_TIMEOUT)) \
                                                    {                                                           \
                                                        _ptsp_res->_tspInfo.LastErr = E_TSP_FAIL_FUNCTION;                 \
                                                        return E_TSP_FAIL_FUNCTION;                             \
                                                    }                                                           \
                                                    TSP_LOCK();

#define TSP_RETURN(_ret)                            TSP_UNLOCK();                                       \
                                                    TSP_OS_MutexRelease(_ptsp_res->_s32MutexId);                   \
                                                    if (_ret != E_TSP_OK) { _ptsp_res->_tspInfo.LastErr = _ret; }  \
                                                    return _ret; // Should be optimized by compiler

// @TODO: Jerry
// Review and check the rule of LOCK
#define TSP_LOCK()                                  TSP_OS_MutexObtain(_ptsp_res->_s32LockId, TSP_OS_MUTEX_TIMEOUT)
#define TSP_UNLOCK()                                TSP_OS_MutexRelease(_ptsp_res->_s32LockId)

#define _TSP_INT_LOCK()                             TSP_OS_MutexObtain(_ptsp_res->_s32IntRegMutexId, TSP_OS_MUTEX_TIMEOUT)
#define _TSP_INT_UNLOCK()                           TSP_OS_MutexRelease(_ptsp_res->_s32IntRegMutexId)

#define TSP_PVR_ENTRY()                             if (!TSP_OS_MutexObtain(_ptsp_res->_s32PvrMutexId, TSP_OS_MUTEX_TIMEOUT)) \
                                                    {                                                              \
                                                        _ptsp_res->_tspInfo.LastErr = E_TSP_FAIL_FUNCTION;                    \
                                                        return E_TSP_FAIL_FUNCTION;                                \
                                                    }

#define TSP_PVR_RETURN(_ret)                        TSP_OS_MutexRelease(_ptsp_res->_s32PvrMutexId);                   \
                                                    if (_ret != E_TSP_OK) { _ptsp_res->_tspInfo.LastErr = _ret; }     \
                                                    return _ret; // Should be optimized by compiler

// TSP event define
#define TSP_EVENT_SECTION                           0x00000001
#define TSP_EVENT_DISABLEFLT                        0x00000080
#define TSP_EVENT_FREEFLT                           0x00000040

#define TSP_EVENT_SECTION_POLL                      0x00000002

#define TSP_TASK_EVENTS                             ( TSP_EVENT_SECTION | TSP_EVENT_FREEFLT|TSP_EVENT_DISABLEFLT )
#define TSP_POLL_EVENTS                             ( TSP_EVENT_SECTION_POLL )

#if __LEGACY__

// @FIXME: move to RASP
#undef  TSP_TASK_EVENTS
#define TSP_TASK_EVENTS                             ( TSP_EVENT_SECTION  | TSP_EVENT_FREEFLT |TSP_EVENT_DISABLEFLT | \
                                                      TSP_EVENT_PVR0_RDY | TSP_EVENT_PVR1_RDY   )

#define TSP_EVENT_PVR0_RDY                          0x00000004
#define TSP_EVENT_PVR1_RDY                          0x00000008
// @FIXME: remove
#define TSP_EVENT_PVR0_RDY_POLL                     0x00000010
#define TSP_EVENT_PVR1_RDY_POLL                     0x00000020

// @FIXME: remove
#undef  TSP_POLL_EVENTS
#define TSP_POLL_EVENTS                             ( TSP_EVENT_SECTION_POLL  | \
                                                      TSP_EVENT_PVR0_RDY_POLL | \
                                                      TSP_EVENT_PVR1_RDY_POLL   )

#endif // __LEGACY__

#define TSP_FLT2BUF_NULL                            0xFFFFFFFF
#define TSP_BUF2FLT_NULL                            TSP_FLT2BUF_NULL

#define TSP_CHECK(_b, _f, _a...)                    { if (!(_b)) { TSP_OS_Print("[TSP_ASSERT]"); TSP_OS_Print(_f, ##_a); return E_TSP_FAIL;} }
#define TSP_CHECK_UNLOCK(_b, _f, _a...)             { if (!(_b)) { TSP_OS_Print("[TSP_ASSERT]"); TSP_OS_Print(_f, ##_a); TSP_UNLOCK(); return E_TSP_FAIL; } }
#ifdef MS_DEBUG
#define TSP_DBG(_l, _f, _a...)                      { if (_u32DbgLevel >= (_l)) { TSP_OS_Print("[TSP_DBG]"); TSP_OS_Print(_f, ##_a); } }
#define TSP_DBG_TASK()                              { _ptsp_res->_tspInfo.u32TaskLine = __LINE__; }
#else
#define TSP_DBG(_l, _f, _a...)                      { }
#define TSP_DBG_TASK()                              { }
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#define DEMUX_EVENT_DATA_DISCARDED                   (0x00000001U)
#define DEMUX_EVENT_CRC_FAILED                       (0x00000010U)
#endif

#define NEED_SEC_FILTER(FltType) (E_TSP_FLT_USER_SEC == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PES == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PKT == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PCR == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_TTX == (FltType & E_TSP_FLT_USER_MASK))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

typedef struct _TSP_Info
{
    MS_BOOL                         bSWInit;
    MS_BOOL                         bISRTaskEn;
    TSP_SW_Status                   Status;
    TSP_Result                      LastErr;

    MS_U32                          Pid[TSP_PIDFLT_NUM_ALL];
    TSP_Id                          OwnerId [TSP_PIDFLT_NUM_ALL];
    TSP_FltState                    FltState[TSP_PIDFLT_NUM_ALL];
    TSP_FltType                     FltType [TSP_PIDFLT_NUM_ALL];
    MS_U32                          Flt2Buf [TSP_PIDFLT_NUM_ALL];

    TSP_FltMode                     FltMode    [TSP_SECFLT_NUM];
    TSP_Event                       FltEvent   [TSP_SECFLT_NUM];
    P_TSP_Callback                  FltCallback[TSP_SECFLT_NUM];

    TSP_FltState                    BufState[TSP_SECBUF_NUM]; // @TODO: FltState, SecFltState?
    MS_U32                          Buf2Flt [TSP_SECBUF_NUM];

    TSP_InitParam                   Param;

    // Legacy
    MS_BOOL                         bSuspend;
    // Debug information
    MS_U32                          u32TaskLine;

    MS_U8                           MulFlt2Buf[TSP_SECFLT_NUM];
} TSP_Info;

typedef struct
{
    MS_U32  u32StcOffset;
    MS_BOOL bAdd;
}STC_Offset;


typedef struct _TSP_RESOURCE_PRIVATE
{
    MS_S32                  _s32TaskId;
    MS_S32                  _s32MutexId;
    MS_S32                  _s32LockId;
    MS_S32                  _s32IntRegMutexId;
    MS_S32                  _s32PvrMutexId;

    TSP_Info                _tspInfo;

    #if defined(__LEGACY__)
    TSP_Event               _PvrEvtNotify;
    P_TSP_Callback          _PvrCallback;
    #endif

    MS_U8                   _u8_ResetPcr[STC_ENG_NUM];
    MS_U32                  _u32_PrevStcBase[STC_ENG_NUM];
    MS_U32                  _u32_PrevPllControl[STC_ENG_NUM];
    STC_Offset              _u32_StcOffset[STC_ENG_NUM];

    MS_U32                  _Current_PVR;

    //[NOTE][Eddie]
    // Since STB and TV architecture are different, STB have to emulate TV behavior to avoid
    // Live/File collision of PLAYBACK channel.
    TSP_FltType             _Current_Live;

}TSP_RESOURCE_PRIVATE;

static MS_S32                  _s32EventId = -1;
static MS_S32                  _s32CrcErrFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32SecOvfFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32SecRdyFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32FltDisFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32FreeFlags  [(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32DoneFlags  [(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };

static MS_S32                  _s32ModeFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };
static MS_S32                  _s32OvfRstFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1 };

static MS_U32                  _u32SecNotifyId[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = 0 }; //for ISR processing
static MS_U32                  _u32PvrNotifyId[(TSP_PVR_IF_NUM+31)>>5] = { [0 ... (((TSP_PVR_IF_NUM+31)>>5)-1)] = 0 };
static MS_U32                  _u32HwPcrNotifyId[(TSP_PCRFLT_NUM+31)>>5] = { [0 ... (((TSP_PCRFLT_NUM+31)>>5)-1)] = 0 };

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifndef TSP_UTOPIA_20
static TSP_RESOURCE_PRIVATE         _tsp_res = {
    ._s32TaskId                     = -1,
    ._s32MutexId                    = -1,
    ._s32LockId                     = -1,
    ._s32IntRegMutexId              = -1,
    ._s32PvrMutexId                 = -1,

    ._tspInfo                       = {
        .bSWInit                    = FALSE,
        .bISRTaskEn                 = FALSE,
        .LastErr                    = E_TSP_OK,
        .bSuspend                   = FALSE,
        .u32TaskLine                = 0,
    },

    #if defined(__LEGACY__)
    ._PvrEvtNotify                  = E_TSP_EVENT_DATA_IDLE,
    ._PvrCallback                   = NULL,
    #endif

    ._u8_ResetPcr                   = {[0 ... TSP_PCRFLT_NUM] = 3},
    ._u32_PrevStcBase               = {[0 ... TSP_PCRFLT_NUM] = 0},
    ._u32_PrevPllControl            = {[0 ... TSP_PCRFLT_NUM] = 0},
    ._u32_StcOffset                 = {[0 ... TSP_PCRFLT_NUM] = {0,TRUE}},
    ._Current_PVR                   = 0,
    ._Current_Live                  = E_TSP_FLT_SRC_LIVE0,
};

static TSP_RESOURCE_PRIVATE*        _ptsp_res = &_tsp_res;
#else
static TSP_RESOURCE_PRIVATE*        _ptsp_res = NULL;
#endif


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MSIF_Version                 _drv_tsp_version = {
                                        .DDI = { TSP_DRV_VERSION, },
                                    };

static TSP_DbgLevel                 _u32DbgLevel = E_TSP_DBG_L1;

MS_U32                              _u32IntNum = E_INT_IRQ_TSP2HK;

#ifdef MSOS_TYPE_LINUX_KERNEL
wait_queue_head_t                   tsp_wq_head[TSP_SECFLT_NUM];
static MS_U32                       _u32SecEvent[TSP_SECFLT_NUM];
#endif // MSOS_TYPE_LINUX_KERNEL


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

TSP_Result MDrv_TSP_FLT_DropEnable(MS_BOOL bSet)
{
    TSP_ENTRY();
    HAL_TSP_SecFlt_DropEnable(bSet);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static TSP_Result _MDrv_FltAllocForRec(MS_U32 u32Eng, TSP_SRC_SEQ eEngSrc, MS_U32 *pu32FltId);
static TSP_Result _MDrv_FltFreeForRec(MS_U32 u32FltId);
static TSP_Result _MDrv_FltSetPIDForRec(MS_U32 u32FltId, MS_U32 u32PID);
static TSP_Result _MDrv_FltGetPIDForRec(MS_U32 u32FltId, MS_U32* pu32PID);
static TSP_Result _MDrv_FltEnableForRec(MS_U32 u32FltId, MS_BOOL bEnable);
static TSP_Result _MDrv_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1);
static TSP_Result _MDrv_PVR_Eng_Start(MS_U32 u32Eng, DrvRASP_RecMode eRecMode, MS_BOOL bStart)                                 ;
static TSP_Result _MDrv_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause);
static TSP_Result _MDrv_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHYADDR *pu32WriteAddr)                                     ;
static TSP_Result _MDrv_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)                        ;
static TSP_Result _MDrv_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet);
static TSP_Result _MDrv_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp);
static TSP_Result _MDrv_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp);
static TSP_Result _MDrv_PVR_Eng_TimeStamp(MS_U32 u32Eng, MS_BOOL bEnable);

/*
static void _TSP_CMDQ_Init(void)
{
    while (HAL_TSP_CMDQ_Count()); // wait command finish
}
*/
// Switch Uart to TSP Fw, function works only when fw support uart functionality
// Compile Fwtsp.c with UART_ENABLE in chip.h

#if SWITCH_TSP_UART
static void _TSP_Switch_Uart(void)
{
        MS_U32 u32Base = 0 ,u32Size;
        MS_U16 u16Temp;

        printf("Switch Uart to TSP firmware !!\n");

        MDrv_MMIO_GetBASE( &u32Base, &u32Size, MS_MODULE_CHIPTOP );
        printf("ChipTop MMIO base = %08lX \n",u32Base);

        if(u32Base)
        {
        u16Temp = READ_WORD((u32Base + 0x1EA6*2));
        printf("ChipTop Uart setting = %x \n",u16Temp);
        u16Temp = ((u16Temp & 0xFFF0) | 0x3 );
        WRITE_WORD((u32Base + 0x1EA6*2),u16Temp);
        }
        else
        {
        printf ("MMIO ChipTOP GetBase Fail\n");
        }

}
#endif

static MS_BOOL _TSP_InitResource(TSP_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii = 0, u32Items = 0;

    if(presource == NULL)
    {
        printf("Utopia2 Resource address is NULL!\n");
        return FALSE;
    }

    presource->_s32TaskId = -1;
    presource->_s32MutexId = -1;
    presource->_s32LockId = -1;
    presource->_s32IntRegMutexId = -1;
    presource->_s32PvrMutexId = -1;

    u32Items = (TSP_PIDFLT_NUM_ALL+31)>>5;

    presource->_tspInfo.bSWInit = FALSE;
    presource->_tspInfo.bISRTaskEn = FALSE;
    presource->_tspInfo.LastErr = E_TSP_OK;
    presource->_tspInfo.bSuspend = FALSE;
    presource->_tspInfo.u32TaskLine = 0;

    #if defined(__LEGACY__)
    presource->_PvrEvtNotify = E_TSP_EVENT_DATA_IDLE;
    presource->_PvrCallback = NULL;
    #endif

    for(u32ii = 0; u32ii < STC_ENG_NUM; u32ii++)
    {
        presource->_u8_ResetPcr[u32ii] = 3;
        presource->_u32_PrevStcBase[u32ii] = 0;
        presource->_u32_PrevPllControl[u32ii] = 0;
    }

    presource->_Current_PVR = 0;
    presource->_Current_Live = E_TSP_FLT_SRC_LIVE0;

    return TRUE;

}

static TSP_TS_PAD _DrvHALPadMapping(TSP_TSPad eTSPad)
{
    switch (eTSPad)
    {
        case E_TSP_TS_PAD0:
            return E_TSP_TS_PAD_EXT0;
        case E_TSP_TS_PAD1:
            return E_TSP_TS_PAD_EXT1;
        case E_TSP_TS_PAD2:
            return E_TSP_TS_PAD_EXT2;
        case E_TSP_TS_PAD3:
            return E_TSP_TS_PAD_EXT3;
        case E_TSP_TS_DEMOD0:
            return E_TSP_TS_PAD_INTER0;
        case E_TSP_TS_DEMOD1:
            return E_TSP_TS_PAD_INTER1;
        case E_TSP_TS_PAD_TSO0:
            return E_TSP_TS_PAD_TSOUT0;
        case E_TSP_TS_PAD_TSO1:
            return E_TSP_TS_PAD_TSOUT1;

        default:
            return E_TSP_TS_PAD_INVALID;
    }
}

static TSP_HAL_TSIF _DrvHALTSIFMapping(TSP_TSIF eTSIF)
{
    switch(eTSIF)
    {
        case E_TSP_TSIF_0:
            return E_TSP_HAL_TSIF_0;
        case E_TSP_TSIF_1:
            return E_TSP_HAL_TSIF_1;
        case E_TSP_TSIF_PVR0:
            return E_TSP_HAL_TSIF_PVR0;
        case E_TSP_TSIF_PVR1:
            return E_TSP_HAL_TSIF_PVR1;
        case E_TSP_TSIF_PVR2:
            return E_TSP_HAL_TSIF_PVR2;

        default:
            return E_TSP_HAL_TSIF_INVALID;
    }
}

TSP_Result MDrv_TSP_FIFO_SourceSelect(TSP_TSId eTspTSid, TSP_FileMode eFifoType)
{
    TSP_ENTRY();

    TSP_DST_SEQ eTspDstSeq = E_TSP_DST_INVALID;
    TSP_SRC_SEQ eTspSrcSeq = E_TSP_SRC_INVALID;

   switch (eFifoType)
   {
     case E_TSP_FILE_2_AUDIO:
        eTspDstSeq = E_TSP_DST_FIFO_AUDIO;
        break;
     case E_TSP_FILE_2_AUDIO2:
        eTspDstSeq = E_TSP_DST_FIFO_AUDIO2;
        break;
     case E_TSP_FILE_2_VIDEO3D:
        eTspDstSeq = E_TSP_DST_FIFO_VIDEO3D;
        break;
     case E_TSP_FILE_2_VIDEO:
        eTspDstSeq = E_TSP_DST_FIFO_VIDEO;
        break;
     case E_TSP_FILE_2_AUDIO3:
        eTspDstSeq = E_TSP_DST_FIFO_AUDIO3;
        break;
     default:
        TSP_OS_Print("[TSP Error][%04d] INVALID FIFO TYPE\n",__LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
   }

   switch (eTspTSid)
   {
     case E_TSP_TSID_TSIF0:
        eTspSrcSeq = E_TSP_SRC_PKTDMX0;
        break;
     case E_TSP_TSID_TSIF1:
        eTspSrcSeq = E_TSP_SRC_PKTDMX1;
        break;
     case E_TSP_TSID_TSIF01_FILE:
        eTspSrcSeq = E_TSP_SRC_PKTDMX2;
        break;
     case E_TSP_TSID_TSIF2:
        eTspSrcSeq = E_TSP_SRC_PKTDMX3;
        break;
     case E_TSP_TSID_TSIF3:
        eTspSrcSeq = E_TSP_SRC_PKTDMX4;
        break;
     case E_TSP_TSID_TSIF23_FILE:
        eTspSrcSeq = E_TSP_SRC_PKTDMX5;
        break;
     case E_TSP_TSID_MMFI0:
        eTspSrcSeq = E_TSP_SRC_MMFI0;
        break;
     case E_TSP_TSID_MMFI1:
        eTspSrcSeq = E_TSP_SRC_MMFI1;
        break;
     default:
        TSP_OS_Print("[TSP Error][%04d] INVALID PKDMX SRC\n",__LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
   }


    HAL_TSP_FIFO_SetSrc(eTspDstSeq, eTspSrcSeq);

    if((eTspTSid == E_TSP_TSID_TSIF01_FILE) || (eTspTSid == E_TSP_TSID_TSIF23_FILE) || (eTspTSid == E_TSP_TSID_MMFI0) ||
        (eTspTSid == E_TSP_TSID_MMFI1))
    {
        HAL_TSP_FIFO_BlockDis(eTspDstSeq, FALSE);
    }
    else
    {
        HAL_TSP_FIFO_BlockDis(eTspDstSeq, TRUE);
    }

    TSP_RETURN(E_TSP_OK);
}




//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo
/// @param  bReset                  \b IN   0->1:reset, 1->0:activate
/// @return TSP_Result
/// @note
/// 0->1 reset, 1->0 activate
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FIFO_Reset(TSP_FltType eFltType, MS_BOOL bReset)
{
    TSP_ENTRY();
    TSP_DST_SEQ eDst = E_TSP_DST_INVALID;
    switch (eFltType)
    {
        case E_TSP_FLT_FIFO_VIDEO:
            eDst = E_TSP_DST_FIFO_VIDEO;
            break;
        case E_TSP_FLT_FIFO_AUDIO:
            eDst = E_TSP_DST_FIFO_AUDIO;
            break;
        case E_TSP_FLT_FIFO_AUDIO2:
            eDst = E_TSP_DST_FIFO_AUDIO2;
            break;
        case E_TSP_FLT_FIFO_VIDEO3D:
            eDst = E_TSP_DST_FIFO_VIDEO3D;
            break;
        case E_TSP_FLT_FIFO_AUDIO3:
            eDst = E_TSP_DST_FIFO_AUDIO3;
            break;

    }
    HAL_TSP_FIFO_Reset(eDst,  bReset);

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_FIFO_BlockEnable(TSP_FltType eFltType, MS_BOOL bDisable)
{
    TSP_ENTRY();
    TSP_DST_SEQ eDst = E_TSP_DST_INVALID;
    switch (eFltType)
    {
        case E_TSP_FLT_FIFO_VIDEO:
            eDst = E_TSP_DST_FIFO_VIDEO;
            break;
        case E_TSP_FLT_FIFO_AUDIO:
            eDst = E_TSP_DST_FIFO_AUDIO;
            break;
        case E_TSP_FLT_FIFO_AUDIO2:
            eDst = E_TSP_DST_FIFO_AUDIO2;
            break;
        case E_TSP_FLT_FIFO_VIDEO3D:
            eDst = E_TSP_DST_FIFO_VIDEO3D;
            break;
        case E_TSP_FLT_FIFO_AUDIO3:
            eDst = E_TSP_DST_FIFO_AUDIO3;
            break;
    }
    HAL_TSP_FIFO_BlockDis(eFltType,bDisable);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo
/// @param  u32Level                \b OUT  0:0-25%, 1:25-50%, 2:50-75%, 3:75-100%, 0xFFFFFFFF:N/A
/// @return TSP_Result
/// @note
/// 0->1 reset, 1->0 activate
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FIFO_GetStatus(TSP_FltType eFltType, MS_U32 *pu32Level)
{
    TSP_ENTRY();

    eFltType &= E_TSP_FLT_FIFO_MASK;
    TSP_DST_SEQ eDst = E_TSP_DST_INVALID;
    switch (eFltType)
    {
        case E_TSP_FLT_FIFO_VIDEO:
            eDst = E_TSP_DST_FIFO_VIDEO;
            break;
        case E_TSP_FLT_FIFO_VIDEO3D:
            eDst = E_TSP_DST_FIFO_VIDEO3D;
            break;
        case E_TSP_FLT_FIFO_AUDIO:
            eDst = E_TSP_DST_FIFO_AUDIO;
            break;
        case E_TSP_FLT_FIFO_AUDIO2:
            eDst = E_TSP_DST_FIFO_AUDIO2;
            break;
        case E_TSP_FLT_FIFO_AUDIO3:
            eDst = E_TSP_DST_FIFO_AUDIO3;
            break;
    }
    *pu32Level = HAL_TSP_FIFO_GetStatus(eDst);
    if (*pu32Level == HAL_TSP_RET_NULL)
    {
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FIFO_Overflow_Status(TSP_FltType eFltType, MS_BOOL *pbOverflow)
{
    *pbOverflow = FALSE;

    TSP_ENTRY();
    *pbOverflow = HAL_TSP_FIFO_Overflow(eFltType);
    TSP_RETURN(E_TSP_OK);
}

// initilize or reset the FLT to be initial state
// PID and ctrls are clear
static void _TSP_FLT_Init(MS_U32 u32FltId)
{
    static MS_U8        u8Mask[TSP_FILTER_DEPTH]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static MS_U8        u8NMask[TSP_FILTER_DEPTH] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


#if HW_PCRFLT_ENABLE
	if (TSP_PCRFLT_PCR0 == u32FltId)
	{
		HAL_TSP_PcrFlt_SetPid(0, TSP_PID_NULL);
		HAL_TSP_PcrFlt_SetSrc(0, E_TSP_PCR_SRC_INVALID);
		HAL_TSP_PcrFlt_Enable(0, FALSE);
		HAL_TSP_PcrFlt_ClearInt(0);
		HAL_TSP_INT_Disable((MS_U16)(TSP_HWINT2_PCR0_UPDATE_END << TSP_HWINT2_STATUS_SHIFT));
	}
	else if (TSP_PCRFLT_PCR1 == u32FltId)
	{
		HAL_TSP_PcrFlt_SetPid(1, TSP_PID_NULL);
		HAL_TSP_PcrFlt_SetSrc(1, E_TSP_PCR_SRC_INVALID);
		HAL_TSP_PcrFlt_Enable(1, FALSE);
		HAL_TSP_PcrFlt_ClearInt(1);
		HAL_TSP_INT_Disable((MS_U16)(TSP_HWINT2_PCR1_UPDATE_END << TSP_HWINT2_STATUS_SHIFT));
	}

#endif

    HAL_TSP_PidFlt_SetPid(u32FltId, TSP_PID_NULL);
    HAL_TSP_PidFlt_SetFltIn(u32FltId, TSP_PIDFLT_TSIF_MAX);                                          // default MAX
    HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);
    HAL_TSP_PidFlt_SetSecFlt(u32FltId, u32FltId);                                                    // PIDFLT <-> SECFLT
    HAL_TSP_PidFlt_SetCCInit(u32FltId);                                                              // init countinuous counter

    _ptsp_res->_tspInfo.Pid[u32FltId]         = TSP_PID_NULL ;
    _ptsp_res->_tspInfo.FltState[u32FltId]    = E_TSP_FLT_STATE_FREE;
    _ptsp_res->_tspInfo.Flt2Buf[u32FltId]     = TSP_FLT2BUF_NULL;
    _ptsp_res->_tspInfo.OwnerId[u32FltId]     = E_TSP_NULL;

    // @NOTE section filters may be less than PID filters
    if(u32FltId < TSP_SECFLT_NUM)
    {
        REG_SecFlt *        pSecFlt =     &(_REGSec->Flt[u32FltId]);

        HAL_TSP_SecFlt_ClrCtrl(pSecFlt);
        HAL_TSP_SecFlt_SelSecBuf(pSecFlt, TSP_SECFLT_SECBUF_MAX);                                       // default MAX
        HAL_TSP_SecFlt_SetMask(pSecFlt, u8Mask);
        HAL_TSP_SecFlt_SetNMask(pSecFlt, u8NMask);
        HAL_TSP_SecFlt_SetType(pSecFlt, TSP_SECFLT_USER_NULL);                                          // default SECTION

        _ptsp_res->_tspInfo.FltMode[u32FltId]     = E_TSP_FLT_MODE_CONTI;
        _ptsp_res->_tspInfo.FltEvent[u32FltId]    = E_TSP_EVENT_DATA_IDLE;
        _ptsp_res->_tspInfo.FltCallback[u32FltId] = NULL;
    }

}


static void _TSP_FLT_Free(MS_U32 u32FltId)
{
    HAL_TSP_SecFlt_Free(&(_REGSec->Flt[u32FltId]));
    TSP_FLAG_ClrModeFlags(u32FltId);
    _ptsp_res->_tspInfo.OwnerId[u32FltId]  = E_TSP_NULL;
    _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_FREE;
    _ptsp_res->_tspInfo.FltType[u32FltId] = E_TSP_FLT_USER_NULL;

}

// Disable FLT output (SEC, AUD, VID)
// NOTE: other bypass route to ECM/EMM/REC/etc. is not disable.
static void _TSP_FLT_Disable(MS_U32 u32FltId)
{
    #if 1
    HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);  //@FIXME do we have to clear pvr and section out at this function?
    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        if (TSP_PCRFLT_PCR0 == u32FltId)
        {
            _TSP_RemoveHwPcrNotifyId(0);
            //HAL_TSP_PcrFlt_SetPid(0, TSP_PID_NULL, FALSE);
            //HAL_TSP_PcrFlt_SetSrc(0, E_TSP_PCR_SRC_INVALID);
            HAL_TSP_PcrFlt_Enable(0, FALSE);
            HAL_TSP_PcrFlt_ClearInt(0);
            HAL_TSP_INT_Disable((MS_U16)(TSP_HWINT2_PCR0_UPDATE_END << TSP_HWINT2_STATUS_SHIFT));
        }
        else if (TSP_PCRFLT_PCR1 == u32FltId)
        {
            _TSP_RemoveHwPcrNotifyId(1);
            //HAL_TSP_PcrFlt_SetPid(1, TSP_PID_NULL, FALSE);
            //HAL_TSP_PcrFlt_SetSrc(1, E_TSP_PCR_SRC_INVALID);
            HAL_TSP_PcrFlt_Enable(1, FALSE);
            HAL_TSP_PcrFlt_ClearInt(1);
            HAL_TSP_INT_Disable((MS_U16)(TSP_HWINT2_PCR1_UPDATE_END << TSP_HWINT2_STATUS_SHIFT));
        }
    }
    _ptsp_res->_tspInfo.FltState[u32FltId] &= (TSP_FltState)(~E_TSP_FLT_STATE_ENABLE);

    #else
    HAL_TSP_PidFlt_SetFltOut(&(_REGPid->Flt[u32FltId]), TSP_PIDFLT_OUT_NULL);
    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        HAL_TSP_SecFlt_PcrReset(&(_REGSec->Flt[u32FltId]));
        HAL_TSP_PCRFlt_SetPid(0x1FFF,false);
        HAL_TSP_PCRFlt_ClearInt();
        HAL_TSP_INT_Disable(TSP_HWINT2_PCR_UPD << 8 );
    }
    _ptsp_res->_tspInfo.FltState[u32FltId] &= (TSP_FltState)(~E_TSP_FLT_STATE_ENABLE);
    #endif


/*
//[Note] Delay for VQ flush, open it if necessary.
#if defined (MSOS_TYPE_LINUX_KERNEL)
    mdelay(5);
#elif defined (MSOS_TYPE_ECOS)
#else
    MsOS_DelayTask(5);
#endif
*/

}


static void _TSP_SEC_Init(MS_U32 u32BufId)
{
    _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] = 0 ;        // Multi Filter to buffer count.
    _ptsp_res->_tspInfo.Buf2Flt[u32BufId] = TSP_BUF2FLT_NULL;
    HAL_TSP_SecBuf_SetBuf(&(_REGBuf->Buf[u32BufId]), 0, 0);
    HAL_TSP_SecBuf_Reset(&(_REGBuf->Buf[u32BufId]));
}


static void _TSP_SEC_Free(MS_U32 u32BufId)
{
    HAL_TSP_SecBuf_Free(&(_REGBuf->Buf[u32BufId]));
    _TSP_RemoveSecNotifyId(u32BufId);
    _ptsp_res->_tspInfo.BufState[u32BufId] = E_TSP_FLT_STATE_FREE;
}

static TSP_Result _TSP_PROC_CrcErr(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;
    MS_U32              flags; // dummy

    TSP_FLAG_GetCrcErr(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR); // if TSP not enable

        if (pEvent)
        {
            if ((_ptsp_res->_tspInfo.FltMode[u32FltId] & E_TSP_FLT_MODE_CRCCHK) ||
                (_ptsp_res->_tspInfo.FltMode[u32FltId] & E_TSP_FLT_MODE_AUTO_CRCCHK))
            {
                *pEvent = (TSP_Event) E_TSP_EVENT_SEC_CRCERROR;
            }
        }
        else
        {
            if (!(_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)) // if TSP not enable
            {
                return E_TSP_FAIL;
            }
            if ((_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_SEC_CRCERROR) &&
                (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
            {
                msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);
                TSP_DBG_TASK();
                _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_SEC_CRCERROR, &msg);
                TSP_DBG_TASK();
            }
        }

    return E_TSP_OK;
}




static TSP_Result _TSP_PROC_SecOvf(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;
    MS_U32              flags; // dummy

    //(POLL) Check any possible filter should be disabled
    //(TASK) Check only overflowed filter and to disable
    TSP_LOCK(); //{
    if (HAL_TSP_PidFlt_GetFltOutput(&(_REGPid1->Flt[u32FltId])) & TSP_PIDFLT_OUT_SECFLT)
    {
        _TSP_FLT_Disable(u32FltId);
    }
    TSP_FLAG_SetOvfRst(u32FltId);
    TSP_UNLOCK(); //}

    TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
    //[Eddie]Clear Event at Filter Enable , Try to avoid redundant Overflow event.
    //       Driver will not call overflow call back until previous overflow have been serverd(Reset Buffer)
    //TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);


    if (pEvent ) // MDrv_TSP_Proc -> _TSP_Poll_Proc
    {
        *pEvent = (MS_U32)E_TSP_EVENT_BUF_OVERFLOW;
    }
    else
    {
        // required event callback
        if ( (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_BUF_OVERFLOW) &&
             (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
        {
            msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);

            TSP_DBG_TASK();
            _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_BUF_OVERFLOW, &msg);
            TSP_DBG_TASK();
        }
    }

    return E_TSP_OK;
}


void MDrv_TSP_SEC_Update( MS_U32 u32SecFltId)
{
    TSP_FLAG_SetSecRdy(u32SecFltId);
}


static TSP_Result _TSP_PROC_SecRdy(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;

    MS_U16              bufid;
    MS_BOOL             data;
    MS_U32              flags;

/*
    if (E_TSP_EVENT_CB_POLL == u32CbOpt)
    {
        TSP_GetEvent(u32Events, TSP_EVENT_SECTION_POLL, TSP_OS_EVENT_OR_CLEAR, 0);
    }
    else
    {
    }
*/

    TSP_FLAG_GetSecRdy(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR); // clear for next incoming

    TSP_LOCK();
    bufid = HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));

    #if 1
    if ((_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_FREE) || (bufid != _ptsp_res->_tspInfo.Flt2Buf[u32FltId]))
    {
        for(flags = 0 ; flags < 10 ; flags ++)
        TSP_OS_Print("[TSP Error][%04d] u32FltId:%u eFltType:%08X Pid:%08X, FltStatus = %08X ,Mode = %08X , Event =%08X , AllMode= %08lX ,Pevent = %08lX,HwBufId %ld != tspInfo Bufid %ld\n",
                                                          __LINE__,
                                                          (unsigned int)u32FltId,
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltType[u32FltId]),
                                                          (unsigned int)_ptsp_res->_tspInfo.Pid[u32FltId],
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltState[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltMode[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltEvent[u32FltId]),
                                                          (MS_U32)_s32ModeFlags,
                                                          (MS_U32)pEvent,
                                                          (MS_U32)bufid,
                                                          _ptsp_res->_tspInfo.Flt2Buf[u32FltId]);
        //while(1);
        TSP_UNLOCK();

        return E_TSP_FAIL;
    }

    #else

    TSP_CHECK_UNLOCK(bufid == _ptsp_res->_tspInfo.Flt2Buf[u32FltId], "[TSP Error][%04d] u32FltId:%u eFltType:%08X Pid:%08X, FltStatus = %08X ,Mode = %08X , Event =%08X , AllMode= %08lX ,Pevent = %08lX,HwBufId %ld != tspInfo Bufid %ld\n",
                                                          __LINE__,
                                                          (unsigned int)u32FltId,
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltType[u32FltId]),
                                                          (unsigned int)_ptsp_res->_tspInfo.Pid[u32FltId],
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltState[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltMode[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltEvent[u32FltId]),
                                                          (MS_U32)_s32ModeFlags,
                                                          (MS_U32)pEvent,
                                                          (MS_U32)bufid,
                                                          _ptsp_res->_tspInfo.Flt2Buf[u32FltId]);
    #endif

    data = (HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[bufid])) != HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[bufid]))) ? TRUE : FALSE;
    TSP_UNLOCK();


    if (data)
    {
        if (pEvent) // POLL mode
        {
            *pEvent = (TSP_Event)E_TSP_EVENT_DATA_READY;
            // if TSP not enable
            if (TSP_FLAG_GetSecOvf(u32FltId,&flags,TSP_OS_EVENT_OR)||TSP_FLAG_GetCrcErr(u32FltId,&flags,TSP_OS_EVENT_OR))
            {
                return E_TSP_FAIL;
            }
        }
        else
        {
            if (!(_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)) // if TSP not enable
            {
                return E_TSP_FAIL;
            }
            if ((_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_DATA_READY) &&
                (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
            {
                msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);

                TSP_DBG_TASK();
                _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_DATA_READY, &msg);
                TSP_DBG_TASK();
            }
        }
    }

    //(POLL) Check any possible filter should be disabled
    //(TASK) Check only one-shot filter and to disable
    if (TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId);
        TSP_UNLOCK();
    }

    if (data)
    {
        return E_TSP_OK;
    }
    return E_TSP_FAIL; // no data valid || not enable
}

static void _TSP_Proc_Pvr(MS_U32 u32Events, MS_U32 u32CbOpt)
{
    printf("%s Wait implementation\n", __FUNCTION__);
}

static void _TSP_ISR_Proc(MS_U32 u32Events)
{
    MS_U32              fltid = TSP_PIDFLT_NUM_ALL, flags = 0,ModeMask,dummy,BaseFlt;
    int                 i;

    TSP_DBG_TASK();

#if __LEGACY__
    if ( (TSP_EVENT_PVR0_RDY_POLL | TSP_EVENT_PVR1_RDY_POLL) & u32Events)
    {
        _TSP_Proc_Pvr(u32Events, E_TSP_EVENT_CB_AUTO);
    }
#endif


    // @NOTE: Jerry
    //
    // WATCH OUT!!
    // It has many issues before due to race condition between TSP_TASK and USER_TASK dealing with FLT_FREE
    // Please pay more attention on it
    //
    // Check all freeing filters
    if (u32Events & TSP_EVENT_FREEFLT)
    {
        for (i = 0; i < ((TSP_PIDFLT_NUM_ALL+31)>>5); i++)
        {
            // Add to FREE list
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFree(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);
            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                TSP_LOCK();
                _TSP_FLT_Disable(fltid); // disable output
                TSP_UNLOCK();
                flags &= ~(1 << BaseFlt);
                TSP_FLAG_SetDone(fltid);

                // Clear other filter activities
                TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecOvf(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
            }
        }
    }

    MsOS_DelayTask(1);
    // Check all section avaiable filters
    if (u32Events & TSP_EVENT_SECTION)
    {
        // @TODO Jerry
        // Is it necessary to balance every filter priority even the event process is at TASK?

        for (i = 0; i < ((TSP_PIDFLT_NUM_ALL+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeCrcErr(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    _TSP_PROC_CrcErr(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt);
            }

            TSP_FLAG_ModeSecRdy(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    _TSP_PROC_SecRdy(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt);
            }
            TSP_FLAG_ModeSecOvf(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only overflowed filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                //if (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE )
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) &&
                    (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE) &&
                    (!TSP_FLAG_GetOvfRst(fltid)))
                {
                    //TSP_OS_Print("TSP_FLT_OVFLOW [%ld] , Buf = %ld ,PID = %04lX \n",fltid, _ptsp_res->_tspInfo.Flt2Buf[fltid], _ptsp_res->_tspInfo.Pid[fltid]);
                    _TSP_PROC_SecOvf(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt );
                // Clear other filter activities
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR); // Clear section ready also
            }
            MsOS_DelayTask(1);
        }
    }


    if (u32Events & TSP_EVENT_DISABLEFLT)//Disable Polling mode filter,CB mode filter disable at SecRdy or SecOvf
    {
        for (i = 0; i < ((TSP_PIDFLT_NUM_ALL+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFltDis(i,~ModeMask, &flags ,TSP_OS_EVENT_OR_CLEAR); // Polling mode
            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                if (TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR))
                {
                    TSP_LOCK();
                    _TSP_FLT_Disable(fltid); // disable output
                    TSP_UNLOCK();
                }
                flags &= ~(1 << BaseFlt);
            }
        }
   }

   MsOS_DelayTask(1);


}


static void _TSP_ISR_Task(void)
{
    MS_U32              events = 0;

    while (_ptsp_res->_tspInfo.bISRTaskEn)
    {
        // @NOTE Jerry
        // Clear task events directly in case there has any incoming event before _TSP_ISR_Proc to clear
        TSP_GetEvent(&events, TSP_TASK_EVENTS, TSP_OS_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);

        _TSP_ISR_Proc(events);
    }
}

#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _TSP_ISR(int irq, void *dev_id)
#else
static void _TSP_ISR(void)
#endif
{
    MS_U32                 hw_int;
    MS_U32                 sw_int;
    MS_U32                 Pcr_H, Pcr, Stc_H, Stc, u32Diff;

    _TSP_INT_LOCK();
    hw_int = HAL_TSP_INT_GetHW();
    //@TODO Need to check why call clr twice will cause one of pcr not work
    sw_int = HAL_TSP_INT_GetSW();
    _TSP_INT_UNLOCK();

#if __LEGACY__
    // Richard: @FIXME: do we need to detect all the int
    if (hw_int & TSP_HWINT_PVR)
    {
        if(_TSP_ChkPvrNotifyId(0))
        {
            TSP_SetEvent(hw_int & TSP_HWINT_PVR); // Trigger task
        }
        else
        {
            hw_int &= ~TSP_HWINT_PVR;
        }
    }
#endif

    //@NOTE  NOW We only consider 32bit stc and pcr calculation. Don't handle 64bit calculation and 32 bit overflow issue
    if (_TSP_ChkHwPcrNotifyId(0) && (hw_int & (TSP_HWINT2_PCR0_UPDATE_END)))
    {
        HAL_TSP_PcrFlt_GetPcr(0, &Pcr_H, &Pcr);

        MS_U32 u32STC_Add_Offset = ( _ptsp_res->_u32_StcOffset[0].bAdd == TRUE) ? (Pcr + _ptsp_res->_u32_StcOffset[0].u32StcOffset) : (Pcr - _ptsp_res->_u32_StcOffset[0].u32StcOffset);

        if(_ptsp_res->_u8_ResetPcr[0]>=2)
        {
            if(_ptsp_res->_u8_ResetPcr[0] == 3)
            {
                #define STC_SYNTH_DEFAULT 0x28000000   // @F_TODO do we have to seperate 27M and 90K clk mode? plz define this value to right place...
                HAL_TSP_SetSTCSynth(0, STC_SYNTH_DEFAULT);
            }
            //HAL_TSP_CMDQ_SetSTC(Pcr_H,Pcr);
            HAL_TSP_STC64_Set(0, Pcr_H,u32STC_Add_Offset);
            _ptsp_res->_u8_ResetPcr[0] = 0;
            _ptsp_res->_u32_PrevStcBase[0] = u32STC_Add_Offset;
            HAL_TSP_GetSTCSynth(0, &_ptsp_res->_u32_PrevPllControl[0]);
        }

        //HAL_TSP_CMDQ_GetSTC(&Stc_H, &Stc);
        HAL_TSP_STC64_Get(0, &Stc_H, &Stc);
        if(( Stc - _ptsp_res->_u32_PrevStcBase[0]) > 0x4000)
        {
            //printf("STC_DIFF = %08X ==== \n\n",Stc - _ptsp_res->_u32_PrevStcBase);
            _ptsp_res->_u32_PrevStcBase[0] = Stc;

            if(_ptsp_res->_u32_PrevStcBase[0] > Pcr) // faster then PCR comming
            {
                u32Diff = _ptsp_res->_u32_PrevStcBase[0] - Pcr ;
                if( u32Diff  <= 0x02) //pretty close
                {
                    _ptsp_res->_u8_ResetPcr[0] = 0;
                }
                else if( u32Diff  < 0x1000)
                {
                    _ptsp_res->_u32_PrevPllControl[0] += ((u32Diff) >> 3 );                         //now = error x alpha + prev
                    HAL_TSP_SetSTCSynth(0, _ptsp_res->_u32_PrevPllControl[0] + ((u32Diff)>>6));        //out = now + error x beta
                    _ptsp_res->_u8_ResetPcr[0] = 0;
                    //printf("======== Set STC CONTROL <++> %08X  PCR/Diff = %08X/%08X \n",_ptsp_res->_u32_PrevPllControl + ((_ptsp_res->_ptsp_res->_u32_PrevStcBase - Pcr)>>6),Pcr,_ptsp_res->_u32_PrevStcBase-Pcr);
                }
                else
                {
                    _ptsp_res->_u8_ResetPcr[0]++;
                }
            }
            else // slower or equal
            {
                u32Diff = Pcr - _ptsp_res->_u32_PrevStcBase[0] ;
                if(( u32Diff) <= 0x02)
                {
                    _ptsp_res->_u8_ResetPcr[0] = 0;
                }
                else if((u32Diff) < 0x1000)
                {
                    _ptsp_res->_u32_PrevPllControl[0]-=((u32Diff)>> 3);                                //now = error x alpha + prev
                    HAL_TSP_SetSTCSynth(0, _ptsp_res->_u32_PrevPllControl[0] - (( u32Diff )>> 6));        //out = now + error x beta
                    _ptsp_res->_u8_ResetPcr[0] = 0;
                    //printf("======== Set STC CONTROL <--> %08X  PCR/Diff = %08X/%08X \n",_ptsp_res->_u32_PrevPllControl - ((Pcr - _ptsp_res->_ptsp_res->_u32_PrevStcBase )>>6),Pcr,Pcr-_ptsp_res->_u32_PrevStcBase );
                }
                else
                {
                    _ptsp_res->_u8_ResetPcr[0]++;
                }
            }
        }

        _TSP_INT_LOCK();
        HAL_TSP_PcrFlt_ClearInt(0);
        _TSP_INT_UNLOCK();
    }//end pcr0
    else
    {
       hw_int &= ~TSP_HWINT2_PCR0_UPDATE_END;
    }

    if (_TSP_ChkHwPcrNotifyId(1) && (hw_int & (TSP_HWINT2_PCR1_UPDATE_END ) ))
    {
        HAL_TSP_PcrFlt_GetPcr(1, &Pcr_H, &Pcr);

        MS_U32 u32STC_Add_Offset = ( _ptsp_res->_u32_StcOffset[1].bAdd == TRUE) ? (Pcr + _ptsp_res->_u32_StcOffset[1].u32StcOffset) : (Pcr - _ptsp_res->_u32_StcOffset[1].u32StcOffset);

        if(_ptsp_res->_u8_ResetPcr[1]>=2)
        {
            if(_ptsp_res->_u8_ResetPcr[1] == 3)
            {
                #define STC_SYNTH_DEFAULT 0x28000000   // @F_TODO do we have to seperate 27M and 90K clk mode? plz define this value to right place...
                HAL_TSP_SetSTCSynth(1, STC_SYNTH_DEFAULT);
            }
            //HAL_TSP_CMDQ_SetSTC(Pcr_H,Pcr);
            HAL_TSP_STC64_Set(1, Pcr_H,u32STC_Add_Offset);
            _ptsp_res->_u8_ResetPcr[1] = 0;
            _ptsp_res->_u32_PrevStcBase[1] = u32STC_Add_Offset;
            HAL_TSP_GetSTCSynth(1, &_ptsp_res->_u32_PrevPllControl[1]);
        }

        //HAL_TSP_CMDQ_GetSTC(&Stc_H, &Stc);
        HAL_TSP_STC64_Get(1, &Stc_H, &Stc);
        if(( Stc - _ptsp_res->_u32_PrevStcBase[1]) > 0x4000)
        {
            //printf("STC_DIFF = %08X ==== \n\n",Stc - _ptsp_res->_u32_PrevStcBase);
            _ptsp_res->_u32_PrevStcBase[1] = Stc;

            if(_ptsp_res->_u32_PrevStcBase[1] > Pcr) // faster then PCR comming
            {
                u32Diff = _ptsp_res->_u32_PrevStcBase[1] - Pcr ;
                if( u32Diff  <= 0x02) //pretty close
                {
                    _ptsp_res->_u8_ResetPcr[1] = 0;
                }
                else if( u32Diff  < 0x1000)
                {
                    _ptsp_res->_u32_PrevPllControl[1] += ((u32Diff) >> 3 );                         //now = error x alpha + prev
                    HAL_TSP_SetSTCSynth(1, _ptsp_res->_u32_PrevPllControl[1] + ((u32Diff)>>6));        //out = now + error x beta
                    _ptsp_res->_u8_ResetPcr[1] = 0;
                    //printf("======== Set STC CONTROL <++> %08X  PCR/Diff = %08X/%08X \n",_ptsp_res->_u32_PrevPllControl + ((_ptsp_res->_ptsp_res->_u32_PrevStcBase - Pcr)>>6),Pcr,_ptsp_res->_u32_PrevStcBase-Pcr);
                }
                else
                {
                    _ptsp_res->_u8_ResetPcr[1]++;
                }
            }
            else // slower or equal
            {
                u32Diff = Pcr - _ptsp_res->_u32_PrevStcBase[1] ;
                if(( u32Diff) <= 0x02)
                {
                    _ptsp_res->_u8_ResetPcr[1] = 0;
                }
                else if((u32Diff) < 0x1000)
                {
                    _ptsp_res->_u32_PrevPllControl[1]-=((u32Diff)>> 3);                                //now = error x alpha + prev
                    HAL_TSP_SetSTCSynth(1, _ptsp_res->_u32_PrevPllControl[1] - (( u32Diff )>> 6));        //out = now + error x beta
                    _ptsp_res->_u8_ResetPcr[1] = 0;
                    //printf("======== Set STC CONTROL <--> %08X  PCR/Diff = %08X/%08X \n",_u32_PrevPllControl - ((Pcr - _ptsp_res->_u32_PrevStcBase )>>6),Pcr,Pcr-_ptsp_res->_u32_PrevStcBase );
                }
                else
                {
                    _ptsp_res->_u8_ResetPcr[1]++;
                }
            }
        }

        _TSP_INT_LOCK();
        HAL_TSP_PcrFlt_ClearInt(1);
        _TSP_INT_UNLOCK();
    }// end pcr1
    else
    {
        hw_int &= ~TSP_HWINT2_PCR1_UPDATE_END;
    }

    if (sw_int & TSP_SWINT_CTRL_FIRE)
    {
        //MS_U32          engid;
        MS_U32          fltid;
        MS_U32          status;
        //REG_SecFlt*     pFilter;
        MS_U32          dummy;

        //engid   = (sw_int & TSP_SWINT_INFO_ENG_MASK)   >> TSP_SWINT_INFO_ENG_SHFT;
        fltid   = (sw_int & TSP_SWINT_INFO_SEC_MASK)   >> TSP_SWINT_INFO_SEC_SHFT;
        status  = (sw_int & TSP_SWINT_STATUS_CMD_MASK) >> TSP_SWINT_STATUS_CMD_SHFT;
        //pFilter = &(_REGSec->Flt[fltid]);

        if(_ptsp_res->_tspInfo.BufState[fltid] == E_TSP_FLT_STATE_FREE)
        {
            goto CLEAR_INT;;
        }

        if(_TSP_ChkSecNotifyId(fltid) == FALSE)
        {
            sw_int &= ~TSP_SWINT_CTRL_FIRE;
            hw_int &= ~TSP_HWINT_TSP_SW_INT_EN;
            status = 0;
        }

        switch (status)
        {

        case TSP_SWINT_STATUS_SEC_RDY_CRCERR:  // CRC error But accept section ;
            //When there's no CRC in the section & E_TSP_FLT_MODE_AUTO_CRCCHK is used,
            //don't report CRC error
            if(!(_ptsp_res->_tspInfo.FltMode[fltid] & E_TSP_FLT_MODE_AUTO_CRCCHK))
        case TSP_SWINT_STATUS_SEC_CRCERR:  // Filter Didn't check CRC and Section Rdy
            {
                TSP_FLAG_SetCrcErr(fltid);                                                          // Set section event flag
            }

            if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(TSP_EVENT_SECTION);                                                    // Wake task up
            }
#ifdef MSOS_TYPE_LINUX_KERNEL
//            TSP_OS_Print("%s[%04d]: 0x%08X\n", __FILE__, __LINE__, status);
            MDrv_TSP_SEC_SetEvent(fltid, DEMUX_EVENT_CRC_FAILED);
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif

        if (TSP_SWINT_STATUS_SEC_CRCERR == status)
            break;

        case TSP_SWINT_STATUS_SEC_RDY:

            if ( (E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[fltid]) &&
                 (E_TSP_FLT_MODE_ONESHOT  & _ptsp_res->_tspInfo.FltMode[fltid]))
            {
                TSP_FLAG_SetFltDis(fltid);                                                    // Set section event flag
                if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                    TSP_SetEvent(TSP_EVENT_DISABLEFLT);
            }

            // During over-night test, we have got section ready interrupt from non-section filter.
            // The interrupt is illegal, and will cause TSP ASSERT "HwBufId != tspInfo Bufid" in _TSP_PROC_SecRdy.
            // Since the root cause was not found yet, we add a patch here to reject the non-section filter interrupt.
            if(((_ptsp_res->_tspInfo.FltType[fltid] & E_TSP_FLT_USER_MASK) == 0) || ((_ptsp_res->_tspInfo.FltState[fltid] & E_TSP_FLT_STATE_ENABLE) == 0) )
            {
                break;
            }

            TSP_FLAG_SetSecRdy(fltid);                                                          // Set section event flag
            // @NOTE Jerry
            // It's set by user and ALL section filters should be the same beucase the TASK will race with POLL
            // Since it's for backward compatible, it's not revised at this time and may be remove in the future.
            if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(TSP_EVENT_SECTION);                                                    // Wake task up
            }
            else if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(TSP_EVENT_SECTION_POLL);                                               // Set POLL flags

            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif
            break;

        case TSP_SWINT_STATUS_BUF_OVFLOW:
            if (!(TSP_FLAG_GetSecOvf(fltid, &dummy, TSP_OS_EVENT_OR))) // ignore redundant overflow
            {
                //TSP_ISR_Print("TSP_SWINT_STATUS_PKT_OVFLOW  , Flt = %ld, Buf = %ld , PID= %04lX\n",fltid,_ptsp_res->_tspInfo.Flt2Buf[fltid],_ptsp_res->_tspInfo.Pid[fltid]);
                if ( (E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[fltid]) )
                {
                    TSP_FLAG_SetFltDis(fltid);                                                          // Set section event flag
                    if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                        TSP_SetEvent(TSP_EVENT_DISABLEFLT);

                }

                TSP_FLAG_SetSecOvf(fltid);             // Set section event flag
                if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                {
                    TSP_SetEvent(TSP_EVENT_SECTION);                                                    // Wake task up
                }
                else if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                {
                   TSP_SetEvent(TSP_EVENT_SECTION_POLL);                                               // Set POLL flags
                }
            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            MDrv_TSP_SEC_SetEvent(fltid, DEMUX_EVENT_DATA_DISCARDED);
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif
            break;

        case TSP_SWINT_STATUS_PKT_OVRUN:
            TSP_ISR_Print("TSP_SWINT_STATUS_PKT_OVRUN [%ld] \n",fltid);
            break;

        case TSP_SWINT_STATUS_SEC_ERROR:
        case TSP_SWINT_STATUS_DEBUG:
            //[ISR print]
//            TSP_OS_Print("%s[%04d]: 0x%08X\n", __FILE__, __LINE__, status);
            break;

        default:
            //[ISR print]
//            TSP_OS_Print(OSAL_TSP_Print("Unknown TSP interrupt %d\n", u32EngId));
            break;
        }
    }

CLEAR_INT:

    _TSP_INT_LOCK();
    if(hw_int)
    {
        HAL_TSP_INT_ClrHW(hw_int >> 8); // @F_TODO: it's changed from U4 make sure we clr them all
    }
    if(sw_int)
    {
        HAL_TSP_INT_ClrSW();
    }
    _TSP_INT_UNLOCK();

#ifdef MSOS_TYPE_LINUX_KERNEL
    return IRQ_HANDLED;
#else
    TSP_OS_IntEnable();
#endif
}


static void _TSP_POLL_Proc(MS_U32 u32Events, MS_U32 u32FltId, TSP_Event *pEvent)
{
    MS_U32              flags; // dummy
//    int                 i;

#if __LEGACY__
    if ( (TSP_EVENT_PVR0_RDY_POLL | TSP_EVENT_PVR1_RDY_POLL) & u32Events )
    {
        _TSP_Proc_Pvr(u32Events, E_TSP_EVENT_CB_POLL);
    }
#endif

/*  [Eddie] Disable filter at ISR_Task
    // Check any possible filter should be disabled (POLL)
    for (i = 0; i < TSP_PIDFLT_NUM; i++)
    {
        if (TSP_FLAG_GetFltDis(i, &flags, TSP_OS_EVENT_OR_CLEAR))
        {
            TSP_LOCK();
            _TSP_FLT_Disable(i);
            TSP_UNLOCK();
        }
    }
*/
    if (TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId);
        TSP_UNLOCK();
    }

    // Process singl specified filter

    // @FIXME: Jerry
    // Check the behavior between ISR_Proc and _Poll_Proc
    // Why isn't _Proc_SecOvf first than _Proc_SecRdy as _ISR_Proc

    if ((_ptsp_res->_tspInfo.FltState[u32FltId] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[u32FltId] != E_TSP_FLT_STATE_ISR_FREE))
    {
        *pEvent = E_TSP_EVENT_DATA_IDLE;
        if (E_TSP_FAIL == _TSP_PROC_SecRdy(u32FltId, pEvent))
        {
            if (TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR)) // @FIXME: clear events at Proc_SecOvf?
            {
                _TSP_PROC_SecOvf(u32FltId, pEvent);
            }
            else if (TSP_FLAG_GetCrcErr(u32FltId, &flags, TSP_OS_EVENT_OR)) // @Note:  Priority Overflow > CRCError > SecRdy
            {
                _TSP_PROC_CrcErr(u32FltId, pEvent);
            }
        }
    }
}


static void _TSP_HW_ResetState(void)
{
    MS_U32              fltid;

    for (fltid = 0; fltid < TSP_PIDFLT_NUM_ALL; fltid++)
    {
        _TSP_FLT_Disable(fltid);
        _TSP_FLT_Init(fltid); // state => FREE
        _TSP_FLT_Free(fltid);
    }

    for (fltid = 0; fltid < TSP_SECBUF_NUM; fltid++)
    {
        _TSP_SEC_Init(fltid); // state => FREE
        _TSP_SEC_Free(fltid);
    }
}


static void _TSP_SW_ResetState(void)
{
    MS_U32              fltid;

    for (fltid = 0; fltid < TSP_SECFLT_NUM; fltid++)
    {
        _ptsp_res->_tspInfo.BufState[fltid]    = E_TSP_FLT_STATE_NA;
        _ptsp_res->_tspInfo.FltMode[fltid]     = E_TSP_FLT_MODE_CONTI;
        _ptsp_res->_tspInfo.FltEvent[fltid]    = E_TSP_EVENT_DATA_IDLE;
        _ptsp_res->_tspInfo.FltCallback[fltid] = NULL;
    }

    for (fltid = 0; fltid < TSP_PIDFLT_NUM_ALL; fltid++)
    {
        _ptsp_res->_tspInfo.Pid[fltid]         = TSP_PID_NULL;
        _ptsp_res->_tspInfo.FltState[fltid]    = E_TSP_FLT_STATE_NA;
        _ptsp_res->_tspInfo.FltType[fltid]     = E_TSP_FLT_USER_NULL;
    }

}


static TSP_Result _TSP_Init(MS_BOOL bResetSW, MS_BOOL bRestoreHW)
{
    int             i;

    HAL_TSP_Power(1);//@F_TODO not implement yet disable unused clk in TSP compare with K2

    // Diable interrupt
    HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8));//@F_TODO need to check what it really disabled.... 0x6c????

    // IntDisable
    // IntDetach
    TSP_OS_IntDisable();
    TSP_OS_IntDetach();

    //@F_TODO DISABLE tsif 0~3
    for (i = 0; i < TSP_TSIF_NUM; i++)
    {
        HAL_TSP_SetTSIF(i, E_TSP_TSIF_DIS, FALSE);
    }

    //HAL_TSP_HW_Lock_Init();//@NOTE HW LOCK is a mechenism to use regiter to protect register modify race between HK and TSP CPU

    HAL_TSP_Reset(1);
    HAL_TSP_Reset(0); // indirect access activated, WB reset, CMDQ reset
//    HAL_TSP_HWPatch();

    HAL_TSP_CPU(0);
    HAL_TSP_INT_ClrSW();
    HAL_TSP_INT_ClrHW(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)); // @F_TODO: need to check this step with Stephen for which interrup to clear

    HAL_TSP_HwPatch(); // @F_TODO no source yet
    //HAL_TSP_SetCAPath(TSP_CA_ROUTE_EN | TSP_CA_OUT_EN | TSP_CA_FLOWCTRL_EN);

/*
    // XXX: Jericho: temp solution to prevent TSP write memory
    HAL_TSP_CPU_WProtect(TRUE, 0, 0);
*/

    if (bResetSW)
    {
        _TSP_SW_ResetState();
    }

    if (bRestoreHW)
    {
        HAL_TSP_RestoreFltState();//@F_TODO not implement yet
    }
    else
    {
        _TSP_HW_ResetState();
    }

    if (TRUE != HAL_TSP_SetVQ(_ptsp_res->_tspInfo.Param.phyVQAddr, _ptsp_res->_tspInfo.Param.u32VQSize))
    {
        printf("TSP VQ is not aligned\n");
        return E_TSP_FAIL;
    }

    if (TRUE != HAL_TSP_LoadFW(_ptsp_res->_tspInfo.Param.phyFWAddr, _ptsp_res->_tspInfo.Param.u32FWSize))
    {
        printf("TSP firmware is not aligned\n");
        return E_TSP_FAIL;
    }

    HAL_TSP_CPU(1);
#if defined (MSOS_TYPE_LINUX_KERNEL)
    mdelay(3);
#else
    MsOS_DelayTask(3);
#endif

    while (FALSE == HAL_TSP_HCMD_Alive())
    {
        TSP_OS_Print("Reset TSP CPU fail, Retry.\n");
        HAL_TSP_CPU(0);
        HAL_TSP_LoadFW(_ptsp_res->_tspInfo.Param.phyFWAddr, _ptsp_res->_tspInfo.Param.u32FWSize);
        HAL_TSP_CPU(1);
        #if defined (MSOS_TYPE_LINUX_KERNEL)
            mdelay(3);
        #else
            MsOS_DelayTask(3);
        #endif
    }
    TSP_OS_Print("Reset TSP CPU Success.\n");
    TSP_OS_Print(">> FwVersion:%08lX , Date:%08lX \n",HAL_TSP_HCMD_GetInfo(INFO_FW_VERSION),HAL_TSP_HCMD_GetInfo(INFO_FW_DATE));

    HAL_TSP_INT_ClrSW();
    HAL_TSP_INT_ClrHW(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)); // @FIXME : it's changed from U4 we need to make sure kaiser is not chaged

    // Implicity MDrv_TSP_Reset()
#if 1
    for(i=0; i<4; i++)  //@F_TODO modify for magic number or move or move the HW related behavior into hal layer
    {
        switch(i)
        {
            case 0:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF0, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case 1:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF1, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case 2:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF2, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case 3:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF3, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            default:
                break;
        }
    }
#else
   HAL_TSP_File_SetDelay(0x0A);                                          // Set TsDma delay, //jyliu.tsp, how come the value?
#endif

    // STC control by TSP need indirect R/W enable
    HAL_TSP_STC_Init();
   // HAL_TSP_SetSTCSynth(STC_SYNTH_DEFAULT); // @NOTE this is done in HAL_TSP_STC_Init


    TSP_OS_IntAttach((void*)_TSP_ISR);
    TSP_OS_IntEnable();

        // [URANUS] Richard: Refine the interrupt selection later.
    HAL_TSP_INT_Enable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8));                                     // Enable TSP hardware interrupt

    if (_ptsp_res->_tspInfo.bSWInit)
    {
        return E_TSP_OK;
    }

#if defined(__LEGACY__)
    _ptsp_res->_PvrEvtNotify = E_TSP_EVENT_DATA_IDLE;
    _ptsp_res->_PvrCallback = NULL;
#endif

    _ptsp_res->_s32MutexId = TSP_OS_MutexCreate((MS_U8*)"TSP_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _ptsp_res->_s32LockId =  TSP_OS_MutexCreate((MS_U8*)"TSP_Ind_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _ptsp_res->_s32IntRegMutexId =  TSP_OS_MutexCreate((MS_U8*)"TSP_Int_Mutex");
    _ptsp_res->_s32PvrMutexId = TSP_OS_MutexCreate((MS_U8*)"TSP_Pvr_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Pvr_Mutex");
    _s32EventId = TSP_OS_EventCreate((MS_U8*)"TSP_Event"); // MsOS_CreateEventGroup("TSP_Event");

    _ptsp_res->_tspInfo.bISRTaskEn = TRUE;
    _ptsp_res->_s32TaskId =  TSP_OS_TaskCreate(_TSP_ISR_Task, (MS_U8*)"TSP_ISR_Task");

    if ( (_ptsp_res->_s32MutexId == -1) || (_ptsp_res->_s32LockId == -1) || (_ptsp_res->_s32PvrMutexId == -1) || (_s32EventId == -1) || (_ptsp_res->_s32TaskId == -1) )
    {
        return E_TSP_FAIL;
    }

    for (i = 0; i < ((TSP_PIDFLT_NUM_ALL+31)>>5); i++)
    {
        _s32SecRdyFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_SecRdy");
        if(_s32SecRdyFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32SecRdyFlags[%d] failed", __LINE__, i); }

        _s32CrcErrFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_CrcErr");
        if(_s32CrcErrFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32CrcErrFlags[%d] failed", __LINE__, i); }

        _s32SecOvfFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_SecOvf");
        if(_s32SecOvfFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32SecOvfFlags[%d] failed", __LINE__, i); }

        _s32FltDisFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_FltDis");
        if(_s32FltDisFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32FltDisFlags[%d] failed", __LINE__, i); }

        _s32FreeFlags[i]   = TSP_OS_EventCreate((MS_U8*)"TSP_FltFree");
        if(_s32FreeFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32FreeFlags[%d] failed", __LINE__, i); }

        _s32DoneFlags[i]   = TSP_OS_EventCreate((MS_U8*)"TSP_FltDone");
        if(_s32DoneFlags[i] == -1)
        { TSP_DBG(0, "[%04d] Create _s32DoneFlags[%d] failed", __LINE__, i); }

        _s32OvfRstFlags[i]= 0 ;
        _s32ModeFlags[i] = 0 ;

        _u32SecNotifyId[i] = 0;

    }

    _ptsp_res->_tspInfo.Status  = E_TSP_SW_INIT;
    _ptsp_res->_tspInfo.bSWInit = TRUE;
    _ptsp_res->_tspInfo.LastErr = E_TSP_OK;

    // Default Live Path as TSIF0
    _ptsp_res->_Current_Live = E_TSP_FLT_SRC_LIVE0;
    _ptsp_res->_Current_PVR = 0 ;

#ifdef MSOS_TYPE_LINUX_KERNEL
    for (i = 0; i < TSP_SECFLT_NUM; i++)
    {
        init_waitqueue_head(&tsp_wq_head[i]);
    }
    memset(_u32SecEvent, 0, sizeof(_u32SecEvent));
#endif

    return E_TSP_OK;

}


static void _TSP_Exit(void)
{
    int                 i;

    HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8));

    TSP_OS_IntDisable();
    TSP_OS_IntDetach();

    for (i = 0; i < TSP_TSIF_NUM; i++) //@FIXME is this enough?
    {
        HAL_TSP_SetTSIF(i, E_TSP_TSIF_DIS, FALSE);
    }
    HAL_TSP_CPU(0);
    HAL_TSP_Reset(1);
    HAL_TSP_Power(0);
}

static void _TSP_SW_Exit(void)
{
    int                 i;

    if(_ptsp_res->_s32MutexId != -1)
    {
        TSP_OS_MutexDelete(_ptsp_res->_s32MutexId);
        _ptsp_res->_s32MutexId = -1;
    }
    if(_ptsp_res->_s32LockId != -1)
    {
        TSP_OS_MutexDelete(_ptsp_res->_s32LockId);
        _ptsp_res->_s32LockId = -1;
    }
    if(_ptsp_res->_s32IntRegMutexId != -1)
    {
        TSP_OS_MutexDelete(_ptsp_res->_s32IntRegMutexId);
        _ptsp_res->_s32IntRegMutexId = -1;
    }
    if(_ptsp_res->_s32PvrMutexId != -1)
    {
        TSP_OS_MutexDelete(_ptsp_res->_s32PvrMutexId);
        _ptsp_res->_s32PvrMutexId = -1;
    }
    if(_s32EventId != -1)
    {
        TSP_OS_EventDelete(_s32EventId);
        _s32EventId = -1;
    }
    for (i = 0; i < ((TSP_PIDFLT_NUM_ALL+31)>>5); i++)
    {
        if(_s32SecRdyFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32SecRdyFlags[i]);
            _s32SecRdyFlags[i] = -1;
        }
        if(_s32CrcErrFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32CrcErrFlags[i]);
            _s32CrcErrFlags[i] = -1;
        }
        if(_s32SecOvfFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32SecOvfFlags[i]);
            _s32SecOvfFlags[i] = -1;
        }
        if(_s32FltDisFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32FltDisFlags[i]);
            _s32FltDisFlags[i] = -1;
        }
        if(_s32FreeFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32FreeFlags[i]);
            _s32FreeFlags[i] = -1;
        }
        if(_s32DoneFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32DoneFlags[i]);
            _s32DoneFlags[i] = -1;
        }

    }

    _ptsp_res->_tspInfo.bISRTaskEn = FALSE;
    if(_ptsp_res->_s32TaskId != -1)
    {
        TSP_OS_TaskDelete(_ptsp_res->_s32TaskId);
        _ptsp_res->_s32TaskId = -1;
    }

    _ptsp_res->_tspInfo.bSWInit = FALSE;
}


static MS_U32 _TSP_DmxFlow_To_PVREng(DRV_TSP_FLOW DmxFlow)
{
    switch (DmxFlow)
    {
        case E_DRV_TSP_FLOW_PVR0:
            return 0;
            break;
        case E_DRV_TSP_FLOW_PVR1:
            return 1;
            break;
        case E_DRV_TSP_FLOW_PVR2:
            return 2;
            break;
        case E_DRV_TSP_FLOW_PVR3:
            return 3;
            break;
        case E_DRV_TSP_FLOW_RASP0:
            return (PVR_NUM);       // @NOTE we arrange rasp eng seq after PVR eng Please follow this rule
            break;                  //             we had better not to use RASP to record if eng is sufficient
        case E_DRV_TSP_FLOW_RASP1:
            return (PVR_NUM+1);
            break;
        default:
            return 0xFFFFFFFF;
            break;
    }

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pu32ResMemAddr                \b IN: Pointer to store resource memory address
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_InitLibResource(void *pu32ResMemAddr)
{
    TSP_CHECK((pu32ResMemAddr != 0),
        "[TSP_ERROR][%s][%06d] pointer is NULL!\n", __FUNCTION__, __LINE__);

    _ptsp_res = (TSP_RESOURCE_PRIVATE*)pu32ResMemAddr;

    if(_TSP_InitResource(_ptsp_res) == FALSE)
        return E_TSP_FAIL;

    return E_TSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSP driver and TSP engine
/// @return TSP_Result
/// @note
/// It should be called before calling any other TSP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Init(TSP_InitParam *pParam)
{
    MS_U32              u32Base, u32Size,*u32Check;
    TSP_Result          ret;

    // MDrv_DSCMB_Init(); // sometimes good/bad // random
    MDrv_MMIO_Init();
#if 1
    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_SetBank(u32Base);
    TSP_DBG(5, "MMIO(MS_MODULE_PM) 0x%08lx\n", u32Base);
#else
    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_TSP))
    {
        TSP_OS_Print("Get MS_MODULE_TSP fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_RegMap(u32Base);
    TSP_DBG(5, "MMIO(MS_MODULE_TSP) 0x%08lx\n", u32Base);

    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_HW fail\n");
        MS_ASSERT(0);
    }
    // MDrv_DSCMB_Init(); // sometimes good/bad // random
    HAL_TSP_HW_RegMap(u32Base);
    TSP_DBG(5, "MMIO(MS_MODULE_HW) 0x%08lx\n", u32Base);


    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_PM))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_PM_RegMap(u32Base);
    TSP_DBG(5, "MMIO(MS_MODULE_PM) 0x%08lx\n", u32Base);
#endif

    printf("************************ Start TSP_Init ************************\n");

    u32Check = (MS_U32*)MsOS_PA2KSEG1(pParam->phyFWAddr);
    if (strcmp((char*)u32Check,FWTSP_CHECK_CODE) != 0 )
    {
        TSP_OS_Print("**** [TSP Firmware] Check Fail ****\n");
    }
    TSP_OS_Print(" %s : ",(char*)u32Check);

    memcpy(&(_ptsp_res->_tspInfo.Param), pParam, sizeof(TSP_InitParam));
    if (E_TSP_OK != (ret = _TSP_Init(TRUE, FALSE)))
    {
        printf("************************   TSP [FAIL]   ************************\n");
        return ret;
    }
    // MDrv_DSCMB_Init(); // bad

    printf("************************* End TSP_Init *************************\n");


    //[NOTE] Switch Uart0 port to TSP
    #if SWITCH_TSP_UART
    _TSP_Switch_Uart();
    #endif

    return E_TSP_OK;
}





//-------------------------------------------------------------------------------------------------
/// Close TSP driver and reset software/register state
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Exit(void)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    MS_U32 u32Size=0;
    MS_U32 u32Base=0;

    printf("============[%s] TSP Exit Start ==============\n",__FUNCTION__);

    if (_ptsp_res->_s32MutexId != -1 )
    {
    TSP_ENTRY();
    }

    if (_ptsp_res->_s32PvrMutexId != -1 )
    {
    TSP_PVR_ENTRY();
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_SetBank(u32Base);
    TSP_DBG(5, "MMIO(MS_MODULE_PM) 0x%08lx\n", u32Base);


    if (!HAL_TSP_HCMD_Alive())
    {
        HAL_TSP_Power(1);
        HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8));

        //HAL_TSP_HW_Lock_Init(); //@F_TODO HW lock is a mechanism we use to avoid race between HK and TSP CPU for those register like filter disable and stc but we dont need that anymore

        HAL_TSP_Reset(1);
        HAL_TSP_Reset(0); // indirect access activated, WB reset, CMDQ reset
    }
    _ptsp_res->_tspInfo.Status = E_TSP_SW_EXIT;

    // SW driver state
    _TSP_SW_ResetState();
    // HW filter state
    _TSP_HW_ResetState();

    _TSP_Exit();
    _TSP_SW_Exit();

    printf("============[%s] TSP Exit End ============\n",__FUNCTION__);

    return E_TSP_OK;
}


//-------------------------------------------------------------------------------------------------
/// Poweroff TSP driver and TSP engine
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PowerOff(void)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    _ptsp_res->_tspInfo.Status = E_TSP_SW_POWEROFF;

    _TSP_Exit();
    _TSP_SW_Exit();

    return E_TSP_OK;
}


///[RESERVED]
TSP_Result MDrv_TSP_Reset(void)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    if(_ptsp_res->_tspInfo.bSWInit != TRUE)
    {
        TSP_DBG(0, "[%s][%d] Please call MDrv_TSP_Init() first\n", __FUNCTION__, __LINE__);
        return E_TSP_FAIL;
    }

    TSP_ENTRY();

    _TSP_Init(TRUE, FALSE);

    TSP_RETURN(E_TSP_OK);

}


//[RESERVED]
TSP_Result MDrv_TSP_Suspend(void)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    _ptsp_res->_tspInfo.Status = E_TSP_SW_SUSPEND;

    HAL_TSP_SaveFltState();

    _TSP_Exit();
    _TSP_SW_Exit();

    _ptsp_res->_tspInfo.bSuspend = TRUE;

    TSP_RETURN(E_TSP_OK);
}


//[RESERVED]
TSP_Result MDrv_TSP_Resume(MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
    TSP_Result          ret;

    TSP_DBG(7, "%s\n", __FUNCTION__);

    // Reset TSP register
    HAL_TSP_Reset(1);
    HAL_TSP_Reset(0);
    HAL_TSP_HwPatch();
    MsOS_DelayTask(500);

    _ptsp_res->_tspInfo.Status = E_TSP_SW_RESUME;
    _ptsp_res->_tspInfo.bSuspend = FALSE;

    TSP_ENTRY();

    ret = _TSP_Init(FALSE, TRUE);

    TSP_RETURN(ret);
}


TSP_TSId _TSP_SRCSEQ_To_TSID(TSP_SRC_SEQ pktDmxId)
{
    switch (pktDmxId)
    {
        case E_TSP_SRC_PKTDMX0:
            return E_TSP_TSID_TSIF0;
            break;
        case E_TSP_SRC_PKTDMX1:
            return E_TSP_TSID_TSIF1;
            break;
        case E_TSP_SRC_PKTDMX2:
            return E_TSP_TSID_TSIF01_FILE;
            break;
        case E_TSP_SRC_PKTDMX3:
            return E_TSP_TSID_TSIF2;
            break;
        case E_TSP_SRC_PKTDMX4:
            return E_TSP_TSID_TSIF3;
            break;
        case E_TSP_SRC_PKTDMX5:
            return E_TSP_TSID_TSIF23_FILE;
            break;
        case E_TSP_SRC_MMFI0:
            return E_TSP_TSID_MMFI0;
            break;
        case E_TSP_SRC_MMFI1:
            return E_TSP_TSID_MMFI1;
            break;
        default:
            return E_TSP_TSID_INVALID;
            break;
    }

}



//get ts_if of DmxFlow
TSP_Result MDrv_TSP_GetFlowSource(DRV_TSP_FLOW DmxFlow, TSP_TSId *eTid)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_SRC_SEQ pktDmxId = E_TSP_SRC_INVALID;
    *eTid = E_TSP_TSID_INVALID;

    //get eTid
    if ((DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1))
    {
        if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0)
        {
            HAL_TSP_FIFO_GetSrc(E_TSP_DST_FIFO_VIDEO,&pktDmxId);
            *eTid = _TSP_SRCSEQ_To_TSID(pktDmxId);
        }
        else if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1)
        {
            HAL_TSP_FIFO_GetSrc(E_TSP_DST_FIFO_VIDEO3D,&pktDmxId);
            *eTid = _TSP_SRCSEQ_To_TSID(pktDmxId);
        }
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_PVR0) || (DmxFlow == E_DRV_TSP_FLOW_PVR1) || (DmxFlow == E_DRV_TSP_FLOW_PVR2) || (DmxFlow == E_DRV_TSP_FLOW_PVR3))
    {
        MS_U32 u32PvrEng = _TSP_DmxFlow_To_PVREng(DmxFlow);
        HAL_PVR_GetEngSrc(u32PvrEng,&pktDmxId);
        *eTid = _TSP_SRCSEQ_To_TSID(pktDmxId);
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_RASP0) || (DmxFlow == E_DRV_TSP_FLOW_RASP1))
    {
        //@F_TODO not implement yet
        //@F_TODO remember to change RASP SRC to file
        return(E_TSP_FAIL);
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_TSO0) || (DmxFlow == E_DRV_TSP_FLOW_TSO1))
    {
        //@F_TODO not implement yet
        //@F_TODO remember to change TSO src to pktdmx
        return(E_TSP_FAIL);
    }
    else
    {
        // @FIXME add error msg
        return(E_TSP_FAIL);
    }

    return(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_SetFlowSource(MS_U32 u32Eng, MS_U32 u32PvrSrc)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32Tid = 0;
    TSP_PIDFLT_SRC ePidFltSrc = E_TSP_PIDFLT_INVALID;
    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & u32PvrSrc;
    MS_BOOL bRASPSrcPktDmx = FALSE;
    switch(u32FltSrc)
    {
        case E_TSP_FLT_SRC_LIVE0:
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            ePidFltSrc = E_TSP_PIDFLT_LIVE1;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            ePidFltSrc = E_TSP_PIDFLT_LIVE2;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            ePidFltSrc = E_TSP_PIDFLT_LIVE3;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_FILE0:
            ePidFltSrc = E_TSP_PIDFLT_FILE0;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_FILE1:
            ePidFltSrc = E_TSP_PIDFLT_FILE1;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_FILE2:
            ePidFltSrc = E_TSP_PIDFLT_FILE2;
            bRASPSrcPktDmx = TRUE;
            break;
        case E_TSP_FLT_SRC_FILE3:
            ePidFltSrc = E_TSP_PIDFLT_FILE3;
            bRASPSrcPktDmx = TRUE;
            break;

        default:
            TSP_DBG(TRUE, "[%04d] Filter input source is not assigned !! \n", __LINE__);
            ePidFltSrc = TSP_PIDFLT_TSIF0;
            break;
    }

    u32Tid = HAL_TSP_FltSrc2PktDmx(ePidFltSrc);
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        HAL_PVR_Init(u32Eng, u32Tid);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_U32 u32RASPEng = u32Eng - E_TSP_PVR_RASPENG_START;
        HAL_RASP_Set_Source(u32RASPEng, u32Tid);
        MDrv_RASP_PVR_FileinEnable(u32RASPEng, bRASPSrcPktDmx);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SetFlowSource(DRV_TSP_FLOW DmxFlow, TSP_TSId eTid)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    // @F_TODO add another layer for adaption, concider for flexibility
    // @NOTE PVR has no source mmfi take care
    // @NOTE setting FIFO SRC is moved to MDrv_TSP_FLT_Enable()
    // @NOTE because setting flowset playback may be for record...
    if ((DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK2) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK3))
    {
        /*
        if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0)
        {
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO, eTid); //@FIXME change pktdmx id don't use drv.h enum as parameter to hal layer
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO, eTid);
        }
        else if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1)
        {
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO2 , eTid); //@FIXME change pktdmx id don't use drv.h enum as parameter to hal layer
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO3D, eTid);
        }
        */
        TSP_RETURN(E_TSP_OK);
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_PVR0) || (DmxFlow == E_DRV_TSP_FLOW_PVR1) || (DmxFlow == E_DRV_TSP_FLOW_PVR2) || (DmxFlow == E_DRV_TSP_FLOW_PVR3))
    {
        MS_U32 u32pktDmxId = (MS_U32)eTid;
        MS_U32 u32Eng = _TSP_DmxFlow_To_PVREng(DmxFlow);
        TSP_SRC_SEQ ePktDmx = HAL_TSP_Eng2PktDmx(u32Eng);
        switch(ePktDmx)
        {
            case E_TSP_SRC_PKTDMX0:
                u32pktDmxId = 0;
                break;
            case E_TSP_SRC_PKTDMX1:
                u32pktDmxId = 1;
                break;
            case E_TSP_SRC_PKTDMX2:
                u32pktDmxId = 2;
                break;
            case E_TSP_SRC_PKTDMX3:
                u32pktDmxId = 3;
                break;
            case E_TSP_SRC_PKTDMX4:
                u32pktDmxId = 4;
                break;
            case E_TSP_SRC_PKTDMX5:
                u32pktDmxId = 5;
                break;
            default:
                u32pktDmxId = 0;
                printf("[TSP_ERR][%s][%d] Wrong PktDmx type!!!\n",__FUNCTION__,__LINE__);
                break;
        }

        if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
        {
            HAL_PVR_Init(u32Eng, u32pktDmxId);
        }
        else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
        {
            MS_U32 u32RASPEng = u32Eng - E_TSP_PVR_RASPENG_START;
            HAL_RASP_Set_Source(u32RASPEng, u32pktDmxId);
            MDrv_RASP_PVR_FileinEnable(u32RASPEng, TRUE);
        }
        /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
        {
        }*/
        else
        {
            printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
        }
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_RASP0) || (DmxFlow == E_DRV_TSP_FLOW_RASP1))
    {//this condition is for the purpose that if RASP & PVR are going to be distinguished by AP in the future
        //@F_TODO not implement yet
        //@F_TODO remember to change RASP SRC to file
        TSP_RETURN(E_TSP_FAIL);
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_TSO0) || (DmxFlow == E_DRV_TSP_FLOW_TSO1))
    {
        //@F_TODO not implement yet
        //@F_TODO remember to change TSO src to pktdmx
        TSP_RETURN(E_TSP_FAIL);
    }
    else
    {
        // @FIXME add error msg
        TSP_RETURN(E_TSP_FAIL);
    }


    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Select ChipTop PAD for ts0 and ts1
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eIf                     \b IN: TSP TS interface
/// @param  pIfSet                  \b IN: TS interface setting
/// @return TSP_Result
/// @note
/// Uranus: only can select pad0/pad1 to ts0
/// Oberon: can select pad0/pad1 to ts0, pad0/pad1 to ts1.
///         Default setting was pad0->ts0, pad1->ts1.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad(TSP_TSIF eTSIF, TSP_TSPad eTSPad)
{
    TSP_Result          ret;
    TSP_TS_PAD          ePad = E_TSP_TS_PAD_INVALID;
    MS_U32 u32TSIF = 0;

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    ePad = _DrvHALPadMapping(eTSPad);

    TSP_HAL_TSIF eHalTSIF = _DrvHALTSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);
    if (HAL_TSP_TSIF_SelPad(u32TSIF, ePad))
    {
        ret = E_TSP_OK;
    }
    else
    {
        ret = E_TSP_FAIL;
    }

    TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
///// Set TSP MUX Clk Invert mode
///// @param  u16TSIF                \b IN: TS MUX Index
///// @param  bClkInv                \b IN: Clock Invert
///// @return TSP_Result
///// @note
///// Set TSP PAD Clk gen invert if the bClkInv is TRUE
///// u16TSIF = TSMux0 , TSMux1 ,TSMux2
/////
///// @sa
////-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad_ClkInv(TSP_TSIF eTSIF ,MS_BOOL bClkInv)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _DrvHALTSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);
    HAL_TSP_TSIF_SelPad_ClkInv(u32TSIF , bClkInv);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
///// Set TSP MUX Clk disable mode
///// @param  u16TSIF                \b IN: TS MUX Index
///// @param  bClkdis                \b IN: Clock Disable
///// @return TSP_Result
///// @note
///// Set TSP PAD Clk gen disabled if the bClkDis is TRUE
///// u16TSIF = TSMux0 , TSMux1 ,TSMux2
/////
///// @sa
////-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad_ClkDis(TSP_TSIF eTSIF ,MS_BOOL bClkDis)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _DrvHALTSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);
    HAL_TSP_TSIF_SelPad_ClkDis(u32TSIF , bClkDis);

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_TSO_ConfigPad(MS_U32 u32TSOEng, TSP_TSPad eTSPad)
{
    TSP_Result eRet = E_TSP_OK;
    TSP_TS_PAD ePad = E_TSP_TS_PAD_INVALID;

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    HAL_TSO_SetTSOOutMUX(TRUE);

    ePad = _DrvHALPadMapping(eTSPad);
    if(!HAL_TSP_TSO_TSIF_SelPad(u32TSOEng, ePad))
        eRet = E_TSP_FAIL;

    TSP_RETURN(eRet);
}

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
/// @sa MDrv_TSP_PidFlt_Alloc, MDrv_TSP_PidFlt_Enable, E_TSP_FLT_TYPE_AUDIO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetTSIF(TSP_TSIF eTSIF, TSP_TSIFCfg u32Cfg, MS_BOOL bFileIn)
{
    TSP_Result          ret;
    TSP_TSIF_CFG        cfg = E_TSP_TSIF_CFG_DIS;

    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    if (u32Cfg & E_TSP_TSIF_EN)
    {
        cfg |= E_TSP_TSIF_CFG_EN;
    }
    if (u32Cfg & E_TSP_TSIF_PARL)
    {
        cfg |= E_TSP_TSIF_CFG_PARA;
    }
    if (u32Cfg & E_TSP_TSIF_EXTSYNC)
    {
        cfg |= E_TSP_TSIF_CFG_EXTSYNC;
    }
    if (u32Cfg & E_TSP_TSIF_CFG_BITSWAP)
    {
        cfg |= E_TSP_TSIF_CFG_BITSWAP;
    }

    TSP_HAL_TSIF eHalTSIF = _DrvHALTSIFMapping(eTSIF);
    MS_U32 u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, bFileIn);
    if (HAL_TSP_SetTSIF(u32TSIF, cfg, bFileIn))
    {
        ret = E_TSP_OK;
    }
    else
    {
        ret = E_TSP_FAIL;
    }

    TSP_RETURN(ret);
}

/*
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, TSP_CtrlMode eCtrlMode) // @FIXME
{
    TSP_ENTRY();

    if (u32EngId >= TSP_ENGINE_NUM)
    {
        TSP_RETURN(TSP_FAIL);
    }

    if ((HAL_TSP_GetCtrlMode() & TSP_CTRL_TSIF0_FILE_EN) ||
        (eCtrlMode == E_TSP_CTRL_MODE_MEM))
    {
        if (!HAL_TSP_CmdQ_IsEmpty())
        {
            TSP_RETURN(TSP_FAIL);
        }
        HAL_TSP_CmdQ_TsDma_Reset();
    }

    switch (eCtrlMode)
    {
    case E_TSP_CTRL_MODE_TS0:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_SetCA(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE); //|TSP_CA_AVPAUSE);
#if 0
        HAL_TSP_DoubleBuf_Disable();
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);
#else
        HAL_TSP_SetTSIF(0, TSP_TSIF0_EN  | TSP_TSIFx_EXTSYNC | TSP_TSIFx_PARL, FALSE);
#endif
        break;
    case E_TSP_CTRL_MODE_TS1:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_DoubleBuf_Disable();
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 1);
        break;
    case E_TSP_CTRL_MODE_MEM:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_DoubleBuf_En(DOUBLE_BUFFER_SWITCH);        // [OBERON]:Enable double buffer, set filein->pinpon livein->single
        HAL_TSP_SetCA(TSP_CA_INPUT_TSIF0_FILEIN|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_AVPAUSE);
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST| TSP_CTRL_TSIF0_FILE_EN, 2);
        break;
    default:
        TSP_RETURN(TSP_FAIL);
        break;
    }

    TSP_RETURN(TSP_OK);

}
*/

//-------------------------------------------------------------------------------------------------
// Ask TSP to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @OBSOLETED
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return TSP_Result
// @note
// It's only for backward compatible
// @seealso
// MDrv_TSP_FLT_SetCallback
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Proc(MS_U32 u32TSPId, MS_U32 u32FltId, TSP_Event* pEvent)                        // for non-OS TSP scheduling
{
    MS_U32              events = 0;

    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);


    TSP_ENTRY();

    if (_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_NA)
    {
        TSP_RETURN(E_TSP_FAIL);
    }

    if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_OS_Print("[%s] Flt %ld is CallBack Mode Filter, Polling this filter will cause unexpected result !!\n",__FUNCTION__,u32FltId);
        //*pEvent = E_TSP_EVENT_DATA_IDLE ;
        //TSP_RETURN(E_TSP_FAIL_NODATA);
    }// Driver do not support Callback mode and Polling mode coexistance at the same filter.


    // @FIXME Jerry
    // For legacy driver compatibility, it call polling and hopes polling will call it back in the same context.
    // Does this kind of misbehavior still live?
    TSP_UNLOCK();
    // @NOTE Jerry
    // Clear poll events directly in case there has any incoming event before _TSP_ISR_Proc to clear
    TSP_GetEvent(&events, TSP_POLL_EVENTS, TSP_OS_EVENT_OR_CLEAR, 0);
    _TSP_POLL_Proc(events, u32FltId, pEvent);
    TSP_LOCK();

    if (*pEvent == E_TSP_EVENT_DATA_IDLE )
    {
        TSP_RETURN(E_TSP_FAIL_NODATA);
    }
    else
    {
         TSP_RETURN(E_TSP_OK);
    }
}


//-------------------------------------------------------------------------------------------------
/// Allocate a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: type of PID filter to be allocated
/// @param  pu32PidFltId            \b OUT: pointer of PID filter id return
/// @return TSP_Result
/// @note
/// These filter types have to select a section filter\n
/// @ref E_TSP_FLT_TYPE_SECTION\n
/// @ref E_TSP_FLT_TYPE_PCR\n
/// @ref E_TSP_FLT_TYPE_PES\n
/// @note
/// These filter types also have to setup section buffer for data output\n
/// @ref E_TSP_FLT_TYPE_SECTION\n
/// @ref E_TSP_FLT_TYPE_PES\n
/// @sa MDrv_TSP_PidFlt_SelSecFlt, MDrv_TSP_SecFlt_SetBuffer
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_Alloc(MS_U32 u32TSPId, TSP_FltType eFltType, MS_U32 *pu32FltId)
{
    // @NOTE We do nothing when create dmx flt to record
    if( E_TSP_FLT_FIFO_REC == (eFltType & E_TSP_FLT_FIFO_MASK) )
    {
        *pu32FltId = HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX);
        return E_TSP_OK;
    }

    REG_SecFlt *        pSecFlt=0;
    MS_U32              type;
    int                 start_id, end_id, i;

    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    *pu32FltId = 0xFFFFFFFF;

    if ( E_TSP_FLT_USER_PCR == (eFltType & E_TSP_FLT_USER_MASK) )
    {
        start_id = TSP_PCRFLT_START_ID;
        end_id   = TSP_PCRFLT_END_ID;
    }
    else if( (E_TSP_FLT_USER_SEC == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_PES == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_PKT == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_TTX == (eFltType & E_TSP_FLT_USER_MASK)) )
    {
        start_id = 0;
        end_id   = TSP_SECFLT_NUM;
    }
    else
    {
        end_id = 0;
#if 0
        if (E_TSP_FLT_FLAG_CA & eFltType)
        {
            start_id = TSP_CAFLT_0;
        }
        else
        {
            start_id = 0;
        }
#else
        start_id = TSP_PIDFLT_REC_NUM;
#endif
    }

    if(NEED_SEC_FILTER(eFltType))
    {
        for (i = start_id; i < end_id; i++) // check which filter is free
        {
            if (_ptsp_res->_tspInfo.FltState[i] == E_TSP_FLT_STATE_FREE)
            {
                pSecFlt = &(_REGSec->Flt[i]);
                if (HAL_TSP_SecFlt_TryAlloc(pSecFlt, (MS_U16)u32TSPId))
                {
                    break;
                }
            }
        }
        if (i >= end_id)
        {
            TSP_DBG(0, "[%04d] No free fitler found 0x%02lx\n", __LINE__, (MS_U32)i);
            TSP_RETURN(E_TSP_FAIL);
        }
    }
    else
    {
        for (i = start_id - 1; i >= end_id; i--) // check which filter is free, allocate from back
            if (_ptsp_res->_tspInfo.FltState[i] == E_TSP_FLT_STATE_FREE)
                break;

        if ((TSP_SECFLT_NUM < TSP_PIDFLT_REC_NUM) && (i < TSP_SECFLT_NUM))
            TSP_DBG(0, "[%04d] Warning, PID filter 0x%02lx for section is allocated for non-section usage. \n", __LINE__, (MS_U32)i);

        if (i < end_id)
        {
            TSP_DBG(0, "[%04d] No free fitler found 0x%02lx\n", __LINE__, (MS_U32)i);
            TSP_RETURN(E_TSP_FAIL);
        }
    }


    //[NOTE] Referace the comment of definition of _ptsp_res->_Current_Live.
#if 0    //@F_TODO do we have to support this here?
    if ((eFltType & E_TSP_FLT_SRC_TSIF0) && (_ptsp_res->_Current_Live != E_TSP_FLT_SRC_TSIF0)) // if current live not @ playback channel
    {
        if (FALSE == (E_TSP_FLT_FIFO_MASK & eFltType ))  // For A/V/PCR are always belone to channel playback.
        {
            eFltType = (eFltType & ~E_TSP_FLT_SRC_MASK) | _ptsp_res->_Current_Live ;
        }
    }
#endif

    _TSP_FLT_Init(i);


    _ptsp_res->_tspInfo.FltState[i] = E_TSP_FLT_STATE_ALLOC;
    _ptsp_res->_tspInfo.OwnerId[i]  = (TSP_Id)u32TSPId;
    _ptsp_res->_tspInfo.FltType[i]  = eFltType;

    // Select FLT in
    //pPidFlt = &(_REGPid->Flt[i]);

    TSP_PIDFLT_SRC eTspPidSrc = E_TSP_PIDFLT_INVALID;
    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & eFltType;

    switch(u32FltSrc)
    {
        case E_TSP_FLT_SRC_LIVE0:
            eTspPidSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            eTspPidSrc = E_TSP_PIDFLT_LIVE1;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            eTspPidSrc = E_TSP_PIDFLT_LIVE2;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            eTspPidSrc = E_TSP_PIDFLT_LIVE3;
            break;
        case E_TSP_FLT_SRC_FILE0:
            eTspPidSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_FLT_SRC_FILE1:
            eTspPidSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_FLT_SRC_FILE2:
            eTspPidSrc = E_TSP_PIDFLT_FILE2;
            break;
        case E_TSP_FLT_SRC_FILE3:
            eTspPidSrc = E_TSP_PIDFLT_FILE3;
            break;
        default:
            TSP_DBG(TRUE, "[%04d] Filter %d input source is not assigned !! \n", __LINE__, i);
            eTspPidSrc = TSP_PIDFLT_TSIF0;
            break;
    }

    MS_U32 u32PktDmx = HAL_TSP_FltSrc2PktDmx(eTspPidSrc);

    HAL_TSP_PidFlt_SetFltIn(i,u32PktDmx);


    // Select FLT out
    // MDrv_TSP_FLT_Enable

    // Set FLT_USR Type
    type = E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[i];
    if (type)
    {
        switch (type)
        {
        case E_TSP_FLT_USER_SEC:
        case E_TSP_FLT_USER_PES:
        case E_TSP_FLT_USER_PKT:
        case E_TSP_FLT_USER_TTX:
        case E_TSP_FLT_USER_PCR:
            HAL_TSP_SecFlt_SetType(pSecFlt, type >> E_TSP_FLT_USER_SHFT);
            break;
        // Non-SECFLT allocated : PIDFLT & special FLT only
        case E_TSP_FLT_USER_EMM:
        case E_TSP_FLT_USER_ECM:
            // select no section type;
            HAL_TSP_SecFlt_SetType(pSecFlt, 0);
            break;
#if defined(__LEGACY__)
        case E_TSP_FLT_FIFO_PVR:
        case E_TSP_FLT_USER_OAD:
#endif
        default:
            TSP_DBG(TRUE, "[%04d] Unsupport FlType = %ld\n", __LINE__, type);
            HAL_TSP_SecFlt_SetType(pSecFlt, 0);
            break;
        }
    }

    *pu32FltId = i;
    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Free a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_Free(MS_U32 u32FltId)
{
    //We do nothing when create dmx flt to record
    MS_U32              u32RECFltId = 0;
    u32RECFltId = HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX);
    if(u32FltId == (u32RECFltId))
    {
        return E_TSP_OK;
    }


    MS_U32              flags;

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

// NO Blocking check in blocking TSP_FLT_Free
//    TSP_ENTRY();

    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        return E_TSP_OK ;
    }

    if(u32FltId >= TSP_SECFLT_NUM)
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId); // disable output
        _TSP_FLT_Init(u32FltId); // reset pid & status
        _ptsp_res->_tspInfo.OwnerId[u32FltId]  = E_TSP_NULL;
        _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_FREE;
        _ptsp_res->_tspInfo.FltType[u32FltId] = E_TSP_FLT_USER_NULL;
        TSP_UNLOCK();
        return E_TSP_OK ;
    }

    if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId); // disable output
        _TSP_FLT_Init(u32FltId); // reset pid & status
        _TSP_FLT_Free(u32FltId);
        TSP_UNLOCK();
        TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetSecRdy(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);

    }
    else if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        if (_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_ISR_FREE)
        {
            // it's freeing
        }
        else
        {
            TSP_LOCK();
            _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_ISR_FREE;
            TSP_UNLOCK();

            // Add to FREE list
            TSP_FLAG_SetFree(u32FltId);
        }
        // Wakeup Task
        TSP_SetEvent(TSP_EVENT_FREEFLT);

        //Since MApi_DMX_Close() already has a mechanism to wait free done without continuously blocking the mutex.
        //Here we only return the result of free event is finished or not to MApi_DMX_Close().
        if (FALSE == TSP_FLAG_GetDone(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
        {
            return E_TSP_FAIL;
        }

        //To avoid the filter being allocated during ISR free process,
        //_TSP_FLT_Init and _TSP_FLT_Free are moved from ISR free process to here.
        //During the ISR free process, we use E_TSP_FLT_STATE_ISR_FREE instead of E_TSP_FLT_STATE_FREE to indicate a freeing filter.
        TSP_LOCK();
        _TSP_FLT_Init(u32FltId); // reset pid & status
        _TSP_FLT_Free(u32FltId);
        TSP_UNLOCK();
    }
	else
	{
	    printf("[DMX_ERR][%s][%d] Free flt error!\n", __FUNCTION__,__LINE__);
	}


// NO Blocking check in blocking TSP_FLT_Free
//    TSP_RETURN(E_TSP_OK);
    return E_TSP_OK;
}


//-------------------------------------------------------------------------------------------------
/// Select section filter of PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter\n
/// @param  u32SecFltId             \b IN: index of section filter
/// @return TSP_Result
/// @note
/// The PID filter and section filter pair is one-to-one mapping. User has to
/// allocate other PID filters if user have more than one section filter for same
/// PID packet.\n
/// @sa MDrv_TSP_PidFlt_Alloc
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SelSEC(MS_U32 u32FltId, MS_U32 u32BufId)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecFlt_SelSecBuf(&(_REGSec->Flt[u32FltId]), u32BufId);

    _ptsp_res->_tspInfo.Flt2Buf[u32FltId] = u32BufId;

    _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] += 1;

    _ptsp_res->_tspInfo.Buf2Flt[u32BufId] = u32FltId;
    //printf("[%s] MulFlt2Buf %ld = %d \n",__FUNCTION__ ,u32BufId ,_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId]);

    TSP_RETURN(E_TSP_OK);
}


///RESERVED
TSP_Result MDrv_TSP_FLT_GetSEC(MS_U32 u32FltId, MS_U32 *pu32BufId)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    //u32BufId = _ptsp_res->_tspInfo.Flt2Buf[u32Fltid];
    *pu32BufId = (MS_U32)HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));

    TSP_RETURN(E_TSP_OK);
}
///RESERVED

TSP_Result MDrv_TSP_FLT_SetPCRSrc(MS_U32 u32FltId,MS_U32 u32PCRSrc)
{
    // @TODO need to take care PCR SRC from MM here
    TSP_PCR_SRC ePcrSrc = E_TSP_PCR_SRC_INVALID;
    TSP_PIDFLT_SRC ePidFltSrc = E_TSP_PIDFLT_INVALID;
    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & u32PCRSrc;

    switch(u32FltSrc)
    {
        case E_TSP_FLT_SRC_LIVE0:
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            ePidFltSrc = E_TSP_PIDFLT_LIVE1;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            ePidFltSrc = E_TSP_PIDFLT_LIVE2;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            ePidFltSrc = E_TSP_PIDFLT_LIVE3;
            break;
        case E_TSP_FLT_SRC_FILE0:
            ePidFltSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_FLT_SRC_FILE1:
            ePidFltSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_FLT_SRC_FILE2:
            ePidFltSrc = E_TSP_PIDFLT_FILE2;
            break;
        case E_TSP_FLT_SRC_FILE3:
            ePidFltSrc = E_TSP_PIDFLT_FILE3;
            break;
        default:
            TSP_DBG(TRUE, "[%04d] Filter input source is not assigned !! \n", __LINE__);
            ePidFltSrc = TSP_PIDFLT_TSIF0;
            break;
    }

    ePcrSrc = HAL_FltSrc2PCRSrc(ePidFltSrc);


    if(TSP_PCRFLT_PCR0 == u32FltId)
    {
        HAL_TSP_PcrFlt_SetSrc(0, ePcrSrc);
    }
    else if (TSP_PCRFLT_PCR1 == u32FltId)
    {
        HAL_TSP_PcrFlt_SetSrc(1, ePcrSrc);
    }
    else
    {
        return E_TSP_FAIL;
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_FLT_SetFltRushPass(MS_U32 u32FltId, MS_U8 u8Enable)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_PidFlt_SetFltRushPass(u32FltId, u8Enable);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  u32PID                  \b IN: PID value
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetPID(MS_U32 u32FltId, MS_U32 u32PID)
{
    //We do nothing when create dmx flt to record
    MS_U32              u32RECFltId = 0;
    u32RECFltId = HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX);
    if(u32FltId == (u32RECFltId))
    {
        return E_TSP_OK;
    }

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    _ptsp_res->_tspInfo.Pid[u32FltId] = u32PID ;

    if(u32FltId < TSP_PIDFLT_NUM)
    {
        HAL_TSP_PidFlt_SetPid(u32FltId, u32PID);
    }

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK) == E_TSP_FLT_USER_PCR )
    {
        printf("Set PCR FLT=%ld PID = %8lX \n\n",u32FltId,u32PID);
        if ( TSP_PCRFLT_PCR0 == u32FltId)
        {
            HAL_TSP_PcrFlt_SetPid(0, u32PID);
			//HAL_TSP_PcrFlt_Enable(0, TRUE);
        }
        else if ( TSP_PCRFLT_PCR1 == u32FltId)
        {
            HAL_TSP_PcrFlt_SetPid(1, u32PID);
			//HAL_TSP_PcrFlt_Enable(1, TRUE);
        }
    }

    TSP_RETURN(E_TSP_OK);
}


///RESERVED
TSP_Result MDrv_TSP_FLT_GetPID(MS_U32 u32FltId, MS_U32 *pu32PID)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32PID = HAL_TSP_PidFlt_GetPid(&(_REGPid0->Flt[u32FltId]));

    if (_ptsp_res->_tspInfo.Pid[u32FltId] != *pu32PID)
        TSP_OS_Print("[%s] Sw/Hw conflict => Pid Value mismatch \n",__FUNCTION__);

    TSP_RETURN(E_TSP_OK);
}

///RESERVED


//-------------------------------------------------------------------------------------------------
// Set section filtering mode
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: section filter of TSP to be free
// @param  eSecFltMode             \b IN: continue/one-shot mode of section filter
// @return TSP_Result
// @attention
// One-shot filter has the disadvantage of interrupt lost becuase it stops filter, a timeout
// to check filter status is better for usage.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetMode(MS_U32 u32FltId, TSP_FltMode eFltMode)
{
    MS_U32         uMode = 0;

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    if((eFltMode & E_TSP_FLT_MODE_CRCCHK) && (eFltMode & E_TSP_FLT_MODE_AUTO_CRCCHK))
    {
        TSP_DBG(1, "[%04d] E_TSP_FLT_MODE_CRCCHK and E_TSP_FLT_MODE_AUTO_CRCCHK can't be set together. \n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    if (eFltMode & E_TSP_FLT_MODE_ONESHOT)
    {
        uMode |= TSP_SECFLT_MODE_ONESHOT;
    }

    if (eFltMode & E_TSP_FLT_MODE_CRCCHK)
    {
        uMode |= TSP_SECFLT_MODE_CRCCHK;
    }

    if (eFltMode & E_TSP_FLT_MODE_AUTO_CRCCHK)
        HAL_TSP_SecFlt_SetAutoCRCChk(&(_REGSec->Flt[u32FltId]), TRUE);
    else
        HAL_TSP_SecFlt_SetAutoCRCChk(&(_REGSec->Flt[u32FltId]), FALSE);

    _ptsp_res->_tspInfo.FltMode[u32FltId] = eFltMode;

    HAL_TSP_SecFlt_SetMode(&(_REGSec->Flt[u32FltId]), uMode);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set Match/Mask filter pattern of section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of filter to be set pattern
/// @param  pu8Match                \b IN: pointer of filter pattern (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  pu8Mask                 \b IN: pointer of pattern bitmask (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  bNotMatch               \b IN: negative the result of comparion of filter
/// @return TSP_Result
/// note: match mask -- must set 0 to be compare (customer request)
///       not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetMatch(MS_U32 u32FltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NMask)
{
    static MS_U8        mask[TSP_FILTER_DEPTH];
    int                 i;

    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);
    TSP_CHECK(pu8Match, "[%04d] Invalid Match Pointer\n", __LINE__);
    TSP_CHECK(pu8Mask,  "[%04d] Invalid Mask  Pointer\n", __LINE__);
    TSP_CHECK(pu8NMask, "[%04d] Invalid NMask Pointer\n", __LINE__);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    for (i = 0 ; i < TSP_FILTER_DEPTH ; i++)
    {   // invert the mask for HW specified
        //*((MS_U32*)(mask+(i<<2))) = ~(*((MS_U32*)(pu8Mask+(i<<2))));
         mask[i] = ~pu8Mask[i];
    }

    HAL_TSP_SecFlt_SetMask (&(_REGSec->Flt[u32FltId]), mask);
    HAL_TSP_SecFlt_SetMatch(&(_REGSec->Flt[u32FltId]), pu8Match);
    HAL_TSP_SecFlt_SetNMask(&(_REGSec->Flt[u32FltId]), pu8NMask);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for specified section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_TSP_EVENT_DATA_READY\n
///                                        @ref E_TSP_EVENT_BUF_OVERFLOW
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// This function register a callback function for a section filter to TSP.
/// TSP calls callback function each time when data is ready in section buffer.\n
/// Data ready of section filter:\n
/// @ref E_TSP_FLT_TYPE_SECTION : a section ready\n
/// @ref E_TSP_FLT_TYPE_PES : PES packet ready or received data over than request size.
/// @sa MDrv_TSP_SecFlt_SetReqCount
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetCallback(MS_U32 u32FltId, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_CHECK(TSP_SECFLT_NUM > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_ENTRY();

    if ((eEvents & (E_TSP_EVENT_DATA_READY | E_TSP_EVENT_BUF_OVERFLOW)) == 0)
    {
        TSP_RETURN(E_TSP_FAIL_PARAMETER);
    }

    _ptsp_res->_tspInfo.FltEvent[u32FltId]    = eEvents;
    _ptsp_res->_tspInfo.FltCallback[u32FltId] = pfCallback;

    if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_FLAG_SetModeFlags(u32FltId);
    }

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FLT_Enable(MS_U32 u32FltId, MS_BOOL bEnable)
{
    //We do nothing when create dmx flt to record
    MS_U32              u32RECFltId = 0;
    u32RECFltId = HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX);
    if(u32FltId == (u32RECFltId))
    {
        return E_TSP_OK;
    }

    REG_SecFlt *        pSecFlt = &(_REGSec->Flt[u32FltId]);
    REG_SecBuf *        pSecBuf;
    MS_U32              type;
    TSP_PIDFLT_SRC      ePidFltSrc = E_TSP_PIDFLT_INVALID;
    MS_U32              u32Src;
    MS_U16              u16BufId = 0;
    MS_U32              dummy;

    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    TSP_FLAG_GetFltDis(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);

    if (bEnable) //clear all filter event.
    {
        // MsOS_DelayTask(1); //[Note] For VQ flush,
        TSP_FLAG_GetSecRdy(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetSecOvf(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetCrcErr(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetFltDis(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);

        TSP_LOCK();
        TSP_FLAG_ClrOvfRst(u32FltId);
        TSP_UNLOCK();
    }

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(3, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    // TSP_FLT_Disable
    if (!bEnable)
    {
        if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE) // if enable
        {
            _TSP_FLT_Disable(u32FltId);
        }
        // OSAL_TSP_Print("[TSP_WARNNING][%04d] Enable an active PidFlt %d\n", __LINE__, u32PidFltId);

        TSP_RETURN(E_TSP_OK);
    }

    // TSP_FLT_Enable
    //connect avfifo to pid flt~
    u32Src = E_TSP_FLT_SRC_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];

    switch (u32Src)
    {
        case E_TSP_FLT_SRC_LIVE0:
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            ePidFltSrc = E_TSP_PIDFLT_LIVE1;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            ePidFltSrc = E_TSP_PIDFLT_LIVE2;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            ePidFltSrc = E_TSP_PIDFLT_LIVE3;
            break;
        case E_TSP_FLT_SRC_FILE0:
            ePidFltSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_FLT_SRC_FILE1:
            ePidFltSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_FLT_SRC_FILE2:
            ePidFltSrc = E_TSP_PIDFLT_FILE2;
            break;
        case E_TSP_FLT_SRC_FILE3:
            ePidFltSrc = E_TSP_PIDFLT_FILE3;
            break;

    }

    u32Src = HAL_TSP_FltSrc2PktDmx(ePidFltSrc);



    type = E_TSP_FLT_FIFO_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];
    if (type)
    {
        switch (type)
        {
            case E_TSP_FLT_FIFO_VIDEO:
                HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO,u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_VFIFO);
                break;
            case E_TSP_FLT_FIFO_AUDIO:
                HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO,u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_AFIFO);
                break;
            case E_TSP_FLT_FIFO_VIDEO3D:
                HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO3D,u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_VFIFO3D);
                break;
            case E_TSP_FLT_FIFO_AUDIO2:
                HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO2,u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_AFIFO2);
                break;
            case E_TSP_FLT_FIFO_AUDIO3:
                HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO3,u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_AFIFO3);
                break;
            case E_TSP_FLT_FIFO_REC:
                // @F_TODO for old style. we might have to check the filter source and it's destination
                break;

            case E_TSP_FLT_FIFO_PVR0:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(0));
                break;
            case E_TSP_FLT_FIFO_PVR1:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(1));
                break;
            case E_TSP_FLT_FIFO_PVR2:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(2));
                break;
            case E_TSP_FLT_FIFO_PVR3:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(3));
                break;
            case E_TSP_FLT_FIFO_PVR4:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(4));
                break;
            case E_TSP_FLT_FIFO_PVR5:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PVRRASPEngMapping(5));
                break;
            default:
    //            TSP_DBG(0, "[%04d] Wrong Destination type : %d!\n", __LINE__, type);
    //            TSP_RETURN(E_TSP_FAIL);
                break;
        }
    }
    else
    {
        type = E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];
        if (type)
        {
#if 0// defined(__LEGACY__)  we don't need this in kaiser we have bit to set for pvr eng
            if (E_TSP_FLT_FIFO_PVR == type) // @FIXME WHY??????????
            {
                TSP_DBG(0, "[%04d] Bad PidFltId %ld\n", __LINE__, u32FltId);
            }
            else
#endif
                u16BufId = HAL_TSP_SecFlt_GetSecBuf(pSecFlt);
                pSecBuf = &(_REGBuf->Buf[u16BufId]);

                //TSP_DBG(0, "[%04d] SecBufId = %d\n", __LINE__, u16BufId);

                switch (type)
                {
                    case E_TSP_FLT_USER_SEC:
                    case E_TSP_FLT_USER_PES:
                    case E_TSP_FLT_USER_PKT:
                    case E_TSP_FLT_USER_TTX:
                        if (_ptsp_res->_tspInfo.Flt2Buf[u32FltId] == TSP_FLT2BUF_NULL)
                        {
                            TSP_RETURN(E_TSP_FAIL_FUNCTION);
                        }
                        if(_ptsp_res->_tspInfo.MulFlt2Buf[u16BufId] == 1) // single filter and single buffer
                        {
                            HAL_TSP_SecBuf_Reset(pSecBuf);
                        }
                        if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
                        {
    #ifdef MSOS_TYPE_LINUX_KERNEL
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,FALSE);
    #else
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,TRUE);
    #endif
                        }
                        else // E_TSP_EVENT_CB_AUTO
                        {
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,FALSE);
                        }

                        HAL_TSP_SecFlt_ResetState(pSecFlt);
                        HAL_TSP_SecFlt_ResetRmnCnt(pSecFlt);
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_SECFLT);
                        break;
                    case E_TSP_FLT_USER_EMM: // @NOTE LEGACY
                    case E_TSP_FLT_USER_ECM:
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);
                        break;

                    case E_TSP_FLT_USER_PCR:
                        #if HW_PCRFLT_ENABLE
                        if (TSP_PCRFLT_PCR0 == u32FltId)
                        {
                            _TSP_SetHwPcrNotifyId(0);
                            _ptsp_res->_u8_ResetPcr[0] = 3 ;
                            _ptsp_res->_u32_PrevStcBase[0] = 0;
                            _ptsp_res->_u32_PrevPllControl[0] =0;
							HAL_TSP_PcrFlt_Enable(0, TRUE);
                            HAL_TSP_INT_Enable((MS_U16)(TSP_HWINT2_PCR0_UPDATE_END << 8) );
                        }
                        else if (TSP_PCRFLT_PCR1 == u32FltId)
                        {
                            _TSP_SetHwPcrNotifyId(1);
                            _ptsp_res->_u8_ResetPcr[1] = 3 ;
                            _ptsp_res->_u32_PrevStcBase[1] = 0;
                            _ptsp_res->_u32_PrevPllControl[1] = 0;
							HAL_TSP_PcrFlt_Enable(1, TRUE);
                            HAL_TSP_INT_Enable((MS_U16)(TSP_HWINT2_PCR1_UPDATE_END << 8) );
                        }
                        #else // @F_TODO i think we do not have to support SW PCR update anymore
                        HAL_TSP_SecFlt_ResetState(pSecFlt);
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_SECFLT| TSP_PIDFLT_OUT_SECAF); //@NOTE what is SECAF for???? SECAF is for those who has adaption field packet usecase: SWPCR
                        #endif
                        break;
                    default:
                        TSP_DBG(0, "[%04d] Unsupport FlType = %ld\n", __LINE__, _ptsp_res->_tspInfo.FltType[u32FltId]);
                        TSP_RETURN(E_TSP_FAIL);
                        break;
                }
        }
    }

    _ptsp_res->_tspInfo.FltState[u32FltId] |= E_TSP_FLT_STATE_ENABLE;
    TSP_RETURN(E_TSP_OK);
}


//[RESERVED] obsoleted
TSP_Result MDrv_TSP_FLT_SetOwner(MS_U32 u32EngId, MS_U32 u32FltStart, MS_U32 u32FltEnd, MS_BOOL bOwner)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    return E_TSP_FAIL_NOT_SUPPORTED;
}


//[RESERVED] obsoleted
TSP_Result MDrv_TSP_FLT_ChkOwner(MS_U32 u32EngId, MS_U32 u32FltId)
{
    TSP_ENTRY();

    if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ALLOC)
    {
        if (_ptsp_res->_tspInfo.OwnerId[u32FltId] == u32EngId)
        {
            TSP_RETURN(E_TSP_OK);
        }
        else
        {
            TSP_RETURN(E_TSP_FAIL);
        }
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }
}


//-------------------------------------------------------------------------------------------------
/// Get current PID filter status
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter
/// @param  peState                 \b OUT: current ORed state flag of PID filter\n
///                                         E_TSP_FLT_STATE_ALLOC\n
///                                         E_TSP_FLT_STATE_ENABLE\n
///                                         E_TSP_FLT_STATE_SCRAMBLED (last TS scrambling control status)
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_GetState(MS_U32 u32FltId, TSP_FltState *pState)
{
    //We do nothing when create dmx flt to record
    MS_U32              u32RECFltId = 0;
    u32RECFltId = HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX);
    if(u32FltId == (u32RECFltId))
    {
        *pState = E_TSP_FLT_STATE_NA;
        return E_TSP_OK;
    }

    MS_U32              u32BufId;


    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_PIDFLT_NUM_ALL > u32FltId, "[%04d] Invalid u32FltId %ld\n", __LINE__, u32FltId);

    // ALLOC / FREE / ENABLE
    *pState =  _ptsp_res->_tspInfo.FltState[u32FltId];

#if 0  // HW remove scramble status register
    // Scramble bit status
    if (HAL_TSP_GetSCMB((MS_U16)u32FltId))
    {
        *pState |= E_TSP_FLT_STATE_SCRAMBLED;
    }
#endif

    // PidFlt Status
    switch (_ptsp_res->_tspInfo.FltType[u32FltId])
    {
    case E_TSP_FLT_FIFO_VIDEO:
    case E_TSP_FLT_FIFO_AUDIO:
    case E_TSP_FLT_FIFO_AUDIO2:
    case E_TSP_FLT_FIFO_AUDIO3:
    case E_TSP_FLT_FIFO_VIDEO3D:
        break;
    case E_TSP_FLT_USER_SEC:
    case E_TSP_FLT_USER_PES:
    case E_TSP_FLT_USER_PKT:
    case E_TSP_FLT_USER_TTX:
        u32BufId = HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));
        if (HAL_TSP_SecFlt_GetState(&(_REGSec->Flt[u32BufId])) & TSP_SECFLT_STATE_OVERFLOW)
        {
            *pState |= E_TSP_FLT_STATE_OVERFLOW;
        }
        if (HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[u32BufId])) != HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[u32BufId])))
        {
            *pState |= E_TSP_FLT_STATE_STREAM_AVAIL;
        }
        break;
    case E_TSP_FLT_USER_PCR:
    default:
        break;
    }
    return E_TSP_OK;



}


//-------------------------------------------------------------------------------------------------
/// Allocate a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32SecFltId            \b OUT: pointer of section filter id return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_Alloc(MS_U32 u32TSPId, MS_U32 *pu32BufId)
{
    REG_SecBuf *        pSecBuf;
    int                 i;

    TSP_DBG(7, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    *pu32BufId = 0xFFFFFFFF;

    for (i = 0; i < TSP_SECBUF_NUM; i++)
    {
        if (_ptsp_res->_tspInfo.BufState[i] == E_TSP_FLT_STATE_FREE)
        {
            pSecBuf = &(_REGBuf->Buf[i]);
            if (HAL_TSP_SecBuf_TryAlloc(pSecBuf, (MS_U16)u32TSPId))
            {
                break;
            }
        }
    }
    if (i >= TSP_SECBUF_NUM)
    {
        TSP_DBG(0, "[%04d] No free buffer found 0x%02lx\n", __LINE__, (MS_U32)i);
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_DBG(7, "[%04d] SEC_Alloc BUF %d\n", __LINE__, i);

    _ptsp_res->_tspInfo.BufState[i] = E_TSP_FLT_STATE_ALLOC;
    _TSP_SEC_Init(i);

    *pu32BufId = i;
    _TSP_SetSecNotifyId(*pu32BufId); //for isr processing checking

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Free a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: section filter of TSP to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result  MDrv_TSP_SEC_Free(MS_U32 u32BufId)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);

    TSP_ENTRY();
    if (_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] == 1)
    {
        if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
        {
            TSP_DBG(3, "[%04d] Invalid Buffer\n", __LINE__);
            TSP_DBG(5, "[%04d] Invalid Buffer %lx, %x\n", __LINE__, u32BufId, _ptsp_res->_tspInfo.BufState[u32BufId]);
            TSP_RETURN(E_TSP_FAIL_INVALID);
        }

        _TSP_SEC_Free(u32BufId);
    }
    else
    {
        _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] -= 1;
    }

    //printf("[%s] MulFlt2Buf %ld = %d ", __FUNCTION__,u32BufId,_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId]);

    TSP_RETURN(E_TSP_OK);

}


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
TSP_Result MDrv_TSP_SEC_SetBuffer(MS_U32 u32BufId, MS_PHYADDR u32Start, MS_U32 u32Size)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    TSP_DBG(7, "SEC_SetBuffer addr 0x%08lX, size  0x%08lX\n", u32Start, u32Size);

    if (_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] == 1)
    {
        HAL_TSP_SecBuf_SetBuf(&(_REGBuf->Buf[u32BufId]), u32Start, u32Size);
    }

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Reset the section buffer read/write pointer to start address and resolve overflow condition
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be reset
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_Reset(MS_U32 u32BufId)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecBuf_Reset(&(_REGBuf->Buf[u32BufId]));

    HAL_TSP_SecFlt_ResetRmnCnt(&(_REGSec->Flt[(_ptsp_res->_tspInfo.Buf2Flt[u32BufId])]));

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
///[RESERVED]
/// Get buffer start address of setction buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufStart            \b OUT:  pointer of buffer start address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetStart(MS_U32 u32BufId, MS_PHYADDR *pu32Start)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);
    TSP_CHECK(pu32Start, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Start = HAL_TSP_SecBuf_GetStart(&(_REGBuf->Buf[u32BufId]));

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
///[RESERVED]
/// Get buffer size of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufSize             \b OUT: pointer of buffer size return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetSize(MS_U32 u32BufId, MS_U32 *pu32Size)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);
    TSP_CHECK(pu32Size, "[%04d] NULL pointer\n", __LINE__);

    *pu32Size = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Size=  HAL_TSP_SecBuf_GetEnd(&(_REGBuf->Buf[u32BufId])) - HAL_TSP_SecBuf_GetStart(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32ReadAddr            \b OUT: pointer of address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetRead(MS_U32 u32BufId, MS_PHYADDR *pu32Read)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);
    TSP_CHECK(pu32Read, "[%04d] NULL pointer\n", __LINE__);

    *pu32Read = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Read= (MS_PHYADDR)HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_SEC_GetWrite(MS_U32 u32BufId, MS_PHYADDR *pu32Write)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);
    TSP_CHECK(pu32Write, "[%04d] NULL pointer\n", __LINE__);

    *pu32Write = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Write= (MS_PHYADDR)HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  u32ReadAddr             \b IN: address of read pointer
/// @return TSP_Result
/// @note
/// User can update the read address to notify TSP where is the end of section
/// data already read back by user.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_SetRead(MS_U32 u32BufId, MS_PHYADDR u32Read)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(TSP_SECFLT_NUM > u32BufId, "[%04d] Invalid u32BufId %ld\n", __LINE__, u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecBuf_SetRead(&(_REGBuf->Buf[u32BufId]), (MS_U32)u32Read);

    TSP_RETURN(E_TSP_OK);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
TSP_Result MDrv_TSP_SEC_ClrEvent(MS_U32 u32BufId)
{
    TSP_ENTRY();
    _u32SecEvent[u32BufId] = 0;
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SEC_GetEvent(MS_U32 u32BufId, MS_U32 *pu32Event)
{
    TSP_ENTRY();
    *pu32Event = _u32SecEvent[u32BufId];
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SEC_SetEvent(MS_U32 u32BufId, MS_U32 u32Event)
{
    TSP_ENTRY();
    _u32SecEvent[u32BufId] |= u32Event;
    TSP_RETURN(E_TSP_OK);
}
#endif // MSOS_TYPE_LINUX_KERNEL


//-------------------------------------------------------------------------------------------------
/// Set packet size to TSP file
/// @param  PacketMode                 \b IN: Mode of TSP file packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_SetPktMode(TSP_PktMode mode)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    switch (mode)
    {
        case E_TSP_PKTMODE_188:
            HAL_TSP_Filein_PktSize(eFileEng,188);
            break;
        case E_TSP_PKTMODE_192:
            HAL_TSP_Filein_PktSize(eFileEng,192);
            break;
        case E_TSP_PKTMODE_204:
            HAL_TSP_Filein_PktSize(eFileEng,204);
            break;
        case E_TSP_PKTMODE_130:
            HAL_TSP_Filein_PktSize(eFileEng,130);
            break;
        case E_TSP_PKTMODE_134:
            HAL_TSP_Filein_PktSize(eFileEng,134);
            break;
        default:
            break;
    }

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_SetRate(MS_U32 u32Div2)
{
    TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_OS_IntDisable();
    HAL_TSP_Filein_ByteDelay(eFileEng,u32Div2,TRUE);
    TSP_OS_IntEnable();

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream start address
/// @param  u32StreamAddr           \b IN: pointer of transport stream in memory
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdAddr(MS_U32 u32Addr)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_Addr(eFileEng, u32Addr);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream data size
/// @param  u32StreamSize           \b IN: size of transport stream data to be copied
/// @return TSP_Result
//  @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdSize(MS_U32 u32Size)
{
    if(u32Size <= 16)
        return E_TSP_FAIL;

    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_Size(eFileEng, u32Size);
    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_CmdSTC(MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC)
{
    TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // STC register mapping is different between MCU and CPU.
    //HAL_TSP_CMDQ_SetSTC(u32STC_H, u32STC);
    //HAL_TSP_SetSTC(u32STC_H, u32STC);
    HAL_TSP_STC33_CmdQSet(u32STC_H, u32STC);

    TSP_RETURN(E_TSP_OK);
}



//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Start stream input
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
/// @note Filein Command Size: 1
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdStart(TSP_FileMode mode)
{
    TSP_ENTRY();

    // @FIXME: Good to assume enum and register definition identical?
    switch (mode)
    {
        case E_TSP_FILE_2_TSP:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for bypass clear
            break;
        case E_TSP_FILE_2_VIDEO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, TRUE);
            break;
        case E_TSP_FILE_2_VIDEO3D:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO2:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3, TRUE);
            break;
        default:
            break;
    }

    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_Start(eFileEng);

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_GetCmdSlot(MS_U32 *pu32EmptySlot)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *pu32EmptySlot = HAL_TSP_Filein_CmdQSlot(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset command queue
/// @return TSP_Result
/// @note
/// SW patch--wait command queue empty
/// wait HW ECO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Reset(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_CmdQRst(eFileEng,TRUE);
    HAL_TSP_Filein_CmdQRst(eFileEng,FALSE);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : pause stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Pause(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_File_Pause(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : resume stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Resume(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_File_Resume(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Stop(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_TSIF_FileEn(eFileEng, FALSE);                    // data port disable @FIXME in k2 we switch data port only but we close every file setting here will there be side effect?

    // @TODO Does it need to call HAL_TSP_FIFO_Bypass() function?
    //HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for clear all bypass setting

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current file in state
/// @return TSP_M2tStatus
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_GetState(TSP_FileState *pState)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *pState = HAL_TSP_Filein_GetState(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable timestamp block scheme
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable)
{
    TSP_ENTRY();
//  HAL_TSP_FileIn_192BlockScheme_En(bEnable);
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_BlockTimeStamp(eFileEng, bEnable); //@F_TODO fix engine problen and make sure they are the same
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Setting PS path
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_PS_Path_Enable(TSP_FileMode mode)
{
    TSP_ENTRY();
    switch (mode)
    {
        case E_TSP_FILE_2_TSP:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for bypass clear
            break;
        case E_TSP_FILE_2_VIDEO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, TRUE);
            break;
        case E_TSP_FILE_2_VIDEO3D:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO2:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3, TRUE);
            break;
        default:
            break;
    }
    TSP_RETURN(E_TSP_OK);
}


// file driver with engine
TSP_Result MDrv_TSP_FILE_Eng_SetPktMode(TSP_FILE_ENG Eng, TSP_PktMode mode)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    MS_U32 u32PktSize = 0;
    switch (mode)
    {
        case E_TSP_PKTMODE_188:
            u32PktSize = 188;
            break;
        case E_TSP_PKTMODE_192:
            u32PktSize = 192;
            break;
        case E_TSP_PKTMODE_204:
            u32PktSize = 204;
            break;
        case E_TSP_PKTMODE_130:
            u32PktSize = 130;
            break;
        case E_TSP_PKTMODE_134:
            u32PktSize = 134;
            break;
        default:
            printf("Invalid File-in Packet Size!\n");
            TSP_RETURN(E_TSP_FAIL);
            break;
    }

    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    HAL_TSP_Filein_PktSize(eFileEng, u32PktSize);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_SetRate(TSP_FILE_ENG Eng, MS_U32 u32Div2)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.
    TSP_OS_IntDisable();
    HAL_TSP_Filein_ByteDelay(eFileEng, u32Div2, TRUE); //@F_TODO when to set false???
    TSP_OS_IntEnable();

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdAddr(TSP_FILE_ENG Eng, MS_U32 u32Addr)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    HAL_TSP_Filein_Addr((MS_U32)eFileEng, u32Addr);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdSize(TSP_FILE_ENG Eng, MS_U32 u32Size)
{
    if(u32Size <= 16)
    return E_TSP_FAIL;

    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    HAL_TSP_Filein_Size(eFileEng, u32Size);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdSTC(TSP_FILE_ENG Eng, MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}

TSP_Result MDrv_TSP_FILE_Eng_CmdStart(TSP_FILE_ENG Eng, TSP_FileMode mode)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    TSP_DST_SEQ fileDstType = E_TSP_DST_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);
    switch (Eng)
    {
        case E_TSP_FILE_ENG_TSIF0:
            // set PES bypass if PES bypass mode
            // @NOTE we can only use TSIF0/MMFI01 filein doing PES to fifo
            switch (mode)
            {
                case E_TSP_FILE_2_TSP:
                    fileDstType = E_TSP_DST_INVALID;
                    break;
                case E_TSP_FILE_2_VIDEO:
                    fileDstType = E_TSP_DST_FIFO_VIDEO;
                    break;
                case E_TSP_FILE_2_VIDEO3D:
                    fileDstType = E_TSP_DST_FIFO_VIDEO3D;
                    break;
                case E_TSP_FILE_2_AUDIO:
                    fileDstType = E_TSP_DST_FIFO_AUDIO;
                    break;
                case E_TSP_FILE_2_AUDIO2:
                    fileDstType = E_TSP_DST_FIFO_AUDIO2;
                    break;
                case E_TSP_FILE_2_AUDIO3:
                    fileDstType = E_TSP_DST_FIFO_AUDIO3;
                    break;
                default:
                    break;
            }
            HAL_TSP_FIFO_Bypass(fileDstType, TRUE);
            break;
        case E_TSP_FILE_ENG_TSIF1:
            break;
        case E_TSP_FILE_ENG_TSIF2:
            break;
        case E_TSP_FILE_ENG_TSIF3:
            break;
        default:
            printf("Invalid File-in Engine number!\n");
            TSP_RETURN(E_TSP_FAIL);
            break;
    }

    HAL_TSP_Filein_Start(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Stop(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    HAL_TSP_TSIF_FileEn(eFileEng, FALSE);             // data port disable @FIXME in k2 we switch data port only but we close every file setting here will there be side effect?

    // @TODO not completely implement yet
    // Fix Path : tsif 0 for video and tsif 1 for video3D
    // need to flexible modification in the future
   /*
    if(eFileEng == E_FILEENG_TSIF0)
    {
        HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for clear all bypass setting
    }
    else if(eFileEng == E_FILEENG_TSIF2)
    {
        HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, FALSE); // for clear all bypass setting
    }
    */


    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_GetCmdSlot(TSP_FILE_ENG Eng, MS_U32 *pu32EmptySlot)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    *pu32EmptySlot = HAL_TSP_Filein_CmdQSlot(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Pause(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    if(HAL_TSP_File_Pause(eFileEng))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}

TSP_Result MDrv_TSP_FILE_Eng_Resume(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    if(HAL_TSP_File_Resume(eFileEng))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }

}

TSP_Result MDrv_TSP_FILE_Eng_GetState(TSP_FILE_ENG Eng, TSP_FileState *pState)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    *pState = HAL_TSP_Filein_GetState(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Reset(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    HAL_TSP_Filein_CmdQRst(eFileEng, TRUE);
    HAL_TSP_Filein_CmdQRst(eFileEng, FALSE);
    HAL_TSP_Filein_Abort(eFileEng, TRUE);
    HAL_TSP_Filein_Abort(eFileEng, FALSE);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_192BlockScheme_En(TSP_FILE_ENG Eng, MS_BOOL bEnable)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);

    HAL_TSP_Filein_BlockTimeStamp(eFileEng, bEnable);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_PS_Path_Enable(TSP_FILE_ENG Eng, TSP_FileMode mode)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}

TSP_Result MDrv_TSP_FILE_Eng_CmdQFifo_Status(TSP_FILE_ENG Eng, MS_U8 *pu8FifoLevel)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    *pu8FifoLevel = HAL_TSP_Filein_CmdQLv(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(TSP_FILE_ENG Eng,MS_BOOL bEnable)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    HAL_TSP_Filein_PacketMode(eFileEng,bEnable);

    TSP_RETURN(E_TSP_OK);
}



TSP_Result MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32 u32Stamp)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    HAL_TSP_Filein_SetTimeStamp(eFileEng, u32Stamp);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampGetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32* u32Stamp)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    *u32Stamp = HAL_TSP_Filein_GetTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
}



TSP_Result MDrv_TSP_FILE_Eng_GetFileInTimeStamp(TSP_FILE_ENG Eng, MS_U32* u32TSLen)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);


    *u32TSLen = HAL_TSP_Filein_PktTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_GetFileInCurReadAddr(TSP_FILE_ENG Eng, MS_U32* pu32Addr)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif((MS_U32)Eng);
    HAL_TSP_Filein_GetCurAddr(eFileEng,pu32Addr);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_H, MS_U32 *pu32STC)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(pu32STC_H, "[%04d] NULL pointer\n", __LINE__);
    TSP_CHECK(pu32STC, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();

    //HAL_TSP_CMDQ_GetSTC(pu32STC_H, pu32STC);
    //HAL_TSP_GetSTC(pu32STC_H, pu32STC);
    HAL_TSP_STC33_CmdQGet(pu32STC_H, pu32STC);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get PRC of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32Pcr_32              \b OUT: pointer of STC[32] return
/// @param  pu32Pcr                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32PCR_H, MS_U32 *pu32PCR)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_CHECK(pu32PCR_H, "[%04d] NULL pointer\n", __LINE__);
    TSP_CHECK(pu32PCR, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();

    HAL_TSP_PcrFlt_GetPcr(u32EngId, pu32PCR_H, pu32PCR);

    TSP_RETURN(E_TSP_OK);
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
        return E_TSP_FAIL;
    }

    *ppVersion = &_drv_tsp_version;

    return E_TSP_OK;
}


TSP_Result MDrv_TSP_GetStatus(TSP_HW_Status *HW_Status, TSP_SW_Status *SW_Status)
{
    TSP_ENTRY();
    if (HAL_TSP_HCMD_Alive())
    {
        *HW_Status = E_TSP_HW_ALIVE;
    }
    else
    {
        *HW_Status = E_TSP_HW_DEALIVE;
    }
    *SW_Status = _ptsp_res->_tspInfo.Status;

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_GetFWVER(MS_U32* u32FWVer)
{
    TSP_ENTRY();
    *u32FWVer = HAL_TSP_HCMD_GetInfo(INFO_FW_VERSION);
    TSP_RETURN(E_TSP_OK);
}

typedef enum //@F_TODO for compile only ask other for how to fix this....
{
    E_DRVTSP_IF_TSIF0               =   0x0,
    E_DRVTSP_IF_TSIF1               =   0x1,
    E_DRVTSP_IF_TSIF2               =   0x2,
    E_DRVTSP_IF_TSIF3               =   0x3,
    E_DRVTSP_IF_TSIF0_FILE          =   0x4,
    E_DRVTSP_IF_TSIF2_FILE          =   0x5,
    E_DRVTSP_IF_INVALID             = 0xFFFFFFFF,
} DrvTSP_If;

//@F_TODO api layer throw a enum that we don't have. check u8IfUID
TSP_Result MDrv_TSP_GetTSIFStatus(TSP_TSIF eTSIF, TSP_TSPad* ePad, MS_BOOL* pbClkInv, MS_BOOL* pbExtSyc, MS_BOOL* pbParl)
{
    TSP_Result Res = E_TSP_OK;
    MS_U16 u16pad  = 0;
    MS_U16 u16clk  = 0;

    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _DrvHALTSIFMapping(eTSIF);
    //if live-in and file-in of a DmxFlow map to diff TSIF, return the status of the live-in one
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);
    HAL_TSP_GetTSIF_Status(u32TSIF, &u16pad, &u16clk, pbClkInv, pbExtSyc, pbParl);

    switch(u16pad)  // @F_TODO we need a enum to setpad from drv to hal and hal will translate it into register
    {               // in the opposite, hal translate register state to that enum to drv layer to communicate...
        case REG_TOP_TS_SRC_EXT0:
            if((u16clk & REG_CLKGEN0_TS_SRC_EXT0) != REG_CLKGEN0_TS_SRC_EXT0)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD0;
            break;
        case REG_TOP_TS_SRC_EXT1:
            if((u16clk & REG_CLKGEN0_TS_SRC_EXT1) != REG_CLKGEN0_TS_SRC_EXT1)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD1;
            break;

#ifdef TS2_IF_SUPPORT
        case REG_TOP_TS_SRC_EXT2:
            if((u16clk & REG_CLKGEN0_TS_SRC_EXT2) != REG_CLKGEN0_TS_SRC_EXT2)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD2;
            break;
#endif

#ifdef TS3_IF_SUPPORT
        case REG_TOP_TS_SRC_EXT3:
            if((u16clk & REG_CLKGEN0_TS_SRC_EXT3) != REG_CLKGEN0_TS_SRC_EXT3)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD3;
            break;
#endif

        case REG_TOP_TS_SRC_DMD0:
            if((u16clk & REG_CLKGEN0_TS_SRC_DMD0) != REG_CLKGEN0_TS_SRC_DMD0)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_DEMOD0;
            break;

        #if 0 //support demod1
        case REG_TOP_TS_SRC_DMD1:
            if((u16clk & REG_CLKGEN0_TS_SRC_DMD1) != REG_CLKGEN0_TS_SRC_DMD1)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_DEMOD1;
            break;
        #endif

        case REG_TOP_TS_SRC_TSO0:
            if((u16clk & REG_CLKGEN0_TS_SRC_TSO0) != REG_CLKGEN0_TS_SRC_TSO0)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD_TSO0;
            break;

        #if 0    //support TSO1
        case REG_TOP_TS_SRC_TSO1:
            if((u16clk & REG_CLKGEN0_TS_SRC_TSO1) != REG_CLKGEN0_TS_SRC_TSO1)
            {
                Res = E_TSP_FAIL_FUNCTION;
            }
            *ePad = E_TSP_TS_PAD_TSO1;
            break;
        #endif

        default:
            return E_TSP_FAIL_FUNCTION;
    }
#if 0 // @TODO fix this
    *pbClkInv = ((u16clk & TS0_CLK_INV) == TS0_CLK_INV);
#endif
    return Res;
}

//--------------------------------------------------------------------------------------------------
/// Get command queue fifo level
/// @param  pu8FifoLevel      \b OUT: fifo level, 0~3
/// @return TSP_Result
/// @note
//--------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *pu8FifoLevel = HAL_TSP_Filein_CmdQLv(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SetDbgLevel(TSP_DbgLevel DbgLevel)
{
    _u32DbgLevel = DbgLevel;
    return E_TSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Query CAPs and get it's information
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetCaps(TSP_Caps eCap, MS_U32 *pu32CapInfo)
{
    if (eCap >= E_TSP_CAP_NULL)
        return E_TSP_FAIL;

    if(eCap == E_TSP_CAP_RESOURCE_SIZE)
    {
        *pu32CapInfo = sizeof(TSP_RESOURCE_PRIVATE);
        return E_TSP_OK;
    }

    *pu32CapInfo = HAL_TSP_GetCaps(eCap);
    if (*pu32CapInfo == HAL_TSP_RET_NULL)
    {
        return E_TSP_FAIL;
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *u32TSLen = HAL_TSP_Filein_PktTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
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
    ret = (TSP_Result) _ptsp_res->_tspInfo.LastErr;
    _ptsp_res->_tspInfo.LastErr = E_TSP_OK;
    return ret;
}


TSP_Result MDRV_TSP_FAIL_NOT_SUPPORTED(void)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}


//[RESERVED]
TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId)
{
    TSP_ENTRY();
    if (HAL_TSP_HCMD_Alive())
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}


//[RESERVED]
TSP_Result MDrv_TSP_DBG_GetDBGStatus(MS_U32 u32Sel, MS_U32* u32Status)
{
    TSP_ENTRY();

   // *u32Status = HAL_TSP_GetDBGStatus((MS_U16)u32Sel);//@F_TODO not implement yet

    TSP_RETURN(E_TSP_OK);
}


//[RESERVED]
MS_U32 MDrv_TSP_DBG_TaskLine(void)
{
    return _ptsp_res->_tspInfo.u32TaskLine;
}


//[RESERVED]
TSP_Result MDrv_TSP_DBG_FltInfo(MS_U32 u32FltId, MS_U32 u32BufId)
{
    TSP_ENTRY();

 //   HAL_TSP_DBG_DumpFlt((MS_U16)u32FltId, (MS_U16)u32BufId); //@F_TODO not implement yet

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_Eng_FltAlloc(MS_U32 u32Eng, MS_U32 *pu32FltId)
{
    TSP_PVR_ENTRY();

    TSP_SRC_SEQ eEngSrc = E_TSP_SRC_INVALID;
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        HAL_PVR_GetEngSrc(u32Eng, &eEngSrc);
        _MDrv_FltAllocForRec(u32Eng, eEngSrc, pu32FltId);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_U32 u32RASPEng = u32Eng - E_TSP_PVR_RASPENG_START;
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32RASPEng, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
        {
            HAL_RASP_Get_Source(u32RASPEng, &eEngSrc);
            _MDrv_FltAllocForRec(u32Eng, eEngSrc, pu32FltId);
        }
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    TSP_PVR_RETURN(E_TSP_OK);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltFree(MS_U32 u32Eng, MS_U32 u32FltId)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_FltFreeForRec(u32FltId);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32Eng - E_TSP_PVR_RASPENG_START, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
            _MDrv_FltFreeForRec(u32FltId);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltSetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32 u32PID)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_FltSetPIDForRec(u32FltId, u32PID);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32Eng - E_TSP_PVR_RASPENG_START, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
            _MDrv_FltSetPIDForRec(u32FltId, u32PID);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltEnable(MS_U32 u32Eng, MS_U32 u32FltId, MS_BOOL bEnable)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_FltEnableForRec(u32FltId, bEnable);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32Eng - E_TSP_PVR_RASPENG_START, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
            _MDrv_FltEnableForRec(u32FltId, bEnable);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltGetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32* u32PID)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_FltGetPIDForRec(u32FltId, u32PID);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32Eng - E_TSP_PVR_RASPENG_START, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
            _MDrv_FltGetPIDForRec(u32FltId, u32PID);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_SetBuffer(u32Eng, u32Start0, u32Start1, u32Size0, u32Size1);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_SetBuffer(u32Eng - E_TSP_PVR_RASPENG_START, u32Start0, u32Start1, u32Size0, u32Size1);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Start(MS_U32 u32Eng, MS_BOOL bPvrAll, MS_BOOL bStart)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        if(bPvrAll)
            _MDrv_PVR_Eng_Start(u32Eng, E_DRVRASP_REC_MODE_ALL, bStart);
        else
            _MDrv_PVR_Eng_Start(u32Eng, E_DRVRASP_REC_MODE_PID, bStart);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        // @NOTE RASP support "Rec ALL" and 'Rec by PID" modes in K3/K3s/keltic
        //  In "Rec by PID" mode we need to open RASP PID Flts or RASP can't receive DATA from pkt_dmx.
        MS_U32 u32RASPEng = u32Eng - E_TSP_PVR_RASPENG_START;
        MS_BOOL bEnabled = FALSE;
        MDrv_RASP_PVR_GetFileinEnable(u32RASPEng, &bEnabled);
        if(bEnabled)//if RASP src's pkt dmx
            MDrv_RASP_PVR_Start(u32RASPEng, E_DRVRASP_REC_MODE_ALL, bStart);//no requirement of RASP PID filters
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_Pause(u32Eng, bPause);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_Pause(u32Eng - E_TSP_PVR_RASPENG_START, bPause);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHYADDR *pu32WriteAddr)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_GetWriteAddr(u32Eng, pu32WriteAddr);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_GetWriteAddr(u32Eng - E_TSP_PVR_RASPENG_START, pu32WriteAddr);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_Notify(u32Eng, eEvents, pfCallback);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_SetPacketMode(u32Eng, bSet);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_SetPacketMode(u32Eng - E_TSP_PVR_RASPENG_START, STR2MIU, bSet);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_TimeStampSetRecordStamp(u32Eng, u32Stamp);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_TimeStampSetRecordStamp(u32Eng - E_TSP_PVR_RASPENG_START, u32Stamp);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_TimeStampGetRecordStamp(u32Eng, u32Stamp);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
        MDrv_RASP_PVR_TimeStampGetRecordStamp(u32Eng - E_TSP_PVR_RASPENG_START, u32Stamp);
    }
    /*else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }*/
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_TimeStamp(MS_U32 u32Eng, MS_BOOL bEnable)
{
    if ((E_TSP_PVR_PVRENG_START <= u32Eng) && (E_TSP_PVR_PVRENG_END > u32Eng)) // PVR Eng
    {
        _MDrv_PVR_Eng_TimeStamp(u32Eng,  bEnable);
    }
    else if ((E_TSP_PVR_RASPENG_START <= u32Eng) && (E_TSP_PVR_RASPENG_END > u32Eng)) // RASP Eng
    {
    }
#if 0
    else if ((E_TSP_PVR_CBPVRENG_START <= u32Eng) && (E_TSP_PVR_CBPVRENG_END > u32Eng)) // channel browser
    {
    }
#endif
    else
    {
        printf ("[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;

}

TSP_Result MDrv_TSP_PVR_Eng_SetCaMode(MS_U32 u32EngId, MS_U16 u16CaMode, MS_BOOL bSpsEnable)
{
    TSP_ENTRY();

    if(HAL_TSP_CAPVR_SPSEnable((MS_U8)u32EngId, u16CaMode, bSpsEnable))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_OK);
    }
}

static TSP_Result _MDrv_FltAllocForRec(MS_U32 u32Eng, TSP_SRC_SEQ eEngSrc, MS_U32 *pu32FltId)
{
    TSP_PIDFLT_SRC ePidFltSrc = HAL_TSP_PktDmx2FltSrc(eEngSrc);
    TSP_FltType eTspFltSrcType = E_TSP_FLT_SRC_LIVE0;
    TSP_FltType eTspFltDstType = E_TSP_FLT_FIFO_PVR0;
    switch (ePidFltSrc)
    {
        case E_TSP_PIDFLT_LIVE0:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE0;
            break;
        case E_TSP_PIDFLT_LIVE1:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE1;
            break;
        case E_TSP_PIDFLT_LIVE2:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE2;
            break;
       case E_TSP_PIDFLT_LIVE3:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE3;
            break;
       case E_TSP_PIDFLT_FILE0:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE0;
            break;
       case E_TSP_PIDFLT_FILE1:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE1;
            break;
       case E_TSP_PIDFLT_FILE2:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE2;
            break;
       case E_TSP_PIDFLT_FILE3:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE3;
            break;
       default:
            break;
    }

    switch (u32Eng)
    {
        case 0:
            eTspFltDstType = E_TSP_FLT_FIFO_PVR0;
            break;
        case 1:
            eTspFltDstType = E_TSP_FLT_FIFO_PVR1;
            break;
        case 2:
            eTspFltDstType = E_TSP_FLT_FIFO_PVR2;
            break;
        case 3:
            eTspFltDstType = E_TSP_FLT_FIFO_PVR3;
            break;
    }
    if (E_TSP_OK != MDrv_TSP_FLT_Alloc(0, (eTspFltSrcType | eTspFltDstType), pu32FltId))
    {
        TSP_PVR_RETURN(E_TSP_FAIL);
    }

    return E_TSP_OK;
}

static TSP_Result _MDrv_FltFreeForRec(MS_U32 u32FltId)
{
    TSP_PVR_ENTRY();

    //@NOTE shouldn't we modify sw flt status here? No, since it's maitain in fle_enable and flt_free
    MDrv_TSP_FLT_Enable( u32FltId, FALSE);
    MDrv_TSP_FLT_SetPID( u32FltId, TSP_PID_NULL);
    MDrv_TSP_FLT_Free( u32FltId);

    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltSetPIDForRec(MS_U32 u32FltId, MS_U32 u32PID)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_SetPID( u32FltId, u32PID);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltGetPIDForRec(MS_U32 u32FltId, MS_U32* pu32PID)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_GetPID(u32FltId, pu32PID);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltEnableForRec(MS_U32 u32FltId, MS_BOOL bEnable)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_Enable( u32FltId, bEnable);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStr2Miu_StartAddr(u32Eng, u32Start0, u32Start1);
    HAL_PVR_SetStr2Miu_MidAddr(u32Eng, u32Start0 + u32Size0 , u32Start1 + u32Size1 );
    HAL_PVR_SetStr2Miu_EndAddr(u32Eng, u32Start0 + u32Size0 , u32Start1 + u32Size1 );
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Start(MS_U32 u32Eng, DrvRASP_RecMode eRecMode, MS_BOOL bStart)
{
    TSP_PVR_ENTRY();

    if (!bStart)
    {
        HAL_PVR_Stop(u32Eng);
    }
    else
    {
        switch (eRecMode)
        {
            case E_DRVRASP_REC_MODE_PID:
            {
                HAL_PVR_RecPid(u32Eng , TRUE);
                break;
            }
            case E_DRVRASP_REC_MODE_ALL:
            {
                HAL_PVR_RecPid(u32Eng , FALSE);
                break;
            }
            default :
                break;
        }
        HAL_PVR_Start(u32Eng);
    }

    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause)
{
    TSP_PVR_ENTRY();
    HAL_PVR_Pause(u32Eng, bPause);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHYADDR *pu32WriteAddr)
{
    TSP_PVR_ENTRY();
    *pu32WriteAddr = HAL_PVR_GetWritePtr(u32Eng);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    #if 0
    if(pfCallback)
    {
        _TSP_SetPvrNotifyId(u32Eng);
    }
    else
    {
        _TSP_RemovePvrNotifyId;
    }
    #endif

    return E_TSP_FAIL_NOT_SUPPORTED;
}

static TSP_Result _MDrv_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStrPacketMode(u32Eng, bSet);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetPVRTimeStamp(u32Eng, u32Stamp);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp)
{
    TSP_PVR_ENTRY();
    *u32Stamp = HAL_PVR_GetPVRTimeStamp(u32Eng);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_TimeStamp(MS_U32 u32Eng, MS_BOOL bEnable)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}

TSP_Result MDrv_TSP_PVR_Select(TSP_FltType eFltType)
{
    TSP_PVR_ENTRY();
    if (eFltType == E_TSP_FLT_FIFO_PVR)
    {
        _ptsp_res->_Current_PVR = E_TSP_RASP0;
    }
    else if (eFltType == E_TSP_FLT_FIFO_PVR2)
    {
        _ptsp_res->_Current_PVR = E_TSP_RASP1 ;
    }
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_FltAlloc(TSP_FltType eFltType, MS_U32 *pu32FltId)
{
    //if (eFltType & E_TSP_FLT_FIFO_PVR || eFltType & E_TSP_FLT_FIFO_PVR2)
    if (eFltType == E_TSP_FLT_FIFO_PVR || eFltType == E_TSP_FLT_FIFO_PVR2) //@FIXME [mozan] modify '&' -> '==' for non-exclusive use
    {
        MDrv_RASP_PVR_AllocFlt(_ptsp_res->_Current_PVR, pu32FltId);
        return E_TSP_OK;
    }
    else
    {
        printf ("[ERROR][%s][%d]Incorrect filter type!\n",__FUNCTION__,__LINE__);
        return E_TSP_FAIL;
    }
}

TSP_Result MDrv_TSP_PVR_FltFree(MS_U32 u32FltId)
{
    MDrv_RASP_PVR_ReleaseFlt(_ptsp_res->_Current_PVR, u32FltId);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_FltSetPID(MS_U32 u32FltId, MS_U32 u32PID)
{
    MDrv_RASP_PVR_SetPid(_ptsp_res->_Current_PVR, u32FltId, u32PID);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_SetBuffer(MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1)
{
    MDrv_RASP_PVR_SetBuffer(_ptsp_res->_Current_PVR, u32Start0, u32Start1, u32Size0, u32Size1);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Start(TSP_RecMode eRecMode, MS_BOOL bStart)
{
    if (eRecMode == E_TSP_REC_MODE_ENG0_BYPASS)
        MDrv_RASP_PVR_Start(_ptsp_res->_Current_PVR, E_DRVRASP_REC_MODE_ALL, bStart);
    if (eRecMode == E_TSP_REC_MODE_ENG0_FLTTYPE)
        MDrv_RASP_PVR_Start(_ptsp_res->_Current_PVR, E_DRVRASP_REC_MODE_PID, bStart);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_GetWriteAddr(MS_PHYADDR *pu32WriteAddr)
{
    MDrv_RASP_PVR_GetWriteAddr(_ptsp_res->_Current_PVR, pu32WriteAddr);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Notify(TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    MDrv_RASP_PVR_Notify(_ptsp_res->_Current_PVR, eEvents, NULL);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_SetPacketMode(MS_BOOL bSet)
{
    MDrv_RASP_PVR_SetPacketMode(_ptsp_res->_Current_PVR, STR2MIU, bSet);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_TimeStampSetRecordStamp(MS_U32 u32Stamp)
{
    MDrv_RASP_PVR_TimeStampSetRecordStamp(_ptsp_res->_Current_PVR, u32Stamp);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_TimeStampGetRecordStamp(MS_U32* u32Stamp)
{
    MDrv_RASP_PVR_TimeStampGetRecordStamp(_ptsp_res->_Current_PVR, u32Stamp);
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp)
{
    TSP_PVR_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_SetTimeStamp(eFileEng, u32Stamp);
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp)
{
    TSP_PVR_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *u32Stamp = HAL_TSP_Filein_GetTimeStamp(eFileEng);
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStrPacketMode(0,bEnable);
    TSP_PVR_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    TSP_ENTRY();
    HAL_TSP_PktDmx_RmDupAVPkt(bEnable);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable or Disable to remove audio,or vedio error packets.
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_TEI_RemoveErrorPkt(TSP_DRV_TEI_RmPktType eDrvPktType, MS_BOOL bEnable)
{
    TSP_ENTRY();

    TSP_HAL_TEI_RmPktType eHalPktType = E_TSP_HAL_TEI_REMOVE_AUDIO_PKT;

    switch ( eDrvPktType )
    {
        case E_TSP_DRV_TEI_REMOVE_AUDIO_PKT:
            eHalPktType = E_TSP_HAL_TEI_REMOVE_AUDIO_PKT;
            break;
        case E_TSP_DRV_TEI_REMOVE_VIDEO_PKT:
            eHalPktType = E_TSP_HAL_TEI_REMOVE_VIDEO_PKT;
            break;
        default:
            TSP_DBG(TRUE, "[%04d] Unsupport PktType = %lu\n", __LINE__, (MS_U32) eDrvPktType);
            TSP_RETURN(E_TSP_FAIL);
    }

    HAL_TSP_TEI_RemoveErrorPkt(eHalPktType, bEnable);

    TSP_RETURN(E_TSP_OK);
}


#ifdef STC64_SUPPORT
//-------------------------------------------------------------------------------------------------
/// Enable or Disable STC64 bit mode. Defalut is STC33 mode.
/// @param  bEnable                 \b IN: STC64 system time mode enable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC64_Mode_Enable(MS_BOOL bEnable)
{
    TSP_ENTRY();
//  HAL_TSP_STC64_Mode_En(bEnable); // @F_TODO not implement yet do we have to enable this in kaiser?
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Memory to TSP stream command : Update Stream STC64
// @param  u32EngId                  \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32STC_32               \b IN: STC[63:32] for next input stream
// @param  u32STC                    \b IN: STC[31:0] for next input stream
// @return TSP_Result
// @note M2T Command Size: 3
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetSTC64(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC)
{
    TSP_ENTRY();

    HAL_TSP_STC64_Set(u32EngId, u32STC_32, u32STC);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set STC control
/// @param u8Eng \b IN: STC Engine ID
/// @param bEnable \b IN: TRUE  -> AP control STC
///                       FALSE -> FW control STC
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_BOOL bEnable)
{
    TSP_ENTRY();
    if (HAL_TSP_STC_UpdateCtrl(u8Eng, bEnable))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}


TSP_Result MDrv_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd)
{
    _ptsp_res->_u32_StcOffset[u32EngId].u32StcOffset = u32Offset;
    _ptsp_res->_u32_StcOffset[u32EngId].bAdd= bAdd;
    return (E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC64) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32:63] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC64(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC)
{
    TSP_ENTRY();

    HAL_TSP_STC64_Get(u32EngId, pu32STC_32, pu32STC);

    TSP_RETURN(E_TSP_OK);
}

#endif



TSP_Result _TSP_FLT_SrcSwitch(TSP_FltType SrcFlow ,TSP_FltType TgtFlow)
{
    MS_U32 i ;//,dummy;

    // Define check policy for filters //
    #define CHECK_FILTER_PROPERTY(_idx_) ((_ptsp_res->_tspInfo.FltType[_idx_] & SrcFlow)\
                                        && (_ptsp_res->_tspInfo.FltState[_idx_] & E_TSP_FLT_STATE_ALLOC) \
                                        && (_ptsp_res->_tspInfo.FltState[_idx_] != E_TSP_FLT_STATE_NA) \
                                        && (FALSE ==(_ptsp_res->_tspInfo.FltType[_idx_] & E_TSP_FLT_FIFO_MASK ))\
                                        && (E_TSP_FLT_USER_PCR != (_ptsp_res->_tspInfo.FltType[_idx_]& E_TSP_FLT_USER_MASK )))


    //printf("Src Flow = %08lX  -- TgtFlow = %08lx \n",SrcFlow,TgtFlow);
#if 0 // @FIXME seeems we dont need this
    if (TgtFlow & E_TSP_FLT_SRC_FILE )
    {
        TSP_OS_Print("[%s] Target Flow Error , Flow = %08lx \n",__FUNCTION__,TgtFlow);
        return E_TSP_FAIL;
    }
#endif

    // Stop all target source Filter
    for ( i = 0 ; i < TSP_PIDFLT_NUM_ALL ; i ++)
    {
        if (CHECK_FILTER_PROPERTY(i))
        {
            TSP_LOCK();
            HAL_TSP_PidFlt_SetFltOut(i, TSP_PIDFLT_OUT_NONE);
            TSP_UNLOCK();
        }
    }

    // wait for all Task solved
    MsOS_DelayTask(100);


    for ( i = 0 ; i < TSP_PIDFLT_NUM_ALL ; i ++)
    {
        if (CHECK_FILTER_PROPERTY(i))
        {

            TSP_LOCK();
            // Setting new Filter input source.

            // @TODO  not review and implement yet: check everthing
            #if 0
            if (TgtFlow & E_TSP_FLT_SRC_LIVE0)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF0);
            }
            else if (TgtFlow & E_TSP_FLT_SRC_TSIF1)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF1);
            }
            else if (TgtFlow & E_TSP_FLT_SRC_TSIF2)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF2);
            }
            else
            {
                TSP_OS_Print("[%s][%d]Error in target source = %lx \n",__FUNCTION__,__LINE__,TgtFlow);
            }
            #endif

            _ptsp_res->_tspInfo.FltType[i] = (_ptsp_res->_tspInfo.FltType[i] & ~E_TSP_FLT_SRC_MASK) | TgtFlow ;
            TSP_UNLOCK();

            // Restart Running Filter
            if (_ptsp_res->_tspInfo.FltState[i] & E_TSP_FLT_STATE_ENABLE)
            {
                MDrv_TSP_FLT_Enable( i , TRUE );
            }
        }
    }

    TSP_LOCK();
    if (SrcFlow == _ptsp_res->_Current_Live)
    {
        _ptsp_res->_Current_Live = TgtFlow & E_TSP_FLT_SRC_MASK;
    }
    TSP_UNLOCK();

    return (E_TSP_OK);

    #undef CHECK_FILTER_PROPERTY

}



TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(TSP_FltType TgtFlow)
{

    //TSP_OS_Print("[%s][%d]\n",__FUNCTION__,__LINE__);

    TSP_Result ret = E_TSP_FAIL;

    if ((TgtFlow&E_TSP_FLT_SRC_MASK ) == _ptsp_res->_Current_Live)
    {
        return E_TSP_FAIL;
    }

    ret = _TSP_FLT_SrcSwitch(_ptsp_res->_Current_Live, TgtFlow);

    return ret;
}



TSP_Result MDrv_TSP_FLT_ChangeSource(MS_U32 u32FltId, TSP_FltType TgtFlow)
{
    TSP_LOCK();
    HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE); // stop filter

    // wait for all Task solved
    MsOS_DelayTask(20);

    TSP_PIDFLT_SRC eTspPidSrc = E_TSP_PIDFLT_INVALID;
    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & TgtFlow;

    switch(u32FltSrc)
    {
        case E_TSP_FLT_SRC_LIVE0:
            eTspPidSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            eTspPidSrc = E_TSP_PIDFLT_LIVE1;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            eTspPidSrc = E_TSP_PIDFLT_LIVE2;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            eTspPidSrc = E_TSP_PIDFLT_LIVE3;
            break;
        case E_TSP_FLT_SRC_FILE0:
            eTspPidSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_FLT_SRC_FILE1:
            eTspPidSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_FLT_SRC_FILE2:
            eTspPidSrc = E_TSP_PIDFLT_FILE2;
            break;
        case E_TSP_FLT_SRC_FILE3:
            eTspPidSrc = E_TSP_PIDFLT_FILE3;
            break;
        default:
            TSP_DBG(TRUE, "[%04d] Filter %ld input source is not assigned !! \n", __LINE__, u32FltId);
            eTspPidSrc = TSP_PIDFLT_TSIF0;
            break;
    }

    MS_U32 u32PktDmx = HAL_TSP_FltSrc2PktDmx(eTspPidSrc);

    HAL_TSP_PidFlt_SetFltIn(u32FltId,u32PktDmx);

    _ptsp_res->_tspInfo.FltType[u32FltId] = (_ptsp_res->_tspInfo.FltType[u32FltId] & ~E_TSP_FLT_SRC_MASK) | TgtFlow ;

    TSP_UNLOCK();

    if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)
    {
        MDrv_TSP_FLT_Enable( u32FltId, TRUE );
    }

    return (E_TSP_OK);

}

TSP_Result MDrv_TSP_PARL_BitOrderSwap(MS_U16 u16TSIF, MS_BOOL bInvert)
{

    TSP_ENTRY();
    HAL_TSP_TSIF_BitSwap(u16TSIF, bInvert); //@F_TODO check if this is the same with `HAL_TSP_SetDataSwap'
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FLTFIFO_FullSel(TSP_FltFIFOFullLV eFIFOFullLV)
{
    TSP_DBG(7, "%s\n", __FUNCTION__);
    TSP_ENTRY();
    HAL_TSP_FIFOPBFltFullSel(eFIFOFullLV);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
{
    if(HAL_TSP_CMD_Run(u32Cmd, u32Config0, u32Config1,pData))
        return (E_TSP_OK);
    else
        return (E_TSP_FAIL);
}

TSP_Result MDrv_TSP_DSCMB_Functionality(TSP_DSCMB_FuncCMD eCmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
{//this func should only be called by DSCMB driver to do DSCMB functionality, and maintained by DSCMB owner
//the mutex _s32MutexId is utilized to avoid race condition between DSCMB & TSP accessing shared registers
    TSP_ENTRY();

    switch(eCmd)
    {
        default:
            break;
    }

    TSP_RETURN(E_TSP_OK);
}

