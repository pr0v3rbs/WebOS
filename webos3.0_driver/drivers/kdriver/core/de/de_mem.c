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
 *	de mem.
 *
 *	author		
 *	version		1.0
 *	date		
 *	note		for exporting de frame memory 
 *
 *	@addtogroup 
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define _SUPPORT_ION_

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#ifdef _SUPPORT_ION_
#include <linux/ion.h>
#endif
#include <linux/slab.h>

#include "os_util.h"
#include "base_device.h"

#include "de_mem.h"
#include "de_def.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern struct ion_client *lg115x_ion_client_create(    \
								unsigned int heap_mask,\
								const char *name);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions  
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
struct mem_client {
	unsigned int addr_phys;
	unsigned int size;
	unsigned int stride;
};

struct mem_handle {
	int *p;
};

/*  s_mem_handle indicates the memory pool which is in ion-management  */
struct s_mem_handle {
#ifdef _SUPPORT_ION_
	struct ion_client *m_ion_client;
#else
	struct mem_client *m_mem_client;
#endif
	struct hlist_head ion_hlist_head;
};

struct mem_struct {
#ifdef _SUPPORT_ION_
	struct ion_handle *ion_handle;
#else
	struct mem_handle *mem_handle;
#endif
	int          share_fd;
	unsigned int frame_address;
	unsigned int frame_size;
};

int g_shared_fd = -1;

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/
struct mem_client *lg115x_mem_client_create(int index, char *name)
{
	struct mem_client *client = NULL;
	return client;
}

struct mem_handle *mem_alloc(struct mem_client *m_mem_client, size_t len, size_t align,\
							unsigned int heap_id_mask, unsigned int flag)
{
	struct mem_handle *handle = NULL;
	return handle;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief init  
 */
struct s_mem_handle *de_mem_open(void)
{
	struct s_mem_handle *mem_handle = (struct s_mem_handle *)kmalloc(\
										sizeof(struct s_mem_handle), \
										GFP_KERNEL);

	if(mem_handle == NULL) return NULL;
#ifdef _SUPPORT_ION_
	mem_handle->m_ion_client = lg115x_ion_client_create(0, "DE");
	if(mem_handle->m_ion_client == NULL)
#else
//	mem_handle->m_mem_client = lg115x_mem_client_create(0, "DE");
//	if(mem_handle->m_mem_client == NULL)
	if(0)
#endif
	{
		DE_ERROR("mem create fail.\n");
		kfree(mem_handle);
		return NULL;
	}

	return mem_handle;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief open  
 */
void de_mem_close(struct s_mem_handle *mem_handle)
{
	if(mem_handle == NULL) return;

	if(1)
	{
#ifdef _SUPPORT_ION_
		ion_client_destroy(mem_handle->m_ion_client);
#else
//		mem_client_destroy(mem_handle->m_mem_client);
#endif
		kfree(mem_handle);
	}
}

#ifdef _SUPPORT_ION_
static unsigned int _mem_alloc(	struct s_mem_handle *mem_handle, \
							unsigned int size)
{
	unsigned int heap_mask = 0;
	unsigned int flags     = 0;
	size_t       align     = 0;
	UINT32 frame_address = 0;
	struct mem_struct *mem_struct = (struct mem_struct *)kmalloc(  \
										sizeof(struct mem_struct), \
										GFP_KERNEL);

	if(mem_struct == NULL)
	{
		DE_ERROR("mem_struct kmalloc fail\n");
		return 0;
	}

#ifdef _SUPPORT_ION_
	heap_mask = ION_HEAP_SYSTEM_CONTIG_MASK | ION_HEAP_CARVEOUT_MASK;

	mem_struct->ion_handle = ion_alloc(mem_handle->m_ion_client, \
								size, align, heap_mask, flags);

	ion_phys(mem_handle->m_ion_client, mem_struct->ion_handle,   \
			(ion_phys_addr_t *)&mem_struct->frame_address,       \
			&mem_struct->frame_size);
#else
	heap_mask = 0;
	flags = 0;
	align = 0;
	mem_struct->mem_handle = mem_alloc(mem_handle->m_mem_client, \
								size, align, heap_mask, flags);
#endif
	frame_address = mem_struct->frame_address;
	DE_PRINT(" done.  (frame_adress = 0x%08x, size = 0x%x)\n",\
					mem_struct->frame_address,\
					mem_struct->frame_size);
	kfree(mem_struct);
	return frame_address;
}
#endif

BOOLEAN _phys_get(	struct s_mem_handle *mem_handle, \
						SINT32 frame_fd,                 \
						UINT32 *p_frame_addr,            \
						UINT32 *p_frame_size)
{
	struct mem_struct *mem_struct = (struct mem_struct *)kmalloc( \
								sizeof(struct mem_struct),\
								GFP_KERNEL);
	
	if(mem_struct == NULL) return FALSE;

#ifdef _SUPPORT_ION_
	mem_struct->ion_handle = ion_import_dma_buf(mem_handle->m_ion_client, frame_fd);
	if(mem_struct->ion_handle == NULL)
	{
		DE_ERROR("fail to get ion handle\n");
		kfree(mem_struct);
		return FALSE;
	}
	mem_struct->share_fd = frame_fd;
	ion_phys(	mem_handle->m_ion_client,                      \
				mem_struct->ion_handle,                        \
				(ion_phys_addr_t *)&mem_struct->frame_address, \
				&mem_struct->frame_size);
#else

#endif
	*p_frame_addr = mem_struct->frame_address;
	*p_frame_size = mem_struct->frame_size;
	g_shared_fd   = frame_fd;

	kfree(mem_struct);
	DE_PRINT("phys frame addr 0x%08x , size 0x%x\n", \
					*p_frame_addr,\
					*p_frame_size);

	return TRUE;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief request frame buffer & get physical address with fd for sharing
 */
int de_mem_init(void)
{
#ifndef _SUPPORT_ION_
	return 0;
#else
	struct s_mem_handle *handle;
	UINT32 frame_addr = 0;
	UINT32 frame_size = 0;
	SINT32 frame_fd   = 0;

	handle = de_mem_open();

	if(_mem_alloc(handle, 114 * 1024 * 1024) == 0)
	{
		DE_ERROR("ion malloc fail.\n");
	}
	if(_phys_get(handle, frame_fd, &frame_addr, &frame_size) == FALSE)
	{
		DE_ERROR("ion malloc fail.\n");
	}
	
	DE_PRINT("de_mem_init done\n");

	return 0;
#endif
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief get file descriptor for frame buffer   
 */
int de_mem_fd(void)
{
	return g_shared_fd;
}

