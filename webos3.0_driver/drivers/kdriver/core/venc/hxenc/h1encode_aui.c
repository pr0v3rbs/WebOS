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
 *  stream infomation(AUI) api implementation for venc device of H13.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2012.06.27
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
#include <linux/slab.h>

#include "venc_drv.h"
#include "h1encode.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
//#define ALIGNMENT_SIZE	32

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#ifndef MIN
#define MIN( a, b ) 	( (a) <= (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX( a, b )		( (a) >= (b) ? (a) : (b) )
#endif

//LIST_HEAD( streamList );

#ifdef H1ENCODE_DEBUG_STREAM_LIST
#define DEBUG_PRINT(...)		VENC_DEBUG("[STREAM_INFO] " __VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef struct {
	u32 u32StreamBase;
	u32 u32StreamSize;

	struct list_head infoList;

} H1ENCODE_STREAM_LIST_T;

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
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
#ifdef H1ENCODE_DEBUG_MEM_LEAK
atomic_t aui_count = ATOMIC_INIT(0);
#endif

#if 0
DEFINE_SPINLOCK(aui_lock);
#define LOCK()				spin_lock(&aui_lock);
#define UNLOCK()			spin_unlock(&aui_lock);
#else
#define LOCK()
#define UNLOCK()
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 *
 * Clear stream infomation list
 *
 * @param	struct list_head *
 * @return	void
 *
 */
void _StreamInfoClear( struct list_head *pList )
{
	struct list_head *pos;
	struct list_head *q;

	if ( pList == NULL )
	{
		return;
	}

	list_for_each_safe( pos, q, pList )
	{
		H1ENCODE_STREAM_LIST_T *stream;

		stream = list_entry( pos, H1ENCODE_STREAM_LIST_T, infoList );
		list_del( pos );

		if ( stream != NULL )
		{
			kfree( stream );
		}
	}
}

/**
 *
 * Insert a new stream information into the list
 *
 * @param	struct list_head *, u32, u32
 * @return	void
 *
 */
void _StreamInfoPush( struct list_head *pList, u32 streamBase, u32 streamSize )
{
	H1ENCODE_STREAM_LIST_T *stream;

	if ( pList == NULL )
	{
		return;
	}

	LOCK();

	stream = (H1ENCODE_STREAM_LIST_T *) kmalloc( sizeof(H1ENCODE_STREAM_LIST_T), GFP_ATOMIC );

	if ( stream == NULL )
	{
		VENC_ERROR("Can't alloc kernel memory\n");
		goto func_exit;
		//stream = (H1ENCODE_STREAM_LIST_T *) kmalloc( sizeof(H1ENCODE_STREAM_LIST_T), GFP_KERNEL );
	}

	stream->u32StreamBase = streamBase;
	stream->u32StreamSize = streamSize;

	//list_add( &stream->infoList, pList );
	list_add_tail( &stream->infoList, pList );
func_exit:
	UNLOCK();
}

/**
 *
 * Get total size of stream infomation list
 *
 * @param	struct list_head *, u32 *, u32 *, u32 *
 * @return	void
 *
 */
void _StreamInfoFlush( struct list_head *pList,  u32 *pu32StreamStart, u32 *pu32StreamEnd, u32 *pu32StreamSize )
{
	struct list_head *pos;
	H1ENCODE_STREAM_LIST_T *stream;

	u32 streamSt = ~0;
	u32 streamEd = 0;
	u32 streamTotal = 0;

	if ( pList == NULL )
	{
		return;
	}

	DEBUG_PRINT("============================================\n");

	LOCK();

	list_for_each( pos, pList )
	{
		u32 st, ed;

		stream = list_entry( pos, H1ENCODE_STREAM_LIST_T, infoList );

		st = stream->u32StreamBase;
		ed = stream->u32StreamBase + stream->u32StreamSize;

		streamTotal += stream->u32StreamSize;

		streamSt = MIN( streamSt, st );
		streamEd = MAX( streamEd, ed );

		DEBUG_PRINT("[ITEM] 0x%08x -- 0x%08x (0x%08x)\n", st, ed, stream->u32StreamSize );
	}

	DEBUG_PRINT("--------------------------------------------\n");
	DEBUG_PRINT("[TOTAL] 0x%08x -- 0x%08x (0x%08x)\n", streamSt, streamEd, streamTotal );

	*pu32StreamStart = streamSt;
	*pu32StreamEnd = streamEd;
	*pu32StreamSize = streamTotal;

	_StreamInfoClear( pList );

	UNLOCK();
}

/**
 *
 * Allocate new struct list_head
 *
 * @param	void
 * @return	list_head *
 *
 */
struct list_head * _StreamListNew( void )
{
	struct list_head *pStreamList;

	LOCK();

	pStreamList = kmalloc( sizeof(struct list_head), GFP_ATOMIC );

	if ( pStreamList == NULL )
	{
		VENC_ERROR("Can't alloc kernel memory\n");
		goto func_exit;

		//pStreamList = kmalloc( sizeof(struct list_head), GFP_KERNEL );
	}

	INIT_LIST_HEAD( pStreamList );

	//VENC_DEBUG("%s: 0x%08x\n", __F__, (u32)pStreamList);

#ifdef H1ENCODE_DEBUG_MEM_LEAK
	atomic_inc( &aui_count );
#endif

func_exit:
	UNLOCK();

	return pStreamList;
}

/**
 *
 * Release struct list_head
 *
 * @param	struct list_head *
 * @return	void
 *
 */
void _StreamListFree( struct list_head *pStreamList )
{
	if ( pStreamList == NULL )
	{
		return;
	}

	LOCK();

	//VENC_DEBUG("%s: 0x%08x\n",  __F__, (u32)pStreamList);

	if ( !list_empty( pStreamList ) )
	{
		// if list is not empty, clear the list entry
		_StreamInfoClear( pStreamList );
	}

#ifdef H1ENCODE_DEBUG_MEM_LEAK
	atomic_dec( &aui_count );
#endif

	kfree( pStreamList );

	UNLOCK();

}

/**
 *
 * Copy stream data to pstMem
 *
 * @param	H1ENCODE_MEM_T *, void *, u32
 * @return	void
 *
 */
static void _StreamCopy2Mem( H1ENCODE_MEM_T *pstMem, void* buf, u32 size )
{
	if ( pstMem == NULL || buf == NULL || size == 0 )
	{
		return;
	}

	LOCK();

	if ( pstMem->u32WriteOffset + size > pstMem->u32Size )
	{
		//  ESBuffer is wrap-aound.

		// First half
		u32 dest1 = pstMem->u32Virt + pstMem->u32WriteOffset;
		u32 size1 = pstMem->u32Size - pstMem->u32WriteOffset;
		u32 src1 = (u32)buf;

		// Second half
		u32 dest2 = pstMem->u32Virt;
		u32 size2 = size - size1;
		u32 src2 = (u32)buf + size1;

		memcpy( (void *)dest1, (void *)src1, size1);
		memcpy( (void *)dest2, (void *)src2, size2);

		pstMem->u32WriteOffset = size2;
	}
	else
	{
		u32 dest = pstMem->u32Virt + pstMem->u32WriteOffset;

		memcpy( (void *)dest, buf, size );

		pstMem->u32WriteOffset += size;
	}

	UNLOCK();

#if 0
	VENC_DEBUG("[ESBuffer] MAX: 0x%08x, RD:0x%04x WR:0x%04x, copy size: %d\n",
		pstMem->u32Size, pstMem->u32ReadOffset, pstMem->u32WriteOffset, size );
#endif

}

/**
 *
 * Stream copy to target memory using stream infomation list
 *
 * @param	struct list_head *, H1ENCODE_MEM_T *
 * @return	int : stream size
 *
 */
int _StreamListCopy2Mem( struct list_head *pList , H1ENCODE_MEM_T *pstMem )
{
	struct list_head *pos;
	H1ENCODE_STREAM_LIST_T *pstStream;

	u32 streamTotal = 0;

	if ( pList == NULL )
	{
		VENC_ERROR("pList is NULL\n");
		return 0;
	}

	DEBUG_PRINT("============================================\n");

	list_for_each( pos, pList )
	{
		pstStream = list_entry( pos, H1ENCODE_STREAM_LIST_T, infoList );

		if ( pstStream != NULL )
		{

#if 0 //def H1ENCODE_DEBUG_STREAM_LIST
			{
				int i;
				u8 *data = (u8 *)pstStream->u32StreamBase;
				u32 size = 32;

				for ( i = 0; i < size; i++ )
				{
					printk("%02x ", (u8)data[i]);

					if ( (i + 1) % 32 == 0 )
					{
						printk("\n");
					}
				}

				printk("\n");
			}
#endif

			DEBUG_PRINT("[ADDR] 0x%08x -- 0x%08x (0x%08x)\n",
				pstStream->u32StreamBase, pstStream->u32StreamBase + pstStream->u32StreamSize, pstStream->u32StreamSize );

			_StreamCopy2Mem(pstMem, (void *)pstStream->u32StreamBase, pstStream->u32StreamSize);

			streamTotal += pstStream->u32StreamSize;
		}
	}

#ifdef ALIGNMENT_SIZE
	if ((streamTotal%ALIGNMENT_SIZE) > 0)
	{
		int paddingSize = ALIGNMENT_SIZE - (streamTotal%ALIGNMENT_SIZE);
		u32 dest = pstMem->u32Virt + pstMem->u32WriteOffset;

		memset((void *)dest, 0x0, paddingSize);

		pstMem->u32WriteOffset += paddingSize;
		streamTotal += paddingSize;
	}
#endif

	DEBUG_PRINT("--------------------------------------------\n");
	DEBUG_PRINT("[TOTAL] 0x%08x \n", streamTotal );

	return streamTotal;
}


