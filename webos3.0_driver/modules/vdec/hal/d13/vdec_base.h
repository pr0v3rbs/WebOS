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
 *  @author     seokjoo.lee
 *  @version    1.0
 *  @date       2011-04-05
 *  @note       Additional information.
 */

#ifndef	_H13_VDEC_REG_BASE_H_
#define	_H13_VDEC_REG_BASE_H_

#include <linux/io.h>

#include "hma_alloc.h"

// Register base address
#define VDEC_REG_BASE				(0xF0000000)
#define VDEC_REG_RC0_BASE			(VDEC_REG_BASE + 0x00010400)
#define VDEC_REG_RC1_BASE			(VDEC_REG_BASE + 0x00010600)
#define VDEC_REG_BASE_PDEC			(VDEC_REG_BASE + 0x00010800)
#define VDEC_REG_BASE_CNM			(VDEC_REG_BASE + 0x00030000)
#define VDEC_REG_BASE_HEVC			(VDEC_REG_BASE + 0x00050000)
#define VDEC_REG_BASE_SYNC			(VDEC_REG_BASE + 0x00060000)

// IRQ number
#define VDEC_IRQ_NUM_PDEC			(2)
#define VDEC_IRQ_NUM_CNM			(3)
#define VDEC_IRQ_NUM_HEVC			(3)
#define VDEC_IRQ_NUM_SYNC			(5)

#define VDEC_IRQ_NUM_GX(i)		\
({ \
	int ret;	\
	if (i==0) \
	 ret = 3; \
	else	\
		ret = -1; \
	ret;	\
})

#define VDEC_BASE_GX(i)		\
	({ \
	 unsigned int base;	\
	 if (i==0) \
	 base = 0xF0062000; \
	 else	\
	 base = 0x0; \
	 base;	\
	 })

// Core number
#define VDEC_CORE_NUM_G1			(0)
#define VDEC_CORE_NUM_CNM			(1)
#define VDEC_CORE_NUM_HEVC			(2)
#define VDEC_CORE_NUM_G2            (3)

#define GX_TYPE_OF_CORE             (1)

#define VDEC_BASE					(0xf0000000)

// TODO : D13
#define VDEC_GET_DE_IF_BASE(rev, ch, base)	\
do {		\
	if (rev<0xB0)		\
		base = (VDEC_BASE + 0x1E00 + (ch * 0x40));		\
	else		\
		base = (VDEC_BASE + 0x1C00 + (ch * 0x40));		\
} while(0)

#define VDEC_VSYNC_BASE					(VDEC_BASE + 0x60D00)
#define VDEC_GET_VSYNC_BASE(rev, base)	\
do {									\
		base = VDEC_VSYNC_BASE;			\
} while(0)

#define PDEC_BASE(ch)				(VDEC_REG_BASE_PDEC + (ch*0x100))
#define CNM_BASE(ch)				(VDEC_REG_BASE_CNM + (ch*0x4000))
#define HEVC_REG_BASE(ch)			VDEC_REG_BASE_HEVC

#define	PDEC_NUM_OF_CHANNEL			2

#define	DE_NUM_OF_CHANNEL			4
#define	LQC_NUM_OF_CHANNEL			4
#define DE_IPC_NUM_OF_REG			0

#define	MSVC_NUM_OF_CORE			2
#define	MSVC_NUM_OF_CHANNEL			4

#define HEVC_NUM_OF_CORE			1
#define HEVC__REG_SIZE				0xC00

#define G1_NUM_OF_CORE				1
#define G2_NUM_OF_CORE				0

#define	NULLVC_NUM_OF_CHANNEL		1
#define	LIPSYNC_NUM_OF_CHANNEL		3	// MAX: LQC_NUM_OF_CHANNEL
#define	DE_IF_NUM_OF_CHANNEL		2
#define	DE_VDO_NUM_OF_CHANNEL		2
#define	DSTBUF_NUM_OF_CHANNEL		1
#define	VDISP_NUM_OF_CHANNEL		4
#define	VSYNC_HW_NUM_OF_CH			4
#define	DE_IPC_NUM_OF_CHANNEL		4

#define	NUM_OF_MSVC_CHANNEL		(MSVC_NUM_OF_CORE * MSVC_NUM_OF_CHANNEL)

#define	VDEC_NUM_OF_CHANNEL		(NUM_OF_MSVC_CHANNEL)

#define VDEC_CPB_SIZE			0x1000000
#define VDEC_CPB_MARGIN			0xB00000

#define UNKNOWN_PTS				0xFFFFFFFE

#define VDEC_BUS_OFFSET			0x60000000
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

#define vdec_bus_to_phys(bus)	((bus)+VDEC_BUS_OFFSET)


#define vdec_chip()		LX_CHIP_D13
#define vdec_chip_rev()		((vdec_chip() << 8) | 0xa0)
#define VDEC_CHIP_REV(a,b)	((VDEC_CHIP_##a << 8) | 0x##b)

#define VDEC_COMP_CHIP(a,b)	(((a)&0xffff00)-((b)<<8))

#ifdef CONFIG_VDEC_ON_FPGA
#define VDEC_HW_RATE		(40)
#else
#define VDEC_HW_RATE		(1)
#endif

#define CNM_CORE_FREQUENCY	(266)

#endif /* _H13_VDEC_REG_BASE_H_ */

