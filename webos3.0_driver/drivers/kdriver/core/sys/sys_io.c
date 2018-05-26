/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  main driver implementation for sys device.
 *
 *  author		ks.hyun (ks.hyun@lge.com)
 *  version		1.0
 *  date		2010.12.13
 *  note		Additional information.
 *
 *  @addtogroup lg1150_sys
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "sys_io.h"
#include "sys_drv.h"
#include "sys_regs.h"


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


/*========================================================================================
	Implementation Group
========================================================================================*/


int SYS_IO_ReadMemArray(UINT32 addr, UINT32 size, void *data)
{
	UINT32 *io_addr;
	io_addr = (UINT32*)ioremap_nocache(addr, size);
	if(io_addr != NULL)
	{
		memcpy((void*)addr, io_addr, size);
		iounmap(io_addr);
		return 0;
	}
	return -1;
}

int SYS_IO_WriteMemArray(UINT32 addr, UINT32 size, const void *data)
{
	UINT32 *io_addr;
	io_addr = (UINT32*)ioremap_nocache(addr, size);
	if(io_addr != NULL)
	{
		memcpy(io_addr, (void*)addr, size);
		iounmap(io_addr);
		return 0;
	}
	return -1;
}


int SYS_IO_ReadRegArray(UINT32 addr, UINT32 size, void *data)
{
	UINT32 *io_addr, *aio_addr;
	int rc = 0;

	io_addr = (UINT32*)ioremap_nocache(addr, size);
	if(io_addr != NULL)
	{
		int i;
		UINT32 temp;
		UINT32 remainder;
		UINT8 *v = (UINT8*)data;

		do {
			aio_addr = (UINT32*)((UINT32)io_addr & ~0x03);
			if(aio_addr != io_addr)	/* Address is not aligned to 4 bytes */
			{
				int ulen;
				UINT8 *s = (UINT8*)&temp;

				remainder = (UINT32)io_addr & 0x03;
				temp = SYS_READ32(aio_addr);
				s += remainder;
				ulen = min(size, 4-remainder);
				for(i=0; i<ulen; i++)
					*v++ = s[i];

				aio_addr++;
				size -= ulen;
				if(size == 0) break;
			}

			remainder = size%4;
			if((UINT32)v&0x03)	/* Data is not aligned to 4 bytes */
			{
				UINT8 *s = (UINT8*)&temp;
				for(i=0; i<size/4; i++)
				{
					temp = SYS_READ32(aio_addr);
					*v++ = s[0];
					*v++ = s[1];
					*v++ = s[2];
					*v++ = s[3];
					aio_addr++;
				}
			}
			else
			{
				for(i=0; i<size/4; i++)
				{
					*((volatile UINT32*)v) = SYS_READ32(aio_addr);
					v += 4;
					aio_addr++;
				}
			}

			if(remainder)
			{
				UINT8 *s = (UINT8*)&temp;
				temp = SYS_READ32(aio_addr);
				for(i=0; i<remainder; i++)
					*v++ = s[i];
			}
		} while(0);

		iounmap(io_addr);
	}
	else
	{
		SYS_ERROR("Can't ioremap !!!\n");
		rc = -1;
	}

	return rc;
}

int SYS_IO_WriteRegArray(UINT32 addr, UINT32 size, const void *data)
{
	UINT32 *io_addr, *aio_addr;

	io_addr = (UINT32*)ioremap_nocache(addr, size);
	if(io_addr != NULL)
	{
		int i;
		UINT32 temp;
		UINT32 remainder;
		UINT8 *v = (UINT8*)data;

		do {
			aio_addr = (UINT32*)((UINT32)io_addr & ~0x03);
			if(aio_addr != io_addr)	/* Address is not aligned to 4 bytes */
			{
				int ulen;
				UINT8 *s = (UINT8*)&temp;

				remainder = (UINT32)io_addr & 0x03;
				temp = SYS_READ32(aio_addr);
				s += remainder;
				ulen = min(size, 4-remainder);
				for(i=0; i<ulen; i++)
					s[i] = *v++;
				SYS_WRITE32(aio_addr, temp);

				aio_addr++;
				size -= ulen;
				if(size == 0) break;
			}

			remainder = size%4;
			if((UINT32)v&0x03)	/* Data is not aligned to 4 bytes */
			{
				//UINT32 pv, nv;	// will modify next time. read one time
				UINT8 *s = (UINT8*)&temp;
				for(i=0; i<size/4; i++)
				{
					s[0] = *v++;
					s[1] = *v++;
					s[2] = *v++;
					s[3] = *v++;
					SYS_WRITE32(aio_addr, temp);
					aio_addr++;
				}
			}
			else
			{
				for(i=0; i<size/4; i++)
				{
					SYS_WRITE32(aio_addr, *((volatile UINT32*)v));
					v += 4;
					aio_addr++;
				}
			}

			if(remainder)
			{
				UINT8 *s = (UINT8*)&temp;
				temp = SYS_READ32(aio_addr);
				for(i=0; i<remainder; i++)
					s[i] = *v++;
				SYS_WRITE32(aio_addr, temp);
			}
		} while(0);

		iounmap(io_addr);
	}

	return 0;

}

#ifdef INCLUDE_M14_CHIP_KDRV
// TODO: Move these functions to chip directory
static int SYS_M14A0_SetSpreadSpectrum(LX_SPREAD_SPECTRUM_T *ss)
{
	struct ss_display_pll
	{
		LX_SS_RATIO_T ratio;
		UINT32	udex_ctrl;
		UINT32	msex_ctrl;
	};

	const struct ss_display_pll ss_display_pll[] =
	{
		{LX_SS_RATIO__OFF, 0x0, 0x00},
		{LX_SS_RATIO_0_75, 0x3, 0x17},
		{LX_SS_RATIO_1_00, 0x4, 0x12},
		{LX_SS_RATIO_1_25, 0x7, 0x0a},
		{LX_SS_RATIO_1_50, 0x6, 0x0c},
	};
	int i;
	int rc = -1;

	if(ss->pll == LX_SS_DISPLAY)
	{
		SYS_DEBUG("set ss disp pll, ratio %d\n", ss->ratio);
		for(i=0; i<sizeof(ss_display_pll)/sizeof(struct ss_display_pll); i++)
		{
			const struct ss_display_pll *pll = &ss_display_pll[i];
			if(ss->ratio == pll->ratio)
			{
				CTOP_CTRL_M14A0_RdFL(ctr83);
				CTOP_CTRL_M14A0_Wr01(ctr83, disp_udex_ctrl, pll->udex_ctrl);
				CTOP_CTRL_M14A0_Wr01(ctr83, disp_msex_ctrl, pll->msex_ctrl);
				CTOP_CTRL_M14A0_WrFL(ctr83);
				rc = 0;
				break;
			}
		}
	}

	return rc;
}

static int SYS_M14B0_SetSpreadSpectrum(LX_SPREAD_SPECTRUM_T *ss)
{
	struct main_pll
	{
		LX_SS_RATIO_T ratio;
		UINT32	value[2];
	};

	struct ss_display_pll
	{
		LX_SS_RATIO_T ratio;
		UINT32	udex_ctrl;
		UINT32	msex_ctrl;
	};

	const struct main_pll main_pll[] =
	{
		{LX_SS_RATIO__OFF, {0x01000000, 0x981D4000}},
		{LX_SS_RATIO_0_25, {0xC12E0000, 0x80244001}},
		{LX_SS_RATIO_0_50, {0x812E0000, 0x80244001}},
		{LX_SS_RATIO_0_75, {0x43220000, 0x80243C01}},
		{LX_SS_RATIO_1_00, {0x432E0000, 0x80243C01}},
	};

	const struct ss_display_pll ss_display_pll[] =
	{
		{LX_SS_RATIO__OFF, 0x0, 0x00},
		{LX_SS_RATIO_0_75, 0x3, 0x17},
		{LX_SS_RATIO_1_00, 0x4, 0x12},
		{LX_SS_RATIO_1_25, 0x7, 0x0a},
		{LX_SS_RATIO_1_50, 0x6, 0x0c},
	};

	int i;
	int rc = -1;

	if(ss->pll == LX_SS_MAIN_PLL_1 || ss->pll == LX_SS_MAIN_PLL_2)
	{
		SYS_DEBUG("set ss main pll %d, ratio %d\n", ss->pll - 1, ss->ratio);
		for(i=0; i<sizeof(main_pll)/sizeof(struct main_pll); i++)
		{
			const struct main_pll *pll = &main_pll[i];
			if(ss->ratio == pll->ratio)
			{
				if(ss->pll == LX_SS_MAIN_PLL_1)
				{
					CTOP_CTRL_M14B0_Wr(TOP, ctr02, pll->value[0]);
					CTOP_CTRL_M14B0_Wr(TOP, ctr03, pll->value[1]);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr02);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr03);
				}
				else
				{
					CTOP_CTRL_M14B0_Wr(TOP, ctr04, pll->value[0]);
					CTOP_CTRL_M14B0_Wr(TOP, ctr05, pll->value[1]);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr04);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr05);
				}
				rc = 0;
				break;
			}
		}
	}
	else if(ss->pll == LX_SS_DISPLAY)
	{
		SYS_DEBUG("set ss disp pll, ratio %d\n", ss->ratio);
		for(i=0; i<sizeof(ss_display_pll)/sizeof(struct ss_display_pll); i++)
		{
			const struct ss_display_pll *pll = &ss_display_pll[i];
			if(ss->ratio == pll->ratio)
			{
				CTOP_CTRL_M14B0_RdFL(LEFT, ctr93);
				CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_udex_ctrl, pll->udex_ctrl);
				CTOP_CTRL_M14B0_Wr01(LEFT, ctr93, disp_msex_ctrl, pll->msex_ctrl);
				CTOP_CTRL_M14B0_WrFL(LEFT, ctr93);
				rc = 0;
				break;
			}
		}
	}

	return rc;
}
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
static int SYS_H15A0_SetSpreadSpectrum(LX_SPREAD_SPECTRUM_T *ss)
{
	int rc = -1;

	if(ss->pll == LX_SS_CPU_PLL)
	{
		SYS_DEBUG("set ss cpu pll, ratio %d\n", ss->ratio);
		rc = 0;
	}
	else if(ss->pll == LX_SS_MAIN_PLL_0 ||
			ss->pll == LX_SS_MAIN_PLL_1 ||
			ss->pll == LX_SS_MAIN_PLL_2)
	{
		SYS_DEBUG("set ss main pll %d, ratio %d\n", ss->pll - 1, ss->ratio);
		rc = 0;
	}
	else if(ss->pll == LX_SS_DISPLAY)
	{
		SYS_DEBUG("set ss disp pll, ratio %d\n", ss->ratio);
		rc = 0;
	}
	else
	{

	}

	return rc;
}
#endif

/********************************************************************************************/



int SYS_IO_SetSpreadSpectrum(LX_SPREAD_SPECTRUM_T *ss)
{

	if(unlikely(ss == NULL))
		return -1;

	if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(H15,A0))
	{
		return SYS_H15A0_SetSpreadSpectrum(ss);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{

	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14,A0))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
			return SYS_M14B0_SetSpreadSpectrum(ss);
		}
		else
		{
			return SYS_M14A0_SetSpreadSpectrum(ss);
		}
	}
#endif

	return -1;
}

/** @} */

