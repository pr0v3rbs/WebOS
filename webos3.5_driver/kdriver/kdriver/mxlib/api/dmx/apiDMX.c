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
/// file   apiDMX.c
/// @brief  Demux API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef UTOPIA_STRIP  

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

#ifdef MMFILEIN
#include "drvMMFilein.h"
#endif

#include "drvTSP.h"
#include "apiDMX.h"
#include "apiDMX_private.h"

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
#include "drvRASP.h"
#endif

#ifdef TSO_ENABLE
#include "drvTSO.h"
#endif
#ifdef FQ_ENABLE
#include "drvFQ.h"
#endif

#include "utopia.h"

//#include "drvMIU.h"

#if DMX_DEBUG
#include "Internal_DMX_debug.h"
#endif


// reserved functions, internal use only
TSP_Result MDrv_TSP_Suspend(void);
TSP_Result MDrv_TSP_Resume(MS_U32 u32FWAddr, MS_U32 u32FWSize);
TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId);
TSP_Result MDrv_TSP_Reset(void);

// Preprocessor warning notification
#if (DMX_SECT_FLT_DEPTH != DRVTSP_FILTER_DEPTH)
#error "DMX_SECT_FLT_DEPTH is inconsistent with DRVTSP_FILTER_DEPTH";
#endif

//------------------------------------------------------------------------------
// Compile options
//------------------------------------------------------------------------------
#define DMX_DBG_LINUX    FALSE

#define DMX_MUTEX

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
#define DMX_FLT_DS_SIZE                    260 //130
#else
#define DMX_FLT_DS_SIZE                    160
#endif

#define DMX_MAX_IF_NUM                     DMX_PVR_ENG_NUM

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
// for debugging test only


#if 1

#define TSPlog	printf

#else

#ifdef MSOS_TYPE_LINUX_KERNEL

#define TSPlog(fmt, ...) { struct file *filep; char buf[255];  \
filep=(struct file *)filp_open("/tmp/tsplog",O_CREAT | O_APPEND | O_RDWR,0);  \
memset(buf, 0x00, 255);  \
sTSPlog(buf, fmt, ##__VA_ARGS__);   \
filep->f_op->write(filep,buf,strlen(buf),&filep->f_pos);  \
filp_close(filep,0);  \
}

#else

#define TSPlog(fmt, ...) { FILE *fp;  \
fp = fopen("/tmp/tsplog", "a+t");  \
fTSPlog(fp, fmt, ##__VA_ARGS__);   \
fclose(fp);  \
}

#endif

#endif

#define DMX_DBG_LINE()                          printf("[%s]-[%d]\n", __FUNCTION__, __LINE__);

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
#else
#define CPY_FROM_USER                   memcpy
#define CPY_to_USER                     memcpy
#endif

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------
#define DMX_MAX_FLTID                      _pdmx_res->_u32TspFltNum
#define _DMX_MUTEX_TIMEOUT                 10000 //MSOS_WAIT_FOREVER

//------------------------------------------------------------------------------
// Internal data structure
//------------------------------------------------------------------------------
typedef struct
{
    MS_U8                        FltId;
    MS_U8                        SecBufId;
    MS_U16                       u16Pid;
    DMX_Flt_info                 FltInfo;
    DMX_FILTER_TYPE              DmxFltType;
    // DMX_NotifyCb           pNotify;
} _DMX_Flt;

typedef struct _DMX_RESOURCE_PRIVATE
{
    MS_BOOL                     bResoureInit;

    _DMX_Flt                    _FltList[DMX_FLT_DS_SIZE];
    DMX_NotifyCb                _PvrNotify[DMX_MAX_IF_NUM];

#ifdef DMX_MUTEX
    MS_S32                      _Dmx_Mutex;
    MS_S32                      _Dmx_Pvr_Mutex;
    MS_S32                      _Dmx_MMFI_Mutex;
    MS_S32                      _Dmx_TSO_Mutex;
#endif // #ifdef DMX_MUTEX

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    MS_BOOL                     _bPVRCA_RecAll[DMX_MAX_IF_NUM];
    MS_U32                      _u32PVREngSrc[DMX_MAX_IF_NUM];
#endif //Not TSP_VER_2_0

    MS_BOOL                     _bFWInit;

#if (defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    MS_BOOL                     _bVQEnabled;
#endif

    MS_U32                      _u32TspFltNum;

}DMX_RESOURCE_PRIVATE;

typedef struct
{
    char            strCap[32];
    int             OutputSize;
    DMX_QUERY_TYPE  eCapType;
}_DMX_CAP_STRTBL;

//------------------------------------------------------------------------------
// Helper Macros
//------------------------------------------------------------------------------
// Filter management
#define _FLT_LIST_RESET()                                                        \
            {                                                                   \
                int     i;                                                      \
                for (i= 0; i< (int)DMX_FLT_DS_SIZE; i++)                          \
                {                                                               \
                    _FLT_LIST_REMOVE(i);                                        \
                }                                                               \
            }

//For TV series, Section Filer ID is the same as section buffer ID, DMXId = FltId
//For Box series, PID filter ID is the same as Section Filter ID, FltId = SecBufId
#define _FLT_LIST_SECFLT_TYPE_SET(DMXId, inFltId, inSecBufId, inDmxFltType)                            \
                do{                                                       \
                    _pdmx_res->_FltList[(DMXId)].FltId = (inFltId);                                          \
                    _pdmx_res->_FltList[(DMXId)].SecBufId = (inSecBufId);                                    \
                    _pdmx_res->_FltList[(DMXId)].DmxFltType = (inDmxFltType);                                \
                }while(0);

#define _FLT_LIST_TYPE_GET(DMXId, pDmxFltType)                                                \
                *(pDmxFltType) = _pdmx_res->_FltList[(DMXId)].DmxFltType;              \

#define _FLT_LIST_INFO_SET(DMXId, pFltInfo)                                                   \
                    memcpy(&(_pdmx_res->_FltList[(DMXId)].FltInfo), (pFltInfo), sizeof(DMX_Flt_info));     \

#define _FLT_LIST_INFO_GET(DMXId, pFltInfo)                                                   \
                    memcpy((pFltInfo), &(_pdmx_res->_FltList[(DMXId)].FltInfo), sizeof(DMX_Flt_info));     \

#define _FLT_LIST_INFO_GET_P(DMXId)              (&(_pdmx_res->_FltList[(DMXId)].FltInfo))

#define _FLT_LIST_PID_SET(DMXId, u16Pid_In)      _pdmx_res->_FltList[(DMXId)].u16Pid = (u16Pid_In)
#define _FLT_LIST_PID_GET(DMXId, u16Pid_In)      (u16Pid_In) = _pdmx_res->_FltList[(DMXId)].u16Pid

#define _FLT_LIST_REMOVE(DMXId)                                    \
                do{                  \
                    _pdmx_res->_FltList[(DMXId)].FltId=     0xFF;                                    \
                    _pdmx_res->_FltList[(DMXId)].SecBufId=  0xFF;                                    \
                    _pdmx_res->_FltList[(DMXId)].DmxFltType = (DMX_FILTER_TYPE)0xFF;                                  \
                    memset(&(_pdmx_res->_FltList[(DMXId)].FltInfo), 0, sizeof(DMX_Flt_info));       \
                }while(0);

#define _MAX(A, B) (((A) > (B))?(A):(B))
#define _MIN(A, B) (((A) < (B))?(A):(B))

// Mutex function
#ifdef DMX_MUTEX
#define _DMX_ENTRY()                while (MsOS_ObtainMutex(_pdmx_res->_Dmx_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)                              \
                                    {                                                                                   \
                                        printf("[MAPI DMX][%s][%06d] DMX Mutex taking timeout\n", __FUNCTION__, __LINE__);                    \
                                        MsOS_DelayTask(1); \
                                    }

#define _DMX_RETURN(_ret)           do{                                                         \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex) == FALSE) { printf("[MAPI DMX][%s][%06d] DMX Mutex Free fail\n", __FUNCTION__, __LINE__); }  \
                                        return _ret;                                            \
                                    }while(0);

#define _DMX_PVR_ENTRY()            while (MsOS_ObtainMutex(_pdmx_res->_Dmx_Pvr_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)                          \
                                    {                                                                                   \
                                        printf("[MAPI DMX][%s][%06d] PVR mutex taking timeout\n", __FUNCTION__, __LINE__);                \
                                        MsOS_DelayTask(1); \
                                    }

#define _DMX_PVR_RETURN(_ret)       do{                                                         \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex) == FALSE) { printf("[MAPI DMX][%s][%06d] PVR Mutex Free fail\n", __FUNCTION__, __LINE__); }  \
                                        return _ret;                                            \
                                    }while(0);

#define _DMX_MMFI_ENTRY()           while (MsOS_ObtainMutex(_pdmx_res->_Dmx_MMFI_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)  \
                                    {                                                                                   \
                                        printf("[MAPI DMX][%06d] MMFI Mutex taking timeout\n", __LINE__);               \
                                        MsOS_DelayTask(1); \
                                    }

#define _DMX_MMFI_RETURN(_ret)      do{                                                         \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex) == FALSE){ printf("[MAPI DMX][%s][%06d] MMFI Mutex Free fail\n", __FUNCTION__, __LINE__); }  \
                                        return _ret;                                            \
                                    }while(0);

#ifdef TSO_ENABLE
#define _DMX_TSO_ENTRY()            while (MsOS_ObtainMutex(_pdmx_res->_Dmx_TSO_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)                              \
                                    {                                                                                   \
                                        printf("[MAPI DMX][%06d] TSO Mutex taking timeout\n", __LINE__);                    \
                                        MsOS_DelayTask(1); \
                                    }

#define _DMX_TSO_RETURN(_ret)       do{                                                         \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex) == FALSE){ printf("[MAPI DMX][%s][%06d] TSO Mutex Free fail\n", __FUNCTION__, __LINE__); }  \
                                        return _ret;                                            \
                                    }while(0);
#endif

#else // #ifdef DMX_MUTEX
#define _DMX_ENTRY()           while (0)
#define _DMX_RETURN(_ret)      return _ret
#define _DMX_PVR_ENTRY()       while (0)
#define _DMX_PVR_RETURN(_ret)  return _ret
#define _DMX_MMFI_ENTRY()      while (0)
#define _DMX_MMFI_RETURN(_ret) return _ret
#define _DMX_TSO_ENTRY()       while (0)
#define _DMX_TSO_RETURN(_ret)  return _ret

#endif // #ifdef DMX_MUTEX

#define DMX_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_PVR_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_PVR_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_MMFI_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_MMFI_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_DBGMSG(_level,_f)                   {if(_u32DMXDbgLevel >= (_level)) (_f);}
#define DMX_PVRDBGMSG(_level,_f)                {if(DMX_DBG_ERR == (_level)) (_f);}
#define DMX_TSOMSG(_level,_f)                   {if(DMX_DBG_ERR >= (_level)) (_f);}
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
static _DMX_CAP_STRTBL _tblDmxCapStr[] =
{
    {"PID_FILTER_NUM",                  4, DMX_CAP_PID_FILTER_NUM},
    {"SEC_FILTER_NUM",                  4, DMX_CAP_SEC_FILTER_NUM},
    {"SEC_BUF_NUM",                     4, DMX_CAP_SEC_BUF_NUM},

    {"PVR_ENG_NUM",                     4, DMX_CAP_PVR_ENG_NUM},
    {"PVR_FILTER_NUM",                  4, DMX_CAP_PVR_FILTER_NUM},
    {"PVR1_FILTER_NUM",                 4, DMX_CAP_PVR1_FILTER_NUM},

    {"MMFI_AUDIO_FILTER_NUM",           4, DMX_CAP_MMFI_AUDIO_FILTER_NUM},
    {"MMFI_V3D_FILTER_NUM",             4, DMX_CAP_MMFI_V3D_FILTER_NUM},

    {"MMFI0_FILTER_NUM",                4, DMX_CAP_MMFI0_FILTER_NUM},
    {"MMFI1_FILTER_NUM",                4, DMX_CAP_MMFI1_FILTER_NUM},

    {"TSIF_NUM",                        4, DMX_CAP_TSIF_NUM},
    {"DEMOD_NUM",                       4, DMX_CAP_DEMOD_NUM},
    {"TS_PAD_NUM",                      4, DMX_CAP_TS_PAD_NUM},
    {"VQ_NUM",                          4, DMX_CAP_VQ_NUM},

    {"CA_FLT_NUM",                      4, DMX_CAP_CA_FLT_NUM},
    {"CA_KEY_NUM",                      4, DMX_CAP_CA_KEY_NUM},

    {"FW_ALIGN",                        4, DMX_CAP_FW_ALIGN},
    {"VQ_ALIGN",                        4, DMX_CAP_VQ_ALIGN},
    {"VQ_PITCH",                        4, DMX_CAP_VQ_PITCH},
    {"SEC_BUF_ALIGN",                   4, DMX_CAP_SEC_BUF_ALIGN},
    {"PVR_ALIGN",                       4, DMX_CAP_PVR_ALIGN},

    {"PVRCA_PATH_NUM",                  4, DMX_CAP_PVRCA_PATH_NUM},

    {"SHAREKEY_FLT_RANGE",              8, DMX_CAP_SHAREKEY_FLT_RANGE},
    {"PVRCA0_FLT_RANGE",                8, DMX_CAP_PVRCA0_FLT_RANGE},
    {"PVRCA1_FLT_RANGE",                8, DMX_CAP_PVRCA1_FLT_RANGE},
    {"PVRCA2_FLT_RANGE",                8, DMX_CAP_PVRCA2_FLT_RANGE},
    {"SHAREKEY_FLT1_RANGE",             8, DMX_CAP_SHAREKEY_FLT1_RANGE},
    {"SHAREKEY_FLT2_RANGE",             8, DMX_CAP_SHAREKEY_FLT2_RANGE},

    {"HW_TYPE",                         4, DMX_CAP_HW_TYPE},

    {"RESOURCE_SIZE",                   4, DMX_CAP_RESOURCE_SIZE},

    {"VFIFO_NUM",                       4, DMX_CAP_VFIFO_NUM},
    {"AFIFO_NUM",                       4, DMX_CAP_AFIFO_NUM},

    {"HWPCR_SUPPORT",                   4, DMX_CAP_HWPCR_SUPPORT},
    {"PCRFLT_START_IDX",                4, DMX_CAP_PCRFLT_START_IDX},
    {"TSP_HWWP_SET_NUM",                4, DMX_CAP_TSP_HWWP_SET_NUM},

    {"DSCMB_ENG_NUM",                   4, DMX_CAP_DSCMB_ENG_NUM},

    {"MAX_MERGESTR_NUM",                4, DMX_CAP_MAX_MERGESTR_NUM},

    {"MAX_SEC_FLT_DEPTH",               4, DMX_CAP_MAX_SEC_FLT_DEPTH},
    {"FW_BUF_SIZE",                     4, DMX_CAP_FW_BUF_SIZE},
    {"FW_BUF_RANGE",                    4, DMX_CAP_FW_BUF_RANGE},
    {"VQ_BUF_RANGE",                    4, DMX_CAP_VQ_BUF_RANGE},
    {"SEC_BUF_RANGE",                   4, DMX_CAP_SEC_BUF_RANGE},
    {"FIQ_NUM",                         4, DMX_CAP_FIQ_NUM},
};


static MSIF_Version _api_dmx_version = {
    .DDI = { DMX_API_VERSION },
};

#ifndef DMX_UTOPIA_20
static DMX_RESOURCE_PRIVATE         _dmx_res = {
    .bResoureInit                               = FALSE,

    ._PvrNotify                                 = {[0 ... (DMX_MAX_IF_NUM-1)] = NULL},
    ._Dmx_Mutex                                 = -1,
    ._Dmx_Pvr_Mutex                             = -1,
    ._Dmx_MMFI_Mutex                            = -1,
    ._Dmx_TSO_Mutex                             = -1,

    #if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    ._bPVRCA_RecAll                             = {[0 ... (DMX_MAX_IF_NUM-1)] = FALSE},
    ._u32PVREngSrc                              = {0},
    #endif

    ._bFWInit                                   = FALSE,

    #if (defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    ._bVQEnabled                                = FALSE,
    #endif

    ._u32TspFltNum                              = 0,

};

static DMX_RESOURCE_PRIVATE*        _pdmx_res       = &_dmx_res;
#else
static DMX_RESOURCE_PRIVATE*        _pdmx_res       = NULL;
#endif

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
MS_U32                              _u32TspFwAddr   = 0;
MS_U32                              _u32TspFwSize   = 0;
MS_U32                              _u32VQAddr      = 0;
MS_U32                              _u32VQSize      = 0;
#endif

MS_U32                              _Owner          = 0;

static MS_U32                       _u32DMXDbgLevel = DMX_DBG_ERR;

#if DMX_DEBUG
extern MS_U32 u32DbgLogFlag;
extern MS_U32 u32DmxDbgFltId;
extern DMX_AV_FIFO_STR stDmxType[];
extern DMX_FLT_DBG_INFO stDmxDbgInfo[];
extern FILE* pfSEC;
#endif

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------

#if 0
static const unsigned long drv_crc32_table[256] =
{
    0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L, 0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
    0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L, 0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
    0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L, 0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
    0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L, 0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
    0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L, 0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
    0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L, 0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
    0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L, 0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
    0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L, 0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
    0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL, 0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
    0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L, 0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
    0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL, 0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
    0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L, 0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
    0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL, 0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
    0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L, 0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
    0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL, 0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
    0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L, 0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
    0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L, 0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
    0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL, 0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
    0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L, 0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
    0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL, 0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
    0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L, 0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
    0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL, 0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
    0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L, 0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
    0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL, 0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
    0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L, 0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
    0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L, 0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
    0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L, 0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
    0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L, 0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
    0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L, 0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
    0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L, 0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
    0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L, 0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
    0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L, 0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L
};

static int _TSP_GetCrc32(unsigned char *buffer, unsigned long length)
{
    unsigned long ulCRC = 0xFFFFFFFF;

    while (length--)
    {
        ulCRC = (ulCRC << 8) ^ drv_crc32_table[((ulCRC >> 24) & 0xFF) ^ *buffer++];
    }

    return (ulCRC);
}
#endif

static MS_BOOL _DMX_InitResource(DMX_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii;

#ifdef DMX_MUTEX
    presource->_Dmx_Mutex =                 -1;
    presource->_Dmx_Pvr_Mutex =             -1;
    presource->_Dmx_MMFI_Mutex =            -1;
    presource->_Dmx_TSO_Mutex =             -1;
#endif

    for(u32ii = 0; u32ii < DMX_MAX_IF_NUM; u32ii++)
    {
        presource->_PvrNotify[u32ii] = NULL;
    }

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    for(u32ii = 0; u32ii < DMX_MAX_IF_NUM; u32ii++)
    {
        presource->_bPVRCA_RecAll[u32ii] = FALSE;
        presource->_u32PVREngSrc[u32ii] = 0;
    }
#endif

    presource->_bFWInit = FALSE;

#if (defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    presource->_bVQEnabled = FALSE;
#endif

    presource->_u32TspFltNum = 0;

    return TRUE;
}

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
static void _DMX_NotifyPvrBySize(DrvRASP_Event eEvent, DrvRASP_Msg *pMsg)
{
    if(_pdmx_res->_PvrNotify[pMsg->u8PVREngNum] == NULL)
    {
        MS_CRITICAL_MSG(DMX_DBGMSG(DMX_DBG_WARN, printf("Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__)));
    }
    else
    {
        _pdmx_res->_PvrNotify[pMsg->u8PVREngNum](pMsg->u8PVREngNum, (DMX_EVENT)eEvent);
    }
}
#endif

#if !(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
static void _DMX_NotifyPvr(DrvTSP_Event eEvent, DrvTSP_Msg *pMsg)
{
    MS_U32 u32PVRID = 0;
#if !(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

#ifdef TS3_IF_SUPPORT
    if(eEvent & (E_DRVTSP_EVENT_PVR2BUF_FULL|E_DRVTSP_EVENT_PVR2BUF_OVERFLOW))
    {
        u32PVRID = 1;
    }
#endif

#ifdef TS2_IF_SUPPORT
    if(eEvent & (E_DRVTSP_EVENT_CBBUF_FULL|E_DRVTSP_EVENT_CBBUF_OVERFLOW))
    {
        if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_PVR_FILTER_NUM, (void*)&u32PVRID))
        {
            u32PVRID = 1;
        }
        else
        {
            u32PVRID--;
    }
    }
#endif
#endif

    if(_pdmx_res->_PvrNotify[u32PVRID] == NULL)
    {
        MS_CRITICAL_MSG(DMX_DBGMSG(DMX_DBG_WARN, printf("Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__)));
    }
    else
    {
        _pdmx_res->_PvrNotify[u32PVRID]((pMsg->PvrBufId)>> MSG_PVRBUF_ID_SHFT, (DMX_EVENT)eEvent);
    }
}
#endif

//Only for section now
static void _DMX_Notify(DrvTSP_Event eEvent, DrvTSP_Msg *pMsg)
{
    MS_U32 u32SecFltId = (pMsg->FltInfo & MSG_FLTINFO_SEC_ID_MASK);
    MS_U32 u32DMXId = u32SecFltId;

    //printf("[%s] FltID [%x]\n",__FUNCTION__, (int)u32SecFltId);
    //printf("[%s] Event [%d]\n",__FUNCTION__, eEvent);

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    //searching mapping PID filter id
    if(u32SecFltId != _pdmx_res->_FltList[u32DMXId].SecBufId)
    {
        MS_U32 u32FltNum = 0;
        if(MDrv_TSP_GetCap(E_DRVTSP_CAP_PID_FILTER_NUM, (void*)&u32FltNum) != DRVTSP_OK)
        {
            return;
        }
        for(u32DMXId = 0; u32DMXId < u32FltNum; u32DMXId++)
        {
            if(_pdmx_res->_FltList[u32DMXId].SecBufId == 0xFF)
                continue;

            if(u32SecFltId == _pdmx_res->_FltList[u32DMXId].SecBufId)
            {
                break;
            }
        }
        if(u32DMXId == u32FltNum)
        {
            return;
        }
    }
#endif

#if DMX_DEBUG
    {
        MS_U32 u32TimeCB = 0;
        MS_BOOL bMonitorCB = FALSE;
        MS_BOOL bMonitorEvent = FALSE;
        dmx_dbg_IsFltMonitor(u32DMXId, bMonitorCB);
        bMonitorCB = bMonitorCB && (u32DbgLogFlag & DMX_DBG_LOG_FLT_CB);

        if(bMonitorCB)
            u32TimeCB = MsOS_GetSystemTime();

        dmx_dbg_IsFltMonitor(u32DMXId, bMonitorEvent);
        if(bMonitorEvent && (u32DbgLogFlag & DMX_DBG_LOG_FLT_SEC_OVF) && (eEvent == E_DRVTSP_EVENT_BUF_OVERFLOW))
            dmx_dbg_print("[DMX OVF]\tFlt[%03d]\n", (int)u32DMXId);

        if(bMonitorEvent && (u32DbgLogFlag & DMX_DBG_LOG_FLT_SEC_RDY) && (eEvent == E_DRVTSP_EVENT_DATA_READY))
            dmx_dbg_print("[DMX RDY]\tFlt[%03d]\n", (int)u32DMXId);

        if(eEvent == E_DRVTSP_EVENT_DATA_READY)
        {
            stDmxDbgInfo[u32SecFltId].u32CB_rdy ++;
        }
        else if(eEvent == E_DRVTSP_EVENT_BUF_OVERFLOW)
        {
            stDmxDbgInfo[u32SecFltId].u32CB_ovf ++;
        }

#endif

    if((_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.Event&DMX_EVENT_CB_SELF_TYPE2) == DMX_EVENT_CB_SELF_TYPE2)
    {
        if(_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify)
        {
            _pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify(u32DMXId, (DMX_EVENT)eEvent,_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.Type2NotifyParam1);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_WARN, printf("Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }
    else
    {
        if(_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pNotify)
        {
            _pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pNotify(u32DMXId, (DMX_EVENT)eEvent);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_WARN, printf("Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }

#if DMX_DEBUG
        if(bMonitorCB)
        {
            dmx_dbg_print("[DMX CB]\tFlt[%03d] PID[0x%x] time[%d ms]\n",
                          (int)u32DMXId,
                          (unsigned int)_pdmx_res->_FltList[(u32DMXId)].u16Pid,
                          (int)(MsOS_GetSystemTime() - u32TimeCB));

        }
    }
#endif

}

// check if buffer range is already used by other filter
#if 1
static MS_BOOL _DMX_ChkBufOverlap(MS_U32 u32DmxId, MS_U32 u32Addr, MS_U32 u32Size)
{
    MS_S32 idx;

    for(idx = _pdmx_res->_u32TspFltNum - 1; idx >= 0 ; idx--)
    {
        DMX_FILTER_TYPE type;

        _FLT_LIST_TYPE_GET(idx, &type);
        if((type == DMX_FILTER_TYPE_SECTION)  ||
           (type == DMX_FILTER_TYPE_TELETEXT) ||
           (type == DMX_FILTER_TYPE_PES)      ||
           (type == DMX_FILTER_TYPE_PACKET))
        {   // filter is opened
            DMX_Sect_info*  pInfo = &(_FLT_LIST_INFO_GET_P(idx)->Info.SectInfo);
            if(idx != (MS_S32)u32DmxId)
            {
                MS_U32 max = _MAX(pInfo->SectBufAddr, u32Addr);
                MS_U32 min = _MIN(pInfo->SectBufAddr, u32Addr);
                MS_U32 size = (min == pInfo->SectBufAddr)? pInfo->SectBufSize : u32Size;

                if((pInfo->SectBufAddr == u32Addr) || ((min + size) > (max)))
                {
                    printf("ERROR: [MAPI DMX][%06d][%ld:%ld] Buffer overlap, 0x%x (0x%x)<==> 0x%x (0x%x)\n",
                    __LINE__,
                    u32DmxId,
                    idx,
                    (int)pInfo->SectBufAddr,
                    (int)pInfo->SectBufSize,
                    (int)u32Addr,
                    (int)u32Size
                    );

                    return TRUE;
                }
            }
        }
    } // for loop

    return FALSE;
}
#endif

static MS_BOOL _DMX_InfoSet_Stream(MS_U8 u8DmxId, DMX_Flt_info* pDmxFltInfo)
{
    _FLT_LIST_INFO_SET(u8DmxId, pDmxFltInfo);
    return TRUE;
}

static MS_BOOL _DMX_InfoSet_Sect(MS_U8 u8DmxId, DMX_Flt_info* pDmxFltInfo)
{
    DMX_FILTER_TYPE        DmxFltType;
    DMX_Sect_info*         pInfo = &(pDmxFltInfo->Info.SectInfo);
    DMX_Sect_info*         pInfoPrev = &(_FLT_LIST_INFO_GET_P(u8DmxId)->Info.SectInfo);

    _FLT_LIST_TYPE_GET(u8DmxId, &DmxFltType);
    if (DMX_FILTER_TYPE_PCR == DmxFltType)
    {
        return TRUE;
    }
    if (0 == pInfo->SectBufSize)
    {
       DMX_DBGMSG(DMX_DBG_WARN, printf("warning: [MAPI DMX][%06d] buffer size = 0, buffer address = 0x%08x\n", __LINE__, (int)pInfo->SectBufAddr));
    }
    if ((pInfoPrev->SectBufAddr != pInfo->SectBufAddr) && (0 != pInfoPrev->SectBufSize) && (pInfoPrev->SectBufSize!= pInfo->SectBufSize))
    {
        MS_ASSERT(pInfoPrev->SectBufAddr);
    }

    MDrv_TSP_SecFlt_SetMode(0, _pdmx_res->_FltList[u8DmxId].SecBufId/*u8DmxId*/, (DrvTSP_FltMode)pInfo->SectMode);

    if(_DMX_ChkBufOverlap(u8DmxId, (MS_U32)pInfo->SectBufAddr, (MS_U32)pInfo->SectBufSize))
    {
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
        //while(1)
        {
            printf("FATAL ERROR: [MAPI DMX][%06d]Section buffer overlap!!!!!!!!!\n", __LINE__);
        }
        return FALSE;
#endif
    }
    MDrv_TSP_SecFlt_SetBuffer(0, _pdmx_res->_FltList[u8DmxId].SecBufId, (MS_PHYADDR)pInfo->SectBufAddr, (MS_U32)pInfo->SectBufSize);

    if (pInfo->pNotify || (((pInfo->Event&DMX_EVENT_CB_SELF_TYPE2) == DMX_EVENT_CB_SELF_TYPE2) && pInfo->pType2Notify))
    {
        if((pInfo->pNotify == NULL) && (pInfo->pType2Notify == NULL))
        {
            DMX_DBGMSG(DMX_DBG_WARN, printf("Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
        MDrv_TSP_SecFlt_Notify(0, _pdmx_res->_FltList[u8DmxId].SecBufId, (DrvTSP_Event)pInfo->Event, _DMX_Notify);
    }
    _FLT_LIST_INFO_SET(u8DmxId, pDmxFltInfo);
    return TRUE;
}

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
static MS_U32 _DMX_ApiDrv_FltSrcMapping(MS_U32 u32TSPFltSource)
{
    switch (u32TSPFltSource & DMX_FILTER_SOURCE_TYPE_MASK)
    {
        case DMX_FILTER_SOURCE_TYPE_LIVE:
            return E_TSP_FLT_SRC_LIVE0;
        case DMX_FILTER_SOURCE_TYPE_TS1:
            return E_TSP_FLT_SRC_LIVE1;
        case DMX_FILTER_SOURCE_TYPE_TS2:
            return E_TSP_FLT_SRC_LIVE2;
        case DMX_FILTER_SOURCE_TYPE_TS3:
            return E_TSP_FLT_SRC_LIVE3;
        case DMX_FILTER_SOURCE_TYPE_FILE:
            return E_TSP_FLT_SRC_FILE0;
        case DMX_FILTER_SOURCE_TYPE_FILE1:
            return E_TSP_FLT_SRC_FILE1;
        case DMX_FILTER_SOURCE_TYPE_FILE2:
            return E_TSP_FLT_SRC_FILE2;
        case DMX_FILTER_SOURCE_TYPE_FILE3:
            return E_TSP_FLT_SRC_FILE3;
            // for backward compatible start
        case DMX_FILTER_SOURCE_TYPE_PVR0:
            return E_TSP_FLT_SRC_LIVE2;
        case DMX_FILTER_SOURCE_TYPE_PVR1:
            return E_TSP_FLT_SRC_LIVE3;
            // for backward compatible end

        default:
            DMX_DBGMSG(DMX_DBG_ERR,  printf("[MAPI DMX][%06d] Bad filter src %ld\n", __LINE__, (MS_U32)u32TSPFltSource));
            return E_TSP_FLT_SRC_LIVE0;
    }
}
#endif

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
static DMX_FILTER_STATUS _DMX_Open_Stream(MS_U8* pu8DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId = 0xFF;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = (DrvTSP_FltType)DMX_FILTER_TYPE_VIDEO;

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    u32TSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);

    if (DMX_FILTER_TYPE_VIDEO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO2== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO2 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO3== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO3 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_VIDEO3D== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO3D | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_REC== DmxFltType)
    {
       eTspFltType= E_DRVTSP_FLT_TYPE_REC | (DrvTSP_FltType)u32TSPFltSource;
    }
    else
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, eTspFltType, &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));


    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFF, DmxFltType);


    *pu8DmxId=          (MS_U8)FltId;
    return DMX_FILTER_STATUS_OK;
}
#else // end TSP_VER_4_0
static DMX_FILTER_STATUS _DMX_Open_Stream(MS_U8* pu8DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId = 0;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = (DrvTSP_FltType)DMX_FILTER_TYPE_VIDEO;

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    if ((DMX_FILTER_TYPE_VIDEO!= DmxFltType) && (DMX_FILTER_TYPE_AUDIO!= DmxFltType) &&
        (DMX_FILTER_TYPE_AUDIO2!= DmxFltType) && (DMX_FILTER_TYPE_VIDEO3D!= DmxFltType)
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
         && (DMX_FILTER_TYPE_AUDIO3!= DmxFltType) && (DMX_FILTER_TYPE_AUDIO4!= DmxFltType)
         && (DMX_FILTER_TYPE_SCMBCHK!= DmxFltType)
#endif
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
         && (DMX_FILTER_TYPE_REC!= DmxFltType)
#endif
         )
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DMX_FILTER_TYPE_VIDEO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO2== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO2 | (DrvTSP_FltType)u32TSPFltSource;
    }
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    else if (DMX_FILTER_TYPE_AUDIO3== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO3 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO4== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO4 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_SCMBCHK== DmxFltType)
    {
        eTspFltType = E_DRVTSP_FLT_TYPE_PVR | (DrvTSP_FltType)u32TSPFltSource; //allocate filter from 127 to 0
    }
#endif
    else if (DMX_FILTER_TYPE_VIDEO3D== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO3D | (DrvTSP_FltType)u32TSPFltSource;
    }
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
    else if (DMX_FILTER_TYPE_REC== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_REC | (DrvTSP_FltType)u32TSPFltSource;
    }
#endif

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, eTspFltType, &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    //For getting PID filter scrambled status only
    if (DMX_FILTER_TYPE_SCMBCHK== DmxFltType)
    {
        if(MDrv_TSP_PidFlt_SetInputSrc(_Owner, FltId, (DrvTSP_FltType)(u32TSPFltSource & (E_DRVTSP_FLT_SOURCE_TYPE_MASK|E_DRVTSP_FLT_SOURCEID_MASK))) != DRVTSP_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
#endif

    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFF, DmxFltType);
    *pu8DmxId=          (MS_U8)FltId;
    return DMX_FILTER_STATUS_OK;
}
#endif // end else

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
static DMX_FILTER_STATUS _DMX_Open_Sect(MS_U8* pu8DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId= 0xFF;
    MS_U32              SecBufId= 0xFF;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_ASSERT(
        (DMX_FILTER_TYPE_SECTION== DmxFltType) || (DMX_FILTER_TYPE_TELETEXT== DmxFltType) ||
        (DMX_FILTER_TYPE_PES== DmxFltType) || (DMX_FILTER_TYPE_PACKET== DmxFltType) ||
        (DMX_FILTER_TYPE_PCR== DmxFltType),
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__)));

    u32TSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner , (DrvTSP_FltType)(eTspFltType|u32TSPFltSource), &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));

    if (DMX_FILTER_TYPE_PCR== DmxFltType)
    {
        //set pcr src
        MDrv_TSP_FLT_SetPCRSrc(FltId,u32TSPFltSource);
        goto _Open_Sect_end;
    }

    if ( (DMX_FILTER_TYPE_SECTION == DmxFltType) || (DMX_FILTER_TYPE_PES == DmxFltType) ||
        (DMX_FILTER_TYPE_PACKET == DmxFltType) || (DMX_FILTER_TYPE_TELETEXT == DmxFltType) )
    {
        if (DRVTSP_OK!= MDrv_TSP_SecFlt_Alloc(_Owner, &SecBufId))
        {
            MDrv_TSP_PidFlt_Free(0 , FltId);
            return DMX_FILTER_STATUS_ERROR;
        }

        MDrv_TSP_PidFlt_SelSecFlt(0, FltId, SecBufId);
    }
_Open_Sect_end:
// @F_TODO check if DmxFltType is use else where for sw status then use it to set HW status
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, SecBufId, DmxFltType);

    *pu8DmxId = (MS_U8)FltId;
    return DMX_FILTER_STATUS_OK;
}
#else // end TSP_VER_4_0
static DMX_FILTER_STATUS _DMX_Open_Sect(MS_U8* pu8DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId= 0;
    MS_U32              SecBufId= 0xFF;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    *pu8DmxId = 0xFF;
    u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK;
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_ASSERT(
        (DMX_FILTER_TYPE_SECTION== DmxFltType) || (DMX_FILTER_TYPE_TELETEXT== DmxFltType) ||
        (DMX_FILTER_TYPE_PES== DmxFltType) || (DMX_FILTER_TYPE_PACKET== DmxFltType) ||
        (DMX_FILTER_TYPE_PCR== DmxFltType)
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
        || (DMX_FILTER_TYPE_SECTION_VER== DmxFltType)
#endif
        , DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__)));

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)))
        case DMX_FILTER_TYPE_SECTION_VER:
            eTspFltType =           E_DRVTSP_FLT_TYPE_SECTION_VER;
            break;
#endif
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner , (DrvTSP_FltType)(eTspFltType|u32TSPFltSource), &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));

    if ((DMX_FILTER_TYPE_PCR== DmxFltType)) // open pid filter only for pcr since we have HWPCR now
    {
        goto _Open_Sect_end;
    }

#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)))
    SecBufId = FltId;
#endif

    if (DRVTSP_OK!= MDrv_TSP_SecFlt_Alloc(_Owner, &SecBufId))
    {
        MDrv_TSP_PidFlt_Free(0 , FltId);
        return DMX_FILTER_STATUS_ERROR;
    }

    MDrv_TSP_PidFlt_SelSecFlt(0, FltId, SecBufId);

_Open_Sect_end:

    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, SecBufId, DmxFltType);
    *pu8DmxId = (MS_U8)FltId;
    return DMX_FILTER_STATUS_OK;
}
#endif

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
static MS_BOOL _DMX_PVR_FlowSet(DMX_PVR_ENG eEng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec)
{
    MS_U32 u32Eng = 0;
    MS_U32 u32PVRSrc = 0;
    switch (eEng)
    {
        case DMX_PVR_EGN0:
            u32Eng = 0;
            break;
        case DMX_PVR_EGN1:
            u32Eng = 1;
            break;
        case DMX_PVR_EGN2:
            u32Eng = 2;
            break;
        case DMX_PVR_EGN3:
            u32Eng = 3;
            break;
        default:
            return FALSE;
    }

    switch (ePvrSrcTSIf)
    {
        case DMX_TSIF_LIVE0:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE2;
            break;
        case DMX_TSIF_LIVE3:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE3;
            break;
        case DMX_TSIF_FILE0:
            u32PVRSrc = E_TSP_FLT_SRC_FILE0;
            break;
        case DMX_TSIF_FILE1:
            u32PVRSrc = E_TSP_FLT_SRC_FILE1;
            break;
        case DMX_TSIF_FILE2:
            u32PVRSrc = E_TSP_FLT_SRC_FILE2;
            break;
        case DMX_TSIF_FILE3:
            u32PVRSrc = E_TSP_FLT_SRC_FILE3;
            break;
        default:
            return FALSE;
    }

    MDrv_TSP_PVR_SetFlowSource(u32Eng,u32PVRSrc);

    return TRUE;

}
#endif

#ifdef TSO_20_ENABLE
static MS_BOOL _TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg)
{
    MS_U8 u8Eng = 0;
    DrvTSO_If_Set IfSet;
    DrvTSO_If eIf = E_DRVTSO_IF_LIVE0;
    DrvTSO_CtrlMode eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
    //MS_U32 u32timeout = 3000, u32StartTime = 0;
    //MS_BOOL bSVQBusy = TRUE;

    if(pstInputCfg == NULL)
        return DMX_FILTER_STATUS_ERROR;

    //printf("[%s][%d] eFlow %x \n", __FUNCTION__, __LINE__, (int)pstInputCfg->eFlow);

    switch(pstInputCfg->eFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8Eng = 0;
            break;
        default:
            return FALSE;
    }

    
    //printf("[%s][%d] eTSOInIf %x \n", __FUNCTION__, __LINE__, (int)pstInputCfg->eTSOInIf);

    switch(pstInputCfg->eTSOInIf)
    {
        case DMX_TSIF_LIVE0:
            eIf = E_DRVTSO_IF_LIVE0;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
            break;
        case DMX_TSIF_LIVE1:
            eIf = E_DRVTSO_IF_LIVE1;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS1;
            break;
        case DMX_TSIF_LIVE2:
            eIf = E_DRVTSO_IF_LIVE2;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS2;
            break;
        case DMX_TSIF_FILE0:
            eIf = E_DRVTSO_IF_FILE0;
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM;
            break;
        case DMX_TSIF_FILE1:
            eIf = E_DRVTSO_IF_FILE1;
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM1;
            break;
        default:
            return FALSE;
    }

    //Disable CH Interface first

    /*if(DRVTSO_OK != MDrv_TSO_Ch_InputEnable(u8Eng, eIf, FALSE))
    {
        return FALSE;
    }*/

    //-------   Check SVQ TX empty  ----------------------
#if 0
    u32StartTime = MsOS_GetSystemTime();
    while((MsOS_GetSystemTime() - u32StartTime) < u32timeout)
    {
        if((bSVQBusy = MDrv_TSO_Check_IsSVQTxBusy(u8Eng, eIf)) == FALSE)
        {
            break;
        }
        MsOS_DelayTask(1);
    }
    if(bSVQBusy == TRUE)
        return FALSE;

    bSVQBusy = TRUE;
    u32StartTime = MsOS_GetSystemTime();
    while((MsOS_GetSystemTime() - u32StartTime) < u32timeout)
    {
        if((bSVQBusy = !MDrv_TSO_Check_IsSVQTxEmpty(u8Eng, eIf)) == FALSE)
        {
            break;
        }
        MsOS_DelayTask(1);
    }

    if(bSVQBusy == TRUE)
        return FALSE;

#endif
    //------------------------------------------------------

    //printf("[%s][%d] u8LocalStrId %x \n", __FUNCTION__, __LINE__, (int)pstInputCfg->u8LocalStrId);

    if(DRVTSO_OK!= MDrv_TSO_LocalStreamId(u8Eng, eIf, &(pstInputCfg->u8LocalStrId), TRUE))
    {
        return FALSE;
    }

    if((pstInputCfg->eTSOInIf != DMX_TSIF_FILE0) && (pstInputCfg->eTSOInIf != DMX_TSIF_FILE1))
    {
        IfSet.bClkInv = pstInputCfg->stInputInfo.bClkInv;
        IfSet.bParallel = pstInputCfg->stInputInfo.bParallel;
        IfSet.bExtSync = pstInputCfg->stInputInfo.bExtSync;
        IfSet.ePad = 0;
        switch (pstInputCfg->stInputInfo.Input)
        {
            case DMX_FLOW_INPUT_DEMOD:
                IfSet.ePad = E_DRVTSO_PAD_DEMOD;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT0;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT1;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT2;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT3;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT4;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT5;
                break;
            default:
                return FALSE;
        }

        if(MDrv_TSO_SelPad(u8Eng, eIf, &IfSet) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    //printf("[%s][%d] eCtrMode %x , bBypassAll %x , bEnable %x\n", __FUNCTION__, __LINE__, (int)eCtrMode, (int)pstInputCfg->bBypassAll, (int)pstInputCfg->bEnable);
    if(MDrv_TSO_SetOperateMode(u8Eng, eCtrMode, pstInputCfg->bBypassAll, pstInputCfg->bEnable) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    return TRUE;

}

static MS_BOOL _TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg)
{
    MS_U8 u8Eng = 0;

    //default: 172.8/2(15+1) = 5.4M
    DrvTSO_OutClk eOutClk = E_DRVTSO_OUTCLK_DIV2N;
    DrvTSO_OutClkSrc eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
    MS_U16  u16DivNum = 0x0F;
    MS_BOOL bClkIv = FALSE;
    //DrvTSO_PadOut eOutPad = (DrvTSO_PadOut)(pstOutputCfg->eOutPad);

    if(pstOutputCfg == NULL)
        return FALSE;

    /*if(MDrv_TSO_OutputEnable(u8Eng, FALSE) != DRVTSO_OK)
    {
        return FALSE;
    }*/

    switch(pstOutputCfg->eFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8Eng = 0;
            break;
        default:
            return FALSE;
    }

#if !defined(TSO_HW_BOX_SERIES) //[temporarily]
    if(MDrv_TSP_OutputPadCfg(((MS_U32)pstOutputCfg->eOutPad)|E_DRVTSP_OUTPAD_FROM_TSO, E_DRVTSP_PAD_TSO, TRUE, TRUE) != DRVTSP_OK)
    {
        //printf("[%s][%d] \n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif

    //default output pad select
    /*if(MDrv_TSO_OutputPad(0, &eOutPad, TRUE) != DRVTSO_OK)
    {
        return FALSE;
    }*/

    //Setting default output clock
    if(pstOutputCfg->bDefOutClk == FALSE)
    {
        eOutClk = pstOutputCfg->eTsoOutClk;
        eOutClkSrc = pstOutputCfg->eTsoOutClkSrc;
        u16DivNum = pstOutputCfg->u16DivNum;
        bClkIv = pstOutputCfg->bOutClkInv;
    }
    //printf("[%s]0x%x , 0x%x, 0x%x, 0x%x, 0x%x\n",__FUNCTION__,u8Eng, eOutClk,eOutClkSrc,u16DivNum,bClkIv);
    if(MDrv_TSO_SetOutClk(u8Eng, eOutClk, eOutClkSrc, u16DivNum, bClkIv) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_OutputPktSize(u8Eng, &(pstOutputCfg->u16OutPktSize), TRUE) != DRVTSO_OK)
    {
        return FALSE;
    }

    /*if(MDrv_TSO_OutputEnable(u8Eng, pstOutputCfg->bEnable) != DRVTSO_OK)
    {
        return FALSE;
    }*/
    return TRUE;
}

#elif (defined(TSO_ENABLE) && (!defined(TSO_HW_BOX_SERIES)))
//TSO HW 1.0 for TV series
static MS_BOOL _DMX_FlowSetTSO(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSO_OutClk eOutClk = E_DRVTSO_OUTCLK_DIV2N;
    DrvTSO_OutClkSrc eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
    DrvTSO_If_Set IfSet;
    DrvTSO_CtrlMode eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
    MS_U16  u16DivNum = 0;
    MS_U8 u8Eng = 0;

    switch(DmxFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8Eng = 0;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
            break;

        default:
            return FALSE;
    }

    IfSet.bClkInv = FALSE;
    IfSet.bParallel = bParallel;
    IfSet.bExtSync = bExtSync;
    IfSet.ePad = 0;
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSO_PAD_DEMOD;
            eOutClk = E_DRVTSO_OUTCLK_Dmd;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT0;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT1;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT2;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
            break;
        case DMX_FLOW_INPUT_MEM:
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM;
            //default: 172.8/2(15+1) = 5.4M
            IfSet.ePad = E_DRVTSO_MEM;
            eOutClk = E_DRVTSO_OUTCLK_DIV2N;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
            u16DivNum = 0x0F;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSO_SetOutClk(u8Eng, eOutClk, eOutClkSrc, u16DivNum, bClkInv) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_SelPad(u8Eng, &IfSet) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_SetOperateMode(u8Eng, eCtrMode, bClkInv, bParallel) != DRVTSO_OK)
    {
        return FALSE;
    }

    return TRUE;
}

#else

#ifdef TSO_ENABLE
//TSO HW 1.0 for BOX series
static MS_BOOL _DMX_FlowSetTSO(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{

    DrvTSO_CtrlMode eDrvTSOCtrlMode= E_DRVTSO_CTRL_MODE_INVALID;
    MS_BOOL bClkDis = FALSE;
    MS_U8 u8TSOEng = 0;
    TSP_TSPad ePad = E_TSP_TS_INVALID;
    // STB drvTSP has different enum from TV's
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            ePad = E_TSP_TS_DEMOD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD0;
            break;
        case DMX_FLOW_INPUT_DEMOD1:
            ePad = E_TSP_TS_DEMOD1;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            ePad = E_TSP_TS_PAD1;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            ePad = E_TSP_TS_PAD2;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            ePad = E_TSP_TS_PAD3;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS3;
            break;
        case DMX_FLOW_INPUT_MEM:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_MEM;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD0;
            bClkDis = TRUE;
            break;

        case DMX_FLOW_INPUT_TSO:// TSO cannot input from TSO
        case DMX_FLOW_INPUT_TSO1:// TSO cannot input from TSO
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

    switch (DmxFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8TSOEng = 0;
            break;
        case DMX_FLOW_TSO_PLAYBACK1:
            u8TSOEng = 1;
            break;

        default:
            return FALSE;
    }
    MDrv_TSO_SetOperateMode(u8TSOEng, (eDrvTSOCtrlMode & 0xFF), bClkInv, bParallel);
    MDrv_TSP_TSO_ConfigPad(u8TSOEng, ePad);
//        MDrv_TSO_SetTSO_FROM_TSOUT_MUX; // @F_TODO not implement yet...

    return TRUE;
}
#endif //end of TSO_ENABLE

#endif //end of TSO_HW_BOX_SERIES

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
//-------------------------------------------------------------------------
// For TSP version 3.0:
//      DMX_FLOW_PLAYBACK                                    : TSIF0 / PIDFLT0 / Live and File
//      DMX_FLOW_PVR                                              : TSIF1 / PIDFLT1 / Rec0 (Could change input source later)
//      DMX_FLOW_PVR1                                             : TSIF2 / PIDFLT2 /Rec1  (Could change input source later)
//      DMX_FLOW_PVRCA                                          : TSIF0 / PIDFLT0 /Rec0 with DSCMB0
//      DMX_FLOW_MMFI0 (DMX_FLOW_FILEIN_MM)      : MMFI Audio / MMFIA PIDFLT / File audio
//      DMX_FLOW_MMFI1 (DMX_FLOW_FILEIN_MM3D)  : MMFI V3D / MMFIV3D PIDFLT / File V3D
//      DMX_FLOW_CIPHSS_PLAYBACK                        : TS1 / PIDFLTF for DSCMB / Live for SCMB packet
//      DMX_FLOW_CIPHSS_PVRCA                             : TS1 / PIDFLT1 for DSCMB / Rec0 with DSCMB
//      DMX_FLOW_PVR2                                             : TSIFCB / PIDFLTCB /RecCB
//
//      DMX_FLOW_TSO_PLAYBACK                             : TSOIF / PIDFLTSO /Live and File
//
//      DMX_FLOW_PLAYBACK1                                   : TSIF1 / PIDFLT1 / Live
//      DMX_FLOW_PVRCA1                                        : TSIF1/ PIDFLT1 /Rec1 with DSCMB1
//--------------------------------------------------------------------------
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSP_If_Set IfSet;
    MS_U32  u32DScmbEngId = 0xFF;
    MS_BOOL bFileMode = FALSE;

#ifdef TSO_20_ENABLE

    if(DmxFlow == DMX_FLOW_TSO_PLAYBACK)
    {
        DMX_TSO_InputCfg stInputCfg;
        DMX_TSO_OutputCfg stOutputCfg;

        stOutputCfg.bDefOutClk = FALSE;
        stOutputCfg.bOutClkInv = FALSE;
        switch (DmxFlowInput)
        {
            case DMX_FLOW_INPUT_DEMOD:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_Dmd;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS3IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS4IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS5IN;
                break;
            case DMX_FLOW_INPUT_MEM:
            case DMX_FLOW_INPUT_MEM1:
            default:
               stOutputCfg.bDefOutClk = TRUE;
               break;
        }

        stOutputCfg.eFlow = DmxFlow;
        stOutputCfg.eOutPad = DMX_FLOW_OUTPUT_EXT_PAD1;
        stOutputCfg.u16OutPktSize = 0xBC;
        stOutputCfg.bEnable = TRUE;
        stOutputCfg.bSet = TRUE;
        if(_TSO_Flow_OutputCfg(&stOutputCfg) == FALSE)
            return FALSE;

        stInputCfg.eFlow = DmxFlow;
        stInputCfg.stInputInfo.Input = DmxFlowInput;
        stInputCfg.stInputInfo.bClkInv = bClkInv;
        stInputCfg.stInputInfo.bExtSync = bExtSync;
        stInputCfg.stInputInfo.bParallel = bParallel;
        stInputCfg.u8LocalStrId = 0x47;
        stInputCfg.bBypassAll = TRUE;
        stInputCfg.bEnable = TRUE;
        stInputCfg.bSet = TRUE;
        if(DMX_FLOW_TSO_PLAYBACK== DmxFlow)
        {
            if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE0;
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM1)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE1;
            }
            else
            {
                stInputCfg.eTSOInIf = DMX_TSIF_LIVE0;
            }
        }
        return _TSO_Flow_InputCfg(&stInputCfg);
    }

#endif

    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput)
        || (DMX_FLOW_INPUT_MEM_PASSCA1 == DmxFlowInput))
    {
        if(DMX_FLOW_PLAYBACK == DmxFlow)
        {
            if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
            {
                MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUXFILE, &u32DScmbEngId);
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, (MS_U8)u32DScmbEngId);
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM_NOPASSCA)
            {
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, 0xFF);
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM_PASSCA1)
            {
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, 1);
            }

            MDrv_TSP_File_CMDQ_Reset();

            return TRUE;
        }
        else if(DMX_FLOW_MMFI0 == DmxFlow)
        {
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS1, FALSE); //Select MMFI CMDQ Src
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH0);
            return TRUE;
        }
        else if(DMX_FLOW_MMFI1 == DmxFlow)
        {
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS2, FALSE); //Select MMFI CMDQ Src
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH1);
            return TRUE;
        }
        else if(DMX_FLOW_PLAYBACK1 == DmxFlow)
        {
            bFileMode = TRUE;
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH0);
        }
        else if(DMX_FLOW_PLAYBACK2 == DmxFlow)
        {
            bFileMode = TRUE;
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH1);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
    }

    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSP_PAD_DEMOD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT4;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT5;
            break;
#ifdef TSO_ENABLE
        case DMX_FLOW_INPUT_TSO:
            IfSet.ePad = E_DRVTSP_PAD_TSO;
            break;
#endif
        default:
            break;
    }

    IfSet.bClkInv = bClkInv;
    IfSet.bExtSync = bExtSync;
    IfSet.bParallel = bParallel;

    DrvTSP_If TspIf;
    DrvTSP_CtrlMode CtrlMode;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX0, &u32DScmbEngId);
            break;
        case DMX_FLOW_PLAYBACK1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX1, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS1, bFileMode); //Select MMFI CMDQ Src
            break;
        case DMX_FLOW_PLAYBACK2:
            TspIf = E_DRVTSP_IF_TS2;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS2;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX2, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS2, bFileMode); //Select MMFI CMDQ Src
            break;
        #ifdef MERGE_STR_SUPPORT
        case DMX_FLOW_PLAYBACK_FI:
            TspIf = E_DRVTSP_IF_FI;
            CtrlMode = E_DRVTSP_CTRL_MODE_TSFI_LIVE;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUXFILE, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_FILE, bFileMode);
            break;
        #endif
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            MDrv_TSP_PVR_Eng_SelSrc(0, E_DRVTSP_PKTSRC_DEMUX0);
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            MDrv_TSP_PVR_Eng_SelSrc(1, E_DRVTSP_PKTSRC_DEMUX1);
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_FLOW_PVR2:   //PVR_CB
            TspIf = E_DRVTSP_IF_TS3;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS3;
            break;
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_CA;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX0, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(0, E_DRVTSP_PKTSRC_DEMUX0);
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_FLOW_PVRCA1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR1_CA;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX1, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(1, E_DRVTSP_PKTSRC_DEMUX1);
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_FLOW_CIPHSS_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSP_SetOperateMode(0, CtrlMode, (MS_U8)u32DScmbEngId) != DRVTSP_OK)
        return FALSE;

    if (!((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput)
        || (DMX_FLOW_INPUT_MEM_PASSCA1 == DmxFlowInput)))
    {
        if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
            return FALSE;
    }

    return TRUE;
}//Version 3.0 End

#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput))
    {
        if(!((DMX_FLOW_PLAYBACK == DmxFlow) || (DMX_FLOW_FILEIN_MM == DmxFlow) || (DMX_FLOW_FILEIN_MM3D == DmxFlow)))
        {
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }

        MDrv_TSP_SetTSIF(0, E_TSP_TSIF_EN | E_TSP_TSIF_EXTSYNC | E_TSP_TSIF_PARL, TRUE);
        return TRUE;
    }

    TSP_TSIFCfg         cfg = E_TSP_TSIF_EN;
    TSP_TSPad           pad = E_TSP_TS_DEMOD0;
    MS_BOOL             filein = FALSE;
    MS_BOOL             bClkDis = FALSE;
    MS_U16              u16TSIF = 0;
    if (bExtSync)
    {
        cfg |= E_TSP_TSIF_EXTSYNC;
    }
    if (bParallel)
    {
        cfg |= E_TSP_TSIF_PARL;
    }

    switch (DmxFlowInput)
    {
    case DMX_FLOW_INPUT_DEMOD:
        pad = E_TSP_TS_DEMOD0;
        break;
    case DMX_FLOW_INPUT_EXT_INPUT0:
        pad = E_TSP_TS_PAD0;
        break;
    case DMX_FLOW_INPUT_EXT_INPUT1:
        pad = E_TSP_TS_PAD1;
        break;
    case DMX_FLOW_INPUT_MEM:
        break;
    case DMX_FLOW_INPUT_EXT_INPUT2:
        pad = E_TSP_TS_PAD2;
        break;
    case DMX_FLOW_INPUT_DISABLE:
        pad = E_TSP_TS_PAD0;
        bClkDis = TRUE;
        break;
    default:
        break;
    }

    if(DMX_FLOW_PVRCA == DmxFlow)
    {
        //since PVRCA not support yet, all will go DMX_FLOW_PVR
        DmxFlow = DMX_FLOW_PVR;
    }

    if(DMX_FLOW_PLAYBACK == DmxFlow)
    {
        u16TSIF = 0;
    }
    else if (DMX_FLOW_PVR == DmxFlow)
    {
        MDrv_RASP_PVR_SetTSIF(0, bParallel, bExtSync, FALSE);
        u16TSIF = 1;
    }
    else if (DMX_FLOW_PVR1 == DmxFlow )
    {
        MDrv_RASP_PVR_SetTSIF(1, bParallel, bExtSync, FALSE);
        u16TSIF = 2;
    }
    else if (DMX_FLOW_PVR2 == DmxFlow )
    {
        MDrv_RASP_PVR_SetTSIF(2, bParallel, bExtSync, FALSE);
        u16TSIF = 3;
    }
    else if (DMX_FLOW_PVR3 == DmxFlow )
    {
        MDrv_RASP_PVR_SetTSIF(3, bParallel, bExtSync, FALSE);
        u16TSIF = 4;
    }
    else if (DMX_FLOW_PVR4 == DmxFlow )
    {
        MDrv_RASP_PVR_SetTSIF(4, bParallel, bExtSync, FALSE);
        u16TSIF = 5;
    }
    else if (DMX_FLOW_PVR5 == DmxFlow )
    {
        MDrv_RASP_PVR_SetTSIF(5, bParallel, bExtSync, FALSE);
        u16TSIF = 6;
    }

    MDrv_TSP_SetTSIF(u16TSIF, cfg, filein);
    MDrv_TSP_SelPad(u16TSIF, pad);
    MDrv_TSP_SelPad_ClkInv(u16TSIF, bClkInv);
    MDrv_TSP_SelPad_ClkDis(u16TSIF, bClkDis);

    return TRUE;
}

#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
#if 0 // we don't have this constrain in K3 every path is able to filein
    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput))
    {
        if(!((DMX_FLOW_PLAYBACK == DmxFlow) || (DMX_FLOW_FILEIN_MM == DmxFlow) || (DMX_FLOW_FILEIN_MM3D == DmxFlow)))
        {
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
        MDrv_TSP_SetTSIF(0, E_TSP_TSIF_EN | E_TSP_TSIF_EXTSYNC | E_TSP_TSIF_PARL, TRUE);
        return TRUE;
    }
#endif

    TSP_TSIFCfg         cfg     = E_TSP_TSIF_EN;
    TSP_TSPad           pad     = E_TSP_TS_INVALID;
    TSP_TSId            etid     = E_TSP_TSID_INVALID;
    DRV_TSP_FLOW        eDst    = E_DRV_TSP_FLOW_INVALID;
    MS_BOOL             filein  = FALSE;
    MS_BOOL             bClkDis = FALSE;
    TSP_TSIF            eTSIF   = E_TSP_TSIF_INVALID;

#if 1 // for TV SW, we add PVRCA support for TV needs.
    // all stb path can do a CA PVR record
    if (DMX_FLOW_PVRCA == DmxFlow)
    {
        //since PVRCA not support yet, all will go DMX_FLOW_PVR
        DmxFlow = DMX_FLOW_PVR;
    }
#endif

    //### organize setting into cfg
    if (bExtSync)
    {
        cfg |= E_TSP_TSIF_EXTSYNC;
    }
    if (bParallel)
    {
        cfg |= E_TSP_TSIF_PARL;
    }

    if ((DmxFlow == DMX_FLOW_TSO_PLAYBACK) || (DmxFlow == DMX_FLOW_TSO_PLAYBACK1))
        return _DMX_FlowSetTSO(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel);

    //### setting input source for each destination
  /*  else if ((DmxFlow == DMX_FLOW_RASP0) || (DmxFlow == DMX_FLOW_RASP1))
    {

    }*/
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            pad = E_TSP_TS_DEMOD0;
            break;
        case DMX_FLOW_INPUT_DEMOD1:
            pad = E_TSP_TS_DEMOD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            pad = E_TSP_TS_PAD0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            pad = E_TSP_TS_PAD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            pad = E_TSP_TS_PAD2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            pad = E_TSP_TS_PAD3;
            break;
        case DMX_FLOW_INPUT_TSO:
            pad = E_TSP_TS_PAD_TSO0;
            break;
        case DMX_FLOW_INPUT_TSO1:
            pad = E_TSP_TS_PAD_TSO1;
            break;
        case DMX_FLOW_INPUT_MEM:
            pad = E_TSP_TS_PAD0;
            filein = TRUE;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            pad = E_TSP_TS_PAD0;
            bClkDis = TRUE;
            break;

        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

    switch (DmxFlow)
    {
        //support file-in and live-in simultaneously
        case DMX_FLOW_PLAYBACK:
            eDst = E_DRV_TSP_FLOW_PLAYBACK0;
            eTSIF = E_TSP_TSIF_0;//default mapping of DMX_FLOW_PLAYBACK to TSIF0
            etid = E_TSP_TSID_TSIF0;
            break;
        //support file-in and live-in simultaneously
        case DMX_FLOW_PLAYBACK1:
            eDst = E_DRV_TSP_FLOW_PLAYBACK1;
            eTSIF = E_TSP_TSIF_1;
            etid = E_TSP_TSID_TSIF1;
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK2:
            eDst = E_DRV_TSP_FLOW_PLAYBACK2;
            eTSIF = E_TSP_TSIF_2;
            etid= E_TSP_TSID_TSIF2;
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK3:
            eDst = E_DRV_TSP_FLOW_PLAYBACK3;
            eTSIF = E_TSP_TSIF_3;
            etid = E_TSP_TSID_TSIF3;
            break;
        case DMX_FLOW_PVR:
            eDst = E_DRV_TSP_FLOW_PVR0;
            eTSIF = E_TSP_TSIF_PVR0;
            etid = E_TSP_TSID_TSIF0;
            break;
        case DMX_FLOW_PVR1:
            eDst = E_DRV_TSP_FLOW_PVR1;
            eTSIF = E_TSP_TSIF_PVR1;
            etid = E_TSP_TSID_TSIF1;
            break;
        case DMX_FLOW_PVR2:
            eDst = E_DRV_TSP_FLOW_PVR2;
            eTSIF = E_TSP_TSIF_PVR2;
            etid = E_TSP_TSID_TSIF2;
            break;
        case DMX_FLOW_PVR3:
            eDst = E_DRV_TSP_FLOW_PVR3;
            eTSIF = E_TSP_TSIF_PVR3;
            etid = E_TSP_TSID_TSIF3;
            break;

        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

#if 0
    if (E_TSP_TSID_INVALID != tid)//kaiser like series  // link dst to tsifX
    {
//            if ((tid != E_TSP_TSID_MMFI0) || (tid != E_TSP_TSID_MMFI1)) //
//            {
            if (u16TSIF != E_TSP_TSIF_INVALID)
            {
                MDrv_TSP_SelPad_ClkInv(u16TSIF, bClkInv);
                MDrv_TSP_SetTSIF(u16TSIF, cfg, filein); // we set TSIF at this function (external sync and  parallel)
            }
//            }
    }
    else if ((E_TSP_TS_INVALID != pad)) // old style we set specific path to dst
#endif
    //File-in case
    //No need setting pad in file-in mod

    if (DRVTSP_OK != MDrv_TSP_SetFlowSource(eDst, etid)) //setting dst source from tid
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Set flow source error!\n", __LINE__));
    }


    if(filein == TRUE)
    {
        MDrv_TSP_SetTSIF(eTSIF, E_TSP_TSIF_EN | E_TSP_TSIF_EXTSYNC | E_TSP_TSIF_PARL ,filein);
    }
    //Live-in and PVR case
    else
    {
        if ((E_TSP_TS_INVALID != pad)) // old style we set specific path to dst
        {
            MDrv_TSP_SelPad(eTSIF, pad);  // only old style has pad setting
        }

        MDrv_TSP_SelPad_ClkDis(eTSIF, bClkDis); //@FIXME check return value for each drv function
        MDrv_TSP_SelPad_ClkInv(eTSIF, bClkInv);
        MDrv_TSP_SetTSIF(eTSIF, cfg, filein); // we set TSIF at this function (external sync and  parallel)
   }

#if 0 // @FIXME what is this? for TV SW?
    if (DMX_FLOW_PVRCA == DmxFlow)
    {
        //since PVRCA not support yet, all will go DMX_FLOW_PVR
        DmxFlow = DMX_FLOW_PVR;
    }
#endif

    return TRUE;
}

#else //Version 4.0 End
//-------------------------------------------------------------------------
// For TSP version 1.0:
//      DMX_FLOW_PLAYBACK              : TSIF0 / PIDFLT0 / Live and File
//      DMX_FLOW_PVR                      : TSIF1 / PIDFLT1 / Rec0
//      DMX_FLOW_PVR1                     : TSIFCB / PIDFLTCB /RecCB
//      DMX_FLOW_PVRCA                   : TSIF1 / PIDFLT1 /Rec0 with DSCMB
//      DMX_FLOW_PVRCA1                  : TSIF1 / PIDFLT1 /Rec1 with DSCMB (Source TSIF2 from TSIF1)
//      DMX_FLOW_FILEIN_MM             : MMFI Audio / MMFIA PIDFLT / File audio
//      DMX_FLOW_FILEIN_MM3D         : MMFI V3D / MMFIV3D PIDFLT / File V3D
//      DMX_FLOW_CIPHSS_PLAYBACK  : TS1 / PIDFLTF for DSCMB / Live for SCMB packet
//      DMX_FLOW_CIPHSS_PVRCA       : TS1 / PIDFLT1 for DSCMB / Rec0 with DSCMB
//--------------------------------------------------------------------------
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSP_If_Set IfSet;

    //version 1.0 has only 1 ca0 engine, and no ca1 engine
    if(DmxFlowInput == DMX_FLOW_INPUT_MEM_PASSCA1)
        return FALSE;

#ifdef TSO_ENABLE
    if(DMX_FLOW_TSO_PLAYBACK== DmxFlow)
    {
        return _DMX_FlowSetTSO(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel);
    }
#endif

    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput))
    {
        #if 0
        if(!((DMX_FLOW_PLAYBACK == DmxFlow) || (DMX_FLOW_FILEIN_MM == DmxFlow) || (DMX_FLOW_FILEIN_MM3D == DmxFlow)))
        {
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
        #endif

        if(DmxFlow == DMX_FLOW_PVRCA)
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM_PVR);
        }
        if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM);
        }
        else
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM_NOCA);
        }

        return TRUE;
    }

    memset(&IfSet, 0x0, sizeof(DrvTSP_If_Set));
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSP_PAD_DEMOD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        //-- Note: Not support for all chip -------------------
        #ifdef TS2_IF_SUPPORT
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        #endif
        //-----------------------------------------------

#ifdef TSO_ENABLE
        case DMX_FLOW_INPUT_TSO:
            IfSet.ePad = E_DRVTSP_PAD_TSO;
            break;
#endif

        default:
            MS_ASSERT(0);
            break;
    }

    IfSet.bClkInv = bClkInv;
    IfSet.bExtSync = bExtSync;
    IfSet.bParallel = bParallel;

    DrvTSP_If TspIf;
    DrvTSP_CtrlMode CtrlMode;

#ifdef TS2_IF_SUPPORT
    MS_U32 u32data = 0;
#endif

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            break;
        case DMX_FLOW_PLAYBACK_NOCA:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0_NOCA;
            break;
        case DMX_FLOW_PLAYBACK_SRC_TS1: //TS0 playback, and the source is from TS1
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1_OUT_LIVE0;
            if(MDrv_TSP_SelPad(0, E_DRVTSP_IF_PLAYBACK, &IfSet) != DRVTSP_OK) //enable tsif0
            {
                return FALSE;
            }
            break;
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            break;
        case DMX_FLOW_PVRCA:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
            {
                return FALSE;
            }
            break;

#ifdef TS2_IF_SUPPORT
        #ifdef TS2_LIVE_SUPPORT
        case DMX_FLOW_PLAYBACK1:
        #endif
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_PVR1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS2;
            break;
        case DMX_FLOW_PVRCA1:
            //For Napoli U04, CA input from TSIIF1, and output to TSIF2
            //Must set TSIF1 mux as TSIF2, and Duplicate TS2 Filter to TS1 filter
            MDrv_TSP_CMD_Run(0x00000002, 0, 0, &u32data);
            if(u32data == 0)
            {
                return FALSE;
            }
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_TS2_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            if(MDrv_TSP_SelPad(0, E_DRVTSP_IF_PVR0, &IfSet) != DRVTSP_OK)
            {
                return FALSE;
            }
            TspIf = E_DRVTSP_IF_PVR1;
            break;
#endif

        case DMX_FLOW_CIPHSS_PLAYBACK:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1_FILEFLT;
            break;
        case DMX_FLOW_CIPHSS_PVRCA:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_TS0_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSP_SetOperateMode(0, CtrlMode) != DRVTSP_OK)
    {
        return FALSE;
    }
    if((DmxFlow != DMX_FLOW_PVRCA) && (DmxFlow != DMX_FLOW_CIPHSS_PVRCA))
    {
        if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
        {
            return FALSE;
        }
    }

    return TRUE;
}

#endif //Version 1.0 End

static inline MS_U32 _DMX_RingBuffer_DataSize(MS_U8* pu8Read, MS_U8* pu8Write, MS_U8* pu8BufStart, MS_U8* pu8BufEnd)
{
    return (pu8Write >= pu8Read) ? (MS_U32)(pu8Write- pu8Read): (MS_U32)((pu8BufEnd- pu8BufStart)- (pu8Read- pu8Write));
}

static MS_BOOL _DMX_RingBuffer_ChkPESLen(MS_U8* pu8Read, MS_U8* pu8Start, MS_U8* pu8End, MS_U32 *pSecLen)
{
    *pSecLen = 0;

    //check PES start code first
    if ((pu8Read + 3) <= pu8End)
    {
        if((*pu8Read != 0) || (*(pu8Read+1) != 0) || (*(pu8Read+2) != 0x01))
            return FALSE;
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        if((*pu8Read != 0) || (*(pu8Read+1) != 0) || (*pu8Start != 0x01))
            return FALSE;
    }
    else if ((pu8Read + 1) <= pu8End)
    {
        if((*pu8Read != 0) || (*pu8Start != 0) || (*(pu8Start+1) != 0x01))
            return FALSE;
    }
    else
    {
        if((*pu8Start != 0) || (*(pu8Start+1) != 0) || (*(pu8Start+2) != 0x01))
            return FALSE;
    }

    //calculate PES length
    if ((pu8Read + 6) <= pu8End)
	{
	    *pSecLen = ((*(pu8Read+ 4) & 0xFF) << 8) + *(pu8Read+ 5);
	}
	else if ((pu8Read + 5) <= pu8End)
	{
	    *pSecLen = ((*(pu8Read+ 4) & 0xFF) << 8) + *(pu8Start);
	}
	else if ((pu8Read + 4) <= pu8End)
	{
	    *pSecLen = ((*(pu8Start) & 0xFF) << 8) + *(pu8Start+ 1);
	}
	else if((pu8Read + 3) <= pu8End)
	{
	    *pSecLen = ((*(pu8Start + 1) & 0xFF) << 8) + *(pu8Start+ 2);
	}
	else if((pu8Read + 2) <= pu8End)
	{
	    *pSecLen = ((*(pu8Start + 2) & 0xFF) << 8) + *(pu8Start+ 3);
	}
	else if((pu8Read + 1) <= pu8End)
	{
	    *pSecLen = ((*(pu8Start + 3) & 0xFF) << 8) + *(pu8Start+ 4);
	}
	else
	{
	    *pSecLen = ((*(pu8Start+ 4) & 0xFF) << 8) + *(pu8Start+ 5);
	}

    if (*pSecLen == 0x00)
    {
        *pSecLen = 0xffff;
        //printf("[DMX][%d] Warning: PES length is 0xFFFF\n", __LINE__);
    }
    else
    {
        *pSecLen += 6;
    }

    return TRUE;
}

static DMX_FILTER_STATUS _DMX_GetRingBuffer(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32SecSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    MS_U32      u32RbSize = 0;
    MS_U32      u32SecBufSize= 0, u32SecLen= 0;
    MS_U8*      pu8Start;
    MS_U8*      pu8End = 0;
    MS_U8*      pu8Read;
    MS_U8*      pu8Write;
    MS_U32      u32CopySize= 0;
    MS_BOOL     bCopy;
    MS_PHYADDR  temp = 0;

    //DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));
    //printf("[%s] %d , %p, %ld, %p %p, %p\n", __FUNCTION__, (int)u8DmxId,pu8Buf, u32BufSize, pu32SecSize,pu32RmnSize,pfCheckCB);


#ifndef MSOS_TYPE_LINUX_KERNEL
    MsOS_Sync();
#endif
    MsOS_ReadMemory();

    // Get Section buffer read pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Read = (MS_U8*)temp;
    // Get Section buffer write pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Write = (MS_U8*)temp;

    *pu32SecSize=       0;
    if (pu8Read== pu8Write)
    {
        //printf("[%s] r %p, w %p \n", __FUNCTION__, pu8Read, pu8Write);
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }

    // Get Section buffer start address
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Start = (MS_U8*)temp;
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32SecBufSize))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    // Get Section buffer end address
    pu8End=             pu8Start+ u32SecBufSize;

    if(pu8Read >= pu8End || pu8Read < pu8Start)
    {
        printf("[DMX][%d][%ld] FATAL ERROR: PES buffer read address out of range!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    if(pu8Write >= pu8End || pu8Write < pu8Start)
    {
        printf("[DMX][%d][%ld] FATAL ERROR: PES buffer write address out of range!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }
	if ((pu8Read==0)||(pu8Write==0)||(pu8Start==0))
    {
        printf("[DMX][%d][%ld] FATAL ERROR: PES buffer address illegal!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    *pu32RmnSize = u32RbSize= _DMX_RingBuffer_DataSize(pu8Read, pu8Write, pu8Start, pu8End);
    if (u32RbSize> u32BufSize)
    {
        u32RbSize = u32BufSize;
    }

    //MS_ASSERT(pu8Start == (MS_U8*)_pdmx_res->_FltList[(u8DmxId)].FltInfo.Info.SectInfo.SectBufAddr);

    // transfer physical address to uncache address
    pu8Read=            (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Read);
    pu8Write=           (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Write);
    pu8Start=           (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Start);
    //pu8End=             (MS_U8*)MS_PA2KSEG1((MS_U32)pu8End);
    pu8End=             pu8Start + u32SecBufSize;

    /*printf("(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (int)pu8Start,
            (int)pu8End,
            (int)pu8Read,
            (int)pu8Write);*/

    bCopy = (pfCheckCB)? pfCheckCB(u8DmxId, pu8Start, pu8End, pu8Read, pu8Write): TRUE;

    //---------------------------------------------
    //    For Customer's request, only copy 1 PES at once
    //---------------------------------------------
    if(_pdmx_res->_FltList[u8DmxId].DmxFltType == DMX_FILTER_TYPE_PES)
    {
        if(_DMX_RingBuffer_ChkPESLen(pu8Read, pu8Start, pu8End, &u32SecLen) == TRUE)
        {
            if(u32RbSize >= u32SecLen)
            {
                u32RbSize = u32SecLen;
            }
        }
    }
    //--------------------------------------------- end

    if ((pu8Read+ u32RbSize)>= pu8End)
    {
        u32CopySize=        pu8End- pu8Read;
        if (bCopy)
        {
            CPY_to_USER(pu8Buf, pu8Read, u32CopySize);
        }
        pu8Read=            pu8Start;
        u32RbSize-=         u32CopySize;
        *pu32SecSize+=      u32CopySize;
        pu8Buf+=            u32CopySize;
    }
    if (u32RbSize)
    {
        if (bCopy)
        {
            CPY_to_USER(pu8Buf, pu8Read, u32RbSize);
        }
        *pu32SecSize+=      u32RbSize;
        pu8Read+=           u32RbSize;
    }
    *pu32RmnSize-= *pu32SecSize;
    MS_ASSERT(pu8Read< pu8End);

//#ifndef MSOS_TYPE_LINUX_KERNEL
    // Don't change the read address on utopia driver. It will control on DDI side due to loop copy data.
    MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, (MS_PHYADDR)MS_VA2PA((MS_U32)pu8Read));
//#endif

    return DMX_FILTER_STATUS_OK;
}

static void _DumpSecHeader(MS_U8* pu8Start, MS_U8* pu8End, MS_U8* pu8Read, MS_U8* pu8Write)
{
#if defined (MS_DEBUG)
    MS_U32 u32SecLen = 0;
    MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, printf("(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (int)pu8Start,
            (int)pu8End,
            (int)pu8Read,
            (int)pu8Write)));
    if ((pu8Read + 3) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, printf("[%d] %d: %02x %02x %02x\n", __LINE__, (int)u32SecLen, (int)pu8Read[0], (int)pu8Read[1], (int)pu8Read[2])));
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, printf("[%d] %d: %02x %02x %02x\n", __LINE__, (int)u32SecLen, (int)pu8Read[0], (int)pu8Read[1], (int)pu8Start[0])));
    }
    else // ((uReadAddr+1) == uEndAddr)
    {
        u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, printf("[%d] %d: %02x %02x %02x\n", __LINE__, (int)u32SecLen, (int)pu8Read[0], (int)pu8Start[0], (int)pu8Start[1])));
    }

#if 0 //left for debug
    for (u32i = 0 ; u32i < u32SecLen+3 ; u32i++)
    {
        if (u32i%16 == 0)
            printf("\n");
        printf("%02x ",(int)pu8Read[u32i]);
    }
    printf("\n");
#endif
#endif
}

static DMX_FILTER_STATUS _DMX_GetSect(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32SecSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    MS_U32      u32RbSize;
    MS_U32      u32SecBufSize= 0;
    MS_U8*      pu8Start;
    MS_U8*      pu8End = 0;
    MS_U8*      pu8Read;
    MS_U8*      pu8Write;
    MS_U32      u32CopySize= 0;
    MS_U32      u32SecLen= 0;
    MS_BOOL     bCopy;
    MS_PHYADDR  temp = 0;

    //DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));
    //printf("[%s] %d , %p, %ld, %p %p, %p\n", __FUNCTION__, (int)u8DmxId,pu8Buf, u32BufSize, pu32SecSize,pu32RmnSize,pfCheckCB);

#ifndef MSOS_TYPE_LINUX_KERNEL
    MsOS_Sync();
#endif

    MsOS_ReadMemory();

    // Get Section buffer read pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Read = (MS_U8*)temp;
    // Get Section buffer write pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Write = (MS_U8*)temp;

    *pu32SecSize = 0;
    *pu32RmnSize = 0;
    if (pu8Read == pu8Write)
    {
        //printf("[%s] r %p, w %p \n", __FUNCTION__, pu8Read, pu8Write);
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR; //fail
    }

    // Get Section buffer start address
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &temp))
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }
    pu8Start = (MS_U8*)temp;
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32SecBufSize))
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }
    //ASSERT(u32SecBufSize == _pdmx_res->_FltList[(u8DmxId)].FltInfo.Info.SectInfo.SectBufSize);
    // Get Section buffer end address
    pu8End =            pu8Start+ u32SecBufSize;

    if(pu8Read >= pu8End || pu8Read < pu8Start)
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        printf("[DMX][%d][%ld] FATAL ERROR: Section buffer read address out of range!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    if(pu8Write >= pu8End || pu8Write < pu8Start)
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        printf("[DMX][%d][%ld] FATAL ERROR: Section buffer write address out of range!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    *pu32RmnSize = u32RbSize = _DMX_RingBuffer_DataSize(pu8Read, pu8Write, pu8Start, pu8End);
    if (3 > u32RbSize)
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        return DMX_FILTER_STATUS_ERROR;
    }

    MS_ASSERT(pu8Start == (MS_U8*)_pdmx_res->_FltList[(u8DmxId)].FltInfo.Info.SectInfo.SectBufAddr);
	if ((pu8Read==0)||(pu8Write==0)||(pu8Start==0))
	{
	    printf("[DMX][%d][%ld] FATAL ERROR: Section buffer address illegal!!!!!!!!\n", __LINE__, MsOS_GetSystemTime());
        printf("[DMX][%d] Start %p, End %p , Read %p, Write %p \n", __LINE__, pu8Start, pu8End, pu8Read, pu8Write);
		return DMX_FILTER_STATUS_ERROR;
	}

    // transfer physical address to uncache address
    pu8Read  =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Read);
    pu8Write =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Write);
    pu8Start =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Start);
    //pu8End   =          (MS_U8*)MS_PA2KSEG1((MS_U32)pu8End);
    pu8End   =          pu8Start + u32SecBufSize;
     /*printf("(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (int)pu8Start,
            (int)pu8End,
            (int)pu8Read,
            (int)pu8Write);*/

    bCopy = (pfCheckCB) ? pfCheckCB(u8DmxId, pu8Start, pu8End, pu8Read, pu8Write) : TRUE;

    // Check not enought section data
    if ((pu8Read + 3) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
    }
    else // ((uReadAddr+1) == uEndAddr)
    {
        u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
    }
    u32SecLen +=        3;

    if (u32SecLen > u32RbSize)
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Section length is larger than what section buffer has %d %d  DMXID : %d\n", __LINE__, (int)u32SecLen, (int)u32RbSize, u8DmxId ));
        _DumpSecHeader(pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    if (u32SecLen> 4096)
    {
        //printf("_DMX_GetSect err %d \n", __LINE__);
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Section length is larger than 4096 bytes DMXID = %d \n", __LINE__,u8DmxId));
        // Vincent.Lin request it
        return DMX_FILTER_STATUS_ERROR;
    }

    if(u32BufSize == 0)
    {
        *pu32SecSize = u32SecLen;
        return DMX_FILTER_STATUS_OK;
    }

    if ((u32SecLen > u32BufSize) || (!pu8Buf))
    {
        DMX_ASSERT(u32SecLen <= u32BufSize, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] fitler %d. Section data size > given buffer size (%d, %d)\n", __LINE__, (int)u8DmxId, (int)u32SecLen, (int)u32BufSize)));
        _DumpSecHeader(pu8Start, pu8End, pu8Read, pu8Write);
        bCopy = FALSE;
    }

    if ((pu8Read+ u32SecLen) >= pu8End)
    {
        u32CopySize =       pu8End - pu8Read;

        if (bCopy)
        {
            CPY_to_USER(pu8Buf, pu8Read, u32CopySize);
        }
        pu8Read=            pu8Start;
        u32SecLen-=         u32CopySize;
        *pu32SecSize+=      u32CopySize;
        pu8Buf+=            u32CopySize;
    }
    if (u32SecLen)
    {
        if (bCopy)
        {
            CPY_to_USER(pu8Buf, pu8Read, u32SecLen);
        }
        *pu32SecSize+=      u32SecLen;
        pu8Read+=           u32SecLen;
    }

    *pu32RmnSize -= *pu32SecSize;
    if (FALSE == bCopy)
    {
        *pu32SecSize = 0;
    }

    MS_ASSERT(pu8Read< pu8End);

//#ifndef MSOS_TYPE_LINUX_KERNEL
    // Don't change the read address on utopia driver. It will control on DDI side due to loop copy data.
    if (DRVTSP_OK != MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, (MS_PHYADDR)MS_VA2PA((MS_U32)pu8Read)))
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] MDrv_TSP_SecFlt_SetReadAddr fail %d %d\n", __FUNCTION__, __LINE__, u8DmxId, _pdmx_res->_FltList[u8DmxId].SecBufId));
    }
//#endif

    if(*pu32RmnSize >= 3)
    {
        extern void MDrv_TSP_SecFlt_Update(MS_U32 u32EngId, MS_U32 u32SecFltId);
        // Check not enought section data
        if ((pu8Read + 3) <= pu8End)
        {
            u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
        }
        else if ((pu8Read + 2) <= pu8End)
        {
            u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
        }
        else // ((uReadAddr+1) == uEndAddr)
        {
            u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
        }
        u32SecLen +=        3;
        if (u32SecLen <= *pu32RmnSize)
        {
            MDrv_TSP_SecFlt_Update(0,_pdmx_res->_FltList[u8DmxId].SecBufId);
        }
    }

    return DMX_FILTER_STATUS_OK;
}

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    #ifdef FQ_ENABLE
static MS_BOOL _DMX_PVRRecordStampFromFQ(DMX_PVR_ENG Eng, MS_U32 u32FQEng)
{
    MS_BOOL bFQIsInit = FALSE;
    MS_BOOL bEnabled = FALSE;
    MDrv_FQ_IsInit(u32FQEng, &bFQIsInit);
    //we assume PVR MW/AP only uses 2 src of the Eng:
    //one is from its TSIF not after FQ, the other one is after FQ
    if(MDrv_RASP_PVR_GetFileinEnable(Eng, &bEnabled) != DRVRASP_OK)
        return FALSE;

    if(bFQIsInit && bEnabled)
        return TRUE;
    else
        return FALSE;
}
    #endif
#endif
//------------------------------------------------------------------------------
// API implementation
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @param pFwAddr \b IN: TSP firmware address in DRAM physical address
/// @param u32FwSize \b IN: TSP firmware size
/// It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFW(MS_PHYADDR pFwAddr, MS_U32 u32FwSize)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    DMX_ASSERT(!(pFwAddr & 0xFF), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] FW address must be 128 byte alignment\n", __LINE__)));
    DMX_ASSERT((u32FwSize > 0), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] FW size is 0\n", __LINE__)));

    _u32TspFwAddr = (MS_U32)pFwAddr;
    _u32TspFwSize = u32FwSize;

#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Init(void)
{
    MS_U32 temp= 0;
    DMX_FILTER_STATUS Res = DMX_FILTER_STATUS_OK;

    DMX_ASSERT((_pdmx_res != 0), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] data pointer is 0!", __FUNCTION__, __LINE__)));

    #ifdef MSOS_TYPE_LINUX_KERNEL
    if(_pdmx_res->_bFWInit == TRUE)
    {
        return DMX_FILTER_STATUS_OK;
    }
    #endif //end of MSOS_TYPE_LINUX_KERNEL

    if (-1 != _pdmx_res->_Dmx_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Initialization more than once\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_Dmx_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MAPI init", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] create mutex fail\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (-1 != _pdmx_res->_Dmx_Pvr_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Create PVR mutex more than once\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_Dmx_Pvr_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"MAPI PVR mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_Pvr_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] create PVR mutex fail\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (-1 != _pdmx_res->_Dmx_MMFI_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Create MMFI mutex more than once\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_Dmx_MMFI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"MAPI MMFI mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_MMFI_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Create MMFI mutex fail\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

#ifdef TSO_ENABLE
    if (-1 != _pdmx_res->_Dmx_TSO_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Create TSO mutex more than once\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_Dmx_TSO_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MAPI TSO mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_TSO_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Create TSO mutex fail\n", __LINE__)));
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    _DMX_ENTRY();
    _DMX_PVR_ENTRY();
    _DMX_MMFI_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    MDrv_RASP_Init();
    if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_FILTER_NUM, &temp))
    {
        MS_ASSERT(0);
    }

    #if (TSP_VERSION == TSP_VER_4_0)
    temp = temp + 1; // 1 record PID filter for backward compatibility
    #endif //end of  drvtsp4

    _pdmx_res->_u32TspFltNum = temp ;

#else //end of drvtsp2 & drvtsp4

    if (DRVTSP_OK != MDrv_TSP_Init(_u32TspFwAddr, _u32TspFwSize))
    {
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] tsp init fail\n", __LINE__)));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_PID_FILTER_NUM, &temp))
    {
        MS_ASSERT(0);
        Res =  DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_u32TspFltNum = temp;

#endif

#ifdef MMFILEIN
    MDrv_MMFI_Init();
#endif

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Init();
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
#endif

    _FLT_LIST_RESET();

    MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);

    #if DMX_DEBUG
    _DMX_Debug(NULL);
    #endif
    _DMX_RETURN(Res);
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for Virtual Quere Enable
/// @param param                \b IN: Init Virtual Quere Setting
/// @return                     DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use virtual quere, please call this API after calling MApi_DMX_Init. (VQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSPInit(DMX_TSPParam *param)
{
    if (_pdmx_res->_bFWInit == TRUE)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    int                 i;
    TSP_InitParam       tsp_param;

    _DMX_ENTRY();

    MDrv_TSP_SetDbgLevel(E_TSP_DBG_INFO);

    // check user param if it's init
    for (i = 0; i < sizeof(param->_zero_reserved); i++)
    {
        if ( (*(((MS_U8*)(&(param->_zero_reserved))) + i)) != 0 )
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    tsp_param.phyFWAddr = param->phyFWAddr;
    tsp_param.u32FWSize = param->u32FWSize;
    tsp_param.phyVQAddr = param->phyVQAddr;
    tsp_param.u32VQSize = param->u32VQSize;

    if (DRVTSP_OK != MDrv_TSP_Init(&tsp_param))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _pdmx_res->_bFWInit = TRUE;

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    #if (defined(VQ_ENABLE))
    _u32VQAddr = param->phyVQAddr;
    _u32VQSize = param->u32VQSize;
    _DMX_ENTRY();
    MDrv_TSP_SetVQueBuf(param->phyVQAddr, param->u32VQSize);
    MDrv_TSP_VQueue_OverflowInt_En(FALSE);
    MDrv_TSP_VQueEnable(TRUE);

    #if (defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    _pdmx_res->_bVQEnabled = TRUE;
    #endif
    #endif //VQ_ENABLE

    _pdmx_res->_bFWInit = TRUE;

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif

}

//-------------------------------------------------------------------------------------------------
/// Exit Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Exit(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    #ifdef MSOS_TYPE_LINUX_KERNEL
    if(_pdmx_res->_bFWInit == FALSE)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #endif

    if (-1!= _pdmx_res->_Dmx_Mutex)
    {
        _DMX_ENTRY();
    }
    else
    {
       ;// do nothing
    }

    if (-1!= _pdmx_res->_Dmx_Pvr_Mutex)
    {
        _DMX_PVR_ENTRY();
    }
    else
    {
       ;// do nothing
    }

    if (-1!= _pdmx_res->_Dmx_MMFI_Mutex)
    {
        _DMX_MMFI_ENTRY();
    }
    else
    {
       ;// do nothing
    }

#ifdef TSO_ENABLE
    if (-1!= _pdmx_res->_Dmx_TSO_Mutex)
    {
        _DMX_TSO_ENTRY();
    }
    else
    {
       ;// do nothing
    }
#endif

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    MDrv_RASP_Exit();
#endif

#ifdef MMFILEIN
    MDrv_MMFI_Exit();
#endif

#ifdef TSO_ENABLE
    MDrv_TSO_Exit();
    if (_pdmx_res->_Dmx_TSO_Mutex != -1 )
    {
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
        MsOS_DeleteMutex(_pdmx_res->_Dmx_TSO_Mutex);
        _pdmx_res->_Dmx_TSO_Mutex= -1;
    }
#endif

    if (MDrv_TSP_Exit() == DRVTSP_OK)
    {
        if (_pdmx_res->_Dmx_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_Mutex);
            _pdmx_res->_Dmx_Mutex= -1;
        }
        if (_pdmx_res->_Dmx_Pvr_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_Pvr_Mutex);
            _pdmx_res->_Dmx_Pvr_Mutex= -1;
        }
        if (_pdmx_res->_Dmx_MMFI_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_MMFI_Mutex);
            _pdmx_res->_Dmx_MMFI_Mutex= -1;
        }
        _pdmx_res->_bFWInit = FALSE;

#ifdef DMX_UTOPIA_20
        _pdmx_res           = NULL;
#endif
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        // do nothing
    }

    if (-1!= _pdmx_res->_Dmx_Mutex)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR; ; //donothing
    }
}

//-------------------------------------------------------------------------------------------------
/// Reset Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_Reset() == DRVTSP_OK)
    {
        #ifdef MMFILEIN
        MDrv_MMFI_ResetAll();
        #endif
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    return (DMX_FILTER_STATUS_ERROR);
}

//-------------------------------------------------------------------------------------------------
/// Force exit Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @special case for MM
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ForceExit(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_PowerOff() == DRVTSP_OK)
    {
#ifdef MMFILEIN
        MDrv_MMFI_Exit();
        MDrv_MMFI_ResetAll();
#endif
        return (DMX_FILTER_STATUS_OK);
    }

    return DMX_FILTER_STATUS_ERROR;
}

//--------------------------------------------------------------------------------------------------
/// Resume TSP driver
/// @param u32FWAddr \b IN: TSP FW address
/// @param u32FWSize \b IN: TSP FW size
/// @return TSP_Result
/// @note
/// Restore TSP driver states from DRAM
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Resume(MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(MDrv_TSP_Resume(u32FWAddr, u32FWSize) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#ifdef MMFILEIN
    MDrv_MMFI_Init();
#endif

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Init();
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
#endif
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    #if (defined(VQ_ENABLE))
    MDrv_TSP_SetVQueBuf(_u32VQAddr, _u32VQSize);
    MDrv_TSP_VQueue_OverflowInt_En(FALSE);
    MDrv_TSP_VQueEnable(TRUE);
    #endif
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
/// Suspend TSP driver
/// @return TSP_Result
/// @note
/// Save TSP driver states to DRAM
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Suspend(void)
{
    TSP_Result ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#ifdef MMFILEIN
    MDrv_MMFI_Exit();
#endif
#ifdef TSO_ENABLE
    MDrv_TSO_Exit();
#endif

    ret= MDrv_TSP_Suspend();
    if (ret == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
/// Release TSP driver HW lock semaphone
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ReleaseSemaphone(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
#else
    MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_INFO, printf("%s [OBSOLETED]\n", __FUNCTION__)));
    // @FIXME: check root cause of why coprocess has unrelease lock condition.
    if (MDrv_TSP_HW_Lock_Release() == DRVTSP_OK)
    {
        return (DMX_FILTER_STATUS_OK);
    }
#endif

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR for recording flow
///                       DMX_FLOW_PVRCA for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1 for second recording flow.
///                       DMX_FLOW_PVR2 for third recording flow.
///                       DMX_FLOW_FILEIN_MM for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D for file-in playback with 2nd video input source .
///
/// @param pDmxFlowInput \b OUT: pointer to store DMX input flow value.
//                              DMX_FLOW_INPUT_DEMOD for input from DVBC
///                            DMX_FLOW_INPUT_EXT_INPUT0 for input from TS0 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT1 for input from TS1 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT2 for input from TS2 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT3 for input from TS3 Interface
///                            DMX_FLOW_INPUT_MEM for input from memory
///                            Below Items are not support for all chips:
///                            DMX_FLOW_INPUT_EXT_INPUT1for input from TS2 Interface
/// @param pbClkInv    \b OUT: pointer to store clock phase inversion
/// @param pbExtSync \b OUT: pointer to store sync by external signal
/// @param pbParallel  \b OUT: pointer to store parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_If_Set Ifset;
    DrvTSP_If eif = E_DRVTSP_IF_TS0;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
            eif = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PLAYBACK1:
        case DMX_FLOW_PVR:
        case DMX_FLOW_CIPHSS_PLAYBACK:
        case DMX_FLOW_PVRCA1:
            eif = E_DRVTSP_IF_TS1;
            break;
        case DMX_FLOW_PLAYBACK2:
        case DMX_FLOW_PVR1:
            eif = E_DRVTSP_IF_TS2;
            break;
        #ifdef MERGE_STR_SUPPORT
        case DMX_FLOW_PLAYBACK_FI:
            eif = E_DRVTSP_IF_FI;
            break;
        #endif
        case DMX_FLOW_PVR2:
            eif = E_DRVTSP_IF_TS3;
            break;
        case DMX_FLOW_MMFI0:
        case DMX_FLOW_MMFI1:
            DMX_DBGMSG(DMX_DBG_INFO,printf("Flow Input: From Memory \n"));
            break;
        default:
            DMX_DBGMSG(DMX_DBG_INFO,printf("Flow Input: Not support \n"));
            return DMX_FILTER_STATUS_ERROR;
    }

    Ifset.ePad = E_DRVTSP_PAD_DEMOD;
    Ifset.bClkInv = FALSE;
    Ifset.bExtSync = FALSE;
    Ifset.bParallel = FALSE;

    if(MDrv_TSP_GetTSIFStatus(eif, &Ifset) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(Ifset.ePad == E_DRVTSP_PAD_DEMOD)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Playback, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS0, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS1, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS2, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS3, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT4)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT4;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS4, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT5)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT5;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS5, "));
    }
    else
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: From Memory , "));
    }

    *pbClkInv = Ifset.bClkInv;
    *pbExtSync = Ifset.bExtSync;
    *pbParallel = Ifset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("Parallel mode \n"));
    }
    return (DMX_FILTER_STATUS_OK);
}
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_TsIf_Set TsIfset;
    TSP_TSIF    eTSIF   = E_TSP_TSIF_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;
    //if live-in and file-in of a DmxFlow map to diff TSIF, return the status of the live-in one
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            eTSIF = E_TSP_TSIF_0;
            break;
        case DMX_FLOW_PLAYBACK1:
            eTSIF = E_TSP_TSIF_1;
            break;
        case DMX_FLOW_PLAYBACK2:
            eTSIF = E_TSP_TSIF_2;
            break;
        case DMX_FLOW_PLAYBACK3:
            eTSIF = E_TSP_TSIF_3;
            break;
        case DMX_FLOW_PVR:
            eTSIF = E_TSP_TSIF_PVR0;
            break;
        case DMX_FLOW_PVR1:
            eTSIF = E_TSP_TSIF_PVR1;
            break;
        case DMX_FLOW_PVR2:
            eTSIF = E_TSP_TSIF_PVR2;
            break;
        case DMX_FLOW_PVR3:
            eTSIF = E_TSP_TSIF_PVR3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_INFO,printf("Flow Input: From Memory \n"));
            return DMX_FILTER_STATUS_ERROR;
    }

    TsIfset.ePad = E_TSP_TS_INVALID;
    TsIfset.bClkInv = FALSE;
    TsIfset.bExtSync = FALSE;
    TsIfset.bParallel = FALSE;

    // get PAD and ClkInv for eTSIF
    if(MDrv_TSP_GetTSIFStatus(eTSIF, &(TsIfset.ePad), // @F_TODO do we have to modify this type convert into original type?
        &(TsIfset.bClkInv), &(TsIfset.bExtSync), &(TsIfset.bParallel)) != E_TSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(TsIfset.ePad == E_TSP_TS_DEMOD0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: DEMOD, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_DEMOD1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD1;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: DEMOD1, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input0, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input1, "));
    }
#ifdef TS2_IF_SUPPORT
    else if(TsIfset.ePad == E_TSP_TS_PAD2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input2, "));
    }
#endif
#ifdef TS3_IF_SUPPORT
    else if(TsIfset.ePad == E_TSP_TS_PAD3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input3, "));
    }
#endif
    else if(TsIfset.ePad == E_TSP_TS_PAD_TSO0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_TSO;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input2, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD_TSO1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_TSO1;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input3, "));
    }

    *pbClkInv = TsIfset.bClkInv;
    *pbExtSync = TsIfset.bExtSync;
    *pbParallel = TsIfset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("Parallel mode \n"));
    }

    return (DMX_FILTER_STATUS_OK);
}

#else //end of version 4.0
DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_If_Set Ifset;
    DrvTSP_If eif = E_DRVTSP_IF_PLAYBACK;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
        case DMX_FLOW_CIPHSS_PLAYBACK:
            eif = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
        case DMX_FLOW_PVRCA1:
            eif = E_DRVTSP_IF_PVR0;
            break;
#ifdef TS2_IF_SUPPORT
        #ifdef TS2_LIVE_SUPPORT
        case DMX_FLOW_PLAYBACK1:
        #endif
        case DMX_FLOW_PVR1:
            eif = E_DRVTSP_IF_PVR1;
            break;
#endif
#ifdef TS3_IF_SUPPORT
        case DMX_FLOW_PVR2:
            eif = E_DRVTSP_IF_PVR2;
            break;
#endif
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
        case DMX_FLOW_PVR3:
            eif = E_DRVTSP_IF_PVR3;
            break;
        case DMX_FLOW_PVR4:
            eif = E_DRVTSP_IF_PVR4;
            break;
        case DMX_FLOW_PVR5:
            eif = E_DRVTSP_IF_PVR5;
            break;
#endif
        default:
            DMX_DBGMSG(DMX_DBG_INFO,printf("Flow Input: From Memory \n"));
            return DMX_FILTER_STATUS_ERROR;

    }

    Ifset.ePad = E_DRVTSP_PAD_DEMOD;
    Ifset.bClkInv = FALSE;
    Ifset.bExtSync = FALSE;
    Ifset.bParallel = FALSE;

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)

    // get PAD and ClkInv for eif

    if(MDrv_TSP_GetTSIFStatus((MS_U8)eif, (MS_U8*)&(Ifset.ePad),
        &(Ifset.bClkInv), &(Ifset.bExtSync), &(Ifset.bParallel)) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    MS_BOOL bDataSWP=FALSE;

    switch (eif)
    {
        case E_DRVTSP_IF_PLAYBACK:
        case E_DRVTSP_IF_PVR0:
        case E_DRVTSP_IF_PVR1:
            break;
        case E_DRVTSP_IF_PVR2:
            if (MDrv_RASP_PVR_GetTSIFStatus(2, &(Ifset.bExtSync), &(Ifset.bParallel), &bDataSWP) != DRVRASP_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;
        case E_DRVTSP_IF_PVR3:
            if (MDrv_RASP_PVR_GetTSIFStatus(3, &(Ifset.bExtSync), &(Ifset.bParallel), &bDataSWP) != DRVRASP_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;
        case E_DRVTSP_IF_PVR4:
            printf("E_DRVTSP_IF_PVR4\n");
            if (MDrv_RASP_PVR_GetTSIFStatus(4, &(Ifset.bExtSync), &(Ifset.bParallel), &bDataSWP) != DRVRASP_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;
        case E_DRVTSP_IF_PVR5:
            if (MDrv_RASP_PVR_GetTSIFStatus(5, &(Ifset.bExtSync), &(Ifset.bParallel), &bDataSWP) != DRVRASP_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
            break;
    }


#else
    if(MDrv_TSP_GetTSIFStatus(eif, &Ifset) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    if(Ifset.ePad == E_DRVTSP_PAD_DEMOD)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Playback, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS0, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS1, "));
    }
#ifdef TS2_IF_SUPPORT
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS2, "));
    }
#endif
#ifdef TS3_IF_SUPPORT
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern TS3, "));
    }
#endif

    *pbClkInv = Ifset.bClkInv;
    *pbExtSync = Ifset.bExtSync;
    *pbParallel = Ifset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, printf("Parallel mode \n"));
    }

    return (DMX_FILTER_STATUS_OK);
}
#endif

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR for recording flow
///                       DMX_FLOW_PVRCA for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1 for second recording flow.
///                       DMX_FLOW_FILEIN_MM for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D for file-in playback with 2nd video input source .
///
/// @param DmxFlowInput \b IN: DMX_FLOW_INPUT_DEMOD for input from DVBC
///                            DMX_FLOW_INPUT_EXT_INPUT0 for input from TS0 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT1 for input from TS1 Interface
///                            DMX_FLOW_INPUT_MEM for input from memory
///                            Below Items are not support for all chips:
///                            DMX_FLOW_INPUT_EXT_INPUT1for input from TS2 Interface
/// @param bClkInv \b IN: TSin options: clock phase inversion
/// @param bExtSync \b IN: TSin options: sync by external signal
/// @param bParallel \b IN: TSin is parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d (%d, %d, %d, %d, %d)\n",
        __FUNCTION__, __LINE__, (int)DmxFlow, (int)DmxFlowInput, (int)bClkInv, (int)bExtSync, (int)bParallel));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FLOWSET)
        dmx_dbg_print("[DMX FlwSet]\tFlow[0x%x] Input[0x%x] Inv[%d] Sync[%d] Parallel[%d]\n",
        (MS_U32)DmxFlow,
        (MS_U32)DmxFlowInput,
        (MS_U32)bClkInv,
        (MS_U32)bExtSync,
        (MS_U32)bParallel);
#endif

    _DMX_ENTRY();

    if(_DMX_FlowSet(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel) == FALSE)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
/// Setting PVR flow
/// @param  Eng                                 \b IN: PVR Engine ID
/// @param  ePvrSrcTSIf                     \b IN: Select the record source
/// @param  bDscmbRec                     \b IN: TRUE for recording dscmbled stream; FALSE for recording orignal stream
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PVR_FlowSet(DMX_PVR_ENG Eng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec)
{
    //DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d (DMX_PVR_ENG:%d, ePvrSrcTSIf:%d, BDSCMBREC:%d)\n",
    //       __FUNCTION__, __LINE__, (int)Eng, (int)ePvrSrcTSIf, (int)bDscmbRec));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
    return DMX_FILTER_STATUS_ERROR;
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    _DMX_ENTRY();

    if(_DMX_PVR_FlowSet(Eng, ePvrSrcTSIf, bDscmbRec) == FALSE)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    MS_U32 u32data = 0;
    DrvTSP_PKTDMXSrcType ePktDmx = E_DRVTSP_PKTSRC_DEMUX0;

    DMX_FLOW       DmxFlowOrg = DMX_FLOW_PLAYBACK;
    DMX_FLOW       DmxFlowNew = DMX_FLOW_PVR2;
    DMX_FLOW_INPUT FlowInput = DMX_FLOW_INPUT_DEMOD;
    MS_BOOL        bClkInv = FALSE;
    MS_BOOL        bExtSync = FALSE;
    MS_BOOL        bParallel = FALSE;

    #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)) //version 1.0 only

    DrvTSP_DscmbCtrl DscmbPath = 0;

    //If not select input src, setting input src depend on CA Ctrl value
    if(ePvrSrcTSIf == DMX_TSIF_MAX)
    {
        MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

        //printf("[%s][%d] DscmbPath 0x%lx \n", __FUNCTION__, __LINE__, DscmbPath);

        if((DscmbPath & E_DRVTSP_CA_OUTPUT_LIVE0) == 0)
        {
            ePvrSrcTSIf = DMX_TSIF_LIVE1;
        }
        else
        {
            ePvrSrcTSIf = DMX_TSIF_LIVE0;
        }
    }

    #endif

    if(MDrv_TSP_GetCap(E_DRVTSP_CAP_PVR_ENG_NUM, (void*)&u32data) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    if(Eng >= u32data)
    {
        return DMX_FILTER_STATUS_OK;
    }

    switch(ePvrSrcTSIf)
    {
        case DMX_TSIF_LIVE0:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_TSIF_FILE0:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUXFILE;
            break;
        case DMX_TSIF_LIVE1:
        case DMX_TSIF_FILE1:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

     _pdmx_res->_u32PVREngSrc[Eng] = (MS_U32)ePktDmx;

    // For Drvtsp3 and except of PVR_CB, just select PVR source directly.
    // For Drvtsp and Drvtsp3 PVR_CB, must set PVR flowset depnend on live flowset
    #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    if(Eng == DMX_PVR_EGN2) //PVRCB only
    {
    #endif //end of DRVTSP3 define

        switch(ePvrSrcTSIf)
        {
            case DMX_TSIF_LIVE0:
                DmxFlowOrg = DMX_FLOW_PLAYBACK;
                break;
            case DMX_TSIF_LIVE1:
                DmxFlowOrg = DMX_FLOW_PLAYBACK1;
                break;
            case DMX_TSIF_LIVE2:
                DmxFlowOrg = DMX_FLOW_PLAYBACK2;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

        #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)) //version 1.0 only

        if(Eng == DMX_PVR_EGN0)
        {
            if(bDscmbRec)
            {
                // for napoli, if PVRCA path soruce is from TSIF1, not need to duplicate FLT0 Pid, and just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE1)
                {
                    DmxFlowNew = DMX_FLOW_PVR;
                }
                else
                {
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

                    //Already TS1 input, just set dscmb path
                    // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                    if(DscmbPath & E_DRVTSP_CA_INPUT_TSIF1)
                    {
                        DmxFlowNew = DMX_FLOW_PVR;
                    }
                    else
                    {
                        DmxFlowNew = DMX_FLOW_PVRCA;
                    }
                }
            }
            else
            {
                DmxFlowNew = DMX_FLOW_PVR;
            }
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            if(bDscmbRec)
            {
                // for napoli, if PVRCA path soruce is from TSIF0, just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE0)
                {
                    DmxFlowNew = DMX_FLOW_PVR1;
                }
                else
                {
                    DmxFlowNew = DMX_FLOW_PVRCA1; //napoli u03 only
                }
            }
            else
            {
                DmxFlowNew = DMX_FLOW_PVR1;
            }
        }

        #endif //endif udef vsersin > 1.0

        if(_MApi_DMX_Get_FlowInput_Status(DmxFlowOrg, &FlowInput, &bClkInv, &bExtSync, &bParallel) != DMX_FILTER_STATUS_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        if(_MApi_DMX_FlowSet(DmxFlowNew, FlowInput, bClkInv, bExtSync, bParallel) == DMX_FILTER_STATUS_ERROR)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
        #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)) //version 1.0 only
        if(bDscmbRec)
        {
            if(Eng == DMX_PVR_EGN0)
            {
                // for napoli, if PVRCA path soruce is from TSIF1, not need to duplicate FLT0 Pid, and just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE1)
                {
                    _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel); //set TSIF1 for CA input
                    DscmbPath = E_DRVTSP_CA_INPUT_TSIF1|E_DRVTSP_CA_OUTPUT_REC0;
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                }
                else
                {
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

                    //Already TS1 input, just set dscmb path
                    // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                    if(DscmbPath & E_DRVTSP_CA_INPUT_TSIF1)
                    {
                        _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel);
                        DscmbPath |= E_DRVTSP_CA_OUTPUT_REC0;
                        MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                    }
                }
            }
            else if(Eng == DMX_PVR_EGN1)
            {
                // for napoli, if PVRCA path soruce is from TSIF0, just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE0)
                {
                    _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel);  //set TSIF1 for CA input
                    DscmbPath = E_DRVTSP_CA_INPUT_TSIF1|E_DRVTSP_CA_OUTPUT_LIVE0|E_DRVTSP_CA_OUTPUT_TSIF2;
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                    //printf("[%s][%d] DscmbPath 0x%lx \n", __FUNCTION__, __LINE__, DscmbPath);
                }
            }
        }
        #endif

        return DMX_FILTER_STATUS_OK;

    #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    }

    _DMX_ENTRY();

    MDrv_TSP_PVR_Eng_SelSrc((MS_U8)Eng, ePktDmx);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

    #endif

#endif

}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter
/// @param DmxFltType \b IN: the filter information to allocate
/// @param pu8DmxId \b OUT: the available demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Open(DMX_FILTER_TYPE DmxFltType, MS_U8* pu8DmxId)
{
    DMX_FILTER_STATUS eRet= DMX_FILTER_STATUS_ERROR;
    MS_U32         u32TSPFltSource;
    u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK;
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    switch (DmxFltType)
    {
    case DMX_FILTER_TYPE_VIDEO:
    case DMX_FILTER_TYPE_AUDIO:
    case DMX_FILTER_TYPE_AUDIO2:
#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_3_0)||(TSP_VERSION == TSP_VER_4_0))))
    case DMX_FILTER_TYPE_AUDIO3:
    case DMX_FILTER_TYPE_AUDIO4:
    case DMX_FILTER_TYPE_SCMBCHK:
#endif
    case DMX_FILTER_TYPE_VIDEO3D:
    case DMX_FILTER_TYPE_REC:
        eRet= _DMX_Open_Stream(pu8DmxId, (DMX_FILTER_TYPE)(DmxFltType|u32TSPFltSource));
        break;
    case DMX_FILTER_TYPE_SECTION:
    case DMX_FILTER_TYPE_TELETEXT:
    case DMX_FILTER_TYPE_PES:
    case DMX_FILTER_TYPE_PACKET:
    case DMX_FILTER_TYPE_PCR:
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case DMX_FILTER_TYPE_SECTION_VER:
#endif
        eRet= _DMX_Open_Sect(pu8DmxId, (DMX_FILTER_TYPE)(DmxFltType|u32TSPFltSource));
        break;

    default:
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad filter type 0x%x\n", __LINE__, (int)DmxFltType)));
        break;
    }
#if DMX_DEBUG
    stDmxDbgInfo[(*pu8DmxId)].u32TimeCreate = MsOS_GetSystemTime();
    if(u32DbgLogFlag & DMX_DBG_LOG_FLT_OPEN)
    {
        if(eRet == DMX_FILTER_STATUS_OK)
            dmx_dbg_print("[DMX OPEN]\tFlt[%03d] Type[%s]\n", (int)(*pu8DmxId), stDmxType[DmxFltType].str);
        else
            dmx_dbg_print("[DMX OPEN]\tFailed\n");
    }
#endif

    _DMX_RETURN(eRet);
}

extern MS_U32 _u32TaskLine;

//-------------------------------------------------------------------------------------------------
/// Close a demux filter
/// @param u8DmxId \b IN: the demux filer Id to free
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Close(MS_U8 u8DmxId)
{
    TSP_Result ret = (TSP_Result)DRVTSP_FAIL;
    MS_BOOL bPidFltReady = FALSE;
    MS_BOOL bPidReady = FALSE;
    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    MS_BOOL bSecFltReady = FALSE;
    #endif

#if DMX_DEBUG
    MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(-1!=_pdmx_res->_Dmx_Mutex, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] Demux has been close, close Demux Id %d fail\n",  __FUNCTION__, __LINE__, u8DmxId)));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u8DmxId, bMonitor);
    if((u32DbgLogFlag & DMX_DBG_LOG_FLT_CLOSE) && bMonitor)
        dmx_dbg_print("[DMX CLOSE]\tFlt[%03d]\n", (int)u8DmxId);
#endif
    do
    {
        _DMX_ENTRY();

        if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
        {
            //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
            _DMX_RETURN(DMX_FILTER_STATUS_OK);
        }

        if (FALSE == bPidReady)
        {
            if (DRVTSP_OK!= MDrv_TSP_PidFlt_SetPid(0, u8DmxId, DRVTSP_PID_NULL))
            {
                DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Try to set pid failed,  Demux id: %d Filter Type: %ld\n", __FUNCTION__, __LINE__, (int)u8DmxId, _pdmx_res->_FltList[u8DmxId].DmxFltType));
                _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            _FLT_LIST_PID_SET(u8DmxId, DRVTSP_PID_NULL);
            bPidReady = TRUE;
        }

        if (FALSE == bPidFltReady)
        {
            if (DRVTSP_OK != MDrv_TSP_PidFlt_Free(0, u8DmxId))
            {
                #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
                extern MS_U32 MDrv_TSP_DBG_TaskLine(void);
                DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Try to close filter %d fail -> TSP task line %d\n", __FUNCTION__, __LINE__, (int)u8DmxId, (int)MDrv_TSP_DBG_TaskLine()));
                #else
                DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Try to close filter %d fail -> TSP task line %d\n", __FUNCTION__, __LINE__, (int)u8DmxId, (int)_u32TaskLine));
                #endif
                goto release_mutex;
            }
            bPidFltReady = TRUE;
        }

        #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
        if (FALSE == bSecFltReady)
        {
            if (0xFF != _pdmx_res->_FltList[u8DmxId].SecBufId)
            {
                if (DRVTSP_OK!= MDrv_TSP_SecFlt_Free(0, _pdmx_res->_FltList[u8DmxId].SecBufId))
                {
                    goto release_mutex;
                }
                bSecFltReady = TRUE;
            }
        }
        #endif

        _FLT_LIST_REMOVE(u8DmxId);

        ret = (TSP_Result)DRVTSP_OK;
release_mutex:
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex);
        if ((TSP_Result)DRVTSP_OK != ret)
        {
            return DMX_FILTER_STATUS_ERROR;
            //MsOS_DelayTask(4);
        }
    } while (DRVTSP_OK != ret);

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Activate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Start(MS_U8 u8DmxId)
{

#if DMX_DEBUG
    MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u8DmxId, bMonitor);
        if((u32DbgLogFlag & DMX_DBG_LOG_FLT_ENABLE) && bMonitor)
            dmx_dbg_print("[DMX ENABLE]\tFlt[%03d]\n", (int)u8DmxId);
#endif

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u8DmxId, TRUE);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to deactivate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stop(MS_U8 u8DmxId)
{
#if DMX_DEBUG
        MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT((DMX_MAX_FLTID > u8DmxId), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u8DmxId, bMonitor);
    if((u32DbgLogFlag & DMX_DBG_LOG_FLT_ENABLE) && bMonitor)
        dmx_dbg_print("[DMX DISABLE]\tFlt[%03d]\n", (int)u8DmxId);
#endif

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u8DmxId, FALSE);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get a demux filter status
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @param pbEnable \b OUT: the demux filer enable/disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_IsStart(MS_U8 u8DmxId, MS_BOOL* pbEnable)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    TSP_FltState    state;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if ( DRVTSP_OK != MDrv_TSP_FLT_GetState(_pdmx_res->_FltList[u8DmxId].FltId, &state) )
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    *pbEnable = (state & E_TSP_FLT_STATE_ENABLE) ? TRUE : FALSE;
#else
    DrvTSP_FltState ePidState = E_DRVTSP_FLT_STATE_FREE;
    DrvTSP_FltState eSecState = E_DRVTSP_FLT_STATE_FREE;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if ((DRVTSP_OK != MDrv_TSP_PidFlt_GetState(0, u8DmxId, &ePidState)))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(_pdmx_res->_FltList[u8DmxId].SecBufId < 0xFF)
    {
        if ((DRVTSP_OK != MDrv_TSP_SecFlt_GetState(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &eSecState)))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    *pbEnable = ((ePidState|eSecState) & E_DRVTSP_FLT_STATE_ENABLE)? TRUE: FALSE;
#endif
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status
/// @param  FltSrc                         \b IN     : Filter source
/// @param  u32FltGroupId             \b IN     : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63, 2: 64~95, 3: 96~127
/// @param  PidFltId                       \b IN     : Pid filter id for getting. If 0xFFFFFFFF, means getting the status of all filters of the same group
/// @param  pu32ScmbSts              \b OUT  : The scrmabling status of pid filter. If getting all group status, every bit means one status of every filter
///                                                               If only checking one filter,  1 means scrambled status and 0 means non-scrambled status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_FltScmbSts(DMX_FILTER_TYPE FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MS_U32            u32TSPFltSource;
    MS_U32            u32GroupId;

    u32TSPFltSource = FltSrc & ((MS_U32)DMX_FILTER_FLT_MASK);

    if( PidFltId == 0xFFFFFFFF)
    {
        u32GroupId = u32FltGroupId;
    }
    else
    {
        u32GroupId = PidFltId >> 5;
    }

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    u32TSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
#endif

    if(MDrv_TSP_PidFlt_GetScmbSts( (DrvTSP_FltType)u32TSPFltSource, u32GroupId, PidFltId, pu32ScmbSts) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status
/// @param u8DmxId \b IN: the demux filer Id
/// @param pu8scmb \b OUT: the PES scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_PesScmbSts(MS_U8 u8DmxId, MS_U8* pu8scmb)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    if (DRVTSP_OK == MDrv_TSP_GetPesScmbSts(u8DmxId, pu8scmb))
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the TS scrambling control status
/// @param u8DmxId \b IN: the demux filer Id
/// @param pu8ScmSts \b OUT: TS scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_TsScmbSts(MS_U8 u8DmxId, MS_U8* pu8ScmSts)
{
#if (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0)||(TSP_VERSION == TSP_VER_4_0)))
            return DMX_FILTER_STATUS_ERROR;
#else

    if (DRVTSP_OK != MDrv_TSP_GetTsScmbSts(u8DmxId, pu8ScmSts))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get demux alive
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ChkAlive(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_Alive(0) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get a demux filter information
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @param pDmxFltInfo \b OUT: the demux filer info
/// @param pDmxFltType \b OUT: the demux filer type
/// @param bSet \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Info(MS_U8 u8DmxId, DMX_Flt_info* pDmxFltInfo, DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet)
{
    MS_BOOL bRet = FALSE;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    _DMX_ENTRY();

    if (!bSet)
    {
        _FLT_LIST_INFO_GET(u8DmxId, pDmxFltInfo);
        _FLT_LIST_TYPE_GET(u8DmxId, pDmxFltType);
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    //printf("[%s] DmxID [%d]\n",__FUNCTION__, u8DmxId);
    //printf("[%s] FltType [%d]\n",__FUNCTION__, _pdmx_res->_FltList[u8DmxId].DmxFltType);
    //printf("[%s] callback [%d]\n",__FUNCTION__, (MS_U32)(pDmxFltInfo->Info.SectInfo.pNotify));

    switch(_pdmx_res->_FltList[u8DmxId].DmxFltType)
    {
    case DMX_FILTER_TYPE_VIDEO:
    case DMX_FILTER_TYPE_AUDIO:
    case DMX_FILTER_TYPE_AUDIO2:
#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_3_0)||(TSP_VERSION == TSP_VER_4_0))))
    case DMX_FILTER_TYPE_AUDIO3:
    case DMX_FILTER_TYPE_AUDIO4:
#endif
    case DMX_FILTER_TYPE_VIDEO3D:
        bRet= _DMX_InfoSet_Stream(u8DmxId, pDmxFltInfo);
        break;
    case DMX_FILTER_TYPE_SECTION:
    case DMX_FILTER_TYPE_TELETEXT:
    case DMX_FILTER_TYPE_PES:
    case DMX_FILTER_TYPE_PACKET:
    case DMX_FILTER_TYPE_PCR:
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case DMX_FILTER_TYPE_SECTION_VER:
#endif

#if DMX_DEBUG
        if(u32DbgLogFlag & DMX_DBG_LOG_FLT_INFO)
        {
            DMX_Sect_info* pInfo = &(pDmxFltInfo->Info.SectInfo);

            dmx_dbg_print("[DMX INFO]\tFlt[%03d] addr[0x%x] size[0x%x] secMode[0x%x] event[0x%x]\n",
                            (int)u8DmxId,
                            (unsigned int)pInfo->SectBufAddr,
                            (unsigned int)pInfo->SectBufSize,
                            (unsigned int)pInfo->SectMode,
                            (unsigned int)pInfo->Event);
        }
#endif

        bRet= _DMX_InfoSet_Sect(u8DmxId, pDmxFltInfo);
        break;
    default:
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad filter type %d\n", __LINE__, (int)_pdmx_res->_FltList[u8DmxId].DmxFltType)));
        break;
    }

    if (bRet)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
/// Set a demux filter PID
/// @param u8DmxId \b IN: the demux filer Id to set
/// @param pu16Pid \b IN/OUT: PID to set
/// @param bSet \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pid(MS_U8 u8DmxId, MS_U16* pu16Pid, MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if (bSet)
    {
#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FLT_PID)
        dmx_dbg_print("[DMX PID]\tFlt[%03d] PID[0x%X]\n", (int)u8DmxId, (unsigned int)(*pu16Pid));
#endif
        //printf ("[%s] fltid: [%d], pid : [0x%x] time %ld\n",__FUNCTION__,(int)u8DmxId, *pu16Pid, MsOS_GetSystemTime());
        MDrv_TSP_PidFlt_SetPid(0, u8DmxId, *pu16Pid);
        _FLT_LIST_PID_SET(u8DmxId, *pu16Pid);
    }
    else
    {
        _FLT_LIST_PID_GET(u8DmxId, *pu16Pid);
    }
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set demux filer match patterns
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pPattern \b IN: the match patterns
/// @param pMask \b IN: the bit mask for match patterns
/// @param pu8NotMask \b IN: the bit mask for negative match patterns
/// @param MatchSize \b IN: the size in bytes for match patterns.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// The match pattern size is 16 bytes
///     match mask -- must set 1 to be compare (customer request)
///     not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectPatternSet(MS_U8 u8DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize)
{
    MS_U8 Pattern[DRVTSP_FILTER_DEPTH], Mask[DRVTSP_FILTER_DEPTH], NMatchMask[DRVTSP_FILTER_DEPTH];

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));
    DMX_ASSERT(0xFF > _pdmx_res->_FltList[u8DmxId].SecBufId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Demux id has not been opened yet %d\n", __LINE__, u8DmxId)));
    DMX_ASSERT(DRVTSP_FILTER_DEPTH>= MatchSize,
               DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Section filter match size\n", __LINE__)));

    _DMX_ENTRY();

    memset(Pattern, 0, DRVTSP_FILTER_DEPTH);
    memset(Mask, 0, DRVTSP_FILTER_DEPTH);
    memset(NMatchMask, 0, DRVTSP_FILTER_DEPTH);
    memcpy(Pattern, pPattern, MatchSize);
    memcpy(Mask, pMask, MatchSize);
    memcpy(NMatchMask, pu8NotMask, MatchSize);
    MDrv_TSP_SecFlt_SetPattern(0, _pdmx_res->_FltList[u8DmxId].SecBufId, Pattern, Mask, NMatchMask);
    _DMX_RETURN (DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReset(MS_U8 u8DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_ENTRY();
    DMX_ASSERT2(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));
    DMX_ASSERT2(0xFF > _pdmx_res->_FltList[u8DmxId].SecBufId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] No section buffer, Demux Id %d\n", __LINE__, u8DmxId)));
    MDrv_TSP_SecFlt_ResetBuffer(0, _pdmx_res->_FltList[u8DmxId].SecBufId);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param Read \b IN: the read pointer to be set
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReadSet(MS_U8 u8DmxId, MS_PHYADDR Read)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    DMX_ASSERT2(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, Read);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pRead \b OUT: the pointer to store the obtained read pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReadGet(MS_U8 u8DmxId, MS_PHYADDR* pRead)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, pRead))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get the write pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pWrite \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectWriteGet(MS_U8 u8DmxId, MS_PHYADDR* pWrite)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, pWrite))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get the start pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pStart \b OUT: the pointer to store the obtained start pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectStartGet(MS_U8 u8DmxId, MS_PHYADDR* pStart)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    return (DRVTSP_OK == MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u8DmxId].SecBufId, pStart))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the end pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pEnd \b OUT: the pointer to store the obtained end pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectEndGet(MS_U8 u8DmxId, MS_PHYADDR* pEnd)
{
    MS_U32 u32Size = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u8DmxId].SecBufId, pEnd))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32Size))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    *pEnd += u32Size;
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// For setting MIU address for TSP aeon
/// @param u32DataAddr       \b IN: MIU phsyical address
/// @param u32size               \b IN: MIU buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFwDataAddr(MS_U32 u32DataAddr, MS_U32 u32size)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    _DMX_ENTRY();
    MDrv_TSP_SetFwDataAddr(u32DataAddr, u32size);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

// special case for TTX
DMX_FILTER_STATUS _MApi_DMX_TTX_WriteGet(MS_U8 u8DmxId, MS_PHYADDR* pWrite)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));
    if(DRVTSP_OK != MDrv_TSP_TTX_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, pWrite))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_SetPlaybackStamp(MS_U32 u32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u32Stamp));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp]\t stamp[0x%08X]\n", (unsigned int)u32Stamp);
#endif

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStampSetPlaybackStamp(u32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_GetPlaybackStamp(MS_U32* pu32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (0x%x)\n", __FUNCTION__, __LINE__, (int)pu32Stamp));

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStampGetPlaybackStamp(pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Enable recording time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampEnable(void)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStamp(TRUE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Disable recording time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampDisable(void)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStamp(FALSE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//--------------------------------------------------------------------------------------
//New interface for multiple PVR engine
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Open(DMX_PVR_ENG Eng ,DMX_Pvr_info* pPvrInfo)
{
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->pPvrBuf0 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] PVR buffer must be 16 bytes alignment 0x%08lx\n", __LINE__, (MS_U32)pPvrInfo->pPvrBuf0)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->pPvrBuf1 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] PVR buffer must be 16 bytes alignment 0x%08lx\n", __LINE__, (MS_U32)pPvrInfo->pPvrBuf1)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->PvrBufSize0 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] PVR buffer size must be 16 bytes alignment 0x%08lx\n", __LINE__, pPvrInfo->PvrBufSize0)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->PvrBufSize1 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] PVR buffer size must be 16 bytes alignment 0x%08lx\n", __LINE__, pPvrInfo->PvrBufSize1)));

    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    if(pPvrInfo->Event & DMX_EVENT_PVRBUF_SIZE_MET)
    {//DMX_EVENT_PVRBUF_SIZE_MEET doesn't support ping-pong buffer
        if(pPvrInfo->pPvrBuf1 || pPvrInfo->PvrBufSize1)
        {
            _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        else
        {
            MDrv_RASP_PVR_SetBuffer(Eng, pPvrInfo->pPvrBuf0, pPvrInfo->pPvrBuf0, (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize0);
            if (pPvrInfo->pNotify)
            {
                _pdmx_res->_PvrNotify[Eng] = pPvrInfo->pNotify;
                MDrv_RASP_PVR_Notify(Eng, (DrvRASP_Event)pPvrInfo->Event, _DMX_NotifyPvrBySize);
            }
        }
    }
    else
        MDrv_RASP_PVR_SetBuffer(Eng, pPvrInfo->pPvrBuf0, pPvrInfo->pPvrBuf1, (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, pPvrInfo->pPvrBuf0, pPvrInfo->pPvrBuf1, (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (pPvrInfo->pNotify)
    {
        _pdmx_res->_PvrNotify[Eng] = pPvrInfo->pNotify;
        MDrv_TSP_PVR_Eng_Notify(Eng, (DrvTSP_Event)pPvrInfo->Event, _DMX_NotifyPvr);
    }

#else

#if (defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    #ifndef FIVQ_ENABLE
    if((Eng == DMX_PVR_EGN1) && _pdmx_res->_bVQEnabled)
    {
        MDrv_TSP_VQueEnable(FALSE);
    }
    #endif
#endif //VQ_ENABLE && TS2_IF_SUPPORT

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, (pPvrInfo->pPvrBuf0), (pPvrInfo->pPvrBuf1),
        (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _pdmx_res->_PvrNotify[Eng] = pPvrInfo->pNotify;
    if (pPvrInfo->pNotify)
    {
        MDrv_TSP_PVR_Eng_Notify(Eng, (DrvTSP_Event)pPvrInfo->Event, _DMX_NotifyPvr);
    }
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Close(DMX_PVR_ENG Eng)
{
    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    MS_U32 u32CallbackSize = 0;
    MDrv_RASP_PVR_CallbackSize(Eng, &u32CallbackSize, TRUE);
    MDrv_RASP_PVR_SetBuffer(Eng, 0, 0, 0, 0);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, 0,0,0,0))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else

    MDrv_TSP_PVRCA_Close(Eng);

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, 0,0,0,0))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#if (defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    #ifndef FIVQ_ENABLE
    if((Eng == 1) && _pdmx_res->_bVQEnabled)
    {
        MApi_DMX_VQ_Enable(TRUE);
    }
    #endif
#endif //VQ_ENABLE && TS2_IF_SUPPORT

#endif

    _pdmx_res->_PvrNotify[Eng] = NULL;

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Stop/Resume recording
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pause(DMX_PVR_ENG Eng ,MS_BOOL bPause)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_PVR_ENTRY();
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    MDrv_RASP_PVR_Pause(Eng,bPause);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    MDrv_TSP_PVR_Eng_Pause(Eng, bPause);
#else
    printf("[%s][%d] Not implement Yet \n",__FUNCTION__,__LINE__);

#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)// @FIXME can we mix in the same function?
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U8* pu8DmxId)
{
    MS_U32 FltId = 0xFF;
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_PVR_ENTRY();

    //@F_TODO mapping dmx open flow sw status with here inorder not to miss....

    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltAlloc(Eng, &FltId)) // Kaiser use tsp pidflt as PVR flt
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
     _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFF, DMX_FILTER_TYPE_REC);

    //enable
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltEnable(Eng, FltId, TRUE))
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    //set pid
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltSetPID(Eng, FltId, Pid)) //@FIXME check return value
    {
        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _FLT_LIST_PID_SET(FltId, Pid);
    *pu8DmxId =  FltId;

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

#else // end tsp 4.0
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U8* pu8DmxId)
{
    MS_U32 FltId = 0;

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (%d, 0x%lx, %p\n", __FUNCTION__, __LINE__, (int)Eng, Pid, pu8DmxId));
    _DMX_PVR_ENTRY();

    if (DRVRASP_OK != MDrv_RASP_PVR_AllocFlt(Eng, &FltId))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    DMX_PVR_ASSERT2(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));
    MDrv_RASP_PVR_SetPid(Eng, FltId, Pid);
    *pu8DmxId =  FltId;
#else

    DrvTSP_FltType ftype = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (%d, 0x%lx, %p\n", __FUNCTION__, __LINE__, (int)Eng, Pid, pu8DmxId));
    _DMX_PVR_ENTRY();

    #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)

    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_PVR_EGN1 :
            ftype = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    switch((DrvTSP_PKTDMXSrcType)(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng]))
    {
        case E_DRVTSP_PKTSRC_DEMUX0:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_PKTSRC_DEMUXFILE:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case E_DRVTSP_PKTSRC_DEMUX1:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case E_DRVTSP_PKTSRC_DEMUX2:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        default:
            break;
    }

    if(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng] == 0)
    {
        if(Eng == DMX_PVR_EGN0)
        {
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
        }
    }

    #else //TSP_VER_1_0
    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype |= E_DRVTSP_FLT_TYPE_PVR;
            break;
        #ifdef TS2_IF_SUPPORT
        case DMX_PVR_EGN1 :
            ftype |= E_DRVTSP_FLT_TYPE_CB;
            break;
        #endif
        default:
            _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #endif //TSP_VER_1_0 & //TSP_VER_3_0

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &FltId))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    DMX_PVR_ASSERT2(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));
    MDrv_TSP_PidFlt_SetPid(0, FltId, Pid);
    _FLT_LIST_PID_SET((MS_U8)FltId, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, FltId, TRUE);
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFF, (DMX_FILTER_TYPE)0xFF);
    *pu8DmxId =  (MS_U8)FltId;
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}
#endif

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng ,MS_U8 u8DmxId)
{
    DMX_PVRDBGMSG(DMX_DBG_FUNC, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u8DmxId));
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    MDrv_RASP_PVR_ReleaseFlt(Eng, u8DmxId);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    MDrv_TSP_PVR_Eng_FltFree(Eng, u8DmxId);
    _FLT_LIST_REMOVE(u8DmxId);
#else
    MDrv_TSP_PidFlt_Enable(0, u8DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u8DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u8DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u8DmxId);
    _FLT_LIST_REMOVE(u8DmxId);
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Check if PVR engine is started
/// @param Eng                                   \b IN: PVR engine ID
/// @param pbIsStart                           \b OUT: Pointer to store PVR engine starting status
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_IsStart(DMX_PVR_ENG Eng, MS_BOOL *pbIsStart)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else

    _DMX_PVR_ENTRY();

    if(MDrv_TSP_PVR_Eng_IsStart((MS_U32)Eng, pbIsStart) != DRVTSP_OK)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid(DMX_PVR_ENG Eng ,MS_U8 u8DmxId , MS_U32 *Pid, MS_BOOL bSet)
{

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)

    if (bSet)
    {
        MDrv_RASP_PVR_SetPid(Eng, u8DmxId, *Pid);
    }
    else
    {
       *Pid = 0 ;
        MDrv_RASP_PVR_GetPid(Eng, u8DmxId,(MS_U16*) Pid);
    }
    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)

    if (bSet)
    {
        MDrv_TSP_PVR_Eng_FltSetPID(Eng, u8DmxId, *Pid);
        _FLT_LIST_PID_SET(u8DmxId, *Pid);
    }
    else
    {
       *Pid =  DMX_PID_NULL;
        MDrv_TSP_PVR_Eng_FltGetPID(Eng, u8DmxId,(MS_U32*) Pid);  //@F_TODO not implement yet
    }
    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#else
    //[TODO] wait for use case //
    //DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d Not implement yet!!\n", __FUNCTION__, __LINE__);
    _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
#endif

}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    #ifdef FQ_ENABLE
    //need to modifiy hard code of 0 if more then 1 FQ eng is supported in the future
    if(_DMX_PVRRecordStampFromFQ(Eng, 0))
        MDrv_RASP_PVR_TimeStampSelRecordStampSrc(Eng, FALSE);
    else
        MDrv_RASP_PVR_TimeStampSelRecordStampSrc(Eng, TRUE);
    #endif
    MDrv_RASP_PVR_Start(Eng, ((bPvrAll)? E_DRVRASP_REC_MODE_ALL: E_DRVRASP_REC_MODE_PID), TRUE);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, bPvrAll, TRUE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, ((bPvrAll)? E_DRVTSP_REC_MODE_ENG0_BYPASS: E_DRVTSP_REC_MODE_ENG0_FLTTYPE), TRUE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Stop(DMX_PVR_ENG Eng)
{
    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    MDrv_RASP_PVR_Start(Eng, E_DRVRASP_REC_MODE_PID, FALSE);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, E_DRVRASP_REC_MODE_PID, FALSE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLTTYPE, FALSE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_ENG Eng ,MS_U32* pu32Write)
{
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    if(MDrv_RASP_PVR_GetWriteAddr(Eng, pu32Write) != DRVRASP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_GetWriteAddr(Eng, pu32Write))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_GetWriteAddr(Eng, pu32Write))
    {
        return (DMX_FILTER_STATUS_ERROR);
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPacketMode(DMX_PVR_ENG Eng ,MS_BOOL bSet)
{
    _DMX_PVR_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    MDrv_RASP_PVR_SetPacketMode(Eng, STR2MIU, bSet);
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_SetPacketMode(Eng, bSet))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetPacketMode(Eng, bSet))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStamp(DMX_PVR_ENG Eng ,MS_U32 u32Stamp)
{

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    #ifdef FQ_ENABLE
    //need to modifiy hard code of 0 if more then 1 FQ eng is supported in the future
    MS_U32 u32FQEng = 0;
    if(_DMX_PVRRecordStampFromFQ(Eng, u32FQEng))
        MDrv_FQ_TimeStampSetRecordStamp(u32FQEng, u32Stamp);
    else
    #endif
    {
        if(MDrv_RASP_PVR_TimeStampSetRecordStamp(Eng, u32Stamp) != DRVRASP_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(Eng, u32Stamp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(Eng, u32Stamp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
#endif

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_ENG Eng ,MS_U32* pu32Stamp)
{

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
    #ifdef FQ_ENABLE
    //need to modifiy hard code of 0 if more then 1 FQ eng is supported in the future
    MS_U32 u32FQEng = 0;
    if(_DMX_PVRRecordStampFromFQ(Eng, u32FQEng))
        MDrv_FQ_TimeStampGetRecordStamp(u32FQEng, pu32Stamp);
    else
    #endif
    {
        if(MDrv_RASP_PVR_TimeStampGetRecordStamp(Eng, pu32Stamp)!=DRVRASP_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(Eng, pu32Stamp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    if(MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(Eng, pu32Stamp) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
#endif

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPlaybackStampClk(MS_U8 u8Eng, DMX_TimeStamp_Clk eClkSrc)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else

    if(u8Eng == 0)
    {
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetPlaybackStampClk(u8Eng, (MS_U32)eClkSrc))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(u8Eng == 2)
            mmpath = DMX_MMFI_PATH1;

		if(DRVMMFI_OK != MDrv_MMFI_File_SetTimeStampClk(mmpath, (MS_U32)eClkSrc))
		{
			return DMX_FILTER_STATUS_ERROR;
		}
		#endif
    }

    return DMX_FILTER_STATUS_OK;

#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStampClk(DMX_PVR_ENG Eng, DMX_TimeStamp_Clk eClkSrc)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetRecordStampClk((MS_U32)Eng, (MS_U32)eClkSrc))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_CallbackSize(DMX_PVR_ENG Eng, MS_U32* pu32CallbackSize, MS_BOOL bSet)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0))
    if(!pu32CallbackSize)
        return DMX_FILTER_STATUS_ERROR;

    if(DRVTSP_OK != MDrv_RASP_PVR_CallbackSize(Eng, pu32CallbackSize, bSet))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Set PVR CA SPS enable mode
/// @param Eng                   \b IN: PVR Engine ID
/// @param eCaMode               \b IN: CA PVR Selection, 0: from TSP, 1: from ca program0, 2: from ca program1
/// @param bspsEnable            \b IN: SPS mode enabled flag
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetCaMode(DMX_PVR_ENG Eng, DMX_CA_PVRMODE eCaMode, MS_BOOL bspsEnable)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    return DMX_FILTER_STATUS_ERROR;
#else
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetCaMode(Eng, (MS_U16)eCaMode, bspsEnable))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif

}

//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID
/// @param Eng                                      \b IN: PVR engine ID
/// @param Pid                                       \b IN: PID to record
/// @param pu8DmxId                             \b IN: Pointer to store PVR PID filter index
/// @param u8ShareKeyType                   \b IN: PID filter share key type. 0: Not use sharekey filters.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s](%d, 0x%08lx, 0x%p) %d\n", __FUNCTION__, (int)Eng, Pid, pu8DmxId, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))  // @F_TODO what is the difference between pvrca pid open and pvrca Eng pid open
    return (MApi_DMX_Pvr_Pid_Open(Pid, pu8DmxId));

#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    return (_MApi_DMX_Pvr_Eng_Pid_Open(Eng,Pid,pu8DmxId));
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    MS_U32 u32fltid = 0;
    DrvTSP_FltType ftype = 0;

    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_PVR_EGN1 :
            ftype = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            return (DMX_FILTER_STATUS_ERROR);
    }

    _DMX_ENTRY();

    switch((DrvTSP_PKTDMXSrcType)(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng]))
    {
        case E_DRVTSP_PKTSRC_DEMUX0:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_PKTSRC_DEMUXFILE:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case E_DRVTSP_PKTSRC_DEMUX1:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case E_DRVTSP_PKTSRC_DEMUX2:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        default:
            break;
    }

    if(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng] == 0)
    {
        if(Eng == DMX_PVR_EGN0)
        {
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
        }
    }

    if(u8ShareKeyType == 1)
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY;
    }
    else
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &u32fltid))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    DMX_ASSERT2(u32fltid < DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, u32fltid)));
    MDrv_TSP_PidFlt_SetPid(0, u32fltid, Pid);
    _FLT_LIST_PID_SET((MS_U8)u32fltid, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, u32fltid, TRUE);

    _FLT_LIST_SECFLT_TYPE_SET(u32fltid, 0xFF, 0xFF, (DMX_FILTER_TYPE)0xFF);
    *pu8DmxId =  (MS_U8)(u32fltid & 0xFF);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    if((Eng != DMX_PVR_EGN0) || _pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1) //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Pid_Open(Eng, Pid, pu8DmxId);
    }

    _DMX_ENTRY();
    if(MDrv_TSP_PVR_PidFlt_Reserved(Pid, pu8DmxId, TRUE) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#endif

    return (DMX_FILTER_STATUS_ERROR);
}

//do nothing, before close path switch to TS1
//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID
/// @param Eng                                      \b IN: PVR engine ID
/// @param u8DmxId                              \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s](%d, %d) %d\n", __FUNCTION__, (int)Eng, (int)u8DmxId, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
    return (MApi_DMX_Pvr_Pid_Close(u8DmxId));
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    return (_MApi_DMX_Pvr_Eng_Pid_Close(Eng,u8DmxId));
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    _DMX_ENTRY();
    MDrv_TSP_PidFlt_Enable(0, u8DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u8DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u8DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u8DmxId);
    _FLT_LIST_REMOVE(u8DmxId);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    if((Eng != DMX_PVR_EGN0) || _pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1)  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Pid_Close(Eng, u8DmxId);
    }

    _DMX_ENTRY();

    if(MDrv_TSP_PVR_PidFlt_Reserved(0x1FFF, &u8DmxId, FALSE) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    return (DMX_FILTER_STATUS_ERROR);
}

//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id
/// @param Eng                                      \b IN: PVR engine ID
/// @param bPvrAll                                 \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s](%d, %d) %d\n", __FUNCTION__, (int)Eng, (int)bPvrAll, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
    return (MApi_DMX_Pvr_Start(bPvrAll));
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    return (_MApi_DMX_Pvr_Eng_Start(Eng,bPvrAll));
#else

    #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))
    if((Eng != DMX_PVR_EGN0) || (_pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1))  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Start(Eng, bPvrAll);
    }
    #endif

    _DMX_ENTRY();

    if(bPvrAll)
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_BYPASS, TRUE) != DRVTSP_OK)
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        _pdmx_res->_bPVRCA_RecAll[Eng] = TRUE;
    }
    else
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLT1CA, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        #if !(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
        if(MDrv_TSP_PVR_FLT1_StartRec(TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        #endif

        _pdmx_res->_bPVRCA_RecAll[Eng] = FALSE;
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif

    return (DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream
/// @param Eng                                      \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_ENG Eng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s](%d) %d\n", __FUNCTION__, (int)Eng, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0))
    return (MApi_DMX_Pvr_Stop());
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    return (_MApi_DMX_Pvr_Eng_Stop(Eng));
#else

    #if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))
    if((Eng != DMX_PVR_EGN0) || (_pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1))  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Stop(Eng);
    }
    #endif

    _DMX_ENTRY();

    if(_pdmx_res->_bPVRCA_RecAll[Eng])
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLTTYPE, FALSE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    #if !(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(MDrv_TSP_PVR_FLT1_StartRec(FALSE) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #endif

    if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLT1CA, FALSE) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif

    return (DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Open & set pid filter for file-in PVR using
/// @param Eng                                       \b IN   : PVR eigine id
/// @param Pid                                        \b IN   : PVR PID to record
/// @param pu8DmxId                             \b OUT: Pointer to store PID filter id
/// @param u8ShareKeyType                    \b IN   : The flag of sharekey filter using. if 0, not use sharekey filters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType)
{
#if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))

    MS_U32 u32fltid = 0;
    DrvTSP_FltType ftype = E_DRVTSP_FLT_SOURCE_TYPE_FILE|E_DRVTSP_FLT_TYPE_PVR;

    if(u8ShareKeyType != 0)
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY;
    }

    _DMX_ENTRY();

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &u32fltid))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    DMX_ASSERT2(u32fltid < DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, u32fltid)));
    MDrv_TSP_PidFlt_SetPid(0, u32fltid, Pid);
    _FLT_LIST_PID_SET((MS_U8)u32fltid, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, u32fltid, TRUE);

    _FLT_LIST_SECFLT_TYPE_SET(u32fltid, u32fltid, 0xFF, (DMX_FILTER_TYPE)0xFF);
    *pu8DmxId =  (MS_U8)(u32fltid & 0xFF);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
/// Close pid filter of file-in PVR
/// @param Eng                                       \b IN   : PVR eigine id
/// @param u8DmxId                               \b IN   : PID filter id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId)
{
#if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))

    DMX_PVRDBGMSG(DMX_DBG_FUNC, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u8DmxId));

    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u8DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u8DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u8DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u8DmxId);
    _FLT_LIST_REMOVE(u8DmxId);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Start file-in PVR recording
/// @param Eng                                       \b IN   : PVR eigine id
/// @param bPvrAll                                  \b IN   : FALSE is record PID, TRUE is record All ts data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
#if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))
    _DMX_PVR_ENTRY();

    if(bPvrAll)
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_BYPASS, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_FLTTYPE, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Stop file-in PVR recording
/// @param Eng                                       \b IN   : PVR eigine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Stop(DMX_PVR_ENG Eng)
{
#if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))

    _DMX_PVR_ENTRY();

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_BYPASS, FALSE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory
/// @param pFileinInfo \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Info(DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Info]\tRate[0x%x] PktMode[0x%x]\n",
            (unsigned int)pFileinInfo->Rate, (unsigned int)pFileinInfo->PKT_Mode);
#endif

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH0, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH0, (MS_U8)(pFileinInfo->Rate & 0xFF));
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH1, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH1, (MS_U8)(pFileinInfo->Rate & 0xFF));
#else //MMFI_VER_2_0

#ifdef MMFILEIN
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PTH_AUDIO, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PTH_AUDIO, (MS_U8)(pFileinInfo->Rate & 0xFF));
#endif //MMFILEIN

#ifdef MMFI_VD3D
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH_VD3D, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH_VD3D, (MS_U8)(pFileinInfo->Rate & 0xFF));
#endif //MMFILEIN

#endif //MMFI_VER_2_0

    MDrv_TSP_File_SetPacketMode((DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));
    return (DRVTSP_OK == MDrv_TSP_File_SetRate(pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

// @FIXME: ignore audio/video PES at this stage
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory
/// @param Dst \b IN: file in destination type
/// @param pBuf \b IN: the memory containing the bit stream
/// @param u32BufSize \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Start(DMX_FILEIN_DST Dst, MS_PHYADDR pBuf, MS_U32 u32BufSize)
{
    DrvTSP_FileinMode eFileMode;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Start]\tDst[0x%08X] Addr[0x%08X] Size[0x%08X]\n",
            (unsigned int)Dst, (unsigned int)pBuf, (unsigned int)u32BufSize);
#endif

    _DMX_ENTRY();
    switch (Dst)
    {
    case DMX_PES_NO_BYPASS_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
        break;
    case DMX_PES_AUDIO2_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
        break;
    case DMX_PES_AUDIO_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
        break;
    case DMX_PES_VIDEO_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
        break;
#ifdef MMFI_VD3D
    case DMX_PES_VIDEO3D_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
        break;
#endif

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    case DMX_PES_AUDIO3_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A3PES;
        break;
    case DMX_PES_AUDIO4_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A4PES;
        break;
#endif

    default:
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_DST %lu\n", __LINE__, (MS_U32)Dst)));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        break;
    }

    if (DRVTSP_OK != MDrv_TSP_File_SetAddr((MS_PHYADDR)(pBuf)))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_File_SetSize(u32BufSize))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK!= MDrv_TSP_File_Start(eFileMode))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Stop(void)
{

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Stop]\n");
#endif

    _DMX_ENTRY();
    if (DRVTSP_OK != MDrv_TSP_File_Stop())
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pause(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Pause]\n");
#endif

    return (DRVTSP_OK== MDrv_TSP_File_Pause())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Resume(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Resume]\n");
#endif

    return (DRVTSP_OK== MDrv_TSP_File_Resume())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_Reset(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn CMDQ Reset]\n");
#endif

    if (MDrv_TSP_File_CMDQ_Reset() == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_GetEmptyNum(MS_U32 *pu32EmptySlot)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_File_CMDQ_GetSlot(pu32EmptySlot);
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue fifo level
/// @param  pu8CmdQStatus      \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_CmdQFifo_Status(pu8CmdQStatus);
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going
/// @return TRUE if idle, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsIdle(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_BUSY;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_IDLE== FileinState);

}

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsBusy(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVTSP_FILEIN_STATE_BUSY== FileinState);
}

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused
/// @return TRUE if paused, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsPause(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVTSP_FILEIN_STATE_PAUSE== FileinState);
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Start(DMX_FILEIN_PATH ePath, DMX_FILEIN_DST Dst, MS_PHYADDR pBuf, MS_U32 u32BufSize)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    DrvTSP_FileinMode eFileMode;
    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    switch (Dst)
    {
        case DMX_PES_NO_BYPASS_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
            break;
        case DMX_PES_AUDIO2_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
            break;
        case DMX_PES_AUDIO_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
            break;
        case DMX_PES_VIDEO_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
            break;
    #ifdef MMFI_VD3D
        case DMX_PES_VIDEO3D_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
            break;
    #endif
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_DST %lu\n", __LINE__, (MS_U32)Dst));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_File_Eng_SetSize(eEng, u32BufSize))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSP_OK != MDrv_TSP_File_Eng_SetAddr(eEng, (MS_PHYADDR)(pBuf)))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_Start(eEng, eFileMode))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Start(Dst, pBuf, u32BufSize);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_DST eDst = 0;

        if(ePath == DMX_FILEIN_TSIF2)
        {
            eDst |= DMX_MMFI1_PES_TYPE_MASK;
        }

        if(Dst == DMX_PES_NO_BYPASS_FIFO)
        {
            eDst |= DMX_MMFI_PES_NO_BYPASS_TS;
        }
        else
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        return _MApi_DMX_MMFI_Filein_Start(eDst, pBuf, u32BufSize);

        #endif
    }
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Stop(DMX_FILEIN_PATH ePath)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_ENTRY();

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }
    if (DRVTSP_OK != MDrv_TSP_File_Eng_Stop(eEng))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Stop();
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Info(DMX_FILEIN_PATH ePath, DMX_Filein_info *pFileinInfo)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    // @F_TODO we set every PATH with same parameter???? add info function for MMFI
#if 0
    #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH0, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH0, (MS_U8)(pFileinInfo->Rate & 0xFF));
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH1, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH1, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #else //MMFI_VER_2_0

    #ifdef MMFILEIN
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PTH_AUDIO, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PTH_AUDIO, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #endif //MMFILEIN

    #ifdef MMFI_VD3D
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH_VD3D, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH_VD3D, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #endif //MMFILEIN

    #endif //MMFI_VER_2_0
#endif

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    MDrv_TSP_File_Eng_SetPacketMode(eEng, (DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));

    return (DRVTSP_OK == MDrv_TSP_File_Eng_SetRate(eEng, pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        MDrv_TSP_File_SetPacketMode((DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));
    	return (DRVTSP_OK == MDrv_TSP_File_SetRate(pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
        {
            mmpath = DMX_MMFI_PATH1;
        }
        return _MApi_DMX_MMFI_Filein_Info(mmpath, pFileinInfo);

        #endif
    }
#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Pause(DMX_FILEIN_PATH ePath)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
       case DMX_FILEIN_TSIF0:
           eEng = E_TSP_FILE_ENG_TSIF0;
           break;
       case DMX_FILEIN_TSIF1:
           eEng = E_TSP_FILE_ENG_TSIF1;
           break;
       case DMX_FILEIN_TSIF2:
           eEng = E_TSP_FILE_ENG_TSIF2;
           break;
       case DMX_FILEIN_TSIF3:
           eEng = E_TSP_FILE_ENG_TSIF3;
           break;
       default:
           DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
           return DMX_FILTER_STATUS_ERROR;
     }

     return (DRVTSP_OK == MDrv_TSP_File_Eng_Pause(eEng))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Pause();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Resume(DMX_FILEIN_PATH ePath)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_Resume(eEng))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Resume();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#endif
}

MS_BOOL _MApi_DMX_Filein_Eng_IsIdle(DMX_FILEIN_PATH ePath)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_BUSY;
    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_IDLE== FileinState);

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsIdle();
    }
    else
    {

        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_IsIdle(mmpath);

        #endif
    }

#endif
}

MS_BOOL _MApi_DMX_Filein_Eng_IsBusy(DMX_FILEIN_PATH ePath)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;
    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
       case DMX_FILEIN_TSIF0:
           eEng = E_TSP_FILE_ENG_TSIF0;
           break;
       case DMX_FILEIN_TSIF1:
           eEng = E_TSP_FILE_ENG_TSIF1;
           break;
       case DMX_FILEIN_TSIF2:
           eEng = E_TSP_FILE_ENG_TSIF2;
           break;
       case DMX_FILEIN_TSIF3:
           eEng = E_TSP_FILE_ENG_TSIF3;
           break;
       default:
           DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
           return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_BUSY == FileinState);

#else
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsBusy();
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_IsBusy(mmpath);

        #endif
    }

#endif


}

MS_BOOL _MApi_DMX_Filein_Eng_IsPause(DMX_FILEIN_PATH ePath)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;
    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
       case DMX_FILEIN_TSIF0:
           eEng = E_TSP_FILE_ENG_TSIF0;
           break;
       case DMX_FILEIN_TSIF1:
           eEng = E_TSP_FILE_ENG_TSIF1;
           break;
       case DMX_FILEIN_TSIF2:
           eEng = E_TSP_FILE_ENG_TSIF2;
           break;
       case DMX_FILEIN_TSIF3:
           eEng = E_TSP_FILE_ENG_TSIF3;
           break;
       default:
           DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
           return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_PAUSE== FileinState);

#else
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsPause();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#endif

}

// if we reset CMDQ when file in in busy .... filein hangs....
// @NOTE this is fixed in Kaiser
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_Reset(DMX_FILEIN_PATH ePath)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK == MDrv_TSP_File_Eng_CMDQ_Reset(eEng))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_Reset();
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_Reset(mmpath);

        #endif
    }

#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(DMX_FILEIN_PATH ePath, MS_U32 *pu32EmptySlot)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK == MDrv_TSP_File_Eng_CMDQ_GetSlot(eEng, pu32EmptySlot))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_GetEmptyNum(pu32EmptySlot);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(mmpath, pu32EmptySlot);

        #endif
    }

#endif

}

void _MApi_DMX_Filein_Eng_BypassFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_BOOL bbypass)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return ;
    }

    MDrv_TSP_FILE_Eng_192BlockScheme_En(eEng, !bbypass);

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        _MApi_DMX_BypassFileInTimeStamp(bbypass);
    }
    else
    {
        #if (defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        _MApi_DMX_MMFI_Filein_BypassTimeStamp(mmpath, bbypass);

        #endif
    }

#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel(DMX_FILEIN_PATH ePath, MS_U8 *pu8CmdQStatus)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK == MDrv_TSP_FILE_Eng_CmdQFifo_Status(eEng, pu8CmdQStatus))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_FIFOWriteLevel(pu8CmdQStatus);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(mmpath, pu8CmdQStatus);

        #endif
    }

#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_U32 *pu32FileInTS)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %lu\n", __LINE__, (MS_U32)ePath));
            return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK == MDrv_TSP_FILE_Eng_GetFileInTimeStamp(eEng, pu32FileInTS))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_GetFileInTimeStamp(pu32FileInTS);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_GetFileInTimeStamp(mmpath, pu32FileInTS);

        #endif
    }

#endif

}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetReadAddr(DMX_FILEIN_PATH ePath, MS_U32* pu32Read)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }
    if (E_TSP_OK == MDrv_TSP_FILE_Eng_GetFileInCurReadAddr(eEng,pu32Read))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_GetReadAddr(pu32Read);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return MDrv_MMFI_File_GetReadAddr(mmpath, pu32Read);

        #endif
    }

#endif
}


//-------------------------------------------------------------------------------------------------
/// Enable FileEng Playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(DMX_FILEIN_PATH ePath)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp enable]\tPath[%d]\n", (unsigned int)ePath);
#endif

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        case DMX_FILEIN_INVALID:
            return  DMX_FILTER_STATUS_ERROR;
            break;
    }

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(eEng,TRUE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
#else

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_TimeStampEnable();
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_TimeStampEnable(mmpath);

        #endif
    }

#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable FileEng Playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(DMX_FILEIN_PATH ePath)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp disable]\tPath[%d]\n", (unsigned int)ePath);
#endif

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        case DMX_FILEIN_INVALID:
            return  DMX_FILTER_STATUS_ERROR;
            break;
    }

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(eEng,FALSE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_TimeStampDisable();
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_TimeStampDisable(mmpath);

        #endif
    }

#endif
}


//-------------------------------------------------------------------------------------------------
/// Set playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_SetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32 u32Stamp)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u32Stamp));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        case DMX_FILEIN_INVALID:
            return  DMX_FILTER_STATUS_ERROR;
            break;
    }

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStamp(eEng,u32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u32Stamp));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_SetPlaybackStamp(u32Stamp);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_SetPlaybackTimeStamp(mmpath, u32Stamp);

        #endif
    }

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32* pu32Stamp)
{

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    TSP_FILE_ENG eEng = E_TSP_FILE_ENG_INVALID;

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (0x%x)\n", __FUNCTION__, __LINE__, (int)pu32Stamp));

    switch (ePath)
    {
        case DMX_FILEIN_TSIF0:
            eEng = E_TSP_FILE_ENG_TSIF0;
            break;
        case DMX_FILEIN_TSIF1:
            eEng = E_TSP_FILE_ENG_TSIF1;
            break;
        case DMX_FILEIN_TSIF2:
            eEng = E_TSP_FILE_ENG_TSIF2;
            break;
        case DMX_FILEIN_TSIF3:
            eEng = E_TSP_FILE_ENG_TSIF3;
            break;
        case DMX_FILEIN_INVALID:
            return  DMX_FILTER_STATUS_ERROR;
            break;
    }

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampGetPlaybackStamp(eEng,pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#else

    DMX_PVRDBGMSG(DMX_DBG_PVR, printf("[%s] %d (0x%x)\n", __FUNCTION__, __LINE__, (int)pu32Stamp));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_GetPlaybackStamp(pu32Stamp);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_GetPlaybackTimeStamp(mmpath, pu32Stamp);

        #endif
    }

#endif
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_MOBF_Enable(DMX_FILEIN_PATH ePath, MS_BOOL bEnable, MS_U32 u32key)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))

    return DMX_FILTER_STATUS_ERROR;

#else

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_MOBF_Enable(bEnable, u32key);
    }
    else
    {
        #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_MOBF_Enable(mmpath, bEnable, u32key);

        #endif
    }

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get STC counter
/// @param u8Eng \b IN: STC Engine ID
/// @param pu32Stc32 \b OUT: STC counter (bit 32)
/// @param pu32Stc \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Stc32, MS_U32* pu32Stc)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_GetSTC(u8Eng, pu32Stc32, pu32Stc)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Set STC counter
/// @param u8Eng \b IN: STC Engine ID
/// @param u32Stc32 \b IN: STC counter (bit 32)
/// @param u32Stc \b IN: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Set(MS_U8 u8Eng, MS_U32 u32Stc32, MS_U32 u32Stc)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_STC_CHANGE);
        dmx_dbg_print("[DMX STC]\teng[%d] STC[0x%x]\n", (int)u8Eng, (unsigned int)u32Stc);
#endif

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_SetSTC(u8Eng, u32Stc32, u32Stc))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Add or subtract STC Offset
/// @param u8Eng                      \b IN: STC Engine ID
/// @param u32Offset                  \b IN: STC offset value
/// @param bAdd                       \b IN:  If TRUE, add the offset value; otherwise subtract the offset value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_SetOffset(MS_U32 u32Eng, MS_U32 u32Offset, MS_BOOL bAdd)
{
#if !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(bAdd == FALSE)  //Not support subtracting yet
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();
    if(MDrv_TSP_SetSTCOffset(u32Eng, u32Offset, bAdd) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set STC control
/// @param u8Eng \b         IN: STC Engine ID
/// @param eMode \b         IN: STC update control options
///                                         0x00 -> Update STC by TSP FW
///                                         0x01 -> Update STC by HK
///                                         0x02 -> Update STC Once when PCR reset
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_STC_UpdateCtrl(MS_U8 u8Eng, eStcUpdateCtrlMode eMode)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_STC_UpdateCtrl(u8Eng, eMode) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Adjust STC Clock
/// @param u32EngId                     \b IN: STC Engine ID
/// @param bUpClk                         \b IN: If TRUE, Clk will be faster, and if FALSE, Clk will be slower
/// @param u32Percentage             \b IN: The percentage of STC clock ratio
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Clk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage)
{

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_STCClk_Adjust(u32EngId, bUpClk, u32Percentage))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
/// Select STC Engine (Only support in driver version 3.0)
/// @param eFltSrc                                 \b IN: Fliter Source type
/// @param u32StcEng                           \b IN: STC engine ID (0, 1)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DMX_FILTER_STATUS ret;
    DrvTSP_FltType eFltType;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    switch(eFltSrc & DMX_FILTER_SOURCE_TYPE_MASK)
    {
        case DMX_FILTER_SOURCE_TYPE_LIVE:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case DMX_FILTER_SOURCE_TYPE_TS1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case DMX_FILTER_SOURCE_TYPE_TS2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_STC_Select(eFltType, u32StcEng)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
/// Get PCR
/// @param pu32Pcr32 \b OUT: PCR counter (bit 32)
/// @param pu32Pcr \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pcr_Get(MS_U32* pu32Pcr32, MS_U32* pu32Pcr)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_GetPCR(0, pu32Pcr32, pu32Pcr)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Get PCR
/// @param u32PcrEng \b INPUT: PCR Eng
/// @param pu32Pcr32 \b OUT: PCR counter (bit 32)
/// @param pu32Pcr \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pcr_Eng_Get(MS_U8 u8PcrEng, MS_U32* pu32Pcr32, MS_U32* pu32Pcr)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_4_0) || (TSP_VERSION == TSP_VER_3_0))
    ret = (DRVTSP_OK== MDrv_TSP_GetPCR(u8PcrEng, pu32Pcr32, pu32Pcr)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    #else
    ret = (DRVTSP_OK== MDrv_TSP_GetPCR(u8PcrEng, pu32Pcr32, pu32Pcr)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    #endif
    _DMX_RETURN(ret);
}

//--------------------------------------------------------------------------------------------------
// Ask demuxer to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Proc(MS_U8 u8DmxId, DMX_EVENT* pEvent)      // for non-OS TSP scheduling
{
    TSP_Result bRet = (TSP_Result)DRVTSP_FAIL;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    if (0xFF != u8DmxId)
    {
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
        bRet = MDrv_TSP_Proc(0, u8DmxId, (TSP_Event*)pEvent);
#else
        MDrv_TSP_Proc(0, u8DmxId, &bRet, (DrvTSP_Event*)pEvent);
#endif
    }

    if (DRVTSP_OK == bRet)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
// Invert Demux parallel input bit order
// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
//                       DMX_FLOW_PVR for record flow
// @param bInvert \b IN: Invert bit order of TS parallel in
//                            TRUE for Invert
//                            FALSE for not invert
// @return DMX_FILTER_STATUS_OK - Success
// @return DMX_FILTER_STATUS_ERROR - Failure
// @note
//------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Parl_Invert(DMX_FLOW DmxFlow, MS_BOOL bInvert)
{
    TSP_Result          ret = (TSP_Result)DRVTSP_FAIL;

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    DrvTSP_If           TspIf = E_DRVTSP_IF_NUM;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    TSP_Result  ret0       = (TSP_Result)DRVTSP_FAIL;
    RASP_Result ret1       = (RASP_Result)DRVRASP_FAIL;
    MS_U16      u16TSIF    = 0;
    MS_U32      u32RASPEng = 0;
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            u16TSIF = 0;
            ret0 = MDrv_TSP_Parl_BitOrderSwap(0, u16TSIF, bInvert);
            ret1 = (RASP_Result)DRVRASP_OK;
            break;
        case DMX_FLOW_PVR:
            u16TSIF = 1;
            u32RASPEng = 0;
            ret0 = MDrv_TSP_Parl_BitOrderSwap(0, u16TSIF, bInvert);
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        case DMX_FLOW_PVR1:
            u32RASPEng = 1;
            ret0 = (TSP_Result)DRVTSP_OK;
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        case DMX_FLOW_PVR2:
            u32RASPEng = 2;
            ret0 = (TSP_Result)DRVTSP_OK;
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        case DMX_FLOW_PVR3:
            u32RASPEng = 3;
            ret0 = (TSP_Result)DRVTSP_OK;
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        case DMX_FLOW_PVR4:
            u32RASPEng = 4;
            ret0 = (TSP_Result)DRVTSP_OK;
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        case DMX_FLOW_PVR5:
            u32RASPEng = 5;
            ret0 = (TSP_Result)DRVTSP_OK;
            ret1 = MDrv_RASP_SetDataSwap(u32RASPEng, bInvert);
            break;
        default:
            MS_ASSERT(0);
            break;
    }

    if((ret0 == DRVTSP_OK) && (ret1 == DRVRASP_OK))
    {
        ret = (TSP_Result)DRVTSP_OK;
    }
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    switch(DmxFlow)
    {
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PLAYBACK1:
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_TS1;
            break;
        case DMX_FLOW_PVRCA1:
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_TS2;
            break;
        case DMX_FLOW_PVR2:
            TspIf = E_DRVTSP_IF_TS3;
            break;
        default:
            MS_ASSERT(0);
            break;
    }
    ret = MDrv_TSP_Parl_BitOrderSwap(0, TspIf, bInvert);
#else
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_PVR0;
            break;
        default:
            MS_ASSERT(0);
            break;
    }
    ret = MDrv_TSP_Parl_BitOrderSwap(0, TspIf, bInvert);
#endif
    if(DRVTSP_OK == ret)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//--------------------------------------------------------------------------------------------------
/// Copy section data to assigned buffer
/// @param  u8DmxId                 \b IN: demux ID
/// @param  pu8Buf                  \b OUT: section buffer address
/// @param  u32BufSize              \b IN: section buffer size
/// @param  pu32ActualSize          \b IN: section data size
/// @param  pu32RmnSize             \b OUT: section buffer remainning data size
/// @param  pfCheckCB               \b IN: callback function to check data correct or not
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_CopyData(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    DMX_FILTER_TYPE        DmxFltType;
    DMX_FILTER_STATUS      ret = DMX_FILTER_STATUS_ERROR;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    _DMX_ENTRY();

    if(_pdmx_res->_FltList[u8DmxId].FltId == 0xFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[%s][%d] Filte %d is already free\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        printf("[%s][%d] Filte %d is already free\n", __FUNCTION__, __LINE__, (int)u8DmxId);
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _FLT_LIST_TYPE_GET(u8DmxId, &DmxFltType);
    switch (DmxFltType & (~DMX_FILTER_FLT_MASK))
    {
    case DMX_FILTER_TYPE_SECTION:
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    case DMX_FILTER_TYPE_SECTION_VER:
#endif
        ret = _DMX_GetSect(u8DmxId, pu8Buf, u32BufSize, pu32ActualSize, pu32RmnSize, pfCheckCB);
        break;
    case DMX_FILTER_TYPE_PES:
    case DMX_FILTER_TYPE_TELETEXT:
    case DMX_FILTER_TYPE_PACKET:
        ret = _DMX_GetRingBuffer(u8DmxId, pu8Buf, u32BufSize, pu32ActualSize, pu32RmnSize, pfCheckCB);
        break;
    default:
        //MS_ASSERT(0);
        DMX_DBGMSG(DMX_DBG_FUNC, printf("copy data error, Line %d\n", __LINE__));
        break;
    }

#if DMX_DEBUG
    {
        MS_BOOL bFltMonitor = FALSE;
        MS_U32 u32TmpS, u32TmpSize, u32TmpR, u32TmpW;
        dmx_dbg_IsFltMonitor(u8DmxId, bFltMonitor);
        int i=0;

        if((u32DbgLogFlag & (DMX_DBG_LOG_COPYDATA | DMX_DBG_LOG_COPYDATA1)) && bFltMonitor)
        {
            MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32TmpS);
            MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32TmpSize);
            MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32TmpR);
            MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u8DmxId].SecBufId, &u32TmpW);

            dmx_dbg_print("[DMX CpData]\tFlt[%03d] Copied[%04d] Rmn[0x%08X] S[0x%08X] E[0x%08X] R[0x%08X] W[0x%08X]\n",
            (int)u8DmxId,
            (int)(*pu32ActualSize),
            (int)(*pu32RmnSize),
            (unsigned int)u32TmpS,
            (unsigned int)(u32TmpS + u32TmpSize),
            (unsigned int)u32TmpR,
            (unsigned int)u32TmpW);
        }

        if((u32DbgLogFlag & DMX_DBG_LOG_COPYDATA1) && bFltMonitor)
        {
            if(pfSEC)
            {
                if(*pu32ActualSize != fwrite(pu8Buf, 1, *pu32ActualSize, pfSEC))
                {
                    dmx_dbg_print("write file error, length %d\n", (int)(*pu32ActualSize));
                    fclose(pfSEC);
                    pfSEC = NULL;
                }
            }
            else
            {
                printf("[dump]\t");
                for(;i<MAX_NUM_CP_DATA_DUMP;i++)
                {
                    if(i%16 == 0)
                        printf("\n\t\t");
                    printf("%02X ", pu8Buf[i]);
                }
                printf("\n");
            }
        }
    }
#endif

    _DMX_RETURN(ret);
}

void _MApi_DMX_WriteProtect_Enable(MS_BOOL bEnable, MS_U32* pu32StartAddr, MS_U32* pu32EndAddr)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    MDrv_TSP_WriteProtect_Enable(bEnable, pu32StartAddr, pu32EndAddr);
#endif
}

void _MApi_DMX_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    MDrv_TSP_OrzWriteProtect_Enable(bEnable, u32StartAddr, u32EndAddr);
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FlowEnable(DMX_FLOW DmxFlow, MS_BOOL bEnable)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))

    DrvTSP_If e_TsIf;
    _DMX_ENTRY();

    switch(DmxFlow)
    {
        #if ((defined(TSP_VERSION)) && (TSP_VERSION == TSP_VER_3_0))
        case DMX_FLOW_PLAYBACK:
            e_TsIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PVR:
            e_TsIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_FILEIN_MM:
            e_TsIf = E_DRVTSP_IF_TS1;
            break;

        #else
        case DMX_FLOW_PLAYBACK:
            e_TsIf = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
            e_TsIf = E_DRVTSP_IF_PVR0;
            break;
        case DMX_FLOW_FILEIN_MM:
            e_TsIf = E_DRVTSP_IF_PVR1;
            break;

        #endif

        case DMX_FLOW_PVRCA:
        case DMX_FLOW_FILEIN_MM3D:
        case DMX_FLOW_NUM:
        default:
             _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    MDrv_TSP_TSIF_Enable(e_TsIf , bEnable);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set/Get DScmbler engine id depend on TSIF path
/// @param  eTsIf                             \b IN: eTsIf
/// @param  pu32EngId                      \b IN/OUT: pointer to store dscmb engine id
/// @param  bSet                              \b IN: TRUE:Set dscmb engine id ; FALSE: Get dscmb engine id
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Flow_DscmbEng(DMX_TSIF eTsIf, MS_U32* pu32EngId, MS_BOOL bSet)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DrvTSP_PKTDMXSrcType ePktDmxSrc = 0;
    
    switch(eTsIf)
    {
        case DMX_TSIF_LIVE0:
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_TSIF_LIVE1:    
        case DMX_TSIF_FILE1:        
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:        
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX2;
            break;    
        case DMX_TSIF_FILE0:    
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUXFILE;
            break;    
        default:
            *pu32EngId = 0xFF;
            return DMX_FILTER_STATUS_ERROR;        
    }

    _DMX_ENTRY();
    if(bSet == FALSE)
    {
        *pu32EngId = 0xFF;
        MDrv_TSP_GetDscmbEngIdx_BySource(ePktDmxSrc, pu32EngId);
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        if(MDrv_TSP_Dscmb_Source(*pu32EngId, &ePktDmxSrc, TRUE) == DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
            
}


//-------------------------------------------------------------------------------------------------
/// Set enable/disable drop scramble packet of fifo/pvr
/// @param  DmxFltType                   \b IN: DmxFltType
/// @param  bEnable                         \b IN: Enable
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_DropScmbPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    DrvTSP_FltType _Flt_type;
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_ENTRY();
    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
        case DMX_FILTER_SOURCE_TYPE_PVR0:
            _Flt_type = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_FILTER_SOURCE_TYPE_PVR1:
            _Flt_type = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for DropScmbPkt\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_DropScmbPkt(_Flt_type, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Configure output pad
/// @param  eOutPad                         \b IN: Select output pad
/// @param  eInSrcPad                       \b IN: Select input source pad
/// @param  bInParallel                       \b IN: TRUE: input source is paralleled; FALSE: input source is serial in
/// @param  u32ResvNum                    \b IN: Reserved value of the number of reserved parameters for future using
/// @param  pu32vRes                        \b IN: Reserved pointer to store parameters for future using
/// @note
/// Only EXT_PAD1 and EXT_PAD3 has output mode
/// if output from S2P mode, input source should be external serial input pad; if output from TSO mode, eInputPad should be E_DRVTSP_PAD_TSO
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TsOutputPadCfg(DMX_FLOW_OUTPUT_PAD eOutPad, DMX_FLOW_INPUT eInSrcPad, MS_BOOL bInParallel, MS_U32 u32ResvNum, MS_U32 *pu32Resv)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DrvTSP_PadIn eDrvOutPad, eInPad;
    MS_BOOL bEnable = TRUE;

    switch(eOutPad)
    {
        case DMX_FLOW_OUTPUT_EXT_PAD1:
            eDrvOutPad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        case DMX_FLOW_OUTPUT_EXT_PAD3:
            eDrvOutPad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    switch(eInSrcPad)
    {
        case DMX_FLOW_INPUT_DEMOD:
            eInPad = E_DRVTSP_PAD_DEMOD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            eInPad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            eInPad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            eInPad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            eInPad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4:
            eInPad = E_DRVTSP_PAD_EXT_INPUT4;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5:
            eInPad = E_DRVTSP_PAD_EXT_INPUT5;
            break;
        case DMX_FLOW_INPUT_TSO:
            eInPad = E_DRVTSP_PAD_TSO;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            eInPad = E_DRVTSP_PAD_DEMOD;
            bEnable = FALSE;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();
    if(MDrv_TSP_OutputPadCfg(eDrvOutPad, eInPad, bInParallel, bEnable) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------/// Configure output pad
/// @param  u16Val                           \b IN: Value of phase
/// @param  bEnable                          \b IN: Enable/Disable phase tuning
/// @param  u32Reserved                   \b IN: Reserved for future using
/// @note
/// Configure for S2P mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TsS2POutputClkPhase(MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    _DMX_ENTRY();
    if(MDrv_TSP_OutputClkPhase(0, u16Val, bEnable, u32S2pOpt) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))

    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = DRVTSP_OK== MDrv_TSP_ReadDropPktCnt(pu16ADropCnt, pu16VDropCnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//--------------------------------------------------------------------------------------------------
// Set demux filter owner
// @param  u8DmxIdStart        \b IN: demux start ID
// @param  u8DmxIdEnd          \b IN: demux end ID
// @param  bOwner                \b IN: TRUE for owner, FALSE for not owner
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetOwner(MS_U8 u8DmxIdStart, MS_U8 u8DmxIdEnd, MS_BOOL bOwner)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))

#else
    if (DRVTSP_OK != MDrv_TSP_Flt_SetOwner(0, (MS_U32) u8DmxIdStart, (MS_U32) u8DmxIdEnd, bOwner))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
// Get demux filter owner
// @param  u8DmxIdStart        \b IN: demux ID
// @param  bOwner                \b IN: pointer to store return value of owner flag
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetOwner(MS_U8 u8DmxId, MS_BOOL* pbOwner)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    *pbOwner = (DRVTSP_OK == MDrv_TSP_FLT_ChkOwner(_Owner, (MS_U32)u8DmxId)) ? ((_Owner)?TRUE:FALSE) : ((_Owner)?FALSE:TRUE);
#else
    *pbOwner = ((DRVTSP_OK == MDrv_TSP_PidFlt_ChkOwner(0, (MS_U32)u8DmxId))? TRUE: FALSE);
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_GetAccess(MS_U32 u32Try)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    return ((DRVTSP_OK == MDrv_TSP_IsAccess(u32Try))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
}

DMX_FILTER_STATUS _MApi_DMX_ReleaseAccess(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    return ((DRVTSP_OK == MDrv_TSP_UnlockAccess())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
}

DMX_FILTER_STATUS _MApi_DMX_SetHK(MS_BOOL bIsHK)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    _Owner = (bIsHK) ? 0 : 1;
    return DMX_FILTER_STATUS_OK;
#else
    return ((DRVTSP_OK == MDrv_TSP_SetHK(bIsHK))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
#endif

}

//--------------------------------------------------------------------------------------------------
/// Reset AVFIFO
/// @param  DmxFltType         \b IN: demux type
/// @param  bReset             \b IN: section buffer size
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE DmxFltType, MS_BOOL bReset)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_AVFIFO)
    {
        if(DmxFltType <= DMX_FILTER_TYPE_SECTION_VER)
            dmx_dbg_print("[DMX FIFO RST]\t[%s]\n", stDmxType[DmxFltType].str);
        else
            dmx_dbg_print("[DMX FIFO RST]\t[unknown %d]\n", (int)DmxFltType);
    }
#endif

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
#ifdef MMFI_VD3D
        case DMX_FILTER_TYPE_VIDEO3D:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
#endif
#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_3_0)||(TSP_VERSION == TSP_VER_4_0))))
        case DMX_FILTER_TYPE_AUDIO3:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
    #if (TSP_VERSION == TSP_VER_3_0)
        case DMX_FILTER_TYPE_AUDIO4:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
#endif
#endif
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for AVFIFO\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_Reset(_AVFifo_type, bReset) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//--------------------------------------------------------------------------------------------------
/// Get AVFIFO status(level)
/// @param  DmxFltType        \b IN: demux type
/// @param  u32FifoLevel      \b OUT: fifo level
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE DmxFltType, DMX_FIFO_STATUS* u32FifoLevel)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
#ifdef MMFI_VD3D
        case DMX_FILTER_TYPE_VIDEO3D:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
#endif
#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_3_0)||(TSP_VERSION == TSP_VER_4_0))))
        case DMX_FILTER_TYPE_AUDIO3:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
    #if (TSP_VERSION == TSP_VER_3_0)
        case DMX_FILTER_TYPE_AUDIO4:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
    #endif
#endif
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for AVFIFO\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_Status(_AVFifo_type, u32FifoLevel) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//--------------------------------------------------------------------------------------------------
/// Enable or disable AV FIFO block mechanism
/// @param  DmxFltType            \b IN: demux type
/// @param  bEnable                 \b OUT: TRUR: Enable block mechanism, FALSE: Disable block mechanism
/// @return DMX_FILTER_STATUS
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
DMX_FILTER_STATUS _MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for AVFIFO\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_BlockEnable(_AVFifo_type, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}
#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
DMX_FILTER_STATUS _MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
    TSP_FltType eFlttype;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            eFlttype = E_TSP_FLT_FIFO_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            eFlttype = E_TSP_FLT_FIFO_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            eFlttype = E_TSP_FLT_FIFO_AUDIO2;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            eFlttype = E_TSP_FLT_FIFO_AUDIO3;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            eFlttype = E_TSP_FLT_FIFO_VIDEO3D;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for AVFIFO\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_FIFO_BlockEnable(eFlttype, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}
#endif

/********************************************************************/
/// Get DMX Version.
///@param ppVersion \b OUT: DMX Version
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    *ppVersion = &_api_dmx_version;
    return DMX_FILTER_STATUS_OK;
}

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability
/// @param  DmxQueryType      \b IN: query type
/// @param  pOutput           \b OUT: answer of query type
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput)
{
    MS_U32 u32size = 0;
    MS_U32 u32Index = 0;

    DMX_ASSERT(DMX_CAP_NULL > DmxQueryType, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Capability type not Support!\n", __LINE__)));

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s][%d](0x%x, %p)\n", __FUNCTION__, __LINE__, DmxQueryType, pOutput));

    if(DmxQueryType == DMX_CAP_FW_BUF_ADDR)
    {
        *((MS_U32*)pOutput) = _u32TspFwAddr;
        return DMX_FILTER_STATUS_OK;
    }
    else if(DmxQueryType == DMX_CAP_FW_BUF_SIZE)
    {
        *((MS_U32*)pOutput) = _u32TspFwSize;
        return DMX_FILTER_STATUS_OK;
    }


    //DMX share resource = DMX own resource + TSP resource + MMFI resource + RASP resource + TSO resource
    if(DmxQueryType == DMX_CAP_RESOURCE_SIZE)
    {
        *((MS_U32*)pOutput) = sizeof(DMX_RESOURCE_PRIVATE);

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
        if(MDrv_TSP_GetCap(E_TSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;

        if(MDrv_RASP_GetCap(E_DRVRASP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVRASP_OK)
            return DMX_FILTER_STATUS_ERROR;
#else
        if(MDrv_TSP_GetCap(E_DRVTSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
#endif
        *((MS_U32*)pOutput) += u32size;

#if defined(MMFILEIN) || defined(MMFI_VD3D)
        if(MDrv_MMFI_GetCap(E_DRVMMFI_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVMMFI_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;
#endif

#ifdef TSO_ENABLE
        if(MDrv_TSO_GetCap(E_DRVTSO_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSO_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;
#endif

        return DMX_FILTER_STATUS_OK;
    }

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    if(MDrv_TSP_GetCap((TSP_Caps)((MS_U32)DmxQueryType), pOutput) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    if(DmxQueryType == DMX_CAP_PIDFLT_MAP)
    {
        MS_U8 *FltId = pOutput;
        for(u32Index=0;u32Index<DMX_MAX_FLTID;u32Index++)
        {
            FltId[u32Index] = _pdmx_res->_FltList[u32Index].FltId;
        }//it is 0xff if pidflt is not be opened
        return DMX_FILTER_STATUS_OK;
    }
    if(DmxQueryType == DMX_CAP_SECFLT_MAP)
    {
        MS_U8 *SecfltId = pOutput;
        for(u32Index=0;u32Index<DMX_MAX_FLTID;u32Index++)
        {
            SecfltId[u32Index] = _pdmx_res->_FltList[u32Index].SecBufId;;
        }//it is 0xff if pidflt is not be opened
        return DMX_FILTER_STATUS_OK;
    }

    if(MDrv_TSP_GetCap((DrvTSP_Cap)((MS_U32)DmxQueryType), pOutput) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif
}

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability by string type
/// @param  pstrQueryType                               \b IN: String of capability item
/// @param  inStrlen                                         \b IN: String size of capability item
/// @param  pOutput                                        \b OUT: answer of query type
/// @param  pOutSize                                       \b OUT: Output data length
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetCap_Ex(char* pstrQueryType, int inStrlen, void* pOutput, int* pOutSize)
{
    int ii;
    char* ptr = pstrQueryType;
    MS_U32 u32TabNum =0;

    *pOutSize = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s][%d](%s, %p)\n", __FUNCTION__, __LINE__, pstrQueryType, pOutput));
    DMX_ASSERT((pstrQueryType != NULL), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Capability type string is NULL!\n", __LINE__)));
    DMX_ASSERT((pOutput != NULL), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Output data pointer is NULL!\n", __LINE__)));
    DMX_ASSERT((inStrlen > 8), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Input Staring type is wrong!\n", __LINE__)));

    if(strncmp(ptr, "DMX_CAP_", 8) != 0)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    u32TabNum = sizeof(_tblDmxCapStr) / sizeof(_DMX_CAP_STRTBL);
    ptr += 8;
    for(ii = 0; ii < u32TabNum; ii++)
    {
        if(strcmp(ptr, _tblDmxCapStr[ii].strCap) == 0)
            break;
    }
    if(ii == (int)u32TabNum)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    *pOutSize = _tblDmxCapStr[ii].OutputSize;

    return _MApi_DMX_GetCap(_tblDmxCapStr[ii].eCapType, pOutput);

}

DMX_FILTER_STATUS _MApi_DMX_SetBurstLen(DMX_BURSTTYPE BurstType)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_BurstLen(BurstType) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

DMX_FILTER_STATUS _MApi_DMX_GetFileInTimeStamp(MS_U32 *pu32FileInTS)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_GetFileInTimeStamp(pu32FileInTS);
    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Filein_GetReadAddr(MS_U32* pu32Read)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    if(MDrv_TSP_File_GetReadAddr((MS_PHYADDR*)pu32Read) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

/********************************************************************/
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
///@param bbypass \b IN: If true, bypass file-in timestamp.
/********************************************************************/
void _MApi_DMX_BypassFileInTimeStamp(MS_BOOL bbypass)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_File_192BlockScheme_En(!bbypass);
}

DMX_FILTER_STATUS _MApi_DMX_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_GetDbgPortInfo(u32DbgSel,u32DbgInfo);
    return DMX_FILTER_STATUS_OK;
}

//[RESERVED] Unofficial function for A/V flow control by APP
DMX_FILTER_STATUS _MApi_DMX_SetAVPause(MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)
    // @TODO: hand over the control to APP if problem on LIVE mode
    // There is no know-how indicates that DSCMB burst output will cuase A/V FIFO overflow due to slow A/V
    // parser or decoder. - Jerry
#else
    MDrv_TSP_SetAVPause(bSet);
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

#if 0
DMX_FILTER_STATUS MApi_DMX_ResetFileinTimestamp(void)
{
    MDrv_TSP_ResetFileinTimestamp();
    return DMX_FILTER_STATUS_OK;
}
#endif

DMX_FILTER_STATUS _MApi_TSP_Get_FW_VER(MS_U32* u32FWVer)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_Get_FW_VER(u32FWVer);
    return DMX_FILTER_STATUS_OK;
}

/********************************************************************/
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVPkt(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_RemoveDupAVPkt(bEnable) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

/********************************************************************/
/// Enable remove duplicate A/V Fifo packets
/// @param  DmxFltType           \b IN: demux type
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVFifoPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    DrvTSP_FltType DrvFltTye;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            DrvFltTye = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            DrvFltTye = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            DrvFltTye = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            DrvFltTye = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf ("Incorrect type for AVFIFO\n"));
            return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_TSP_RemoveDupAVFifoPkt(DrvFltTye, bEnable) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }

#endif
    return DMX_FILTER_STATUS_ERROR;
}

/********************************************************************/
/// Enable or Disable to remove TEI  audio or video error packets.
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_TEI_RemoveErrorPkt(DMX_TEI_RmPktType eDmxPktType, MS_BOOL bEnable )
{
  DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));


#if (defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0))

    TSP_DRV_TEI_RmPktType eDrvPktType = E_TSP_DRV_TEI_REMOVE_AUDIO_PKT;

    switch ( eDmxPktType )
    {
        case E_DMX_TEI_REMOVE_AUDIO_PKT:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_AUDIO_PKT;
            break;
        case E_DMX_TEI_REMOVE_VIDEO_PKT:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_VIDEO_PKT;
            break;
        case E_DMX_TEI_REMOVE_PKT0_LIVE:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT0_LIVE;
            break;
        case E_DMX_TEI_REMOVE_PKT0_FILE:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT0_FILE;
            break;
        case E_DMX_TEI_REMOVE_PKT1:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT1;
            break;
        case E_DMX_TEI_REMOVE_PKT2:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT2;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad TEI__TYPE %lu\n", __LINE__, (MS_U32) eDmxPktType));
            return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_TSP_TEI_RemoveErrorPkt( eDrvPktType, bEnable ) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

  #else

     return DMX_FILTER_STATUS_ERROR;

  #endif

}

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow packet mode
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR: for recording flow
///                       DMX_FLOW_PVRCA: for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1: for second recording flow.
///                       DMX_FLOW_FILEIN_MM: for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D: for file-in playback with 2nd video input source .
///
/// @param ePktMode \b IN: Packet mode of flow path
///                            DMX_PKTMODE_188: Normal 188 packet mode
///                            DMX_PKTMODE_192: Normal 192 packet mode
///                            DMX_PKTMODE_204: Normal 204 packet mode
///                            DMX_PKTMODE_130: RVU 130 packet mode
///                            DMX_PKTMODE_134: RVU 134 packet mode
///                            DMX_PKTMODE_200: Open cable 200 packet mode
///                            DMX_PKTMODE_MERG188: Merge stream 188 packet mode
///                            DMX_PKTMODE_MERG192: Merge stream 192 packet mode
///                            DMX_PKTMODE_MERG200: Merge stream 200 packet mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetPacketMode(DMX_FLOW DmxFlow, DMX_PacketMode ePktMode)
{
#if (!(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))

    MS_U32 u32TSIf = 0;
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d (%d, %d)\n", __FUNCTION__, __LINE__, (int)DmxFlow, (int)ePktMode));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
        case DMX_FLOW_PVR:
        case DMX_FLOW_PVRCA:
            u32TSIf = 0;
            break;
        case DMX_FLOW_PLAYBACK1:
        case DMX_FLOW_PVR1:
        case DMX_FLOW_PVRCA1:
            u32TSIf = 1;
            break;
        case DMX_FLOW_PLAYBACK2:
        case DMX_FLOW_PVR2:
            u32TSIf = 2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }
#else
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            u32TSIf = 0;
            break;
        case DMX_FLOW_PVR:
        case DMX_FLOW_PVRCA:
            u32TSIf = 1;
            break;
        case DMX_FLOW_PVR1:
            u32TSIf = 2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }
#endif

    _DMX_ENTRY();

    if(MDrv_TSP_SetPacketMode(u32TSIf, (DrvTSP_PacketMode)ePktMode) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Sync Byte value to Stream source ID or TS source ID
/// @param u32SrcID \b               IN: Index of stream source id or TS source id
///                                                   Stream source ID is from 0 ~7
///                                                   TS source ID is mapping TSIF index
/// @param u8SyncByte \b            IN: Sync byte, defalut value is 0x47
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte)
{
#if (!(defined(TSP_VERSION) && ((TSP_VERSION >= TSP_VER_2_0))))

    _DMX_ENTRY();

    if(MDrv_TSP_SetMergeStrSyncByte(u32SrcID, u8SyncByte) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

/********************************************************************/
/// Change DMX filter source type
/// @param  u8DmxId                      \b IN: DMX filter ID
/// @param  DmxFltSrcType                \b IN: DMX filter Source Type
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_Change_FilterSource(MS_U8 u8DmxId, DMX_FILTER_TYPE DmxFltSrcType)
{
    _DMX_ENTRY();

    MS_U32 u32TSPFltSource = DmxFltSrcType & ((MS_U32)DMX_FILTER_FLT_MASK);

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    u32TSPFltSource  = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
#endif

    MDrv_TSP_PidFlt_ChangeSource(0, u8DmxId, u32TSPFltSource);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set memory buffer information for TSP AEON to print debug message
/// @param phyAddr \b IN: physical address of buffer
/// @param u32Size \b IN: size of buffer
/// @param u32DbgWord \b IN: control word to filter debug message
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFwDbgParam(MS_U32 u32Addr, MS_U32 u32Size, MS_U32 u32DbgWord)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d, u32Addr = 0x%x, u32Size = 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)u32Addr, (unsigned int)u32Size));
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else
    DMX_ASSERT((!(u32Addr & 0x3) && !(u32Size & 0x3)), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] memory address 0x%x size 0x%x not alignment\n", __LINE__, (unsigned int)u32Addr, (unsigned int)u32Size)));
    memset((void*)u32Addr, 0x0, u32Size);
    if(DRVTSP_OK == MDrv_TSP_SetFwDBGParam((MS_PHYADDR)MS_VA2PA(u32Addr), u32Size, u32DbgWord))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif
}

DMX_FILTER_STATUS _MApi_DMX_SetDbgLevel(DMX_DBGMSG_LEVEL level)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    DrvTSP_DbgLevel drvLayerLevel = E_DRVTSP_DBG_L1;
#endif

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
#else

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (level)
    {
        case DMX_DBG_ERR:
        case DMX_DBG_WARN:
        case DMX_DBG_INFO:
        case DMX_DBG_FUNC:
            drvLayerLevel = E_DRVTSP_DBG_L1;
            break;
        case DMX_DBG_NONE:
            drvLayerLevel = E_DRVTSP_DBG_Release;
            break;
        default:
            break;
    }

    MDrv_TSP_SetDbgLevel(drvLayerLevel);
#endif
    _u32DMXDbgLevel = level;
    return DMX_FILTER_STATUS_OK;
}

//--------------------------------------------------------------------------------------------------
/// Enable or Disable STC64 bit mode. Defalut is STC33 mode.
/// @param  bEnable           \b IN: Enable or Disable STC64 mode
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_STC64_Mode_Enable(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef STC64_SUPPORT
    MDrv_TSP_STC64_Mode_Enable(bEnable);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter and attach to a existed filter and buffer
/// @param DmxFltType \b IN: the filter information to allocate
/// @param pu8DmxId \b OUT: the available demux filer Id
/// @param u8TargetDmxId \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// @note  API for U4 TSP N filter to 1 Buffer HW architecture.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Open_MultiFlt(DMX_FILTER_TYPE DmxFltType , MS_U8* pu8DmxId, MS_U8 u8TargetDmxId)
{
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0) // @F_TODO add tsp ver 4?
    MS_U32              FltId= 0xFF;
    MS_U32              BufId= _pdmx_res->_FltList[u8TargetDmxId].SecBufId;
    DMX_FILTER_TYPE     TgtDmxFltType;
    MS_U32              u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK ;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8TargetDmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Target Demux id %d\n", __LINE__, u8TargetDmxId)));

    _DMX_ENTRY();

    printf("input Filter Type = %08lX \n",DmxFltType);

    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);
    _FLT_LIST_TYPE_GET(u8TargetDmxId,&TgtDmxFltType);

    printf("Get FlType %08lX \n",TgtDmxFltType);

    switch (TgtDmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
    }

    if( DmxFltType != TgtDmxFltType)
    {
        printf("[MAPI DMX][%06d] MultiFlt Type mismatch %lu\n", __LINE__, FltId);
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner , (DrvTSP_FltType)(eTspFltType|u32TSPFltSource), &FltId))
    {
          _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    DMX_ASSERT2(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad FltId %lu\n", __LINE__, FltId)));

    printf("MultiFlt FltId = %ld \n",FltId);

    MDrv_TSP_PidFlt_SelSecFlt(0, FltId, BufId);

    printf("MultiFlt AttachBuffer flt %ld Buf %ld\n",FltId,BufId);

    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, BufId, TgtDmxFltType);
    *pu8DmxId = (MS_U8)FltId;

    _DMX_RETURN( DMX_FILTER_STATUS_OK);
#else

    printf("[%s] Function Not support!! \n",__FUNCTION__);
    return DMX_FILTER_STATUS_ERROR ;

#endif
}

/********************************************************************/
/// Get interrupt count
///@param pu32Count \b OUT: interrupt count
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_Get_Intr_Count(MS_U32* pu32Count)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_TSP_Get_Intr_Count(pu32Count);
    return DMX_FILTER_STATUS_OK;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Drop_Enable(MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)
     MDrv_TSP_FLT_DropEnable(bSet);
     return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check if MMFilein no memory transfer is under going
/// @param  ePath                      \b IN: MM File-in Path
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_MMFI_Filein_IsIdle(DMX_MMFI_PATH ePath)
{
#if defined(MMFILEIN)

    DrvMMFI_FileinState     FileinState = E_DRVMMFI_STATE_UNKNOWN;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVMMFI_OK!= MDrv_MMFI_File_GetState((DrvMMFI_Path)ePath, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_MMFI_File_GetState fail\n", __LINE__)));
        return FALSE;
    }
    return (E_DRVMMFI_STATE_IDLE== FileinState);
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check if MMFilein memory transfer is under going
/// @param  ePath                      \b IN: MM File-in Path
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_MMFI_Filein_IsBusy(DMX_MMFI_PATH ePath)
{
#if defined(MMFILEIN)
    DrvMMFI_FileinState     FileinState = E_DRVMMFI_STATE_UNKNOWN;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVMMFI_OK!= MDrv_MMFI_File_GetState((DrvMMFI_Path)ePath, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_MMFI_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVMMFI_STATE_BUSY== FileinState);
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Reset MMFilein Command queue
/// @param  ePath                      \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_Reset(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#if defined(MMFILEIN)
    MDrv_MMFI_File_CmdQ_Reset((DrvMMFI_Path)ePath);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @param  ePath                            \b IN: MM File-in Path
/// @param  pu32EmptySlot                  \b OUT: Empty slot
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(DMX_MMFI_PATH ePath, MS_U32 *pu32EmptySlot)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    MDrv_MMFI_File_CmdQ_GetSlot((DrvMMFI_Path)ePath, pu32EmptySlot);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory
/// @param eDst             \b IN: file in destination path
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Start(DMX_MMFI_DST eDst, MS_PHYADDR pBuf, MS_U32 u32BufSize)
{
#if !(defined(MMFILEIN))
        return DMX_FILTER_STATUS_ERROR;
#else

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    DrvMMFI_Path path= E_DRVMMFI_PATH0;
#else
    DrvTSP_FileinMode FileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
    DrvMMFI_Path path= E_DRVMMFI_PTH_AUDIO;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_MMFI_ENTRY();

    if(MDrv_MMFI_File_ModeEnable((DrvMMFI_FileinMode)eDst, TRUE) != DRVMMFI_OK)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
    switch(eDst & (~DMX_MMFI1_PES_TYPE_MASK))
    {
        case DMX_MMFI_PES_NO_BYPASS_TS:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
            break;
        case DMX_MMFI_PES_PS_AU:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
            break;
        case DMX_MMFI_PES_PS_AUB:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
            break;

        #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        case DMX_MMFI_PES_PS_VD:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
            break;
        #endif //end MMFI_VER_2_0

        #ifdef MMFI_VD3D
        case DMX_MMFI_PES_PS_V3D:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
            break;
        #endif //end MMFI_VD3D
        default:
            break;
    }

    MDrv_TSP_File_PS_Path_Enable(FileMode);

#endif

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    if(eDst & DMX_MMFI1_PES_TYPE_MASK)
    {
        path= E_DRVMMFI_PATH1;
    }
#else //MMFI_VER_2_0
    #ifdef MMFI_VD3D
    if((eDst == DMX_MMFI_PES_V3DPID_BYPASS) || (eDst == DMX_MMFI_PES_NO_BYPASS_V3D)
        || (eDst == DMX_MMFI_PES_PS_V3D))
    {
        path = E_DRVMMFI_PATH_VD3D;
    }
    #endif

#endif

    if (DRVMMFI_OK != MDrv_MMFI_File_SetAddr(path, (MS_PHYADDR)(pBuf)))
    {
           _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVMMFI_OK != MDrv_MMFI_File_SetSize(path, u32BufSize))
    {
           _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVMMFI_OK!= MDrv_MMFI_File_Start(path))
    {
           _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);

#endif //!(defined(MMFILEIN))
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory
/// @param  ePath                                    \b IN: MM File-in Path
/// @param pFileinInfo                              \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Info(DMX_MMFI_PATH ePath, DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_File_SetPacketMode((DrvMMFI_Path)ePath, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer((DrvMMFI_Path)ePath, (MS_U8)(pFileinInfo->Rate & 0xFF));
    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @param  ePath                                    \b           IN: MM File-in Path
/// @param bbypass                                  \b          IN: If true, bypass file-in timestamp.
//-------------------------------------------------------------------------------------------------
void _MApi_DMX_MMFI_Filein_BypassTimeStamp(DMX_MMFI_PATH ePath, MS_BOOL bbypass)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_File_192BlockScheme_En((DrvMMFI_Path)ePath, !bbypass);
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein
/// @param  ePath                      \b IN: MM File-in Path
/// @param pu32FileInTS                       \b OUT: pointer for timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetFileInTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32FileInTS)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    MDrv_MMFI_GetFileInTimeStamp((DrvMMFI_Path)ePath, pu32FileInTS);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set the PID to be MM File-in
/// @param Pid \b IN: The target PID for MM Filein
/// @param pu8DmxId \b OUT: The demux filter Id for this MMFilein PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Open(DMX_MMFI_FLTTYPE flttype, MS_U16 u16Pid, MS_U8* pu8DmxId)
{

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else

    DrvMMFI_FltType    type = E_DRVMMFI_FLTTYPE_NONE;

#if defined(MMFI_VERSION) && (MMFI_VERSION == MMFI_VER_2_0)
    DrvMMFI_Path epath = E_DRVMMFI_PATH0;

    #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)
    TSP_TSId eTspTSid = E_TSP_TSID_INVALID;
    TSP_FileMode eFifoType = E_TSP_FILE_2_TSP;
    #endif

#endif

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_MMFI_ENTRY();

    switch(flttype & ~(DMX_MMFI1_FLTTYPE_MASK))
    {
        case DMX_MMFI_FLTTYPE_AUD:
            type = E_DRVMMFI_FLTTYPE_AUD;
            break;
        case DMX_MMFI_FLTTYPE_AUDB:
            type = E_DRVMMFI_FLTTYPE_AUDB;
            break;
#ifdef MMFI_VD3D
        case DMX_MMFI_FLTTYPE_VD3D:
            type = E_DRVMMFI_FLTTYPE_VD3D;
            break;
#endif
#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        case DMX_MMFI_FLTTYPE_VD:
            type = E_DRVMMFI_FLTTYPE_VD;
            break;
        case DMX_MMFI_FLTTYPE_AUDC:
            type = E_DRVMMFI_FLTTYPE_AUDC;
            break;
        case DMX_MMFI_FLTTYPE_AUDD:
            type = E_DRVMMFI_FLTTYPE_AUDD;
            break;
#endif
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    if(flttype & DMX_MMFI1_FLTTYPE_MASK)
    {
        epath = E_DRVMMFI_PATH1;
    }
    if(MDrv_MMFI_PidFlt_Alloc(epath, type, pu8DmxId) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    MDrv_TSP_AVFifo_SourceSelect((MS_U32)epath + (MS_U32)E_DRVTSP_PKTSRC_DEMUXMMFI0, (MS_U32)(flttype & ~(DMX_MMFI1_FLTTYPE_MASK)));
#elif defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0)

    switch(epath)
    {
        case E_DRVMMFI_PATH0:
            eTspTSid = E_TSP_TSID_MMFI0;
            break;
        case E_DRVMMFI_PATH1:
            eTspTSid = E_TSP_TSID_MMFI1;
            break;
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

    switch(type)
    {
        case E_DRVMMFI_FLTTYPE_AUD:
            eFifoType = E_TSP_FILE_2_AUDIO;
            break;
        case E_DRVMMFI_FLTTYPE_AUDB:
            eFifoType = E_TSP_FILE_2_AUDIO2;
            break;
        case E_DRVMMFI_FLTTYPE_AUDC:
            eFifoType = E_TSP_FILE_2_AUDIO3;
            break;
        case E_DRVMMFI_FLTTYPE_VD:
            eFifoType = E_TSP_FILE_2_VIDEO;
            break;
        case E_DRVMMFI_FLTTYPE_VD3D:
            eFifoType = E_TSP_FILE_2_VIDEO3D;
            break;
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

    MDrv_TSP_AVFifo_SourceSelect(eTspTSid,eFifoType);
    #endif

#else //else MMFI_VER_2_0

    if(MDrv_MMFI_PidFlt_Alloc(type, pu8DmxId) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif //endif MMFI_VER_2_0

    if(MDrv_MMFI_PidFlt_SetPid(*pu8DmxId, u16Pid) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_MMFI_PidFlt_Enable(*pu8DmxId, TRUE) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein
/// @param u8DmxId \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Close(MS_U8 u8DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else

    if(MDrv_MMFI_PidFlt_Free(u8DmxId) == DRVMMFI_FAIL)
    {
        return (DMX_FILTER_STATUS_ERROR);
    }

    return (DMX_FILTER_STATUS_OK);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get MMFilein Command queue fifo level
/// @param  ePath                             \b IN: MM File-in Path
/// @param  pu8CmdQStatus                  \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(DMX_MMFI_PATH ePath, MS_U8 *pu8CmdQStatus)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_File_CmdQFifo_Status((DrvMMFI_Path)ePath, pu8CmdQStatus);
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Abort to get bit stream by memeory
/// @param  ePath                      \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Abort(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
        return DMX_FILTER_STATUS_ERROR;
#else
    if (DRVMMFI_OK!= MDrv_MMFI_File_Abort((DrvMMFI_Path)ePath))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set MMFI playback timestamp
/// @param  ePath                          \b IN: MM File-in Path
/// @param u32pcr2                       \b IN: LPCR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32pcr2)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_SetPlaybackTimeStamp((DrvMMFI_Path)ePath, u32pcr2);
    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get MMFI Playback timestamp
/// @param ePath                          \b IN: MM File-in Path
/// @param pu32pcr2                     \b OUT: pointer to store LCPR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32pcr2)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
        return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_GetPlaybackTimeStamp((DrvMMFI_Path)ePath, pu32pcr2);
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_RemoveDupAVPkt(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    if(MDrv_MMFI_RemoveDupAVPkt(bEnable) == DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Enable MMFI timestamp mode
/// @param ePath                          \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampEnable(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_File_192Mode_En((DrvMMFI_Path)ePath, TRUE);
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable MMFI timestamp mode
/// @param ePath                          \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampDisable(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_File_192Mode_En((DrvMMFI_Path)ePath, FALSE);
    return DMX_FILTER_STATUS_OK;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_MMFI_MOBF_Enable(DMX_MMFI_PATH ePath, MS_BOOL bEnable, MS_U32 u32key)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MM_MOBF
    MDrv_MMFI_Set_MOBF_FileinKey((DrvMMFI_Path)ePath, u32key);
    MDrv_MMFI_MOBF_FileIn_Enable((DrvMMFI_Path)ePath, bEnable);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

#if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_SetLevel(DMX_MMFI_PATH epath, MS_U8 u8level)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MM_MOBF
    MDrv_MMFI_MOBF_FileIn_SetLevel((DrvMMFI_Path)epath, u8level);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}
#endif //MMFI_VER_2_0

DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampClk(DMX_MMFI_PATH ePath, DMX_TimeStamp_Clk eClk)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    if(DRVMMFI_OK == MDrv_MMFI_File_SetTimeStampClk((DrvMMFI_Path)ePath, (MS_U32)eClk))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt
/// @param bEnable \b IN: PVR MOBF Enable/Disable
/// @param u32key0 \b IN: MOBF encrypt key0
/// @param u32key1 \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(1, u32key0, u32key1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(1, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;
#elif (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    if(!bEnable)
    {
        MDrv_TSP_MOBF_PVR_Enable(FALSE);
        return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Set_MOBF_PVRKey(u32key0, u32key1);
    MDrv_TSP_MOBF_PVR_Enable(TRUE);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt
/// @param u8Eng \b IN: MOBF Engine ID
/// @param bEnable \b IN: PVR MOBF Enable/Disable
/// @param u32key0 \b IN: MOBF encrypt key0
/// @param u32key1 \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1)
{
#if defined(MOBF_ENABLE)

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(u8Eng+1, u32key0, u32key1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(u8Eng+1, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;

    #elif (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))

    MS_U32 u32MobfKey0 = u32key0, u32MobfKey1 = u32key1;

    if(bEnable == FALSE)
    {
        u32MobfKey0 = 0;
        u32MobfKey1 = 0;
    }

    if(u8Eng == 0)
    {
        MDrv_TSP_Set_MOBF_PVRKey(u32MobfKey0, u32MobfKey1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_PVR1Key(u32MobfKey0, u32MobfKey1);
    }

    return DMX_FILTER_STATUS_OK;
    #else
    return DMX_FILTER_STATUS_ERROR;
    #endif

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key
/// @param bEnable \b IN: File-in MOBF Enable/Disable
/// @param u32key   \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(0, u32key, 0);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(0, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;

#elif (!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    if(!bEnable)
    {
        MDrv_TSP_MOBF_FileIn_Enable(FALSE);
        return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Set_MOBF_FileinKey(u32key);
    MDrv_TSP_MOBF_FileIn_Enable(TRUE);
    return DMX_FILTER_STATUS_OK;

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

/********************************************************************/
/// Enable or Disable VQ
///@param bEnable           \b IN: Enable/Disable
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_VQ_Enable(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(VQ_ENABLE) && !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))))
    _DMX_ENTRY();
    MDrv_TSP_VQueEnable(bEnable);

    #if (defined(TS2_IF_SUPPORT) && !(defined(TSP_VERSION) && (TSP_VERSION >= TSP_VER_2_0)))
    _pdmx_res->_bVQEnabled = bEnable;
    #endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif //VQ_ENABLE
}

/********************************************************************/
///MApi_DMX_FilterFlowSwitch : Switch filter source from one channel to another.
///@param SrcFlow, TgtFlow
/// TSP1.0:
/// DMX_FILTER_SOURCE_TYPE_LIVE
/// DMX_FILTER_SOURCE_TYPE_TS1
/// TSP2.0:
/// DMX_FILTER_SOURCE_TYPE_PLAYBACK
/// DMX_FILTER_SOURCE_TYPE_PVR0
/// DMX_FILTER_SOURCE_TYPE_PVR1
/// TSP3.0:
/// DMX_FILTER_SOURCE_TYPE_LIVE
/// DMX_FILTER_SOURCE_TYPE_FILE
/// DMX_FILTER_SOURCE_TYPE_TS1
/// DMX_FILTER_SOURCE_TYPE_TS2
/// DMX_FILTER_SOURCE_TYPE_TS3
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_LiveSrcSwitch(DMX_FILTER_TYPE TgtFlow)
{
    _DMX_ENTRY();

    MDrv_TSP_FLT_LiveSrcSwitch(TgtFlow&DMX_FILTER_SOURCE_TYPE_MASK);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

/********************************************************************/
/// Enable or Disable Audio AB S
///@param bEnable           \b IN: Enable/Disable
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    return DMX_FILTER_STATUS_ERROR;
#else
    _DMX_ENTRY();

    if(MDrv_TSP_AU_BD_Mode_Enable(bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory of TSO path
/// @param u8Eng       \b IN: TSO engine ID
/// @param pFileinInfo \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Info(MS_U8 u8Eng, DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_SetPacketMode(u8Eng, (DrvTSO_PacketMode)(pFileinInfo->PKT_Mode));
    MDrv_TSO_Filein_Rate(u8Eng, (MS_U16)(pFileinInfo->Rate & 0xFFFF));
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going of TSO path
/// @param u8Eng       \b IN: TSO engine ID
/// @return TRUE if idle, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_TSO_Filein_IsIdle(MS_U8 u8Eng)
{
#ifdef TSO_ENABLE
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    DrvTSO_FileinState     TsoFileinState;

    if (DRVTSO_OK!= MDrv_TSO_Filein_GetState(u8Eng, &TsoFileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Call MDrv_TSO_Filein_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSO_STATE_IDLE == TsoFileinState);
#else

    return FALSE;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ empty number
/// @param u8Eng       \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum(MS_U8 u8Eng, MS_U32 *pu32EmptySlot)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    MDrv_TSO_Filein_CmdQ_GetSlot(u8Eng, pu32EmptySlot);
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ reset
/// @param u8Eng       \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_Reset(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Filein_CmdQ_Reset(u8Eng);
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#endif

    return DMX_FILTER_STATUS_OK;
}


//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory of TSO path
/// @param u8Eng            \b IN: TSO engine ID
/// @param pBuf              \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Start(MS_U8 u8Eng, MS_PHYADDR pBuf, MS_U32 u32BufSize)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE

    _DMX_TSO_ENTRY();
    if (DRVTSO_OK != MDrv_TSO_Filein_SetAddr(u8Eng, (MS_PHYADDR)pBuf))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSO_OK != MDrv_TSO_Filein_SetSize(u8Eng, u32BufSize))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSO_OK!= MDrv_TSO_Filein_Start(u8Eng))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory of TSO path
/// @param u8Eng            \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Stop(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE

    _DMX_TSO_ENTRY();
    if (DRVTSO_OK != MDrv_TSO_Filein_Stop(u8Eng))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set TSO playback time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param u32Stamp            \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u32Stamp));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK != MDrv_TSO_SetPlaybackTimeStamp(u8Eng, u32Stamp))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO playback time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_GetPlaybackStamp(MS_U8 u8Eng, MS_U32* pu32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d (0x%x)\n", __FUNCTION__, __LINE__, (int)pu32Stamp));

#ifdef TSO_ENABLE
    return ((DRVTSO_OK== MDrv_TSO_GetPlaybackTimeStamp(u8Eng, pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32 *pu32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    MDrv_TSO_GetFileInTimeStamp(u8Eng, pu32Stamp);
    //printf("MApi_DMX_TSO_GetFileInTimeStamp %x\n", (int)(*pu32Stamp));
#endif
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  read address
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Read            \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_U32* pu32Read)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(MDrv_TSO_Filein_GetReadAddr(u8Eng, (MS_PHYADDR*)pu32Read) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode block scheme to bypass fill-in timestamp
/// @param bbypass                  \b IN: If true, bypass file-in timestamp.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_BypassFileInTimeStamp(MS_U8 u8Eng, MS_BOOL bbypass)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK == MDrv_TSO_Filein_192BlockScheme_En(u8Eng, !bbypass))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Enable TSO file in  time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampEnable(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY()
    if(DRVTSO_OK== MDrv_TSO_Filein_192Mode_En(u8Eng, TRUE))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable TSO file in time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampDisable(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_Filein_192Mode_En(u8Eng, FALSE))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
 #else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set TSO Out Clock
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eTsOutClk                                    \b IN: TSO out clock select
/// @param  eTsOutClkSrc                               \b IN: TSO out clock source select
/// @param  u16DivNum                                  \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv                                        \b IN: If Out Clock invert
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_SetOutClk(MS_U8 u8Eng, DMX_TSO_OutClk eTsOutClk, DMX_TSO_OutClkSrc eTsOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_SetOutClk(u8Eng, (DrvTSO_OutClk)eTsOutClk, (DrvTSO_OutClkSrc)eTsOutClkSrc, u16DivNum, bClkInv))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable)
{
#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();

    if(MDrv_TSO_OutputEnable(u8Eng, bEnable) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//TSO2
//-------------------------------------------------------------------------------------------------
/// Set or get local stream ID of TSO Out
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eIf                                        \b IN: TSO input TSIF
/// @param  pu8StrId                                   \b IN or OUT: pointer to store local stream ID
/// @param  bSet                                       \b IN: If TRUE, set local stream id, otherwise get local stream id
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_LocalStreamId(MS_U8 u8Eng, DMX_TSIF eIf, MS_U8* pu8StrId, MS_BOOL bSet)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE
    DrvTSO_If eTsoIf = E_DRVTSO_IF_LIVE0;

    switch(eIf)
    {
        case DMX_TSIF_LIVE0:
            eTsoIf = E_DRVTSO_IF_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            eTsoIf = E_DRVTSO_IF_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            eTsoIf = E_DRVTSO_IF_LIVE2;
            break;
        case DMX_TSIF_FILE0:
            eTsoIf = E_DRVTSO_IF_FILE0;
            break;
        case DMX_TSIF_FILE1:
            eTsoIf = E_DRVTSO_IF_FILE1;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_LocalStreamId(u8Eng, eTsoIf, pu8StrId, bSet))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_PHYADDR u32Addr, MS_U32 u32BufSize)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef TSO_20_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK == MDrv_TSO_Set_SVQBuf(0,  u32Addr, u32BufSize))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif
    }

DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE    

    //printf("[%s] %d, bSet %d\n", __FUNCTION__, __LINE__, (int)pstInputCfg->bSet);

    if(pstInputCfg->bSet == TRUE)
    {       
        _DMX_TSO_ENTRY();
        
        //printf("[%s] %d\n", __FUNCTION__, __LINE__);
        if(_TSO_Flow_InputCfg(pstInputCfg) == TRUE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        DrvTSO_If stIf;
        MS_U8 u8Eng = 0;
        DrvTSO_If_Set stIfSet;

        if(pstInputCfg->eFlow == DMX_FLOW_TSO_PLAYBACK)
        {
            u8Eng = 0;
        }
        else if(pstInputCfg->eFlow == DMX_FLOW_TSO_PLAYBACK1)
        {
            u8Eng = 1;
        }
        else
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        switch(pstInputCfg->eTSOInIf)
        {
            case DMX_TSIF_LIVE0:
                stIf = E_DRVTSO_IF_LIVE0;
                break;
            case DMX_TSIF_LIVE1:
                stIf = E_DRVTSO_IF_LIVE1;
                break;
            case DMX_TSIF_LIVE2:
                stIf = E_DRVTSO_IF_LIVE2;
                break;
            case DMX_TSIF_FILE0:
                stIf = E_DRVTSO_IF_FILE0;
                break;
            case DMX_TSIF_FILE1:
                stIf = E_DRVTSO_IF_FILE1;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

        if((pstInputCfg->eTSOInIf == DMX_TSIF_FILE0) || (pstInputCfg->eTSOInIf == DMX_TSIF_FILE1))
        {
            pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_MEM;
            return DMX_FILTER_STATUS_OK;
        }        

        _DMX_TSO_ENTRY();
        
        if(MDrv_TSO_LocalStreamId(u8Eng, stIf, &(pstInputCfg->u8LocalStrId), FALSE) == FALSE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        if(MDrv_TSO_GetTSIFStatus(u8Eng, stIf, &stIfSet, &pstInputCfg->bBypassAll, &pstInputCfg->bEnable) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        switch(stIfSet.ePad)
        {
            case E_DRVTSO_PAD_EXT_INPUT0:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT0;
                break;
            case E_DRVTSO_PAD_EXT_INPUT1:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT1;
                break;
            case E_DRVTSO_PAD_EXT_INPUT2:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT2;
                break;
            case E_DRVTSO_PAD_EXT_INPUT3:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT3;
                break;
            case E_DRVTSO_PAD_EXT_INPUT4:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT4;
                break;
            case E_DRVTSO_PAD_EXT_INPUT5:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT5;
                break;
            case E_DRVTSO_PAD_DEMOD:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_DEMOD;
                break;
            default:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_MEM;
                _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        
        pstInputCfg->stInputInfo.bClkInv = stIfSet.bClkInv;
        pstInputCfg->stInputInfo.bExtSync = stIfSet.bExtSync;
        pstInputCfg->stInputInfo.bParallel = stIfSet.bParallel;

        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE
    //printf("[%s]eFlow= 0x%x, eOutPad= 0x%x, u16OutPktSize=0x%x\n",__FUNCTION__,pstOutputCfg->eFlow ,pstOutputCfg->eOutPad,pstOutputCfg->u16OutPktSize);
    //printf("bEnable= 0x%x, bDefOutClk= 0x%x, bOutClkInv=0x%x\n",pstOutputCfg->bEnable ,pstOutputCfg->bDefOutClk,pstOutputCfg->bOutClkInv);
    //printf("eTsoOutClk= 0x%x, eTsoOutClkSrc= 0x%x, u16DivNum=0x%x, bSet=0x%x\n",pstOutputCfg->eTsoOutClk ,pstOutputCfg->eTsoOutClkSrc,pstOutputCfg->u16DivNum,pstOutputCfg->bSet);

    if(pstOutputCfg->bSet == TRUE)
    {
        _DMX_TSO_ENTRY();
        if(_TSO_Flow_OutputCfg(pstOutputCfg) == TRUE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        MS_U8 u8Eng = 0;
        DrvTSO_PadOut eOutPad = E_DRVTSO_PAD_NONE;
        DrvTSO_OutClk eOutClk = 0xFF;
        DrvTSO_OutClkSrc eOutClkSrc = 0xFF;

        switch(pstOutputCfg->eFlow)
        {
            case DMX_FLOW_TSO_PLAYBACK:
                u8Eng = 0;
                break;
            case DMX_FLOW_TSO_PLAYBACK1:
                u8Eng = 1;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

        _DMX_TSO_ENTRY();
        if(MDrv_TSO_OutputPktSize(u8Eng, &(pstOutputCfg->u16OutPktSize), FALSE) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        if(MDrv_TSO_OutputPad(0, &eOutPad, FALSE) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        if(pstOutputCfg->bDefOutClk == TRUE)
        {
            pstOutputCfg->eTsoOutClk = E_DRVTSO_OUTCLK_DIV2N;
            pstOutputCfg->eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
            pstOutputCfg->u16DivNum = 0x0F;
            pstOutputCfg->bOutClkInv = FALSE;
        }
        else
        {
            if(MDrv_TSO_GetOutClk(u8Eng, &eOutClk, &eOutClkSrc, &(pstOutputCfg->u16DivNum), &(pstOutputCfg->bOutClkInv)) != DRVTSO_OK)
            {
                _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            switch(eOutClk)
            {
                case E_DRVTSO_OUTCLK_DIV2N:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_DIV2N;
                    break;
                case E_DRVTSO_OUTCLK_DIVN:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_DIVN;
                    break;
                case E_DRVTSO_OUTCLK_62M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_62M;
                    break;
                case E_DRVTSO_OUTCLK_54M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_54M;
                    break;
                case E_DRVTSO_OUTCLK_PTSOOUT:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_PTSOOUT;
                    break;
                case E_DRVTSO_OUTCLK_PTSOOUT_DIV8:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_PTSOOUT_DIV8;
                    break;
                case E_DRVTSO_OUTCLK_Dmd:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_Dmd;
                    break;
                case E_DRVTSO_OUTCLK_27M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_27M;
                    break;
                default:
                    _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            switch(eOutClkSrc)
            {
                case E_DRVTSO_OUTCLKSRC_172M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_172M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_288M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_288M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_432M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_432M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_216M_N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_216M_N;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS0IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS0IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS1IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS1IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS2IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS2IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS3IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS3IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS4IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS4IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS5IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS5IN;
                    break;
                default:
                    break;
            }
        }

        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_TSO_Configure(DMX_TSO_Cfg* pstTsoCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef TSO_20_ENABLE
    DrvTSO_Cfg stDrvTsoCfg;
    memset(&stDrvTsoCfg, 0x0, sizeof(stDrvTsoCfg));
    stDrvTsoCfg.u32TsoEng    = pstTsoCfg->u32TsoEng;
    stDrvTsoCfg.bSet         = pstTsoCfg->bSet;
    stDrvTsoCfg.eCfgCmd      = pstTsoCfg->eCfgCmd;
    stDrvTsoCfg.u32CmdData0  = pstTsoCfg->u32CmdData0;
    if(MDrv_TSO_Configure(&stDrvTsoCfg) == DRVTSO_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}
//FIQ API
DMX_FILTER_STATUS _MApi_DMX_FQ_SetFltRushPass(MS_U8 u8DmxId, MS_U8 u8Enable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    MDrv_TSP_PidFlt_SetFltRushPass(0, u8DmxId, u8Enable);
    _FLT_LIST_PID_SET(u8DmxId, u8Enable);

    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for FQ Enable
/// @param param                \b IN: Init FQ Setting
/// @return                     DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use FQ, please call this API after calling MApi_DMX_Init. (FQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_FQ_Init(MS_U32 u32FQEng, DMX_FQ_Info* pFQInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();

    if(!pFQInfo)
    {
        printf("ERROR!! pFQInfo is NULL!!\n");
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(pFQInfo->u32BufSize % 192)
    {
        printf("ERROR!! u32BufSize is not 192 aligned!!\n");
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(MDrv_FQ_Init(u32FQEng, pFQInfo->u8AddrMode) != DRVFQ_OK)
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);

    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    MDrv_TSP_PidFltFIFO_FullSel(E_TSP_PIDFLT_FIFO_FULL_LV_50);//to avoid data lost when rush
    #endif

    MDrv_FQ_SetBuffer(u32FQEng, pFQInfo->u32BufStart, pFQInfo->u32BufSize);
    MDrv_FQ_Start(u32FQEng, TRUE);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_Exit(MS_U32 u32FQEng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();
    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    MDrv_TSP_PidFltFIFO_FullSel(E_TSP_PIDFLT_FIFO_FULL_LV_100);
    #else
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    #endif

    MDrv_FQ_Start(u32FQEng, FALSE);
    MDrv_FQ_Exit(u32FQEng);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_RushEnable(MS_U32 u32FQEng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();
    MDrv_FQ_RushEnable(u32FQEng);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_SkipRushData(MS_U32 u32FQEng, DMX_FQ_SkipPath eSkipPath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();

    if((eSkipPath & E_DMX_FQ_SKIP_CFG1_MASK) == 0)
    {
        MDrv_FQ_SkipRushData(0UL, (MS_U16)eSkipPath);
    }
    else
    {
        MDrv_FQ_SkipRushData(1UL, (MS_U16)(eSkipPath & ~E_DMX_FQ_SKIP_CFG1_MASK));
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Hardware general config for driver interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_CMD_Run(u32Cmd, u32Config, u32DataNum, pData) != DRVTSP_OK)
        return DMX_FILTER_STATUS_ERROR;
    else
        return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
// Debug Table
//-------------------------------------------------------------------------------------------------
///Get dis-continue count by ts source and specific FIFO.
/// @param pDmxInfo          \b IN: set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_DisContiCnt(DMX_DisContiCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT
    DMX_FILTER_STATUS ret;
    DrvTSP_DisContiCnt_info tspInfo;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_DisContiCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspFifo = pDmxInfo->TspFifo;
    tspInfo.TspSrc  = pDmxInfo->TspSrc;

    ret = (DRVTSP_OK == MDrv_TSP_Get_DisContiCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
#else
        return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get drop packet count by ts source and specific FIFO.
/// @param pDmxInfo          \b IN: set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: drop packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_DropPktCnt(DMX_DropPktCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT
    DMX_FILTER_STATUS ret;
    DrvTSP_DropPktCnt_info tspInfo;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_DropPktCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspFifo = pDmxInfo->TspFifo;
    tspInfo.TspSrc  = pDmxInfo->TspSrc;

    ret = (DRVTSP_OK == MDrv_TSP_Get_DropPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
#else
     return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get locked packet count by ts source and specific FIFO.
/// @param pDmxInfo          \b IN: set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: Lock packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_LockPktCnt(DMX_LockPktCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT
    DMX_FILTER_STATUS ret;
    DrvTSP_LockPktCnt_info tspInfo;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_LockPktCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspTsif = pDmxInfo->TspTsif;

    ret = (DRVTSP_OK == MDrv_TSP_Get_LockPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
///Get dis-continue count by ts source and specific FIFO.
/// @param pDmxInfo          \b IN: set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: AV packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_AVPktCnt(DMX_AVPktCnt_info *DmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT
    DMX_FILTER_STATUS ret;
    DrvTSP_AVPktCnt_info tspInfo;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_AVPktCnt_info));
    tspInfo.TspCmd     = DmxInfo->TspCmd;
    tspInfo.TspFifo    = DmxInfo->TspFifo;
    tspInfo.TspFifoSrc = DmxInfo->TspFifoSrc; // for new chip(after Nasa), old chip is not used.

    ret = (DRVTSP_OK == MDrv_TSP_Get_AVPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);

    _DMX_RETURN(ret);
#else
     return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get section TEI packet count by ts source
/// @param FltSrc                     \b IN: TS source
/// @param pu32PktCnt            \b OUT: TEI packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc, MS_U32* pu32PktCnt)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    _DMX_ENTRY();
    if(MDrv_TSP_Get_SecTEI_PktCount(FltSrc, pu32PktCnt) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
///Reset section TEI packet count by DMX filter id
/// @param FltSrc                       \b IN: TS source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    _DMX_ENTRY();
    if(MDrv_TSP_Reset_SecTEI_PktCount(FltSrc) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
///Get section dis-continue packet count
/// @param u32DmxID                     \b IN: DMX filter Id
/// @param pu32PktCnt                    \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_SecDisCont_PktCount(MS_U32 u32DmxID, MS_U32* pu32PktCnt)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    _DMX_ENTRY();
    if(MDrv_TSP_Get_SecDisCont_PktCount(u32DmxID, pu32PktCnt) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
///Reset section dis-continue packet count
/// @param u32DmxID                     \b IN: DMX filter Id
/// @param pu32PktCnt                    \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset_SecDisCont_PktCount(MS_U32 u32DmxID)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    _DMX_ENTRY();
    if(MDrv_TSP_Reset_SecDisCont_PktCount(u32DmxID) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pu32ResMemAddr                \b IN: Pointer to store resource memory addresses
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_InitLibResource(void *pResMemAddr)
{
    MS_U32     u32size = 0;
    MS_U32     u32MemPos =  (MS_U32)pResMemAddr;

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s][%d](%p)\n", __FUNCTION__, __LINE__, pResMemAddr));

    DMX_ASSERT((pResMemAddr != 0), DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] data pointer is 0x%p!", __FUNCTION__, __LINE__, pResMemAddr)));

    _pdmx_res = (DMX_RESOURCE_PRIVATE*)pResMemAddr;

    if(_pdmx_res->bResoureInit != TRUE)
    {
        _DMX_InitResource(_pdmx_res);
    }

    u32MemPos += sizeof(DMX_RESOURCE_PRIVATE);

    if(MDrv_TSP_InitLibResource((void*)u32MemPos) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0))
    if(MDrv_TSP_GetCap(E_TSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != E_TSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    u32MemPos += u32size;
    if(MDrv_RASP_InitLibResource((void*)u32MemPos) != DRVRASP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    if(MDrv_RASP_GetCap(E_DRVRASP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVRASP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    #else
    if(MDrv_TSP_GetCap(E_DRVTSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    #endif

    u32MemPos += u32size;

    #if defined(MMFILEIN) || defined(MMFI_VD3D)
    if(MDrv_MMFI_InitLibResource((void*)u32MemPos) != DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_MMFI_GetCap(E_DRVMMFI_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    u32MemPos += u32size;
    #endif

    #ifdef TSO_ENABLE
    if(MDrv_TSO_InitLibResource((void*)u32MemPos) != DRVTSO_OK)
        return DMX_FILTER_STATUS_ERROR;
    #endif

    _pdmx_res->bResoureInit = TRUE;

    return DMX_FILTER_STATUS_OK;
}

//Merger stream API
//-------------------------------------------------------------------------------------------------
///Set or Get merge stream source id and sync byte
/// @param eIf                                 \b IN:  Eunm value of DMX TSIF selection
/// @param u8StrId                           \b IN:  Stream index
/// @param pu8SyncByte                   \b IN:  Pointer to sync bytearray of merege streams
/// @param bSet                              \b IN:  TRUE to setting data or FALSE to getting table
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note: Currently, maxmum number is 8, and don't call this API when stream processing is started
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MStr_SyncByte(DMX_TSIF eIf,  MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet)
{
#ifdef MERGE_STR_SUPPORT
    DrvTSP_If eTspIf = E_DRVTSP_IF_TS0;

    switch(eIf)
    {
        case DMX_TSIF_LIVE0:
            eTspIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_TSIF_LIVE1:
        case DMX_TSIF_FILE1:
            eTspIf = E_DRVTSP_IF_TS1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:
            eTspIf = E_DRVTSP_IF_TS2;
            break;
        case DMX_TSIF_FILE0:
            eTspIf = E_DRVTSP_IF_FI;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();
    if(MDrv_TSP_MStr_SyncByte(eTspIf, u8StrId, pu8SyncByte, bSet) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// STD general function
/// @param u16PowerState              \b IN: TSP power state
/// @param u32FWAddr                  \b IN: FW address
/// @param u32FWSize                  \b IN: FW size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 _MApi_DMX_SetPowerState(EN_POWER_MODE u16PowerState, MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))
    DMX_FILTER_STATUS _ret = DMX_FILTER_STATUS_ERROR;
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    switch (u16PowerState)
    {
        case E_POWER_SUSPEND:
            _ret = _MApi_DMX_Suspend();
            break;

        case E_POWER_RESUME:
            _ret = _MApi_DMX_Resume(u32FWAddr, u32FWSize);
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__));
            break;
    }

    return ((DMX_FILTER_STATUS_OK == _ret)? UTOPIA_STATUS_SUCCESS: UTOPIA_STATUS_FAIL);
#else
    return 0;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Wait_TspIoSignal(DMX_TSP_IOSIGNAL *pDmxTspIoSig)
{
#if !(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_2_0) || (TSP_VERSION == TSP_VER_4_0)))

    TSP_Result Res;

    pDmxTspIoSig->u32Eng    = 0;
    pDmxTspIoSig->u32TspEvt = 0;
    if((Res = MDrv_TSP_Wait_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig)) != DRVTSP_OK)
    {   
        pDmxTspIoSig->u32DmxEvt = 0;
        
        if(Res == DRVTSP_FUNC_ERROR) 
            return DMX_FILTER_STATUS_ERROR;
        else
            return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Proc_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig);

    pDmxTspIoSig->u32DmxEvt = 0;
    if(pDmxTspIoSig->u32PvrBufFullFlag[0] == 0xFF)
    {
        pDmxTspIoSig->u32DmxEvt |= DMX_EVENT_PVR1BUF_FULL;
    }
    else if(pDmxTspIoSig->u32PvrBufFullFlag[0] > 0)
    {
        pDmxTspIoSig->u32DmxEvt |= DMX_EVENT_PVRBUF_FULL;
    }

    if(pDmxTspIoSig->u32TspEvt & TSP_TASK_EVENT_SECTION_SELF)
    {
        pDmxTspIoSig->u32DmxEvt |= (DMX_EVENT_DATA_READY|DMX_EVENT_BUF_OVERFLOW);
    }

    pDmxTspIoSig->u32TspEvt &= ~TSP_TASK_EVENT_FLT_FREE;

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

#endif //undef UTOPIA_STRIP


