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

#include <unistd.h>     //close
#include <fcntl.h>      // open
#include <sys/ioctl.h>  // ioctl
#include <sys/mman.h>
#include "MsCommon.h"
#include "ion_uapi.h"
#include "halMPool.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include <string.h>
#include <errno.h>
#include <pthread.h>

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
#include "MsOS.h"
#include "utopia.h"
#endif

#if defined (ANDROID)
#include <cutils/log.h>
#endif

#if defined (TV_OS)
#include "mdrv_semutex_io.h"
#include <pthread.h>
#endif

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#ifdef _Debug
#define MPOOL_DEBUG
#endif

//--------------------------------------------------------------------------------------------------
//  New Internal Variable of MPool Implementation
//--------------------------------------------------------------------------------------------------
#define SUPPORT_PARTIAL_MAPPING 1
static MS_U32		MPOOL_MAPPING;
#define MAX_MAPPINGSIZE 100

#define MMAP_NONCACHE true
#define MMAP_CACHE false

#define ION_ALIGN 0x1000    // align to PAGE_SIZE, remember this ALIGN must <= PAGE_SIZE
#define NORMAL_MPOOL_INIT 0     //1: normal flow, 0: improve mpool init time

/*
static MS_BOOL             _Mpool_use[MAX_MAPPINGSIZE];//={-1}
static MS_U8               _Mpool_NonCache[MAX_MAPPINGSIZE];

static void*               _pMpoolAddrVStart[MAX_MAPPINGSIZE];// = { NULL, NULL};
static void*               _pMpoolAddrVEnd[MAX_MAPPINGSIZE];// = { NULL, NULL};
static MS_U32              _u32MpoolAddrPhys[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u64MpoolSize[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u32MPoolVa2PaOff[MAX_MAPPINGSIZE];// = { 0, 0};
*/

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_S32              _s32MPoolFd = -1;
static MS_S32              _s32FdION = -1;
static void*               _pAddrVirtStart[2] = { NULL, NULL};
static void*               _pAddrVirtEnd[2] = { NULL, NULL};
#if (!SUPPORT_PARTIAL_MAPPING)
static MS_SIZE              _tAddrPhys[2] = { 0, 0};
static MS_U32              _u32Size[2] = { 0, 0};
#endif
static MS_SIZE              _tVa2PaOff[2] = { 0, 0};

static MS_VIRT u32MpoolRegBase;

typedef struct
{
    MS_VIRT u64VirtStart;
    MS_VIRT u64VirtEnd;
    MS_U64 u64Phyaddr;
    MS_U64 u64MpoolSize;
    MS_BOOL bIsUsed;
    MS_SIZE s32V2Poff;
    MS_BOOL bNonCache;
    MS_U8 u8MiuSel;
    MS_BOOL bIsDyn;

    /* for ION, if mpool, set bIsION to 0, others set to -1 */
    MS_S32 handle_id;  // use this to specify different ion_mappings
    MS_S32  shared_mmap_fd;
    MS_BOOL bIsION;
} MPOOL_INFO;

static MsOSMPool_DbgLevel    _u32MPoolDBGLevel;
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];
static pthread_mutex_t _MsOS_MPool_Mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined (TV_OS)
static  MS_S32  _s32SemutexFD = -1;
static  void*     _psAddr = 0;
static  MS_U32    _u32ShmSize = 0;
static pthread_mutex_t          _MsOS_Openfd_Mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
#ifdef MPOOL_DEBUG
    #if defined (ANDROID)
    #ifndef LOGI // android 4.1 rename LOGx to ALOGx
    #define LOGI ALOGI
    #endif
    #define MPOOL_ERROR(fmt, args...)           LOGI("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            LOGI("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           LOGI("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #else
    #define MPOOL_ERROR(fmt, args...)           printf("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            printf("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           printf("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #endif

    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); MS_ASSERT(0); }
#else
    #define MPOOL_ERROR(fmt, args...)           do{} while (0)
    #define MPOOL_WARN(fmt, args...)            do{} while (0)
    #define MPOOL_PRINT(fmt, args...)           do{} while (0)
    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); }
#endif

#define MPOOL_IS_CACHE_ADDR(addr)              ( ( ((MS_VIRT)_pAddrVirtStart[0] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[0] > (MS_VIRT)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)            ( ( ((MS_VIRT)_pAddrVirtStart[1] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[1] > (MS_VIRT)addr) ) ? TRUE : FALSE )

#define MPOOL_DBG_MSG(debug_level, x)     do { if (_u32MPoolDBGLevel >= (debug_level)) (x); } while(0)

static MS_S32 _s32MapMutexId = -1;
static MS_U8  _u8MapMutex[] = {"MPool_Mutex"};

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static void cleanup_mmap(void)
{
    int i;
    for (i=0;i<MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed==true && mpool_info[i].u64VirtStart)
        {
            munmap((void *)mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize);
            mpool_info[i].bIsUsed = false;
            mpool_info[i].u64Phyaddr = 0;
            mpool_info[i].u64VirtStart = 0;
            mpool_info[i].u64VirtEnd = 0;
            mpool_info[i].u64MpoolSize = 0;
            mpool_info[i].s32V2Poff = 0;
        }
    }
}

static int __attribute__((destructor)) _cleanup(void)
{
    MsOS_MPool_Close();
    return 0;
}

MS_BOOL MsOS_MPool_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32MPoolFd)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] MPOOL is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}

	return FALSE;
}

MS_BOOL MsOS_MPool_Init(void)
{
    MS_U32 u32size;

	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

    if (0 <= _s32MPoolFd)
    {
        MPOOL_WARN("%s is initiated more than once\n", __FUNCTION__);
        return TRUE;
    }

    memset(mpool_info, 0, sizeof(MPOOL_INFO)*MAX_MAPPINGSIZE);

    if (0 > (_s32MPoolFd = open("/dev/malloc", O_RDWR)))
    {
        MPOOL_ERROR("Open /dev/malloc fail\n");
        MS_ASSERT(0);
        return FALSE;
    }
	else
	{
	}

    //Create Mutex
    if(_s32MapMutexId < 0)
    {
		MsOS_Init();  // Before MsOS_CreateMutex, need to do MsOS_Init()
        _s32MapMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8MapMutex, MSOS_PROCESS_SHARED);
    }
    if(_s32MapMutexId < 0)
    {
        printf("%s MsOS_CreateMutex failed!!\n", __FUNCTION__);
        return FALSE;
    }

#if defined (TV_OS)
	pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
		_s32SemutexFD = open("/dev/semutex", O_RDWR);
        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
#endif

    if(!MDrv_MMIO_GetBASE(&u32MpoolRegBase, &u32size, MS_MODULE_PM))
    {
        MPOOL_ERROR("%s Get IOMAP Base faill!\n",__FUNCTION__);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MPOOL_MutexLock(void)
    {
    if (_s32MapMutexId != -1)
    {
        return MsOS_ObtainMutex(_s32MapMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_ION_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32FdION)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] ION is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}
	else
		printf("\033[35mFunction = %s, Line = %d, ION is not Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);

	return FALSE;
}

MS_BOOL MsOS_ION_Init(void)
{
	printf("\033[35mOpen /dev/ion ...\033[m\n");
    if (0 <= _s32FdION)
    {
        printf("\033[35m ION Fd is opened\033[m\n");
        return FALSE;
    }

    if (0 > (_s32FdION = open("/dev/ion", O_RDWR)))
    {
        printf("\033[35mOpen /dev/ion fail\033[m\n");
        return FALSE;
    }

	printf("\033[35mOpen /dev/ion success\033[m\n");
    return TRUE;
}

static MS_BOOL _MPOOL_MutexUnlock(void)
{
    if (_s32MapMutexId != -1)
    {
        return MsOS_ReleaseMutex(_s32MapMutexId);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_MPool_Close(void)
{
    if (0 > _s32MPoolFd)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    if(_s32MapMutexId != -1)
    {
        MsOS_DeleteMutex(_s32MapMutexId);
        _s32MapMutexId = -1;
    }
    cleanup_mmap();
    close(_s32MPoolFd);
    _s32MPoolFd = -1;

#if defined (TV_OS)
    if (0 > _s32SemutexFD)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }
    close(_s32SemutexFD);
    _s32SemutexFD = -1;
    _psAddr = NULL;
#endif

    if (_psAddr)
    {
        munmap(_psAddr, _u32ShmSize);
        _psAddr = NULL;
    }
    
    return TRUE;
}

MS_BOOL MsOS_ION_Close(void)
{
    if(0 > _s32FdION)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    close(_s32FdION);
    _s32MPoolFd = -1;

    return TRUE;
}

#if defined (TV_OS)
//~!~ map sharemem
MS_VIRT MsOS_Mapping_SharedMem(MS_U32 u32ShmSize,MS_BOOL *bInit)
{
    //void * pAddr; unused variable - fixed compile warning
    *bInit = FALSE;

    if(_s32SemutexFD < 0)
    {
        MPOOL_ERROR("%s: mapping sharemem with invalid dev fd \n", __FUNCTION__);
        //return NULL; fixed compile warning
        return 0;
    }

//!!!
//there is a limitation, you must map sharemem first with the most largest len!
//that is , if you map with len A, then map with B(B > A), problem happens!!!

    if(_psAddr)
    {
        MPOOL_ERROR("~!~ 2 MsOS_Mapping_SharedMem = %p \n",_psAddr);
        return (MS_VIRT)_psAddr;
    }

    MPOOL_ERROR("~!~!~%s: pid = %d \n", __FUNCTION__,getpid());

    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_SHAREMEMORY,&u32ShmSize))
    {
        MPOOL_ERROR("create an anonymous sharememory failed! \n");
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    *bInit = ioctl(_s32SemutexFD,MDRV_SEMUTEX_QUERY_ISFIRST_SHAREMEMORY,NULL);

    _psAddr = mmap(NULL,u32ShmSize,PROT_READ | PROT_WRITE,MAP_SHARED,_s32SemutexFD,0);

    //LOGE("u32ShmSize= %d, _s32SemutexFD = %d, pAddr= %p \n",u32ShmSize,_s32SemutexFD,_psAddr);

    if (_psAddr == MAP_FAILED)
    {
        MPOOL_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    //LOGE("~!~ 1 MsOS_Mapping_SharedMem = %p \n",(MS_U32)_psAddr);
    _u32ShmSize = u32ShmSize;
    //*bInit = TRUE;
    return (MS_VIRT)_psAddr;

}

//~!~create namedmutex
MS_S32 MsOS_CreateNamedMutex(MS_S8 *ps8MutexName)
{
    MS_S32  s32Index;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);

#if 0
    CREATE_MUTEX_ARG createmutex;
    createmutex.pnameaddr = (unsigned long)ps8MutexName;
    createmutex.length = strlen(ps8MutexName);
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,&createmutex);
#else
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,ps8MutexName);
#endif

    if(s32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("the returned mutex index invalid!!! \n");
        return -1;
    }

    //s32Index -= MUTEX_INDEX_BEGIN;

    return s32Index;
}

//~!~lock mutex
MS_BOOL MsOS_LockMutex(MS_S32 u32Index,MS_S32 flag)
{
    MS_S32 res = 0;
    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    if(flag == 0)
    {
        res = -1;
        while(res == -1)
        {
            res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK,&u32Index);
            if(res == -1 && errno == EINTR)
                usleep(1000);
        }
    }
    else if(flag == -1)
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_TRY_LOCK,&u32Index);
    else
    {
        LOCKARG lockarg;
        lockarg.index = u32Index;
        lockarg.time = flag;
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK_WITHTIMEOUT,&lockarg);
    }
    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


//~!~lock mutex
MS_BOOL MsOS_UnlockMutex(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index  : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_UNLOCK,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


//~!~lock mutex
MS_BOOL MsOS_DeleteNamedMutexbyIndex(MS_S32 u32Index)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index \n", __FUNCTION__);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_DEL_MUTEX,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}
#endif

// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
#if(!SUPPORT_PARTIAL_MAPPING)
    if(MPOOL_MAPPING) return TRUE; //we had already mapped by function MsOS_MPool_Mapping()

    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32            bCache = (bNonCache) ? 0: 1;
    MS_U32                  u32Idx = (bCache) ? 0: 1;

    if (NULL == _pAddrVirtStart[u32Idx])
    {
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_INFO, &stPoolInfo))
        {
            return FALSE;
        }
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, stPoolInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }

        _pAddrVirtStart[u32Idx] = (void*)ptrAddrVirt;
        _pAddrVirtEnd[u32Idx]   = (void*)(ptrAddrVirt + stPoolInfo.u32Size);
        _tAddrPhys[u32Idx]    = stPoolInfo.u32Addr;
        _u32Size[u32Idx]        = stPoolInfo.u32Size;
        _tVa2PaOff[u32Idx]    = (MS_SIZE)_pAddrVirtStart[u32Idx] - (MS_SIZE)_tAddrPhys[u32Idx];
    }

    if (ppAddrVirt)
    {
        *ppAddrVirt   = _pAddrVirtStart[u32Idx];
    }
    if (pu32AddrPhys)
    {
        *pu32AddrPhys = _tAddrPhys[u32Idx];
    }
    if (pu32Size)
    {
        *pu32Size     = _u32Size[u32Idx];
    }
#endif
    return TRUE;
}

static void _MPool_Check_aligned(MS_U64 u64Offset, MS_U64 u64MapSize)
{
    if(u64Offset&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping address is not aligned, please make sure of that\n");
        MS_ASSERT(0);
		//assert(0);
    }

    if(u64MapSize&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping size is not aligned, please make sure of that\n");
		MS_ASSERT(0);
        //assert(0);
    }

    return;
}

static MS_U8 _MPOOL_DELAY_BINDING(MS_S32 idx)
{
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_U64                  u64AddrVirt;
    MS_U32            bCache = (mpool_info[idx].bNonCache) ? 0: 1;

	MS_U8 	u8Check_MiuSel = 0;
	MS_U64 	u64Check_Offset = 0;

    stPoolInfo.u64Addr = mpool_info[idx].u64Phyaddr;
    stPoolInfo.u64Size = mpool_info[idx].u64MpoolSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = mpool_info[idx].u8MiuSel;

	/* use u64Phyaddr to get MIU, offset */
	_phy_to_miu_offset(u8Check_MiuSel, u64Check_Offset, stPoolInfo.u64Addr);

	if(u8Check_MiuSel != stPoolInfo.u8MiuSel)
	{
		printf("\033[35mFunction = %s, Line = %d, [Error] miu_setting is wrong\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return FALSE;
	}
	stPoolInfo.u64Addr = u64Check_Offset;

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (u64AddrVirt = (MS_VIRT)mmap(0, mpool_info[idx].u64MpoolSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = u64AddrVirt;
    mpool_info[idx].u64VirtEnd = (u64AddrVirt + mpool_info[idx].u64MpoolSize);

	mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;
#if defined (__aarch64__)
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%lx\n", idx,mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%lx\n", idx,mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
#else
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%llx\n", idx,mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%llx\n", idx,mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
#endif

    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
    if (MPOOL_IS_CACHE_ADDR(pAddrVirt))
    {
        return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[0]);
    }

    if (MPOOL_IS_UNCACHE_ADDR(pAddrVirt))
    {
       return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[1]);
    }

    // we currently will go to here
    MS_S32 i;
    for(i = 0; i <MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed)
        {
            if(!((mpool_info[i].u64VirtStart <= pAddrVirt) && (pAddrVirt < mpool_info[i].u64VirtEnd)))
                continue;

            return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt, mpool_info[i].s32V2Poff);
        }
    }
	#if defined (__aarch64__)
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%lx) TO PA(%x)\n",pAddrVirt,0);
	#else
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%tx) TO PA(%x)\n",pAddrVirt,0);
	#endif
    return (MS_PHY) 0xFFFFFFFFUL;
}

inline MS_BOOL MsOS_MPool_IsVAMapped(MS_VIRT pAddrVirt)
{
    if (MPOOL_IS_CACHE_ADDR(pAddrVirt))
    {
        return TRUE;
    }
    
    if (MPOOL_IS_UNCACHE_ADDR(pAddrVirt))
    {
       return TRUE;
    }

    int i;
    for(i=0;i<MAX_MAPPINGSIZE;i++)
    {
        if(mpool_info[i].bIsUsed)
        {
            if(!((mpool_info[i].u64VirtStart<=pAddrVirt)&&(pAddrVirt<mpool_info[i].u64VirtEnd)))
                continue;

            return TRUE;
        }
        else
            break;

    }
    return FALSE;
}


inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys) // un-cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[1])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE;i ++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(!mpool_info[i].bNonCache)
            {
                continue;
            }

	        // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
	        if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
	        {
	            continue;
	        }

            if(mpool_info[i].bIsDyn)
            {
                _MPOOL_MutexLock();
	            if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
                {
                    if(!_MPOOL_DELAY_BINDING(i))
                    {
						#if defined (__aarch64__)
                        printf("PA2KSEG1 delay binding fail!, pa = %lx", mpool_info[i].u64Phyaddr);
						#else
                        printf("PA2KSEG1 delay binding fail!, pa = %llx", mpool_info[i].u64Phyaddr);
						#endif
						_MPOOL_MutexUnlock();
			            return (MS_VIRT)NULL;
                    }
		        }
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			#if defined (__aarch64__)
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%lx\n", idx,mpool_info[idx].u64Phyaddr));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%lx\n", idx,mpool_info[idx].u64MpoolSize));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
			#else
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%llx\n", idx,mpool_info[idx].u64Phyaddr));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%llx\n", idx,mpool_info[idx].u64MpoolSize));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
			#endif
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG1 return NULL pAddrPhys =%x\n", pAddrPhys));

        //assert(0);
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[1], printf("MsOS_MPool_Init/MsOS_MPool_Get for un-cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    return pAddrVirt;
}

inline MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY pAddrPhys) // cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[0])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE; i++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(mpool_info[i].bNonCache)
            {
                continue;
            }

    	    // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
    	    if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
    	    {
    		    continue;
    	    }

            if(mpool_info[i].bIsDyn)
            {
    	        _MPOOL_MutexLock();
    	        if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
        	    {
        		    if(!_MPOOL_DELAY_BINDING(i))
    	            {
						#if defined (__aarch64__)
    	                printf("PA2KSEG0 delay binding fail!, pa = %lx",mpool_info[i].u64Phyaddr);
						#else
    	                printf("PA2KSEG0 delay binding fail!, pa = %llx",mpool_info[i].u64Phyaddr);
						#endif

            			_MPOOL_MutexUnlock();
                        return (MS_VIRT)NULL;
    	            }
    	        }
                //mpool_info[i].bIsDyn = false    // joe, need to be discussed
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			#if defined (__aarch64__)
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%lx\n", idx,mpool_info[idx].u64Phyaddr));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%lx\n", idx,mpool_info[idx].u64MpoolSize));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
			#else
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%llx\n", idx,mpool_info[idx].u64Phyaddr));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%llx\n", idx,mpool_info[idx].u64MpoolSize));
				MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
			#endif
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG0 return NULL\n"));
        //assert(0);
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[0], printf("MsOS_MPool_Init/MsOS_MPool_Get for cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    return pAddrVirt;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush_All()
{
    if (HAL_MsOS_MPool_Dcache_Flush_All(_s32MPoolFd) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_VIRT pAddrVirt, MS_SIZE tSize)
{
    MS_PHY pAddrPhys;

    pAddrPhys = MsOS_MPool_VA2PA(pAddrVirt);

    /* to prevent the va is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
    if(pAddrPhys == 0xFFFFFFFF)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    /* to prevent the va_end is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
	if( MsOS_MPool_VA2PA(pAddrVirt + (tSize - 1)) == 0xFFFFFFFF ) 
	{
		MsOS_MPool_Dcache_Flush_All();
		return TRUE;
	}

    if (HAL_MsOS_MPool_Dcache_Flush(_s32MPoolFd, pAddrVirt, tSize, pAddrPhys) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_BOOL bNonCache)
{
#if (NORMAL_MPOOL_INIT == 1)
    MS_S32 i,idx = 0;
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32            bCache = (bNonCache) ? 0: 1;

    if (tMapSize <= 0)
    {
        printf("[User][WARNING][%s] Mapping size should not <= 0 !!!!! \n\n", __FUNCTION__);
        return TRUE;
    }

    stPoolInfo.u64Addr = tOffset;
    stPoolInfo.u64Size = tMapSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = u8MiuSel;

    _MPool_Check_aligned(tOffset, tMapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %tx, tMapSize = %tx, bNonCache = %02x\n", tOffset, tMapSize, bNonCache));

    if(mpool_info[MAX_MAPPINGSIZE-1].bIsUsed)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;

            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            break;
        }
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, tMapSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = ptrAddrVirt;
    mpool_info[idx].u64VirtEnd = (ptrAddrVirt + tMapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;

    _miu_offset_to_phy(u8MiuSel, tOffset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = tMapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    /* ION Setting */
    mpool_info[idx].handle_id = 0;
    mpool_info[idx].shared_mmap_fd = 0;
    mpool_info[idx].bIsION = false;
    /* ION Setting */

	#if defined (__aarch64__)
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%lx\n", idx,mpool_info[idx].u64Phyaddr));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%lx\n", idx,mpool_info[idx].u64MpoolSize));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
	#else
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%llx\n", idx,mpool_info[idx].u64Phyaddr));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%llx\n", idx,mpool_info[idx].u64MpoolSize));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
	#endif

    MPOOL_MAPPING = 1;

    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    MS_S32              _s32UtopiaFd = -1;
	KDrvMPool_Info_t    stPoolInfoKernel;

    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");

    }else
    {
        stPoolInfoKernel.u32Addr = tOffset;
        stPoolInfoKernel.u32Size = tMapSize;
        stPoolInfoKernel.u32Interval = MIU_INTERVAL;
        stPoolInfoKernel.u8MiuSel = u8MiuSel;
        stPoolInfoKernel.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfoKernel))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");

        }
        close(_s32UtopiaFd);
    }
    #endif

    return TRUE;
#else
    
    return MsOS_MPool_Mapping_Dynamic(u8MiuSel, tOffset, tMapSize, bNonCache);
  
#endif
}

MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_BOOL bNonCache)
{
    MS_S32 i,idx=0;
    KDrvMPool_Info_t  stPoolInfo;
    MS_S32 u32MaxCount = 0;
    MS_S32              _s32UtopiaFd = -1;
    MS_U32 Phyaddr = 0, PhyaddrEnd = 0;

    if (u32MapSize <= 0)
    {
        printf("[User][WARNING][%s] Mapping size should not <= 0 !!!!! \n\n", __FUNCTION__);
        return TRUE;
    }

    _MPool_Check_aligned(u32Offset, u32MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %x, u32MapSize = %x, bNonCache = %02x\n", u32Offset, u32MapSize, bNonCache));

    Phyaddr = u32Offset;
    PhyaddrEnd = u32Offset + u32MapSize - 1;
#if 0
    if(mpool_info[MAX_MAPPINGSIZE-1].bIsUsed)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }
#endif

    //check mmap table
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if (mpool_info[i].bIsUsed == false)
        {
            continue;
        }
        else
        {
            u32MaxCount++;
            if (bNonCache != mpool_info[i].bNonCache || u8MiuSel != mpool_info[i].u8MiuSel)
                continue;

            if ((mpool_info[i].u64Phyaddr <= Phyaddr && Phyaddr < mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize) ||
                (mpool_info[i].u64Phyaddr <= PhyaddrEnd && PhyaddrEnd < mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))
            {
                printf("[Utopia2K][User space] Duplicated PA(0x%lx ~ 0x%lx) mapping with Table[%ld](0x%llx ~0x%llx).\n",
                         Phyaddr, PhyaddrEnd, i, mpool_info[i].u64Phyaddr, (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize));
                return TRUE;
            }
        }
    }

    if (u32MaxCount >= (MAX_MAPPINGSIZE - 1))
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }
    pthread_mutex_lock(&_MsOS_MPool_Mutex); //avoid thread race condition for mpool init
    
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;

            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            break;
        }
    }

    mpool_info[idx].u64VirtStart = 0;
    mpool_info[idx].u64VirtEnd = 0;
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = true;

    _miu_offset_to_phy(u8MiuSel, u32Offset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = u32MapSize;
    mpool_info[idx].s32V2Poff = 0;

    /* ION Setting */
    mpool_info[idx].handle_id = 0;
    mpool_info[idx].shared_mmap_fd = 0;
    mpool_info[idx].bIsION = false;
    /* ION Setting */

    MPOOL_MAPPING = 1;

    pthread_mutex_unlock(&_MsOS_MPool_Mutex);  //avoid thread race condition for mpool init
   
    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");

    }
    else
    {
        stPoolInfo.u32Addr = u32Offset;
        stPoolInfo.u32Size = u32MapSize;
        stPoolInfo.u32Interval = MIU_INTERVAL;
        stPoolInfo.u8MiuSel = u8MiuSel;
        stPoolInfo.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfo))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");

        }
        close(_s32UtopiaFd);
    }
    #endif

    return TRUE;
}

MS_BOOL MsOS_MPool_UnMapping(MS_VIRT ptrVirtStart, MS_SIZE tMapSize)
{
    MS_S32 i, res;

    union {
        struct ion_fd_data fd;  // ION_IOC_MAP will fill this struct, we will get a mmap_fd for a buffer
        struct ion_allocation_data allocation;  // alloc related data will be stored here
        struct ion_handle_data handle;  // only a handler_id, used to find handle, by ion_handle_get_by_id(). You should save allocation.handle to handle.handle once you do ION_IOC_ALLOC
        struct ion_custom_data custom;
        struct ion_user_data bus_addr_info; // for keeping bus_addr to msos layer, msos layer will change to miu/offset
		struct ion_cust_allocation_data cust_allocation;
    } data;

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == true)
        {
            if( (mpool_info[i].u64VirtStart == ptrVirtStart) && (mpool_info[i].u64MpoolSize == tMapSize) )
            {
                if( !munmap((MS_VIRT *)mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize) )
                {
					#if defined (__aarch64__)
                	printf("mpool[%d] Unmapping PA:[%lx], VA:[%tx], Size:[%tx] done.\n", i, mpool_info[i].u64Phyaddr, mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize);
					#else
                	printf("mpool[%d] Unmapping PA:[%llx], VA:[%tx], Size:[%llx] done.\n", i, mpool_info[i].u64Phyaddr, mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize);
					#endif
					mpool_info[i].bIsUsed = false;
                    mpool_info[i].u64Phyaddr = 0;
                    mpool_info[i].u64VirtStart = 0;
                    mpool_info[i].u64VirtEnd = 0;
                    mpool_info[i].u64MpoolSize = 0;
                    mpool_info[i].s32V2Poff = 0;

                    /* cause default value is 0 */
                    mpool_info[i].bNonCache = 0;
                    mpool_info[i].u8MiuSel = 0;
                    mpool_info[i].bIsDyn = false;                    
                }
                else
                {
					#if defined (__aarch64__)
                    printf("Unmapping PA:[%lx], VA:[%lx], Size:[%lx] fail.\n", mpool_info[i].u64Phyaddr, mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize);
					#else
                    printf("Unmapping PA:[%llx], VA:[%tx], Size:[%llx] fail.\n", mpool_info[i].u64Phyaddr, mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize);
					#endif

                    break;
                }

                /* for ION, we also need to release the allocated buffer */
                if(mpool_info[i].bIsION)
                {
					printf("\033[35mFunction = %s, Line = %d, release handle id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, mpool_info[i].handle_id);
					data.handle.handle = mpool_info[i].handle_id;

                    res = ioctl(_s32FdION, ION_IOC_FREE, &data);
                    if (res < 0)
                    {
                        perror("ion free buffer failed");
                        perror("ion free buffer failed");
                        perror("ion free buffer failed");
                        MS_ASSERT(0);
                        return FALSE;
                    }
                }
				
				printf("\033[35mFunction = %s, Line = %d, releasing cma_memory is done\033[m\n", __PRETTY_FUNCTION__, __LINE__);
                mpool_info[i].handle_id = 0;
                mpool_info[i].shared_mmap_fd = 0;
                mpool_info[i].bIsION = false;

                return TRUE;
            }
        }
    }
	#if defined (__aarch64__)
    printf("Unmapping Fail !! VA:[%lx], Size:[%lx].\n", ptrVirtStart, tMapSize);
	#else
    printf("Unmapping Fail !! VA:[%tx], Size:[%llx].\n", ptrVirtStart, tMapSize);
	#endif

    return FALSE;
}

MS_BOOL MsOS_MPool_Kernel_Detect(MS_PHY *lx_addr, MS_U64 *lx_size, MS_PHY *lx2_addr, MS_U64 *lx2_size)
{
    DevMalloc_MPool_Kernel_Info_t kernel_info;
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_KERNELDETECT, &kernel_info))
    {
        return FALSE;
    }

   *lx_addr = kernel_info.u64lxAddr;
   *lx_size = kernel_info.u64lxSize;
   *lx2_addr = kernel_info.u64lx2Addr-0x40000000;
   *lx2_size = kernel_info.u64lx2Size;

   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx_addr = %lx, lx_size = %lx\n", (MS_U64)lx_addr, lx_size));
   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx2_addr = %lx, lx2_size = %lx\n", (MS_U64)lx2_addr, lx2_size));

  return TRUE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
    _u32MPoolDBGLevel = DbgLevel;
}

MS_BOOL MsOS_MPool_SetWatchPT(MS_VIRT ptrAddrVirt, MS_U32 u32ASID, MS_U8 u8Global, MS_U8 u8WType, MS_U32 u32Mask)
{
	DevMalloc_MPool_Watchpt_Info_t stWPTInfo;
	stWPTInfo.mask = u32Mask;
	stWPTInfo.u64AddrVirt = ptrAddrVirt;
	stWPTInfo.ASID = u32ASID;
	stWPTInfo.global = u8Global;
	stWPTInfo.rwx = u8WType;
	if(ioctl(_s32MPoolFd, MALLOC_IOC_SETWATCHPT, &stWPTInfo)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("this is api test after ioctl by york\n");
	return TRUE;
}

MS_BOOL MsOS_MPool_GetWatchPT(char *str)
{
	printf("Read watchpoint register\n");
	if(ioctl(_s32MPoolFd, MALLOC_IOC_GETWATCHPT, str)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("%s\n",str);
	return TRUE;
}

/*
 * we can use ION_IOC_MAP to get a share_mmap_fd,
 * then we can use this share_mmap_fd to do dma_buf mapping
 * but we use _s32MPoolFd(/dev/malloc) instead
 */
MS_U32 MsOS_ION_Mapping(MS_U32 u32ION_Alloc_Flag, MS_U8 u8ION_FixAddr_MiuSel, MS_U32 u32ION_FixAddrOffset, MS_U32 u32MapSize, MS_U32 u32Driver_Heap_ID, MS_BOOL bNonCache)
{
    MS_S32 res, i, mpool_index;

	/* mmap related */
    MS_VIRT ptrAddrVirt;
    MS_U32 u32AddrOffset;
	
	/* ion_releated */
    MS_S32 ion_handle_id;
    MS_U32 ion_Buffer_Phyaddr;
	MS_U32 ion_Alloc_Start_Phyaddr = 0;
    MS_U8 ion_MiuSel;

    DevMalloc_MPool_Info_t stPoolInfo; // for using mpool_mapping only

    MS_U32 bCache = (bNonCache) ? 0: 1;

    union {
        struct ion_fd_data fd;  // ION_IOC_MAP will fill this struct, we will get a mmap_fd for a buffer
        struct ion_allocation_data allocation;  // alloc related data will be stored here
        struct ion_handle_data handle;  // only a handler_id, used to find handle, by ion_handle_get_by_id(). You should save allocation.handle to handle.handle once you do ION_IOC_ALLOC
        struct ion_custom_data custom;
        struct ion_user_data bus_addr_info; // for keeping bus_addr to msos layer, msos layer will change to miu/offset
		struct ion_cust_allocation_data cust_allocation;
    } data;

    // check page_align first(only size, since we do not know the phy_offset for now)
    _MPool_Check_aligned(0, u32MapSize);

    // find an available mpool_info index
    for(i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;

            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            mpool_index = i;
            break;
        }
    }
    if(i == MAX_MAPPINGSIZE)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }

    // step 1. ask a buffer from ION CMA_HEAP(according to the heap_id) and get the bus_addr ==> we should get a handle_id
	if(u32ION_Alloc_Flag & ION_FLAG_CONTIGUOUS)
	{
    	data.cust_allocation.start = ion_Alloc_Start_Phyaddr;	// alloc contiguous memory from cma_buffer
	}
	else if(u32ION_Alloc_Flag & ION_FLAG_STARTADDR)
	{
		_miu_offset_to_phy(u8ION_FixAddr_MiuSel, u32ION_FixAddrOffset, ion_Alloc_Start_Phyaddr);
		data.cust_allocation.start = HAL_MsOS_MPool_PA2BA(ion_Alloc_Start_Phyaddr);	// want to alloc contiguous memory, start form ion_Alloc_Start_Phyaddr
	}
	else if(u32ION_Alloc_Flag & ION_FLAG_DISCRETE)
	{
		data.cust_allocation.start = ion_Alloc_Start_Phyaddr;   // just alloc from cma_buffer, and do not need to be contiguous
	}
	else
	{
		printf("Strange u32ION_Alloc_Flag 0x%X!!\n", (unsigned int)u32ION_Alloc_Flag);
		return FALSE;
	}

    data.cust_allocation.len = u32MapSize;   // get mapping size from caller
    data.cust_allocation.align = ION_ALIGN;
    data.cust_allocation.heap_id_mask = (1 << u32Driver_Heap_ID); // get allocated heap_id from caller
    data.cust_allocation.flags = u32ION_Alloc_Flag | ION_FLAG_CACHED_NEEDS_SYNC;	// map to non-cached, however we do not care this mapping

    /* We alloc a buffer with specified size from specified heap.
     * This buffer has not mapped to user space,
     * and we will get a handle_id as data.allocation.handle.
     */
    res = ioctl(_s32FdION, ION_IOC_CUST_ALLOC, &data);
    if (res < 0)
    {
        MS_ASSERT(0);
        return FALSE;
    }

#if 0
	/* this should keep, for ion_mmap in the future */
    data.handle.handle = data.allocation.handle;		// for ION_IOC_MAP
    data.fd.handle = data.allocation.handle;			// not sure what this is for
#endif

    /* keep this handle for the allocated buffer */
    ion_handle_id = data.cust_allocation.handle;
	printf("\033[35mFunction = %s, Line = %d, asking for heap %u with handle is %d, done\033[m\n", __PRETTY_FUNCTION__, __LINE__, u32Driver_Heap_ID, ion_handle_id); // joe.liu

    // step 2. use ION_IOC_GET_CMA_BUFFER_INFO to get the buffer start Bus_Addr(we can refer this ION_IOC_GET_CMA_BUFFER_INFO example to add new ioctl cmd we want)
    data.bus_addr_info.handle = ion_handle_id; // the ION_IOC_GET_CMA_BUFFER_INFO will use handle_id from data.bus_addr_info
    res = ioctl(_s32FdION, ION_IOC_GET_CMA_BUFFER_INFO, &data);
    if (res < 0)
    {
        MS_ASSERT(0);
        return FALSE;
    }
    ion_Buffer_Phyaddr = HAL_MsOS_MPool_BA2PA(data.bus_addr_info.bus_addr);   // get pa
    _phy_to_miu_offset(ion_MiuSel, u32AddrOffset, ion_Buffer_Phyaddr);   // get miu
    printf("\033[35mFunction = %s, Line = %d, bus_addr is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)data.bus_addr_info.bus_addr);
    printf("\033[35mFunction = %s, Line = %d, phy_addr is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)ion_Buffer_Phyaddr);
    printf("\033[35mmiu is %d, offset is 0x%X\033[m\n", ion_MiuSel, (unsigned int)u32AddrOffset);

    // step 3. use mpool_mapping to map to user space
    stPoolInfo.u64Addr = u32AddrOffset;
    stPoolInfo.u64Size = u32MapSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = ion_MiuSel;

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, u32MapSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);
	printf("\033[35mFunction = %s, Line = %d, va is %td\033[m\n", __PRETTY_FUNCTION__, __LINE__, ptrAddrVirt); // joe.liu

    // set mpool_info
    mpool_info[mpool_index].handle_id = ion_handle_id;
    mpool_info[mpool_index].u64Phyaddr = ion_Buffer_Phyaddr;
    mpool_info[mpool_index].u8MiuSel = ion_MiuSel;
    mpool_info[mpool_index].u64MpoolSize = u32MapSize;
    mpool_info[mpool_index].bIsION = true;
    mpool_info[mpool_index].bIsDyn = false;
    mpool_info[mpool_index].shared_mmap_fd = -1;    // we do not use share_mmap for now
    mpool_info[mpool_index].u64VirtStart = ptrAddrVirt;
    mpool_info[mpool_index].u64VirtEnd = ptrAddrVirt + mpool_info[mpool_index].u64MpoolSize;
    mpool_info[mpool_index].s32V2Poff = mpool_info[mpool_index].u64VirtStart - mpool_info[mpool_index].u64Phyaddr;

	printf("\033[35mFunction = %s, Line = %d, return pa is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (MS_U32)mpool_info[mpool_index].u64Phyaddr);
    return mpool_info[mpool_index].u64Phyaddr;
}

MS_BOOL __attribute__((weak)) MsOS_MPool_Add_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL find = FALSE;
    MS_U64 u64AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_U32 i, idx = 0;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %llx, u64MapSize = %llx, u64VirtAddr = %tx\n", (unsigned long long)u64PhysAddr, (unsigned long long)u64MapSize, u64VirtAddr));

    //for multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);
   
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;
            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;
            
            idx = i;
            find = TRUE;
            
            break;
        }
    }

    if(!find)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;         
    }

    _phy_to_miu_offset(u8MiuSel, u64AddrOffset, u64PhysAddr);   // get miu & offset

    mpool_info[idx].u64VirtStart = u64VirtAddr;
    mpool_info[idx].u64VirtEnd = (u64VirtAddr + u64MapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;
    mpool_info[idx].u64Phyaddr = u64PhysAddr;
    mpool_info[idx].u64MpoolSize = u64MapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

#if defined (__aarch64__)
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%lx\n", idx,mpool_info[idx].u64Phyaddr));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%lx\n", idx,mpool_info[idx].u64MpoolSize));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
#else
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtStart =%tx\n", idx,mpool_info[idx].u64VirtStart));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64VirtEnd = %tx\n", idx,mpool_info[idx].u64VirtEnd));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64Phyaddr =%llx\n", idx,mpool_info[idx].u64Phyaddr));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].u64MpoolSize =%llx\n", idx,mpool_info[idx].u64MpoolSize));
	MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%d].s32V2Poff = %tx\n", idx,mpool_info[idx].s32V2Poff));
#endif

    MPOOL_MAPPING = 1;    
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);
    
    return TRUE;
}

MS_BOOL __attribute__((weak)) MsOS_MPool_Remove_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL ret = FALSE, mNonCache = MMAP_NONCACHE;
    MS_U32 i;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %llx, u64MapSize = %llx, u64VirtAddr = %tx\n", (unsigned long long)u64PhysAddr, (unsigned long long)u64MapSize, u64VirtAddr));

    if(bNonCache)
        mNonCache= MMAP_NONCACHE;
    else
        mNonCache= MMAP_CACHE;

    //multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);
    
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if((mpool_info[i].bIsUsed == false) || (mpool_info[i].bNonCache != mNonCache))
            continue;

        if((mpool_info[i].u64VirtStart == u64VirtAddr)
            && (mpool_info[i].u64Phyaddr == u64PhysAddr)
            && (mpool_info[i].u64MpoolSize == u64MapSize))
        {                
             ret = TRUE;
             memset(&mpool_info[i], 0, sizeof(mpool_info[i]));
             break;
        }
    }   

    pthread_mutex_unlock(&_MsOS_MPool_Mutex);
    return ret;
}

