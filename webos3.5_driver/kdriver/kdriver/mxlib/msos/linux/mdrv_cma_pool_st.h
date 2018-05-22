////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   cma_mpool_manager.h
/// @brief  CMA mpool Manager interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CMA_POOL_ST_H__
#define __DRV_CMA_POOL_ST_H__
#define KERN_CHUNK_NUM 3

struct cma_alloc_args {
    MS_U64 offset_in_heap;
    MS_VIRT cpu_addr;
    MS_SIZE length;
    MS_U64 align;
    MS_U32 heap_id;
    MS_U32 flags;
};

struct cma_free_args {
    MS_U32 heap_id;
    MS_U64 offset_in_heap;
    MS_SIZE length;
};

struct cma_heap_info {
    MS_U32 heap_id;
    MS_VIRT flags;

    MS_U32 miu;
    MS_U64 bus_addr;
    MS_U64 heap_miu_start_offset;
    MS_SIZE heap_length;
    MS_VIRT virt_addr;//out:if need,shoud give this out put
};

#endif
