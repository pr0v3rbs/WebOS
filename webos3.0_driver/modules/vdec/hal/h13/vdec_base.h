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

#ifdef	__linux__
#include <linux/io.h>

#include "os_util.h"
#include "hma_alloc.h"
#endif	//__linux__

#ifdef H13_VDEC_BASE
#undef H13_VDEC_BASE
#endif

// Register base address
#define VDEC_REG_BASE				((vdec_chip_rev() < VDEC_CHIP_REV(H13, B0))? 0xC0004000 : 0xC0005000)
#define VDEC_REG_BASE_CNM			(VDEC_REG_BASE + 0x00000000)
#define VDEC_REG_RC0_BASE			(VDEC_REG_BASE + 0x00000400)
#define VDEC_REG_RC1_BASE			(VDEC_REG_BASE + 0x00000600)
#define VDEC_REG_BASE_PDEC			(VDEC_REG_BASE + 0x00000800)
#define VDEC_REG_BASE_SYNC			(VDEC_REG_BASE + 0x00000E00)
#define VDEC_REG_BASE_HEVC			(VDEC_REG_BASE + 0x00000000)

// IRQ number
#define VDEC_IRQ_NUM_PDEC			(H13_IRQ_VDEC0)
#define VDEC_IRQ_NUM_CNM			(H13_IRQ_VDEC0)
#define VDEC_IRQ_NUM_HEVC			(H13_IRQ_VDEC0)
#define VDEC_IRQ_NUM_SYNC			(H13_IRQ_VDEC0)

// Core number
#define VDEC_CORE_NUM_CNM			(1)

// vdec register base
#define H13_VDEC_BASE				(0xc0000000+0x004000)
#define VDEC_BASE					H13_VDEC_BASE

#if defined(CHIP_NAME_h13) && (CHIP_REV < 0xb0)
#define VDEC_DE_IF_BASE(ch)			(H13_VDEC_BASE + 0x0E00 + (ch * 0x40))
#else
#define VDEC_DE_IF_BASE(ch)			(H13_VDEC_BASE + 0x1E00 + (ch * 0x40))
#endif

#define VDEC_GET_DE_IF_BASE(rev, ch, base)	\
do {		\
		base = VDEC_DE_IF_BASE(ch);		\
} while(0)


#if defined(CHIP_NAME_h13)
#define VDEC_VSYNC_BASE_A0				(H13_VDEC_BASE + 0x0700)
#define VDEC_VSYNC_BASE_B0				(H13_VDEC_BASE + 0x1700)
#define VDEC_GET_VSYNC_BASE(rev, base)	\
do {									\
	if (rev<0xB0)						\
		base = VDEC_VSYNC_BASE_A0;		\
	else								\
		base = VDEC_VSYNC_BASE_B0;		\
} while(0)
#else	// maybe H14??
#define VDEC_VSYNC_BASE					(H13_VDEC_BASE + 0x1700)
#define VDEC_GET_VSYNC_BASE(rev, base)	\
do {									\
		base = VDEC_VSYNC_BASE;			\
} while(0)
#endif

#define MCU_ROM_BASE				0x50000000
#define MCU_RAM0_BASE				0x60000000
#define MCU_RAM1_BASE				0x80000000
#define MCU_RAM2_BASE				0xa0000000
#define MCU_REG_BASE				0xf0000000

#define MCU_ROM_MAX_SIZE			0x10000000
#define MCU_RAM_MAX_SIZE			0x20000000

#define VDEC_REG_OFFSET				0x00004000

#define PDEC_BASE(ch)				(VDEC_REG_BASE_PDEC + (ch*0x100))
#define CNM_BASE(ch)				VDEC_REG_BASE_CNM
#define HEVC_REG_BASE(ch)			VDEC_REG_BASE_HEVC

#define	PDEC_NUM_OF_CHANNEL			3

#define	LQC_NUM_OF_CHANNEL			4

#define	MSVC_NUM_OF_CORE			1
#define	MSVC_NUM_OF_CHANNEL			4

#define HEVC_NUM_OF_CORE			0
#define HEVC__REG_SIZE				0x0

#define	NULLVC_NUM_OF_CHANNEL		1
#define	LIPSYNC_NUM_OF_CHANNEL		4	// MAX: LQC_NUM_OF_CHANNEL
#define	DE_NUM_OF_CHANNEL			4
#define DE_IPC_NUM_OF_REG			DE_NUM_OF_CHANNEL
#define	DE_VDO_NUM_OF_CHANNEL		2
#define	DSTBUF_NUM_OF_CHANNEL		1
#define	VDISP_NUM_OF_CHANNEL		4
#define	VSYNC_HW_NUM_OF_CH			4
#define	DE_IPC_NUM_OF_CHANNEL		4

#define	NUM_OF_MSVC_CHANNEL			(MSVC_NUM_OF_CORE * MSVC_NUM_OF_CHANNEL)
#define	NUM_OF_SWDEC_CHANNEL		1

#define	VDEC_BASE_CHANNEL_SWDEC		(NUM_OF_MSVC_CHANNEL)

#define	VDEC_NUM_OF_CHANNEL			(NUM_OF_MSVC_CHANNEL + NUM_OF_SWDEC_CHANNEL)

/* For B0 codes */
#define MAX_WIDTH					2048
#define MAX_HEIGHT					1088

#define MAX_NUM_OF_TILED_BUFFER		22

#define VDEC_CPB_SIZE			0x700000

#define UNKNOWN_PTS				0xFFFFFFFE

#ifdef	__linux__
#define VDEC_BUS_OFFSET			0x00000000
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


#define vdec_chip()		lx_chip()
#define vdec_chip_rev()		lx_chip_rev()
#define VDEC_CHIP_REV(a,b)	LX_CHIP_REV(a,b)

#define VDEC_COMP_CHIP(a,b)	LX_COMP_CHIP(a,b)

#ifdef CONFIG_VDEC_ON_FPGA
#define VDEC_HW_RATE		(40)
#else
#define VDEC_HW_RATE		(1)
#endif

#define CNM_CORE_FREQUENCY	(266)

#endif /* _H13_VDEC_REG_BASE_H_ */

