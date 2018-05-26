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
 *  main configuration file for venc device
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.05.19
 *  note		Additional information.
 *
 *  @addtogroup lg1152_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include "os_util.h"
#include "venc_cfg.h"
#include "venc_drv.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#ifdef INCLUDE_H15_CHIP_KDRV
#define H15_VENC_BASE_CORE0     (H15_VENC_BASE)
#define H15_VENC_BASE_CORE1     (H15_VENC_BASE + 0x800)
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/
LX_VENC_MEM_CFG_S_T	*gpstVencMemConfig;
LX_VENC_CFG_T const	*gpstVencConfig;

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
LX_VENC_MEM_CFG_T	gMemCfgVenc = {};
//LX_VENC_MEM_CFG_S_T gMemCfgVencInternal[VENC_MAX_MEM_CONFIG] =
LX_VENC_MEM_CFG_S_T gMemCfgVencInternal[] =
{
	// For H13B0 (H1Encode) (Total = 0x02300000, 35Mb) (FHD)
	{
		.pcInBufName	= "VencBuf",		// for DE Frame buffer (FHD 3 Frames)
		.uiInBufBase	= 0,
		.uiInBufSize	= 0x00CC0000,

		.pcVencBufName	= "VencBuffer", 	// for working memory
		.uiVencBufBase	= 0,
		.uiVencBufSize	= 0x01740000,		//
	},

	// For M14 (H1Encode) (Total = 0x01440000, 23.81Mb->20.25) (HD)
	{
		// HD Single
		.pcInBufName	= "VencBuf",		// for DE Frame buffer ( 1280(2048) x 720 x 16bits x 3 frames )
		.uiInBufBase	= 0,
		.uiInBufSize	= 0x00C00000,		// 2048*1024*6 (12MB)

		.pcVencBufName	= "VencBuffer", 	// for working memory
		.uiVencBufBase	= 0,
		.uiVencBufSize	= 0x00800000,		// 8Mb
	},

	// For H15
	{
		// FHD Dual Channel
		.pcInBufName	= "VencBuf",		// for DE Frame buffer
		.uiInBufBase	= 0,
		.uiInBufSize	= 0x01800000,		// 2048*1024*12 (24MB) ( 2048 x 1088 x 16bits x 3 frames )
		//.uiInBufSize	= 0x02000000,		// 2048*1024*12 (32MB) ( 2048 x 1088 x 16bits x 4 frames )

		.pcVencBufName	= "VencBuffer", 	// for working memory
		.uiVencBufBase	= 0,
		.uiVencBufSize	= 0x02800000,		// 40Mb
	},

};

static LX_VENC_MEM_USAGE_T _MemUsageHD =
{
	.total = 0x01400000,	// 20Mb
	.esb = {
		.name = "ESB",
		.size = 0x001F0000
	},
	.auib = {
		.name = "AUIB",
		.size = 0x00010000
	},
	.cpb = {
		.name = "CPB",
		.size = 0x000E0000
	},
	.input = {
		.name = "INPUT",
		.size = 0x00C00000
	},
	.thumb = {
		.name = "THUMB",
		.size = 0x0021C000
	},
	.hxenc = {
		.name = "HXENC MEM",
		.size = 0x00304000
	}
};

static LX_VENC_MEM_USAGE_T _MemUsageFHD =
{
	.total = 0x02000000,	// 32Mb
	//.total = 0x02300000,	// 35Mb
	.esb = {
		.name = "ESB",
		.size = 0x003F0000
	},
	.auib = {
		.name = "AUIB",
		.size = 0x00020000
	},
	.cpb = {
		.name = "CPB",
		.size = 0x00200000
	},
	.input = {
		.name = "INPUT",
		.size = 0x00C00000
	},
	.thumb = {
		.name = "THUMB",
		.size = 0x00300000
	},
	.hxenc = {
		.name = "HXENC MEM",
		.size = 0x00C00000
	}
};

/*
static LX_VENC_MEM_USAGE_T _MemUsageFHD_Testing =
{
	.total = 0x01800000,	// 24.0Mb
	.esb = {
		.name = "ESB",
		.size = 0x00100000
	},
	.auib = {
		.name = "AUIB",
		.size = 0x00008000
	},
	.cpb = {
		.name = "CPB",
		.size = 0x00080000
	},
	.input = {
		.name = "INPUT",
		.size = 0x00C00000
	},
	.thumb = {
		.name = "THUMB",
		.size = 0x00000000
	},
	.hxenc = {
		.name = "HXENC MEM",
		.size = 0x00678000	// 6.47Mb
	}
};
*/

const LX_VENC_CFG_T	 gVencCfgTable[] = {
	[0]	= {
		.chip = LX_CHIP_REV( H13, A0 ),
		.num_device = 1,
		.cfg_flag = LX_VENC_CFG_FLAG_NONE,
		.mem_cfg = &gMemCfgVencInternal[0],
		.core = {
			[0] = {
				.reg_base_core = H13_VENC_BASE,
				.reg_size_core = 0x600,
				.reg_base_internal = H13_VENC_BASE + 0x800,
				.reg_size_internal = 0x100,
				.irq_num = H13_IRQ_VENC0,
				.hw_clock = 300000000,	// 300Mhz
				.hw_clock_mhz = 300,

				.mem_usage = &_MemUsageFHD,
			},
		}
	},

	[1]	= {
		.chip = LX_CHIP_REV( M14, A0 ),
		.num_device = 1,
		.mem_cfg = &gMemCfgVencInternal[1],
		.cfg_flag = LX_VENC_CFG_FLAG_NONE,
		.core = {
			[0] = {
				.reg_base_core = M14_A0_VENC_BASE,
				.reg_size_core = 0x600,
				.reg_base_internal = M14_A0_VENC_BASE + 0x800,
				.reg_size_internal = 0x100,
				.irq_num = M14_A0_IRQ_VENC0,

				.hw_clock = 300000000,	// 300Mhz
				.hw_clock_mhz = 300,

				.mem_usage = &_MemUsageHD,
			}
		}
	},

	[2] = {
		.chip = LX_CHIP_REV( M14, B0 ),
		.num_device = 1,
		.mem_cfg = &gMemCfgVencInternal[1],
		.cfg_flag = LX_VENC_CFG_FLAG_NONE,
		.core = {
			[0] = {
				.reg_base_core = M14_B0_VENC_BASE,
				.reg_size_core = 0x600,
				.reg_base_internal = M14_B0_VENC_BASE + 0x800,
				.reg_size_internal = 0x100,
				.reg_base_external = 0xC0025300,
				.reg_size_external = 0x100,
				.irq_num = M14_B0_IRQ_VENC0,
				.hw_clock = 320000000,	// 320Mhz
				.hw_clock_mhz = 320,

				.mem_usage = &_MemUsageHD,
			},
		}
	},

	[3] = {
		.chip = LX_CHIP_REV( H14, A0 ),
		.num_device = 1,
		.mem_cfg = &gMemCfgVencInternal[0],
		.cfg_flag = LX_VENC_CFG_FLAG_NONE,
		.core = {
			[0] = {
				.reg_base_core = H14_VENC_BASE,
				.reg_size_core = 0x600,
				.reg_base_internal = H14_VENC_BASE + 0x800,
				.reg_size_internal = 0x100,
				.irq_num = H14_IRQ_VENC0,
				.hw_clock = 320000000,	// 320Mhz
				.hw_clock_mhz = 320,

				.mem_usage = &_MemUsageFHD,
			},
		}
	},

#ifdef INCLUDE_H15_CHIP_KDRV
#if 0
	[4] = {
		.chip = LX_CHIP_REV( H15, A0 ),
		.num_device = 2,
		.mem_cfg = &gMemCfgVencInternal[2],
		.cfg_flag = LX_VENC_CFG_FLAG_PM,
		.core = {
			[0] = {
				.reg_base_core = H15_VENC_BASE_CORE0,
				.reg_size_core = 0x600,
				.reg_base_internal = H15_VENC_BASE_CORE0 + 0x700,
				.reg_size_internal = 0x100,
				.reg_base_external = 0xC8026380,
				.reg_size_external = 0x30,
				.irq_num = H15_IRQ_VENC0,
				.hw_clock = 396000000,	// 396Mhz
				.hw_clock_mhz = 396,

				.mem_usage = &_MemUsageFHD,
			},
			[1] = {
				.reg_base_core = H15_VENC_BASE_CORE1,
				.reg_size_core = 0x600,
				.reg_base_internal = H15_VENC_BASE_CORE1 + 0x700,
				.reg_size_internal = 0x100,
				.reg_base_external = 0xC80263B0,
				.reg_size_external = 0x30,
				.irq_num = H15_IRQ_VENC1,
				.hw_clock = 396000000,	// 396Mhz
				.hw_clock_mhz = 396,

				.mem_usage = &_MemUsageFHD,
			},
		}
	},
#else
	[4] = {
		.chip = LX_CHIP_REV( H15, A0 ),
		.num_device = 1,
		.mem_cfg = &gMemCfgVencInternal[1],
		.cfg_flag = LX_VENC_CFG_FLAG_PM,
		.core = {
			[0] = {
				.reg_base_core = H15_VENC_BASE_CORE0,
				.reg_size_core = 0x600,
				.reg_base_internal = H15_VENC_BASE_CORE0 + 0x700,
				.reg_size_internal = 0x100,
				.reg_base_external = 0xC8026380,
				.reg_size_external = 0x30,
				.irq_num = H15_IRQ_VENC0,
				.hw_clock = 396000000,	// 396Mhz
				.hw_clock_mhz = 396,

				.mem_usage = &_MemUsageHD,
			},
		}
	},
#endif
#endif

#define M16_A0_VENC_BASE		(0xC8B06000)	// modified

#define M16_A0_IRQ_VENC0		(M14_IRQ_GIC_START + 50)
#define M16_A0_IRQ_VENC1		(M14_IRQ_GIC_START + 51)

	[5] = {
		.chip = LX_CHIP_REV( M16, A0 ),
		.num_device = 1,
		.mem_cfg = &gMemCfgVencInternal[1],
		.cfg_flag = LX_VENC_CFG_FLAG_NONE,
		.core = {
			[0] = {
				.reg_base_core = M16_A0_VENC_BASE,
				.reg_size_core = 0x600,
				.reg_base_internal = M16_A0_VENC_BASE + 0x700,
				.reg_size_internal = 0x100,
				.reg_base_external = 0xC0025300,				// TOBE From DE registre
				.reg_size_external = 0x30,
				.irq_num = M16_A0_IRQ_VENC0,	// TOBE 0x50
				.hw_clock = 396000000,	// 396Mhz
				.hw_clock_mhz = 396,

				.mem_usage = &_MemUsageFHD,
			},
		}
	},
};

/*========================================================================================
	Implementation Group
========================================================================================*/
static int _VENC_CFG_InternalMmoryMap( LX_VENC_MEM_CFG_T *pstMemCFG, LX_VENC_MEM_CFG_S_T	*pstVencInternalMemCFG )
{
	if ( pstMemCFG == NULL )
	{
		return RET_ERROR;
	}
	else
	{
		UINT32	StartAddr = 0;
		UINT32	EndAddr = 0;
		UINT32	TotalSize = 0, AllocatedSize = 0;

		LX_MEMCFG_T*	pM = (LX_MEMCFG_T*)pstVencInternalMemCFG;
		int 			nM = sizeof(*pstVencInternalMemCFG)/sizeof(LX_MEMCFG_T);

		StartAddr =  pstMemCFG->uiMEMBase;
		EndAddr = pstMemCFG->uiMEMBase;
		AllocatedSize = pstMemCFG->uiMEMSize;

		for ( ; nM > 0; nM--, pM++)
		{

			pM->base = EndAddr;
			EndAddr += pM->size;

			VENC_DEBUG("[MEM_CFG] %12s: 0x%08X ~ 0x%08X [0x%08X]\n", pM->name, pM->base, pM->base + pM->size, pM->size);
		}

		TotalSize = EndAddr - StartAddr;

		VENC_DEBUG("[MEM_CFG] -----------------------------------------------------------\n");
		VENC_DEBUG("[MEM_CFG] %12s: 0x%08X ~ 0x%08X [0x%08X]\n", "Total", StartAddr, EndAddr, TotalSize);

		if ( TotalSize > AllocatedSize )
		{
			VENC_ERROR("Not enough allocated memory. (Allocated: 0x%08X, Required: 0x%08X)\n",
				AllocatedSize, TotalSize);

			return RET_ERROR;
		}
	}

	return RET_OK;
}

//void VENC_CFG_Init( LX_VENC_CFG_T *venc_cfg )
int VENC_CFG_Init( void )
{
	int ret = RET_ERROR;
	int i, flag = 0;
	unsigned int chip_rev = lx_chip_rev();

	//printk("chip_rev : 0x%X\n", chip_rev);

	for(i = 0; i < NELEMENTS(gVencCfgTable); i++)
	{
		//printk("gVencCfgTable chip_rev : 0x%X\n", gVencCfgTable[i].chip);
		if(chip_rev == gVencCfgTable[i].chip)
		{
			flag = 1;
			gpstVencConfig = &gVencCfgTable[i];

			VENC_NOTI("found chip rev 0x%x\n", chip_rev);
			ret = RET_OK;
			break;
		}
	}

	if ( !flag )
	{
		int shift = 0;

		while(1)
		{
			shift += 4;

			if (shift > 8)
			{
				break;
			}

			chip_rev = lx_chip_rev() >> shift;
			//printk("chip_rev : 0x%X\n", chip_rev);

			for(i = 0; i < NELEMENTS(gVencCfgTable); i++)
			{
				//printk("gVencCfgTable chip_rev : 0x%X\n", gVencCfgTable[i].chip >> shift);

				if(chip_rev == gVencCfgTable[i].chip >> shift)
				{
					gpstVencConfig = &gVencCfgTable[i];

					flag = 1;
					break;
				}
				else if(chip_rev < gVencCfgTable[i].chip >> shift)
				{
					gpstVencConfig = &gVencCfgTable[i-1];

					flag = 1;
					break;
				}
			}

			if(flag)
			{
				VENC_NOTI("matched chip rev 0x%0X to 0x%X\n", lx_chip_rev(), gpstVencConfig->chip);
				ret = RET_OK;
				break;
			}
		}
	}

#if 1
	if ( gpstVencConfig == NULL )
	{
		return RET_ERROR;
	}

	gMemCfgVenc.uiMEMSize = 0;

	for( i = 0; i < gpstVencConfig->num_device; i++ )
	{
		gMemCfgVenc.uiMEMSize += gpstVencConfig->core[i].mem_usage->total;
	}

	gMemCfgVenc.pcMEMName = "VENC MEM";
	gMemCfgVenc.uiMEMBase = 0x0;

	gpstVencMemConfig = gpstVencConfig->mem_cfg;
#else
	VENC_CFG_MemoryMap();
#endif

	return ret;
}

void VENC_CFG_MemoryMap ( void )
{
	_VENC_CFG_InternalMmoryMap( &gMemCfgVenc, gpstVencMemConfig );
}

/** @} */

