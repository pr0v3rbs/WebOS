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

#ifndef _ADEC_BUF_H_
#define _ADEC_BUF_H_

#ifdef _OUT_
#undef _OUT_
#endif

#ifdef _IN_OUT_
#undef _IN_OUT_
#endif

#define _OUT_
#define _IN_OUT_

#include "common/adec_config.h"
#include "adec_au_info.h"

typedef struct _ADEC_BUF_T
{
	////////////////////////////////////////////////////////////////////////////
	// Clear Function
	////////////////////////////////////////////////////////////////////////////

	/**
	 *	Initialize buffer.
	 *	Clear all information in the buffer.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*init)(void* _buf);

	////////////////////////////////////////////////////////////////////////////
	// Status Function
	////////////////////////////////////////////////////////////////////////////

	/**
	 *	Get size of the free buffer.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : buffer size
	 *	@return otherwise : err.
	 */
	int (*get_free_size)(void* _buf);

	/**
	 *	Get used buffer size.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : buffer size
	 *	@return otherwise : err.
	 */
	int (*get_used_size)(void* _buf);

	/**
	 *	Get count of the total au, stored in this buffer.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : AU Count
	 *	@return otherwise : err.
	 */
	int (*get_au_cnt)(void* _buf);

	////////////////////////////////////////////////////////////////////////////
	// Property Function
	////////////////////////////////////////////////////////////////////////////

	/**
	 *	Get allocated size of the buffer.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : buffer size
	 *	@return otherwise : err.
	 */
	int (*get_max_size)(void* _buf);

	/**
	 *	Get count of the max au. The length of the index table.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : AU Count
	 *	@return otherwise : err.
	 */
	int (*get_max_au)(void* _buf);

	/**
	 *	Get the max access unit size. This value means the maximum size can be read at once.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return GT or ET 0 : buffer size
	 *	@return otherwise : err.
	 */
	int (*get_max_au_length)(void* _buf);

	////////////////////////////////////////////////////////////////////////////
	// Write Function
	////////////////////////////////////////////////////////////////////////////

	/**
	 *	Complete previous AU and create new au.
	 *	 All data, feeded after creating au, will be contained in this AU.
	 *	@param _info : Pointer of the structure contains information about the AU.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*create_new_au)(
		ADEC_AU_INFO_T* _info,
		void* _buf);

	/**
	 *	Write data in the buffer. New data will belong to previously created AU.
	 *	@param _src : Pointer of the data to be written.
	 *	@param _length : Size of the data.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*write_data)(
		unsigned char* _src,
		int _length,
		void* _buf);

	/**
	 *	Create new AU, and write data.
	 *	@param _src : Pointer of the data to be written.
	 *	@param _length : Size of the data.
	 *	@param _info : Pointer of the structure contains information about the AU.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*insert_au)(
		unsigned char* _src,
		int _length,
		ADEC_AU_INFO_T* info,
		void* _buf);

	/**
	 *	Update current AU info as new one.
	 *	@param _info : Pointer of the structure contains information about the AU.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*update_au)(
		ADEC_AU_INFO_T* info,
		void* _buf);

	// Read Function
	/**
	 *	Get AU info.
	 *	@param _index : 0-base index of the AU, want to read, from the reading position.
	 *	@param _offset : Out parameter. Offset of the start of the AU from the current read PTR.
	 *	@param _au_info : Out parameter. Pointer of the structure contains information about the AU.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*get_au_info)(int index,
		_OUT_ int* _offset,
		_OUT_ ADEC_AU_INFO_T* _au_info,
		void* _buf);

	/**
	 *	Get AU info, currently read.
	 *	@param _offset : Out parameter. Offset of the start of the AU from the current read PTR.
	 *	@param _au_info : Out parameter. Pointer of the structure contains information about the AU.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*get_current_info)(_OUT_ int* offset,
		_OUT_ ADEC_AU_INFO_T* _au_info,
		void* _buf);

	/**
	 *	Read actual data.
	 *	@param _dst : Out parameter. Pointer of the data in the buffer.
	 *	@param _size : In & Out parameter. Size of the data want to read.
	 *		Actualy read size will be retrieved by this parameter.
	 *	@param _au_in_data : Out parameter. Total au the data contains.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*read_data)(_OUT_ unsigned char** _dst,
		_IN_OUT_ int* _size,
		_OUT_ int* _au_in_data,
		void* _buf);

	/**
	 *	Flush buffer. Increase read Ptr.
	 *	@param _size : Size of the data, already has been read.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*flush_data)(
		int _size,
		void* _buf);

	/**
	 *	Read data as AU.
	 *	@param _dst : Out parameter. Pointer of the data in the buffer.
	 *	@param _size : Out parameter. Actualy read size will be retrieved by this parameter.
	 *	@param _au_cnt : Number of AU you want to read.
	 *	@param _buf : The pointer of the structure of the buffer.
	 *	@return 0 : success.
	 *	@return otherwise : err.
	 */
	int (*read_au)(_OUT_ unsigned char** _dst,
		_OUT_ int* _size,
		_IN_OUT_ int* _au_cnt,
		void* _buf);

#if (ADEC_OPT_BUF_USE_REPEAT == 1)
	int (*set_repeat)(int _cnt,
		void* _buf);
#endif

}ADEC_BUF_T;

#endif
