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

#ifndef	_M16_VDEC_REG_BASE_H_
#define	_M16_VDEC_REG_BASE_H_

#ifdef	__linux__
#include <linux/io.h>

#ifndef __XTENSA__
#include "os_util.h"
#endif // __XTENSA__
#include "hma_alloc.h"
#endif	//__linux__

#ifdef VDEC_BASE
#undef VDEC_BASE
#endif

// Register base address
#ifdef __XTENSA__
#define VDEC_REG_BASE               (0xF0000000)
#define VDEC1_REG_BASE              (0xF0005000)
#define VDEC2_REG_BASE              (0xF0007000)
#else
#define VDEC_REG_BASE				(0xC8B00000)
#define VDEC1_REG_BASE              (0xC8B02000)
#define VDEC2_REG_BASE              (0xC8B04000)
#endif // __XTENSA__
#define VDEC_REG_MCU_RC				(VDEC_REG_BASE + 0x00000700)
#define VDEC_REG_MCU_IPC			(VDEC_REG_BASE + 0x00000800)
#define VDEC_REG_MCU_DMA			(VDEC_REG_BASE + 0x00000D00)
#define VDEC_REG_RC0_BASE			(VDEC_REG_BASE + 0x00001500)
#define VDEC_REG_RC1_BASE			(VDEC_REG_BASE + 0x00001600)
#define VDEC_REG_BASE_SYNC			(VDEC_REG_BASE + 0x00001700)
#define VDEC_REG_BASE_PDEC			(VDEC_REG_BASE + 0x00001800)
#define VDEC_REG_BASE_HEVC			(VDEC_REG_BASE + 0x00002000)

#define VDEC_BASE_GX(i)		\
	({ \
	 unsigned int base;	\
	 if ((i)==0) \
	 base = VDEC_REG_BASE + 0x1000; \
	 else \
	 base = 0x0; \
	 base;	\
	 })

#define VDEC1_REG_TOP_BASE          (VDEC1_REG_BASE + 0x00001000)
#define VDEC2_REG_TOP_BASE          (VDEC2_REG_BASE + 0x00001000)

// Irq settings
#ifdef __XTENSA__
#define VDEC_IRQ_NUM_IPC			(0)
#define VDEC_IRQ_NUM_PDEC			(1)
#define VDEC_IRQ_NUM_CNM			(1)
#define VDEC_IRQ_NUM_SYNC			(1)
#define VDEC_IRQ_NUM_HEVC			(4)
#define VDEC_IRQ_NUM_HEVC_1			(5)
#define VDEC_IRQ_NUM_GX(i)		\
({ \
	int ret;	\
	if ((i)==0) \
		ret = 1; \
	else	\
		ret = -1; \
	ret;	\
})
#else
// IRQ number : fixme
#define VDEC_IRQ_NUM_IPC			(32 + 45)
#define VDEC_IRQ_NUM_PDEC			(32 + 44)
#define VDEC_IRQ_NUM_CNM			(32 + 44)
#define VDEC_IRQ_NUM_SYNC			(32 + 44)
#define VDEC_IRQ_NUM_HEVC			(32 + 46)
#define VDEC_IRQ_NUM_HEVC_1			(32 + 48)
#define VDEC_IRQ_NUM_GX(i)		\
({ \
	int ret;	\
	if ((i)==0) \
		ret = (32 + 44); \
	else	\
		ret = -1; \
	ret;	\
})
#endif // __XTENSA__

// Core number
#define VDEC_CORE_NUM_G1            (0)
#define VDEC_CORE_NUM_CNM			(1)
#define VDEC_CORE_NUM_HEVC			(2)
#define VDEC_CORE_NUM_G2			(3)

/* below define means number of vdu sub driver */
#define GX_TYPE_OF_CORE             (2)

// vdec register base
#define VDEC_BASE				VDEC_REG_BASE

#define VDEC_GET_DE_IF_BASE(rev, ch, base)	\
do {		\
		base = (VDEC_BASE + 0x1C00 + (ch * 0x40));		\
} while(0)
#define VDEC_GET_VSYNC_BASE(rev, base)	\
do {									\
		base = VDEC_REG_BASE_SYNC;			\
} while(0)

#define PDEC_BASE(ch)				(VDEC_REG_BASE_PDEC + (ch*0x100))
#define CNM_BASE(ch)				VDEC_BASE_GX(ch)
#define HEVC_REG_BASE(ch)			VDEC_REG_BASE_HEVC

#define	PDEC_NUM_OF_CHANNEL			3

#define	MSVC_NUM_OF_CORE			1
#define	MSVC_NUM_OF_CHANNEL			4

#define HEVC_NUM_OF_CORE			2
#define HEVC_NUM_OF_CHANNEL			1
#define HEVC__REG_SIZE				0xC00 // not defined yet

#define G1_NUM_OF_CORE				1
#ifdef CONFIG_VDEC_ON_FPGA
#define G2_NUM_OF_CORE				0 // current fpga doesn't have g2
#else
#define G2_NUM_OF_CORE				0
#endif /* CONFIG_VDEC_ON_FPGA */

#define	DE_NUM_OF_CHANNEL			6

#define	LQC_NUM_OF_CHANNEL			4
#define DE_IPC_NUM_OF_REG			DE_NUM_OF_CHANNEL

#define LIPSYNC_NUM_OF_CHANNEL		4
#define	DE_VDO_NUM_OF_CHANNEL		2
#define	DSTBUF_NUM_OF_CHANNEL		1

#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
#define	VDISP_NUM_OF_CHANNEL		6
#define	VSYNC_HW_NUM_OF_CH		6
#define	DE_IPC_NUM_OF_CHANNEL		6
#else
#define	VDISP_NUM_OF_CHANNEL		4
#define	VSYNC_HW_NUM_OF_CH		4
#define	DE_IPC_NUM_OF_CHANNEL		4
#endif

#define	NUM_OF_MSVC_CHANNEL		(MSVC_NUM_OF_CORE * MSVC_NUM_OF_CHANNEL)

#define	VDEC_NUM_OF_CHANNEL		(NUM_OF_MSVC_CHANNEL + HEVC_NUM_OF_CHANNEL)

/* For B0 codes */
#define MAX_WIDTH			2048
#define MAX_HEIGHT			1088

#define MAX_NUM_OF_TILED_BUFFER		22

#define VDEC_CPB_SIZE			0x1000000

#define UNKNOWN_PTS			0xFFFFFFFE

#if defined (__XTENSA__)
#define VDEC_BUS_OFFSET			(0x60000000-*(unsigned int*)0xf0000714)
#define VDEC_BUS8_OFFSET		(0x80000000-*(unsigned int*)0xf0000718)
#define VDEC_BUSa_OFFSET		(0xa0000000-*(unsigned int*)0xf000071c)
#else
#define VDEC_BUS_OFFSET			0x00000000
#define VDEC_BUS8_OFFSET		0x00000000
#define VDEC_BUSa_OFFSET		0x00000000
#endif // __XTENSA__
#ifdef	__linux__
#ifdef __XTENSA__
#define vdec_alloc(size,align,name)	({ unsigned int a; \
	extern unsigned int ipc_vdec_alloc (int, unsigned int, const char *); \
	a = ipc_vdec_alloc (size, align, name); \
	a; })
#define vdec_free(addr)			{ \
	extern void ipc_vdec_free (unsigned int); \
	ipc_vdec_free (addr); }
#else
#define vdec_alloc(size,align,name)	({ \
		unsigned long r = hma_alloc_user ("vdec", size, align, name); \
		if (r) \
			r -= VDEC_BUS_OFFSET; \
		r; \
	})
#define vdec_free(addr)			hma_free ("vdec", addr+VDEC_BUS_OFFSET)
#endif
#define vdec_alloc1(size,align,name)	({ \
		unsigned long r = hma_alloc_user ("hevc", size, align, name); \
		if (r) \
			r -= VDEC_BUSa_OFFSET; \
		r; \
	})
#define vdec_free1(addr)		hma_free ("hevc", addr+VDEC_BUSa_OFFSET)
#define vdec_alloc2(size,align,name)	({ \
		unsigned long r = hma_alloc_user ("dpb", size, align, name); \
		if (r) \
			r -= VDEC_BUS8_OFFSET; \
		r; \
	})
#define vdec_free2(addr)		hma_free ("dpb", addr+VDEC_BUS8_OFFSET)
#define vdec_alloc3(size,align,name)	({ \
		unsigned long r = hma_alloc_user ("hevc_sb", size, align, name); \
		if (r) \
			r -= VDEC_BUSa_OFFSET; \
		r; \
	})
#define vdec_free3(addr)		hma_free ("hevc_sb", addr+VDEC_BUSa_OFFSET)

#define vdec_alloc_from(pool,size,align,name)	({							\
	unsigned long r = hma_alloc_user ("vdec", size, align, name);			\
	if (r) r -= VDEC_BUS_OFFSET;											\
	r;																		\
})
#define vdec_free_from(pool,addr)				({							\
	hma_free ("vdec", addr+VDEC_BUS_OFFSET);								\
})

#ifdef __XTENSA__
#define vdec_remap(addr,size)		({ \
	void *r; \
	if (*(unsigned int*)0xf000071c <= addr) \
		r = ioremap ((addr+VDEC_BUSa_OFFSET), size); \
	else if (*(unsigned int*)0xf0000718 <= addr) \
		r = ioremap ((addr+VDEC_BUS8_OFFSET), size); \
	else if (*(unsigned int*)0xf0000714 <= addr) \
		r = ioremap ((addr+VDEC_BUS_OFFSET), size); \
	else { \
		printk ("%s.%d: unknown address to vdec_remap(0x%x)\n", __func__, __LINE__, (unsigned int)(addr)); \
		r = NULL; \
	} \
	r; \
	})
#else
#define vdec_remap(addr,size)		ioremap ((addr+VDEC_BUS_OFFSET), size)
#endif

#define vdec_unmap(addr)		iounmap (addr)
#endif	//__linux__

#define vdec_bus_to_phys(bus)	((bus)+VDEC_BUS_OFFSET)


#ifdef __XTENSA__
#define vdec_chip()		LX_CHIP_M16
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
#define VDEC_HW_RATE		(80)
#else
#define VDEC_HW_RATE		(1)
#endif

#define CNM_CORE_FREQUENCY	(320)

#endif /* _M16_VDEC_REG_BASE_H_ */

