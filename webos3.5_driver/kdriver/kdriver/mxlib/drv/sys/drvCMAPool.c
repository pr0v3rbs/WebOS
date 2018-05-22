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
/// file    drvCMAPool.c
/// @brief  CMA Pool Driver
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_LINUX)
#include<sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>
#include<fcntl.h>
#include <unistd.h>
#include <string.h>

#include "MsCommon.h"
#include "halCHIP.h"
#include "halMPool.h"
#include "mdrv_cma_pool_io.h"
#include "mdrv_cma_pool_st.h"
#include "drvCMAPool.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAX_CMAPOOLSIZE 10
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 pool_handle_id;
    MS_U32 VirtStart;
    MS_U32 Physaddr;
    MS_U32 CMAPoolSize;
    MS_BOOL bNonCache;
    MS_BOOL bIsUsed;
} CMAPOOL_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_S32              _s32FdCMAPool = -1;
static pthread_mutex_t  _CMA_POOL_Mutex = PTHREAD_MUTEX_INITIALIZER;
static CMAPOOL_INFO cpool_info[MAX_CMAPOOLSIZE];

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL  _findEmpty_CMA_Pool_Entry( int * index)
{
    MS_BOOL find = FALSE;
    int  i;

    *index = 0;
    for (i = 0; i < MAX_CMAPOOLSIZE; i++)
    {
        if(cpool_info[i].bIsUsed == FALSE)
        {
            find = TRUE;
            *index = i;
            break;
        }
    }

    if(find == FALSE)
        printf("Not enough CMAPool, must increase MAX_CMAPOOLSIZE!!\n");
    
    return find;
}


static MS_BOOL  _findPoolHandleId_InCMA_Pool_Table( MS_U32 pool_handle_id, int * index)
{
    MS_BOOL find = FALSE;
    int  i;

    *index = 0;
    for (i = 0; i < MAX_CMAPOOLSIZE; i++)
    {
        if((cpool_info[i].bIsUsed == TRUE) && (cpool_info[i].pool_handle_id == pool_handle_id))
        {
            find = TRUE;
            *index = i;
            break;
        }
    }
    
    return find;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// System initialzation
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Init(struct CMA_Pool_Init_Param * Init_Param)
{	
    struct cma_heap_info heap_info;
    MS_U32 u32PhyAddr = 0, u32AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_BOOL bNonCache, ret = TRUE;
    int idx = 0;
    char * ptr = NULL;

	memset(&heap_info, 0, sizeof(struct cma_heap_info));

    pthread_mutex_lock(&_CMA_POOL_Mutex); 
    if (_s32FdCMAPool <= 0)
    {
        if ((_s32FdCMAPool = open("/dev/cmapool", O_RDWR)) < 0)
        {
            printf("Open /dev/cmapool fail\n");
            ret =  FALSE;
            goto CMA_POOL_INIT_DONE;
        }

        memset(cpool_info, 0, sizeof(CMAPOOL_INFO)*MAX_CMAPOOLSIZE);
    }
    
    heap_info.heap_id = Init_Param->heap_id;
    heap_info.flags = Init_Param->flags;
    
    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_INIT, &heap_info))
    {
        printf("init  heap_id %d fail\n", heap_info.heap_id);
        ret =  FALSE;
        goto CMA_POOL_INIT_DONE;
    }

    Init_Param->pool_handle_id = heap_info.heap_id;
    Init_Param->miu = heap_info.miu;											// the miu this heap located at
    Init_Param->heap_miu_start_offset = heap_info.heap_miu_start_offset;		// heap_start_offset(pa), with respect to the miu
    Init_Param->heap_length = heap_info.heap_length;

    //not need virtual address for this pool
    if(!(Init_Param->flags & CMA_FLAG_MAP_VMA))
    {
         ret = TRUE;
         goto CMA_POOL_INIT_DONE;
    }

    //avoid mmap more than one time
    ret = _findPoolHandleId_InCMA_Pool_Table(Init_Param->pool_handle_id, &idx);
    if(ret == TRUE)
    {
         printf("heap_id %lu already init!\n", Init_Param->pool_handle_id);
         goto CMA_POOL_INIT_DONE;
    }

    ret = _findEmpty_CMA_Pool_Entry(&idx);
    if(ret == FALSE)
    {
        printf("heap_id %lu init fail!\n", Init_Param->pool_handle_id);
        goto CMA_POOL_INIT_DONE;
    }

	/* we reserve all heap_length in va at beginning(keep vma, if CMA_FLAG_MAP_VMA)!!
	 * really mapping will be done while MApi_CMA_Pool_GetMem
	 * also, we will defined this reserved va as cached/noncached here
	 * and then, you can see the reserved vma @ /proc/pid/maps
	 */
    ptr = mmap(NULL, Init_Param->heap_length, PROT_READ|PROT_WRITE, MAP_SHARED, _s32FdCMAPool, 0);
    
    if(ptr == (char*)(-1))
    {
         printf("pool id %lu, size %lu mmap failed\n", Init_Param->pool_handle_id, Init_Param->heap_length);
   	  ret =  FALSE;
         goto CMA_POOL_INIT_DONE;
    }

    u32PhyAddr = HAL_MsOS_MPool_BA2PA(heap_info.bus_addr);   // get pa
    _phy_to_miu_offset(u8MiuSel, u32AddrOffset, u32PhyAddr);   // get miu

    if(Init_Param->flags & CMA_FLAG_CACHED)
        bNonCache = FALSE;
    else
        bNonCache = TRUE;
    ret = MsOS_MPool_Add_PA2VARange(u32PhyAddr, (MS_U32)ptr, Init_Param->heap_length, bNonCache);

    if(ret)
    {
        /* CMA Pool setting*/
        cpool_info[idx].pool_handle_id = Init_Param->pool_handle_id;
        cpool_info[idx].VirtStart = (MS_U32)ptr;
        cpool_info[idx].Physaddr = u32PhyAddr;
        cpool_info[idx].CMAPoolSize = Init_Param->heap_length;
        cpool_info[idx].bNonCache = bNonCache;
        cpool_info[idx].bIsUsed = TRUE;        
    }

CMA_POOL_INIT_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);  
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_GetMem(struct CMA_Pool_Alloc_Param * alloc_param)
{
    struct cma_alloc_args alloc_args;
    MS_BOOL ret = TRUE;

	memset(&alloc_args, 0, sizeof(struct cma_alloc_args));

    pthread_mutex_lock(&_CMA_POOL_Mutex);  
    if (_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }

    alloc_args.heap_id = alloc_param->pool_handle_id;
    alloc_args.align = 4096;
    alloc_args.offset_in_heap = alloc_param->offset_in_pool;
    alloc_args.length = alloc_param->length;
    alloc_args.flags = alloc_param->flags;

    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_ALLOC, &alloc_args))
    {  
        printf("cma pool get memory fail: heap id %u, offset %lu, size %lu, flags %08x\n", alloc_args.heap_id,  
            alloc_args.offset_in_heap, alloc_args.length, alloc_args.flags);
        ret = FALSE;
        goto CMA_POOL_GETMEM_DONE;
    }

    if(alloc_args.flags & CMA_FLAG_VIRT_ADDR)
    {
        alloc_param->virt_addr = alloc_args.cpu_addr;
		printf("\033[35mFunction = %s, Line = %d, alloc_param->virt_addr is 0x%lX\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long)alloc_param->virt_addr);
	}

CMA_POOL_GETMEM_DONE:
   pthread_mutex_unlock(&_CMA_POOL_Mutex);
   
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_PutMem(struct CMA_Pool_Free_Param * free_param)
{
    struct cma_free_args free_args;
    MS_BOOL ret = TRUE;

	memset(&free_args, 0, sizeof(struct cma_free_args));

    pthread_mutex_lock(&_CMA_POOL_Mutex);
    if (_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }
         
    free_args.heap_id = free_param->pool_handle_id;
    free_args.offset_in_heap = free_param->offset_in_pool;
    free_args.length = free_param->length;
    if(ioctl(_s32FdCMAPool, CMA_POOL_IOC_FREE, &free_args))
    {        
        printf("cma pool put memory fail: heap id %u, offset %lu, size %lu\n", free_args.heap_id,  
            free_args.offset_in_heap, free_args.length);
        ret = FALSE;
        goto CMA_POOL_PUTMEM_DONE;
    }

CMA_POOL_PUTMEM_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);
    return ret;
}

MS_BOOL __attribute__((weak)) MApi_CMA_Pool_Release(MS_U32 pool_handle_id)
{
    MS_U32  heap_id = pool_handle_id;
    MS_BOOL ret = TRUE;
    int idx = 0;

    pthread_mutex_lock(&_CMA_POOL_Mutex);
    if (_s32FdCMAPool < 0)
    {
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }

    if (ioctl(_s32FdCMAPool, CMA_POOL_IOC_RELEASE, &heap_id))
    {        
        printf("cma pool release fail: pool id %lu\n", pool_handle_id);            
        ret = FALSE;
        goto CMA_POOL_RELEASE_DONE;
    }

    ret = _findPoolHandleId_InCMA_Pool_Table(pool_handle_id, &idx);
    if(ret == TRUE)
    {
        MsOS_MPool_Remove_PA2VARange(cpool_info[idx].Physaddr, cpool_info[idx].VirtStart, 
                cpool_info[idx].CMAPoolSize, cpool_info[idx].bNonCache);              
        munmap((void *)(cpool_info[idx].VirtStart), cpool_info[idx].CMAPoolSize);
        cpool_info[idx].bIsUsed = FALSE;
        cpool_info[idx].pool_handle_id = 0;         
    }
    ret = TRUE; //not find the pool hand id

CMA_POOL_RELEASE_DONE:
    pthread_mutex_unlock(&_CMA_POOL_Mutex);   
    return ret;    
}
#endif