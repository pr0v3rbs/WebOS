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

#ifndef _ADEC_BUF_REG_API_H_
#define _ADEC_BUF_REG_API_H_

#include "common/adec_config.h"
#include "common/adec_reg_api.h"

#if (ADEC_OPT_BUF_USE_REG == 1)

#define BUF_REG_READ(_ADDR_, _BITS_, _BIT_OFFSET_, _VAL_) \
	_VAL_ = (ADEC_ReadRegister(_ADDR_) >> _BIT_OFFSET_) & (0xffffffff >> (32 - _BITS_));
#define BUF_REG_WRITE(_ADDR_, _BITS_, _BIT_OFFSET_, _VAL_)	\
	ADEC_WriteRegister(_ADDR_, \
		(ADEC_ReadRegister(_ADDR_) & (~((0xffffffff >> (32 - _BITS_ - _BIT_OFFSET_))<< _BIT_OFFSET_))) | \
		((_VAL_ & (0xffffffff >> (32 - _BITS_)))<< _BIT_OFFSET_) );

#if (ADEC_OPT_BUF_USE_4REG_PER_BUF == 1)
#define BUF_REG_READ_WRITE_OFFSET(_BASE_, _VAL_)	BUF_REG_READ(((unsigned char*)_BASE_) + 0, 28, 0, _VAL_)
#define BUF_REG_READ_READ_OFFSET(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 4, 28, 0, _VAL_)
#define BUF_REG_READ_WRITE_INDEX(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 8, 27, 0, _VAL_)
#define BUF_REG_READ_READ_INDEX(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 12, 27, 0, _VAL_)
#define BUF_REG_READ_IS_ALLOCATED(_BASE_, _VAL_)	BUF_REG_READ(((unsigned char*)_BASE_) + 8, 1, 27, _VAL_)

#define BUF_REG_WRITE_WRITE_OFFSET(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 0, 28, 0, _VAL_)
#define BUF_REG_WRITE_READ_OFFSET(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 4, 28, 0, _VAL_)
#define BUF_REG_WRITE_WRITE_INDEX(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 8, 27, 0, _VAL_)
#define BUF_REG_WRITE_READ_INDEX(_BASE_, _VAL_)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 12, 27, 0, _VAL_)
#define BUF_REG_WRITE_IS_ALLOCATED(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 8, 1, 27, _VAL_)

#if (ADEC_OPT_BUF_USE_DEBUG_REG == 1)
#define BUF_REG_READ_TOTAL_WRITE(_BASE_, _VAL)		BUF_REG_READ(((unsigned char*)_BASE_) + 16, 28, 0 _VAL_)
#define BUF_REG_READ_TOTAL_READ(_BASE_, _VAL)		BUF_REG_READ(((unsigned char*)_BASE_) + 20, 28, 0 _VAL_)
#define BUF_REG_WRITE_TOTAL_WRITE(_BASE_, _VAL)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 16, 28, 0 _VAL_)
#define BUF_REG_WRITE_TOTAL_READ(_BASE_, _VAL)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 20, 28, 0 _VAL_)
#else
#define BUF_REG_READ_TOTAL_WRITE(_BASE_, _VAL)
#define BUF_REG_READ_TOTAL_READ(_BASE_, _VAL)
#define BUF_REG_WRITE_TOTAL_WRITE(_BASE_, _VAL)
#define BUF_REG_WRITE_TOTAL_READ(_BASE_, _VAL)
#endif
#else
#define BUF_REG_READ_WRITE_OFFSET(_BASE_, _VAL_)	BUF_REG_READ(((unsigned char*)_BASE_) + 0, 27, 0, _VAL_)
#define BUF_REG_READ_READ_OFFSET(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 4, 27, 0, _VAL_)
#define BUF_REG_READ_WRITE_INDEX(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 8, 14, 0, _VAL_)
#define BUF_REG_READ_READ_INDEX(_BASE_, _VAL_)		BUF_REG_READ(((unsigned char*)_BASE_) + 8, 14, 14, _VAL_)
#define BUF_REG_READ_IS_ALLOCATED(_BASE_, _VAL_)	BUF_REG_READ(((unsigned char*)_BASE_) + 0, 1, 27, _VAL_)

#define BUF_REG_WRITE_WRITE_OFFSET(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 0, 27, 0, _VAL_)
#define BUF_REG_WRITE_READ_OFFSET(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 4, 27, 0, _VAL_)
#define BUF_REG_WRITE_WRITE_INDEX(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 8, 14, 0, _VAL_)
#define BUF_REG_WRITE_READ_INDEX(_BASE_, _VAL_)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 8, 14, 14, _VAL_)
#define BUF_REG_WRITE_IS_ALLOCATED(_BASE_, _VAL_)	BUF_REG_WRITE(((unsigned char*)_BASE_) + 0, 1, 27, _VAL_)

#if (ADEC_OPT_BUF_USE_DEBUG_REG == 1)
#define BUF_REG_READ_TOTAL_WRITE(_BASE_, _VAL)		BUF_REG_READ(((unsigned char*)_BASE_) + 12, 28, 0 _VAL_)
#define BUF_REG_READ_TOTAL_READ(_BASE_, _VAL)		BUF_REG_READ(((unsigned char*)_BASE_) + 16, 28, 0 _VAL_)
#define BUF_REG_WRITE_TOTAL_WRITE(_BASE_, _VAL)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 12, 28, 0 _VAL_)
#define BUF_REG_WRITE_TOTAL_READ(_BASE_, _VAL)		BUF_REG_WRITE(((unsigned char*)_BASE_) + 16, 28, 0 _VAL_)
#else
#define BUF_REG_READ_TOTAL_WRITE(_BASE_, _VAL)
#define BUF_REG_READ_TOTAL_READ(_BASE_, _VAL)
#define BUF_REG_WRITE_TOTAL_WRITE(_BASE_, _VAL)
#define BUF_REG_WRITE_TOTAL_READ(_BASE_, _VAL)
#endif
#endif

#else

#define BUF_REG_READ_WRITE_OFFSET(_BASE_, _VAL_)
#define BUF_REG_READ_READ_OFFSET(_BASE_, _VAL_)
#define BUF_REG_READ_WRITE_INDEX(_BASE_, _VAL_)
#define BUF_REG_READ_READ_INDEX(_BASE_, _VAL_)
#define BUF_REG_READ_IS_ALLOCATED(_BASE_, _VAL_)

#define BUF_REG_WRITE_WRITE_OFFSET(_BASE_, _VAL_)
#define BUF_REG_WRITE_READ_OFFSET(_BASE_, _VAL_)
#define BUF_REG_WRITE_WRITE_INDEX(_BASE_, _VAL_)
#define BUF_REG_WRITE_READ_INDEX(_BASE_, _VAL_)
#define BUF_REG_WRITE_IS_ALLOCATED(_BASE_, _VAL_)

#endif //(ADEC_OPT_BUF_USE_REG == 1)


#define READ_WRITE_INDEX()		BUF_REG_READ_WRITE_INDEX(intBuf->regBase,intBuf->status.writeIndex)
#define READ_READ_INDEX()		BUF_REG_READ_READ_INDEX(intBuf->regBase,intBuf->status.readIndex)
#define READ_IS_ALLOCATED()		BUF_REG_READ_IS_ALLOCATED(intBuf->regBase,intBuf->status.indexAllocated)
#define READ_WRITE_OFFSET()		BUF_REG_READ_WRITE_OFFSET(intBuf->regBase,intBuf->status.writeOffset)
#define READ_READ_OFFSET()		BUF_REG_READ_READ_OFFSET(intBuf->regBase,intBuf->status.readOffset)

#define WRITE_WRITE_INDEX()		BUF_REG_WRITE_WRITE_INDEX(intBuf->regBase,intBuf->status.writeIndex)
#define WRITE_READ_INDEX()		BUF_REG_WRITE_READ_INDEX(intBuf->regBase,intBuf->status.readIndex)
#define WRITE_IS_ALLOCATED()	BUF_REG_WRITE_IS_ALLOCATED(intBuf->regBase,intBuf->status.indexAllocated)
#define WRITE_WRITE_OFFSET()	BUF_REG_WRITE_WRITE_OFFSET(intBuf->regBase,intBuf->status.writeOffset)
#define WRITE_READ_OFFSET()		BUF_REG_WRITE_READ_OFFSET(intBuf->regBase,intBuf->status.readOffset)

#define READ_TOTAL_READ()
#define WRITE_TOTAL_READ()
#define READ_TOTAL_WRITE()
#define WRITE_TOTAL_WRITE()


#endif
