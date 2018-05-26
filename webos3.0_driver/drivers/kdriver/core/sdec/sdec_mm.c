/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "sdec_impl.h"

#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_mm.h"

#include "hma_alloc.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define USE_SDEC_HMA_MEMORY
#define SDEC_HMA_POOL_NAME			"sdec"
#define SDEC_CIPLUS_HMA_POOL_NAME	"sdec_ciplus"


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

typedef struct
{
	UINT32		inited;
	UINT32		start;
	UINT32		end;

#ifndef USE_SDEC_HMA_MEMORY
	OS_RGN_T	rgn;
#endif

	OS_SEM_T	mutex;
} SDEC_MM_DEV_T;


typedef struct
{
	SDEC_MM_DEV_T	dev[SDEC_MM_TYPE_MAX];
} SDEC_MM_CTRL_T;


typedef struct SDEC_MEM_CHUNK
{
	void *ptr;
	struct SDEC_MEM_CHUNK *next;
} SDEC_MEM_CHUNK_T;


typedef struct SDEC_MM_CTX
{
	SDEC_MEM_CHUNK_T *head[SDEC_MM_TYPE_MAX];
} SDEC_MM_CTX_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static const char* hma_pool_name[SDEC_MM_TYPE_MAX] =
{
	SDEC_HMA_POOL_NAME,
	SDEC_CIPLUS_HMA_POOL_NAME
};

static SDEC_MM_CTRL_T	*mm_ctrl;


/**
********************************************************************************
* @brief
*   initialize memory pool manager.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  DTV_STATUS_T
********************************************************************************
*/
int SDEC_MM_Init(void)
{
	int 				ret;
	LX_MEMCFG_T 		*cfg;
	int					i;
	SDEC_MM_DEV_T		*mm_dev;

	SDEC_CHECK_ERROR( mm_ctrl != NULL, return RET_ERROR, "sdec mem is alreay inited" );

	mm_ctrl = (SDEC_MM_CTRL_T*)kmalloc(sizeof(SDEC_MM_CTRL_T), GFP_KERNEL);
	SDEC_CHECK_ERROR(mm_ctrl == NULL, return RET_ERROR, "kmalloc fail");
	memset(mm_ctrl, 0, sizeof(SDEC_MM_CTRL_T));

	for(i=0; i<SDEC_MM_TYPE_MAX; i++)
	{
		mm_dev = &mm_ctrl->dev[i];

		/* get config */
		if(i == SDEC_MM_TYPE_NORMAL)
		{
			cfg = SDEC_CFG_GetMemConfig();
		}
		else
		{
			cfg = SDEC_CFG_GetCiPlusMemConfig();
		}

		if(cfg->size > 0)
		{
#ifdef USE_SDEC_HMA_MEMORY
			ret = hma_pool_register(hma_pool_name[i], cfg->base, cfg->size);
#else
			ret = OS_InitRegion(&mm_dev->rgn, (void *)cfg->base, cfg->size);
#endif

			if(ret == 0)
			{
				mm_dev->start	= cfg->base;
				mm_dev->end		= cfg->base + cfg->size;

				OS_InitMutex(&mm_dev->mutex, OS_SEM_ATTR_DEFAULT);
				mm_dev->inited	= 1;
			}
			else
			{
				SDEC_PRINT_ERROR("can't init sdec memory[%d]", i);
			}
		}
	}

	return RET_OK;
}


int SDEC_MM_GetMemInfo(SDEC_MM_TYPE_T type, LX_SDEC_MEM_INFO_T *info)
{
	SDEC_MM_DEV_T *mm_dev;

	SDEC_CHECK_ERROR( info == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	SDEC_CHECK_ERROR( mm_ctrl == NULL, return RET_ERROR, "sdec mem is not ready" );
	SDEC_CHECK_ERROR(type >= SDEC_MM_TYPE_MAX, return RET_ERROR, "invalid type[%d]", type);

	mm_dev = &mm_ctrl->dev[type];
	SDEC_CHECK_ERROR(!mm_dev->inited, return RET_ERROR, "not inited dev[%d]", type);

	info->addr = mm_dev->start;
	info->size = mm_dev->end - mm_dev->start;

	return RET_OK;
}

/**
********************************************************************************
* @brief
*   Finalize memory pool manager.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  DTV_STATUS_T
********************************************************************************
*/
int SDEC_MM_Final(void)
{
	int	ret	= RET_OK;
	int i;
	SDEC_MM_DEV_T *mm_dev;

	SDEC_CHECK_ERROR( mm_ctrl == NULL, return RET_ERROR, "sdec mem is not ready" );

	for(i=0; i<SDEC_MM_TYPE_MAX; i++)
	{
		mm_dev = &mm_ctrl->dev[i];
		if(mm_dev->inited)
		{
#ifdef USE_SDEC_HMA_MEMORY
			hma_pool_unregister(hma_pool_name[i]);
#else
			OS_CleanupRegion(&mm_dev->rgn);
#endif
		}
	}

	kfree(mm_ctrl);
	mm_ctrl = NULL;

	return ret;
}


SDEC_MM_CTX_T* SDEC_MM_Open(void)
{
	SDEC_MM_CTX_T *ctx;

	SDEC_CHECK_ERROR(mm_ctrl == NULL, return NULL, "sdec mem is not ready");

	ctx = (SDEC_MM_CTX_T*)kmalloc(sizeof(SDEC_MM_CTX_T), GFP_KERNEL);
	SDEC_CHECK_ERROR(ctx == NULL, return NULL, "kmalloc fail");
	memset(ctx, 0, sizeof(SDEC_MM_CTX_T));

	return ctx;
}




/**
********************************************************************************
* @brief
*   allocate memory
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
* @return
*  DTV_STATUS_T
********************************************************************************
*/
void* SDEC_MM_Alloc(SDEC_MM_CTX_T* h, SDEC_MM_TYPE_T type, UINT32 size)
{
	void *ptr;
	SDEC_MEM_CHUNK_T *chunk = NULL;
	int rc;
	SDEC_MM_DEV_T *mm_dev;

	SDEC_CHECK_ERROR( mm_ctrl == NULL, return NULL, "sdec mem is not ready" );
	SDEC_CHECK_ERROR( h == NULL || size == 0, return NULL, "invalid param");
	SDEC_CHECK_ERROR(type >= SDEC_MM_TYPE_MAX, return NULL, "invalid type[%d]", type);

	mm_dev = &mm_ctrl->dev[type];
	SDEC_CHECK_ERROR(!mm_dev->inited, return NULL, "not inited dev[%d]", type);

	chunk = (SDEC_MEM_CHUNK_T*)kmalloc(sizeof(SDEC_MEM_CHUNK_T), GFP_KERNEL);
	SDEC_CHECK_ERROR( chunk == NULL, return NULL, "kmalloc fail");

	rc = OS_LockMutex(&mm_dev->mutex);
	SDEC_CHECK_ERROR( rc != 0, goto alloc_fail, "lock fail");

	SDEC_PRINT_MM("ALLOC[%d] - size=%d", type, size);

#ifdef USE_SDEC_HMA_MEMORY
	ptr = (void*)hma_alloc(hma_pool_name[type], (int)size + 0x1000, 1024);
#else
	ptr = OS_MallocRegion(&mm_dev->rgn, size);
#endif
	SDEC_CHECK_ERROR( ptr == NULL, goto error, "alloc fail");

	SDEC_PRINT_MM("ALLOCATED - ptr=%p", ptr);
	chunk->ptr = ptr;
	chunk->next = h->head[type];
	h->head[type] = chunk;
	OS_UnlockMutex(&mm_dev->mutex);

	return ptr;

error:
	OS_UnlockMutex(&mm_dev->mutex);
alloc_fail:
	if(chunk) kfree(chunk);
	return NULL;
}



static inline int _SDEC_MM_Free(SDEC_MM_TYPE_T type, void *ptr)
{
	int ret = 0;

#ifdef USE_SDEC_HMA_MEMORY
	hma_free(hma_pool_name[type], (unsigned long)ptr);
#else
	ret = OS_FreeRegion(&mm_dev->rgn, ptr);
#endif

	return ret;
}

/**
********************************************************************************
* @brief
*   Free allocated memory
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
* @return
*  DTV_STATUS_T
********************************************************************************
*/
int SDEC_MM_Free(SDEC_MM_CTX_T* h, SDEC_MM_TYPE_T type, void* ptr)
{
	SDEC_MEM_CHUNK_T *chunk;
	SDEC_MEM_CHUNK_T *pre = NULL;
	int rc;
	int found = 0;
	SDEC_MM_DEV_T *mm_dev;

	SDEC_CHECK_ERROR( mm_ctrl == NULL, return RET_ERROR, "sdec mem is not ready" );
	SDEC_CHECK_ERROR( h == NULL || ptr == NULL, return RET_INVALID_PARAMS, "invalid param");
	SDEC_CHECK_ERROR(type >= SDEC_MM_TYPE_MAX, return RET_INVALID_PARAMS, "invalid type[%d]", type);

	mm_dev = &mm_ctrl->dev[type];
	SDEC_CHECK_ERROR(!mm_dev->inited, return RET_ERROR, "not inited dev[%d]", type);

	rc = OS_LockMutex(&mm_dev->mutex);
	SDEC_CHECK_ERROR( rc != 0, return RET_ERROR, "lock fail");

	chunk = h->head[type];
	while(chunk != NULL)
	{
		if(chunk->ptr == ptr)
		{
			found = 1;
			break;
		}
		pre = chunk;
		chunk = chunk->next;
	}
	SDEC_CHECK_ERROR(found == 0, goto error, "invalid ptr:%p", ptr);
	SDEC_PRINT_MM("SDEC FREE - ptr=%p", ptr);

	if(pre) pre->next = chunk->next;
	else h->head[type] = chunk->next;
	kfree(chunk);

#ifdef USE_SDEC_HMA_MEMORY
	hma_free(hma_pool_name[type], (unsigned long)ptr);
#else
	rc = OS_FreeRegion(&mm_dev->rgn, ptr);
#endif
	OS_UnlockMutex(&mm_dev->mutex);
	return rc;

error:
	OS_UnlockMutex(&mm_dev->mutex);
	return RET_ERROR;
}



void SDEC_MM_Close(SDEC_MM_CTX_T* h)
{
	SDEC_MEM_CHUNK_T *chunk, *nchunk;
	SDEC_MM_TYPE_T type;

	SDEC_CHECK_ERROR( mm_ctrl == NULL, return, "sdec mem is not ready" );
	SDEC_CHECK_ERROR( h == NULL, return, "null parameter");

	for(type=0; type<SDEC_MM_TYPE_MAX; type++)
	{
		chunk = h->head[type];
		while(chunk != NULL)
		{
			SDEC_PRINT_MM("FREE:%p", chunk->ptr);
			_SDEC_MM_Free(type, chunk->ptr);
			nchunk = chunk->next;
			kfree(chunk);
			chunk = nchunk;
		}
	}
}


/**
********************************************************************************
* @brief
* 	get memory statistics for sdec memory
*	application can use this information to monitor memroy usage.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
* @return
*  DTV_STATUS_T
********************************************************************************
*/
int SDEC_MM_GetMemStat (SDEC_MM_TYPE_T type, LX_SDEC_MM_STAT_T *stat)
{
#ifdef USE_SDEC_HMA_MEMORY
	// TODO: Can't get the mem state from hma
	return RET_ERROR;
#else
	OS_RGN_INFO_T	mem_info;
	SDEC_MM_DEV_T	*mm_dev;

	SDEC_CHECK_ERROR( mm_ctrl == NULL, return RET_INVALID_PARAMS, "sdec region is not ready" );
	SDEC_CHECK_ERROR( stat == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	SDEC_CHECK_ERROR(type >= SDEC_MM_TYPE_MAX, return NULL, "invalid type[%d]", type);

	mm_dev = &mm_ctrl->dev[type];
	SDEC_CHECK_ERROR(!mm_dev->inited, return NULL, "not inited dev[%d]", type);

	OS_GetRegionInfo(&mm_dev->rgn, &mem_info);

	stat->mem_base			= mem_info.phys_mem_addr;
	stat->mem_length 		= mem_info.length;
	stat->mem_alloc_size 	= mem_info.mem_alloc_size;
	stat->mem_free_size 	= mem_info.mem_free_size;

	return OK;
#endif

}

