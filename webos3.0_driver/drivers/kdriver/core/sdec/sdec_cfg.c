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
 *  main configuration file for sdec device
 *  sdec device will teach you how to make device driver with new platform.
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */

#include "sdec_cfg.h"
#include "os_util.h"
#include "debug_util.h"

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/





/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	base;		///< physical base address in BYTE!!! of media ip
	UINT32	size;		///< size in BYTE!!!
} LX_SDEC_MEMCFG_T;


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
LX_MEMCFG_T gMemCfgSDEC[2] =
{
	{.name = "sdec_gpb",},
	{.name = "sdec_ciplus"},
};


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
/* H13 A0 ~ M14 A0 */
static const LX_SDEC_CHA_INFO_T _sdecChannelCfg_SingleCore[] =
{
	// A
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// B
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// C
	{
		.capa_lev	= 0,	//* simple channel, just for SDT Parsing*/
		.num_pidf	= 4,	//* # of pid filter */
		.num_secf	= 4,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// D
	{
		.capa_lev	= 0,	//* simple channel, just for all pass recording */
		.num_pidf	= 0,	//* # of pid filter */
		.num_secf	= 0,	//* # of section filter */
		.flt_dept	= 0,	//* depth of section filter mask */
	},
};

static const LX_SDEC_CHA_INFO_T _sdecChannelCfg_DualCore[] =
{
	// A
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// B
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// C
	{
		.capa_lev	= 0,	//* simple channel, just for SDT Parsing*/
		.num_pidf	= 4,	//* # of pid filter */
		.num_secf	= 4,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// D
	{
		.capa_lev	= 0,	//* simple channel, just for all pass recording */
		.num_pidf	= 0,	//* # of pid filter */
		.num_secf	= 0,	//* # of section filter */
		.flt_dept	= 0,	//* depth of section filter mask */
	},
	// E
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// F
	{
		.capa_lev	= 1,	//* Full feature channel */
		.num_pidf	= 64,	//* # of pid filter */
		.num_secf	= 64,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// G
	{
		.capa_lev	= 0,	//* simple channel, just for SDT Parsing*/
		.num_pidf	= 4,	//* # of pid filter */
		.num_secf	= 4,	//* # of section filter */
		.flt_dept	= 8,	//* depth of section filter mask */
	},
	// H
	{
		.capa_lev	= 0,	//* simple channel, just for all pass recording */
		.num_pidf	= 0,	//* # of pid filter */
		.num_secf	= 0,	//* # of section filter */
		.flt_dept	= 0,	//* depth of section filter mask */
	},
};


static LX_SDEC_CFG_T _sdecHwCfg[] =
{
	[0] = /* H13, M14Ax */
	{
		.nCore			= 1,
		.nChannel		= 4,
		.nVdecOutPort	= 3,
		.noPesBug		= 1,
		.staticGPB		= 1,
		.noFixedClock	= 1,
		.irqNum			= {H13_IRQ_SDEC},
		.sdtopRegBase	= H13_SDEC_TOP_REG_BASE,
		.sdioRegBase	= {H13_SDEC_IO_REG_BASE,},
		.mpgRegBase		= {
			{H13_SDEC_MPG_REG_BASE0, H13_SDEC_MPG_REG_BASE1},
		},
		.chInfo			= _sdecChannelCfg_SingleCore
	},
	[1] = /* M14Bx */
	{
		.nCore			= 2,
		.nChannel		= 8,
		.nVdecOutPort	= 3,
		.noPesBug		= 1,
		.staticGPB		= 1,
		.sdtopRegBase	= M14_B0_SDEC_TOP_REG_BASE,
		.irqNum			= {M14_B0_IRQ_SDEC, M14_B0_IRQ_SDEC1},
		.sdioRegBase	= {M14_B0_SDEC_CORE0_IO_REG_BASE, M14_B0_SDEC_CORE1_IO_REG_BASE},
		.mpgRegBase		= {
			{M14_B0_SDEC_CORE0_MPG_REG_BASE0, M14_B0_SDEC_CORE0_MPG_REG_BASE1},
			{M14_B0_SDEC_CORE1_MPG_REG_BASE0, M14_B0_SDEC_CORE1_MPG_REG_BASE1},
		},
		.chInfo			= _sdecChannelCfg_DualCore
	},
	[2] = /* H14 */
	{
		.nCore			= 2,
		.nChannel		= 8,
		.nVdecOutPort	= 3,
		.noPesBug		= 1,
		.staticGPB		= 1,
		.sdtopRegBase	= H14_SDEC_TOP_REG_BASE,
		.irqNum			= {H14_IRQ_SDEC0, H14_IRQ_SDEC1},
		.sdioRegBase	= {H14_SDEC_CORE0_IO_REG_BASE, H14_SDEC_CORE1_IO_REG_BASE},
		.mpgRegBase		= {
			{H14_SDEC_CORE0_MPG_REG_BASE0, H14_SDEC_CORE0_MPG_REG_BASE1},
			{H14_SDEC_CORE1_MPG_REG_BASE0, H14_SDEC_CORE1_MPG_REG_BASE1},
		},
		.chInfo			= _sdecChannelCfg_DualCore
	},
	[3] = /* H15 */
	{
		.nCore			= 2,
		.nChannel		= 8,
		.nVdecOutPort	= 3,
		.noPesBug		= 1,
		.staticGPB		= 1,
		.inTsSelector	= 1,
		.instantBoot	= 1,
		.ciPlus			= 1,
		.sdtopRegBase	= H15_SDEC_TOP_REG_BASE,
		.irqNum			= {H15_IRQ_TE0, H15_IRQ_TE2},
		.sdioRegBase	= {H15_SDEC_CORE0_IO_REG_BASE, H15_SDEC_CORE1_IO_REG_BASE},
		.mpgRegBase		= {
			{H15_SDEC_CORE0_MPG_REG_BASE0, H15_SDEC_CORE0_MPG_REG_BASE1},
			{H15_SDEC_CORE1_MPG_REG_BASE0, H15_SDEC_CORE1_MPG_REG_BASE1},
		},
		.chInfo			= _sdecChannelCfg_DualCore
	},
};


static LX_SDEC_MEMCFG_T _sdecMemCfg[] =
{
	[0] = /* L9, H13, M14 */
	{
		.base = 0x0,
#if (KDRV_PLATFORM == KDRV_COSMO_PLATFORM)	// GCD
		.size = 0x00800000,
#else
		.size = 0x00900000,
#endif
	},
	[1] = /* H14, H15 */
	{
		.base = 0x0,
#if (KDRV_PLATFORM == KDRV_COSMO_PLATFORM)	// GCD
		.size = 0x01000000,
#else
		.size = 0x01200000,
#endif
	},
	[2] = /* H13 + Dx UD */
	{
		.base = 0x00000000,
		.size = 0x01100000,
	},
	[3] = /* H14 + Dx UD */
	{
		.base = 0x00000000,
		.size = 0x01900000,
	},
};

static UINT8 _sdecNormalChannel[8] = {1, 1, 0, 0, 1, 1, 0, 0};

static LX_SDEC_MEMCFG_T _sdecCiPlusMemCfg[] =
{
	[0] = /* H15 */
	{
		.base = 0x0,
		.size = 0x00A00000,
	},
};


static LX_SDEC_CFG_T *_sdecCfg;

/*========================================================================================
	Implementation Group
========================================================================================*/

void SDEC_CFG_PreInit(void)
{
	LX_SDEC_MEMCFG_T *memCfg;
	LX_SDEC_MEMCFG_T *memCiPlusCfg = NULL;
	UINT32 chip = lx_chip();

	if(0){}
	else if	(chip == LX_CHIP_H15)
	{
		memCfg = &_sdecMemCfg[1];
		_sdecCfg = &_sdecHwCfg[3];

//		if(_sdecCfg->ciPlus)
		memCiPlusCfg = &_sdecCiPlusMemCfg[0];
	}
	else if	(chip == LX_CHIP_H14)
	{
		if(lx_chip_plt() == LX_CHIP_PLT_UHD)
			memCfg = &_sdecMemCfg[3];
		else
			memCfg = &_sdecMemCfg[1];

		_sdecCfg = &_sdecHwCfg[2];
	}
	else if (chip == LX_CHIP_M14)
	{
		memCfg = &_sdecMemCfg[0];

		if(lx_chip_rev() < LX_CHIP_REV(M14,B0))
		{
			_sdecCfg = &_sdecHwCfg[0];
		}
		else
		{
			_sdecCfg = &_sdecHwCfg[1];
			if(lx_chip_rev() >= LX_CHIP_REV(M14,C0))
			{
				_sdecCfg->inTsSelector = 1;
				_sdecCfg->newCDIP = 1;
			}
		}
	}
	else if (chip == LX_CHIP_H13)
	{
		if(lx_chip_plt() == LX_CHIP_PLT_UHD)
			memCfg = &_sdecMemCfg[2];
		else
			memCfg = &_sdecMemCfg[0];

		_sdecCfg = &_sdecHwCfg[0];
	}
	else
	{
		DBG_PRINT_ERROR("Not defined CHIP !!!");
		memCfg = &_sdecMemCfg[1];
		_sdecCfg = &_sdecHwCfg[2];
	}

	gMemCfgSDEC[0].base = memCfg->base;
	gMemCfgSDEC[0].size = memCfg->size;

	/* Configure CiPlus Mem */
	if(memCiPlusCfg != NULL)
	{
		gMemCfgSDEC[1].base = memCiPlusCfg->base;
		gMemCfgSDEC[1].size = memCiPlusCfg->size;
	}

}


/**
********************************************************************************
* @brief
*	Get conf which is fit to chip revision
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*  LX_SDEC_CFG_T* : pointer of config structure
********************************************************************************
*/
LX_SDEC_CFG_T* SDEC_CFG_GetConfig(void)
{
	return _sdecCfg;
}

LX_MEMCFG_T* SDEC_CFG_GetMemConfig(void)
{
	return &gMemCfgSDEC[0];
}

LX_MEMCFG_T* SDEC_CFG_GetCiPlusMemConfig(void)
{
	return &gMemCfgSDEC[1];
}

int SDEC_CFG_IsValidChannel(UINT8 lxch)
{
	return (lxch < _sdecCfg->nChannel) ? 1 : 0;
}

int SDEC_CFG_IsNormalChannel(UINT8 lxch)
{
	if(lxch < _sdecCfg->nChannel)
	{
		return _sdecNormalChannel[lxch];
	}
	return 0;
}

/** @} */

