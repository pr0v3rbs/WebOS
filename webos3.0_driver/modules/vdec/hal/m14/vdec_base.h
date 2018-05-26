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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     junghyun.son@lge.com
 *  @version    1.0
 *  @date       2013-09-25
 *  @note       Additional information.
 */

#ifndef	_M14_VDEC_REG_BASE_H_
#define	_M14_VDEC_REG_BASE_H_

#ifdef	__linux__
#include <linux/io.h>

#ifndef __XTENSA__
#include "os_util.h"
#endif
#include "hma_alloc.h"
#endif	//__linux__

#ifdef VDEC_BASE
#undef VDEC_BASE
#endif

// Register base address
#ifdef __XTENSA__
#define VDEC_REG_BASE				(0xF0000000)
#else
#define VDEC_REG_BASE				(0xC0004000)
#endif
#define VDEC_REG_MCU_RC				(VDEC_REG_BASE + 0x00000700)
#define VDEC_REG_MCU_IPC			(VDEC_REG_BASE + 0x00000800)
#define VDEC_REG_BASE_CNM			(VDEC_REG_BASE + 0x00001000)
#define VDEC_REG_RC0_BASE			(VDEC_REG_BASE + 0x00001400)
#define VDEC_REG_RC1_BASE			(VDEC_REG_BASE + 0x00001600)
#define VDEC_REG_BASE_SYNC			(VDEC_REG_BASE + 0x00001700)
#define VDEC_REG_BASE_PDEC			(VDEC_REG_BASE + 0x00001800)
#define VDEC_REG_BASE_HEVC			(VDEC_REG_BASE + 0x00002000)

// IRQ number
#ifdef __XTENSA__
#define VDEC_IRQ_NUM_IPC			(0)
#define VDEC_IRQ_NUM_PDEC			(1)
#define VDEC_IRQ_NUM_CNM			(1)
#define VDEC_IRQ_NUM_HEVC			(1)
#define VDEC_IRQ_NUM_SYNC			(5)
#else
#define VDEC_IRQ_NUM_IPC			((vdec_chip_rev() < VDEC_CHIP_REV(M14,B0))? M14_A0_IRQ_VDEC0 : M14_B0_IRQ_VDEC0)
#define VDEC_IRQ_NUM_PDEC			((vdec_chip_rev() < VDEC_CHIP_REV(M14,B0))? M14_A0_IRQ_VDEC0 : M14_B0_IRQ_VDEC0)
#define VDEC_IRQ_NUM_CNM			((vdec_chip_rev() < VDEC_CHIP_REV(M14,B0))? M14_A0_IRQ_VDEC0 : M14_B0_IRQ_VDEC0)
#define VDEC_IRQ_NUM_HEVC			((vdec_chip_rev() < VDEC_CHIP_REV(M14,B0))? M14_A0_IRQ_VDEC0 : M14_B0_IRQ_HEVC)
#define VDEC_IRQ_NUM_SYNC			((vdec_chip_rev() < VDEC_CHIP_REV(M14,B0))? M14_A0_IRQ_VDEC0 : M14_B0_IRQ_VDEC0)
#endif

// Core number
#define VDEC_CORE_NUM_CNM			(1)
#define VDEC_CORE_NUM_HEVC			(2)

// vdec register base
#define VDEC_BASE					VDEC_REG_BASE		// 0xc0000000 + 0x004000

#define VDEC_GET_DE_IF_BASE(rev, ch, base)	\
do {		\
	if ((rev)<0xB0)		\
		base = (VDEC_BASE + 0x1E00 + ((ch) * 0x40));		\
	else		\
		base = (VDEC_BASE + 0x1C00 + ((ch) * 0x40));		\
} while(0)
#define VDEC_VSYNC_BASE					(VDEC_BASE + 0x1700)
#define VDEC_GET_VSYNC_BASE(rev, base)	\
do {									\
		base = VDEC_VSYNC_BASE;			\
} while(0)

#define PDEC_BASE(ch)				(VDEC_REG_BASE_PDEC + ((ch)*0x100))
#define CNM_BASE(ch)				VDEC_REG_BASE_CNM
#define HEVC_REG_BASE(ch)			VDEC_REG_BASE_HEVC

#define	PDEC_NUM_OF_CHANNEL			3

#define	MSVC_NUM_OF_CORE			1
#define	MSVC_NUM_OF_CHANNEL			4

#define HEVC_NUM_OF_CORE			1
#define HEVC_NUM_OF_CHANNEL			1
#define HEVC__REG_SIZE				0xC00

#define	DE_NUM_OF_CHANNEL			4
#define	LQC_NUM_OF_CHANNEL			4
#define DE_IPC_NUM_OF_REG			DE_NUM_OF_CHANNEL

#define LIPSYNC_NUM_OF_CHANNEL		4
#define	DE_VDO_NUM_OF_CHANNEL		2
#define	DSTBUF_NUM_OF_CHANNEL		1
#define	VDISP_NUM_OF_CHANNEL		4
#define	VSYNC_HW_NUM_OF_CH			4
#define	DE_IPC_NUM_OF_CHANNEL		4

#define	NUM_OF_MSVC_CHANNEL			(MSVC_NUM_OF_CORE * MSVC_NUM_OF_CHANNEL)

#define	VDEC_NUM_OF_CHANNEL			(NUM_OF_MSVC_CHANNEL + HEVC_NUM_OF_CHANNEL)

/* For B0 codes */
#define MAX_WIDTH					2048
#define MAX_HEIGHT					1088

#define MAX_NUM_OF_TILED_BUFFER		22

#define VDEC_CPB_SIZE			0x700000

#define UNKNOWN_PTS				0xFFFFFFFE

#ifdef	__linux__
#define VDEC_BUS_OFFSET				0x00000000
#define vdec_alloc(size,align,name)	({ \
		unsigned long r = hma_alloc_user ("vdec", size, align, name); \
		if (r) \
			r -= VDEC_BUS_OFFSET; \
		r; \
	})
#define vdec_free(addr)			hma_free ("vdec", addr+VDEC_BUS_OFFSET)

#define vdec_alloc_from(pool,size,align,name)	({							\
	unsigned long r = hma_alloc_user ("vdec", size, align, name);			\
	if (r) r -= VDEC_BUS_OFFSET;											\
	r;																		\
})
#define vdec_free_from(pool,addr)				({							\
	hma_free ("vdec", addr+VDEC_BUS_OFFSET);								\
})

#define vdec_remap(addr,size)		ioremap (addr+VDEC_BUS_OFFSET, size)
#define vdec_unmap(addr)		iounmap (addr)
#endif	//__linux__

#define vdec_bus_to_phys(bus)	((bus)+VDEC_BUS_OFFSET)


#ifdef __XTENSA__
#define vdec_chip()	    LX_CHIP_M14
#define vdec_chip_rev()		((vdec_chip() << 8) | 0xa0)
#define VDEC_CHIP_REV(a,b)	((VDEC_CHIP_##a << 8) | 0x##b)
#define VDEC_COMP_CHIP(a,b)	(((a)&0xffff00)-((b)<<8))
#else
#define vdec_chip()		lx_chip()
#define vdec_chip_rev()		lx_chip_rev()
#define VDEC_CHIP_REV(a,b)	LX_CHIP_REV(a,b)
#define VDEC_COMP_CHIP(a,b)	LX_COMP_CHIP(a,b)
#endif

#ifdef CONFIG_VDEC_ON_FPGA
#define VDEC_HW_RATE		(40)
#else
#define VDEC_HW_RATE		(1)
#endif

#define CNM_CORE_FREQUENCY	(266)

#endif /* _M14_VDEC_REG_BASE_H_ */

