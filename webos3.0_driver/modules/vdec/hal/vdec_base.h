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

#define LX_CHIP_D13		0x12
#define LX_CHIP_D14		0x16

#ifdef __XTENSA__
#define LX_CHIP_H13		0x13
#define LX_CHIP_M14		0x14
#define LX_CHIP_H14		0x15
#define LX_CHIP_H15		0x17
#define LX_CHIP_M16     0x18
#endif

#define VDEC_CHIP_D13		LX_CHIP_D13
#define VDEC_CHIP_D14		LX_CHIP_D14
#define VDEC_CHIP_H13		LX_CHIP_H13
#define VDEC_CHIP_M14		LX_CHIP_M14
#define VDEC_CHIP_H14		LX_CHIP_H14
#define VDEC_CHIP_H15		LX_CHIP_H15
#define VDEC_CHIP_M16       LX_CHIP_M16

#if defined (CHIP_NAME_h13vdec)
#include "h13b0vdec/vdec_base.h"
#elif defined (CHIP_NAME_d13)
#include "d13/vdec_base.h"
#elif defined (CHIP_NAME_d14)
#include "d14/vdec_base.h"
#elif defined (CHIP_NAME_h13) || defined (CHIP_NAME_h14)
#include "h13/vdec_base.h"
#elif defined (CHIP_NAME_m14)
#include "m14/vdec_base.h"
#elif defined (CHIP_NAME_h15)
#include "h15/vdec_base.h"
#elif defined (CHIP_NAME_m16)
#include "m16/vdec_base.h"
#else
#warning "unknown chip name..."
#endif

