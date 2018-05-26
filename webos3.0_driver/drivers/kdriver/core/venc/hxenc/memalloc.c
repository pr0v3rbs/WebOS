/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 2013 by LG Electronics Inc.
 
This program is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
GNU General Public License for more details.
*/ 

/** @file
 *
 *  hal api implementation for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2014.05.15
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
//#include <asm/io.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
//#include <linux/ioport.h>
#include <linux/list.h>

#include "os_util.h"
#include "venc_drv.h"
#include "venc_cfg.h"
#include "hma_alloc.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#ifndef MROUND
#define MROUND( number, multiple ) ((((number) / (multiple)) + (((number) % (multiple)) ? 1 : 0) ) * (multiple) )
#endif
#ifndef MIN	
#define MIN( a, b ) 	( (a) <= (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX( a, b )		( (a) >= (b) ? (a) : (b) )
#endif

#define _MB_(_sz)   (( _sz)>>20)
#define _KB_(_sz)   ((_sz)>1024)?(((_sz)>>10)&0x3ff):((_sz)?1:0)

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32  base;
	UINT32  size;
	UINT32	request;

	char	name[9];

	struct list_head list;
} MEMALLOC_LIST_T;

typedef struct {
	char * const		pool_name;
	struct list_head * 	mem_list;
	spinlock_t			spinlock;

	UINT32				base;
	UINT32				size;
} MEMALLOC_STATUS_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/

static MEMALLOC_STATUS_T memalloc_status[2] = 
{
	[0] = {
		.pool_name = "venc0",
	},

	[1] = {
		.pool_name = "venc1",
	},
};

/*========================================================================================
	Implementation Group
========================================================================================*/

int MemallocOpen( UINT32 id, UINT32 base, UINT32 size )
{
	if ( id > gpstVencConfig->num_device )
	{
		VENC_ERROR("Can't open #%d\n", id);
		return RET_ERROR;
	}

	if ( hma_pool_register( memalloc_status[id].pool_name, base, size ) != RET_OK )
	{
		VENC_ERROR("Can't register hma pool\n");
		return RET_ERROR;
	}

	VENC_INFO("%s :: [%d] base=0x%08X size=0x%08X\n", __FUNCTION__, id, base, size);

	memalloc_status[id].mem_list = kmalloc(sizeof(struct list_head), GFP_KERNEL);
	INIT_LIST_HEAD(memalloc_status[id].mem_list);

	memalloc_status[id].base = base;
	memalloc_status[id].size = size;

	return RET_OK;
}
EXPORT_SYMBOL(MemallocOpen);

int MemallocClose( UINT32 id )
{
	if ( id > gpstVencConfig->num_device )
	{
		VENC_ERROR("Can't open #%d\n", id);
		return RET_ERROR;
	}

	hma_pool_unregister( memalloc_status[id].pool_name );

	if ( !list_empty(memalloc_status[id].mem_list) )
	{
		//_ListClear(memalloc_status[id].mem_list);
		VENC_WARN("%s :: [%d] list is not empty\n", __FUNCTION__, id);
	}

	if ( memalloc_status[id].mem_list != NULL )
	{
		kfree( memalloc_status[id].mem_list );
		memalloc_status[id].mem_list = NULL;
	}

	memalloc_status[id].base = 0;
	memalloc_status[id].size = 0;

	return RET_OK;
}
EXPORT_SYMBOL(MemallocClose);

int MemallocGetBuffer( UINT32 id, UINT32 size, UINT32 *pAddr, char *name )
{
	UINT32 addr = 0;

	if ( id > gpstVencConfig->num_device )
	{
		VENC_ERROR("Can't open #%d\n", id);
		return RET_ERROR;
	}

	if ( pAddr == NULL )
	{
		return RET_INVALID_PARAMS;
	}

	//addr = hma_alloc( memalloc_status[id].pool_name, MROUND(size, 0x1000), 0x1000 ); // 12bit aligned (4Kbyte)
	addr = hma_alloc_user( memalloc_status[id].pool_name, MROUND(size, 0x1000), 0x1000, name ); // 12bit aligned (4Kbyte)

	if ( addr == 0 )
		return RET_ERROR;

	*pAddr = addr;

	{
		MEMALLOC_LIST_T *item = NULL;

		item = kmalloc(sizeof(MEMALLOC_LIST_T), GFP_KERNEL);
		memset(item, 0x0, sizeof(MEMALLOC_LIST_T));

		item->base = addr;
		item->size = MROUND( size, 0x1000 );
		item->request = size;

		if ( name != NULL )
		{
			strncpy(item->name, name, 8);
		}

		list_add_tail(&item->list, memalloc_status[id].mem_list);
	}

	return RET_OK;
}
EXPORT_SYMBOL(MemallocGetBuffer);

int MemallocFreeBuffer( UINT32 id, UINT32 addr )
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	
	if ( id > gpstVencConfig->num_device )
	{
		VENC_ERROR("Can't open #%d\n", id);
		return RET_ERROR;
	}

	hma_free( memalloc_status[id].pool_name, addr );

	list_for_each_safe( pos, q, memalloc_status[id].mem_list )
	{
		MEMALLOC_LIST_T *cur = NULL;

		cur = list_entry(pos, MEMALLOC_LIST_T, list);

		if ( cur->base == addr )
		{
			list_del( pos );
			kfree( cur );
		}
	}

	return RET_OK;
}
EXPORT_SYMBOL(MemallocFreeBuffer);

int MemallocReadStatus( char * buffer )
{
	int len = 0;
	int i = 0;

	for ( i = 0; i < gpstVencConfig->num_device; i++ )
	{
		int index = 0;
		struct list_head *pos = NULL;
		struct list_head *list = NULL;

		UINT32 used_base = ~0;
		UINT32 used_end  = 0;
		UINT32 used_size = 0;
		UINT32 used_request = 0;

		UINT32 free_base = 0;
		UINT32 free_size = 0;
		UINT32 free_end = 0;

		list = memalloc_status[i].mem_list;

		if ( list == NULL )
		{
			continue;
		}

		len += sprintf(buffer + len, "MEMALLOC STATUS #%d\n", i);
		len += sprintf(buffer + len, "    --------------------------------------------------------------------------------\n");
		len += sprintf(buffer + len, "    ID,     NAME, Alloc Base ~ Alloc End  [Alloc Size] (Alloc Size MB), Request Size\n");
		len += sprintf(buffer + len, "    --------------------------------------------------------------------------------\n");

#if 1
		list_for_each( pos, list )
		{
			MEMALLOC_LIST_T *cur = NULL;

			cur = list_entry( pos, MEMALLOC_LIST_T, list );

			if ( cur != NULL )
			{
				UINT32 start = cur->base;
				UINT32 end = cur->base + cur->size;
				UINT32 size = cur->size;
				UINT32 req = cur->request;

				used_base = MIN( used_base, start );
				used_end  = MAX( used_end , end   );
				used_size += size;
				used_request += req;

				len += sprintf( buffer+len, "    %2d, %8s, 0x%08X ~ 0x%08X [0x%08X] (%4dMb+%4dkB),   0x%08X\n",
							index, cur->name, start, end, size, _MB_(size), _KB_(size), req);
			}

			index++;
		}
#endif

		free_base = memalloc_status[i].base + used_size;
		free_size = memalloc_status[i].size - used_size;
		free_end = memalloc_status[i].base + memalloc_status[i].size;

		len += sprintf(buffer + len, "    --------------------------------------------------------------------------------\n");
		len += sprintf( buffer+len, "        %8s, 0x%08X ~ 0x%08X [0x%08X] (%4dMb+%4dkB),   0x%08X\n",
					"USED", used_base, used_end, used_size, _MB_(used_size), _KB_(used_size), used_request);
		len += sprintf( buffer+len, "        %8s, 0x%08X ~ 0x%08X [0x%08X] (%4dMb+%4dkB),   0x%08X\n",
					"FREE", free_base, free_end, free_size, _MB_(free_size), _KB_(free_size), free_size);
		len += sprintf(buffer + len, "    --------------------------------------------------------------------------------\n");
		len += sprintf(buffer + len, "\n");

	}

	return len;
}

/** @} */


