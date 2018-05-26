/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.

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
 *  main configuration file for mjpeg device
 *	mjpeg device will teach you how to make device driver with new platform.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.04.09
 *  note		Additional information.
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include "base_types.h"
#include "os_util.h"
#include "mjpeg_cfg.h"
#include "mjpeg_kapi.h"
#include "mjpeg_drv.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef struct
{
	UINT32			chip;
	LX_MJPEG_CFG_T	cfg;
	LX_MJPEG_DEF_T	def;
}
MJPEG_CFG_TBL_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/
MJPEG_MEM_CFG_T gMemCfgMjpeg =
{
	.pcMemory_name = "mjpeg_buffer",
	.ui32Memory_base = 0x00000000,
	//.ui32Memory_size = 0x00200000	/* 2Mb */
	.ui32Memory_size = 0x00100000	/* 1Mb */
};

/** JPG H/W decoder configuration
 *
 */
MJPEG_CFG_TBL_T	g_mjpeg_cfg_table[] =
{

	[0] = { .chip = LX_CHIP_REV( H13, A0 ),
			.cfg  = { 	.max_decode_width 	= 8<<10,
						.max_decode_height	= 8<<10,
						.decode_caps 		= LX_MJPEG_DECODE_CAPS_NONE },
			.def  = {	.jpeg_reg_base		= H13_IOCD_BASE,
						.icod_reg_base		= H13_IOCD_BASE + 0x0C00,
						.irq_num			= H13_IRQ_ICOD1,
						.num_device			= 1 							}
	},

	[1] = { .chip = LX_CHIP_REV( H13, B0 ),
			.cfg  = {	.max_decode_width 	= 32<<10,
						.max_decode_height	= 32<<10,
						.decode_caps 		= LX_MJPEG_DECODE_CAPS_PARTIAL },
			.def  = {	.jpeg_reg_base		= H13_IOCD_BASE,
						.icod_reg_base		= H13_IOCD_BASE + 0x0C00,
						.irq_num			= H13_IRQ_ICOD1,
						.num_device 		= 1								}
	},

	[2] = { .chip = LX_CHIP_REV( M14, A0 ),
			.cfg  = {	.max_decode_width 	= 32<<10,
						.max_decode_height	= 32<<10,
						.decode_caps 		= LX_MJPEG_DECODE_CAPS_PARTIAL },
			.def  = {	.jpeg_reg_base		= M14_A0_IOCD_BASE,
						.icod_reg_base		= M14_A0_IOCD_BASE + 0x0C00,
						.irq_num			= M14_A0_IRQ_ICOD1,
						.num_device 		= 1								}
	},

	[3] = { .chip = LX_CHIP_REV( M14, B0 ),
			.cfg  = {	.max_decode_width 	= 32<<10,
						.max_decode_height	= 32<<10,
						.decode_caps 		= LX_MJPEG_DECODE_CAPS_PARTIAL },
			.def  = {	.jpeg_reg_base		= M14_A0_IOCD_BASE,
						.icod_reg_base		= M14_A0_IOCD_BASE + 0x0C00,
						.irq_num			= M14_B0_IRQ_ICOD1,
						.num_device 		= 1								}
	},

	[4] = { .chip = LX_CHIP_REV( H14, A0 ),
			.cfg  = {	.max_decode_width 	= 32<<10,
						.max_decode_height	= 32<<10,
						.decode_caps 		= LX_MJPEG_DECODE_CAPS_PARTIAL },
			.def  = {	.jpeg_reg_base		= H14_IOCD_BASE,
						.icod_reg_base		= H14_IOCD_BASE + 0x0C00,
						.irq_num			= H14_IRQ_ICOD1,
						.num_device 		= 1								}
	},

	[5] = { .chip = LX_CHIP_REV( H15, A0 ),
			.cfg  = {   .max_decode_width   = 32<<10,
						.max_decode_height  = 32<<10,
						.decode_caps        = LX_MJPEG_DECODE_CAPS_PARTIAL },
			.def  = {   .jpeg_reg_base      = H15_ICOD_BASE,
						.icod_reg_base      = H15_ICOD_BASE + 0x0C00,
						.irq_num            = H15_IRQ_ICOD1,
						.num_device         = 1                            }
	},
};

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/
LX_MJPEG_CFG_T*	MJPEG_GetCfg( void )
{
	int	i;
	int num_cfg = NELEMENTS( g_mjpeg_cfg_table );

	UINT32	chip_rev = lx_chip_rev();

	for ( i=num_cfg; i>0; i--)
	{
		if (  chip_rev >= g_mjpeg_cfg_table[i-1].chip )
		{
			return &(g_mjpeg_cfg_table[i-1].cfg);
		}
	}

	return NULL;
}

LX_MJPEG_DEF_T *MJPEG_GetDef( void )
{
	//int ret = RET_ERROR;
	int i;
	unsigned int chip_rev = lx_chip_rev();
	MJPEG_CFG_TBL_T *pConfig = NULL;

	int shift = 0;

	while(1)
	{
		chip_rev = lx_chip_rev() >> shift;
		//printk("chip_rev : 0x%X\n", chip_rev);

		for(i = 0; i < NELEMENTS(g_mjpeg_cfg_table); i++)
		{
			//printk("g_mjpeg_cfg_table chip_rev : 0x%X\n", g_mjpeg_cfg_table[i].chip >> shift);

			if (shift == 0)
			{
				if(chip_rev == g_mjpeg_cfg_table[i].chip)
				{
					pConfig = &g_mjpeg_cfg_table[i];
					break;
				}
			}
			else
			{
				if(chip_rev == g_mjpeg_cfg_table[i].chip >> shift)
				{
					pConfig = &g_mjpeg_cfg_table[i];
					break;
				}
				else if(chip_rev < g_mjpeg_cfg_table[i].chip >> shift)
				{
					pConfig = &g_mjpeg_cfg_table[i-1];
					break;
				}
			}
		}

		if (pConfig != NULL)
			break;
		
		shift += 4;
		if (shift > 8)
		{
			break;
		}
	}

	if( pConfig != NULL )
	{
		MJPEG_NOTI("matched chip rev 0x%0X to 0x%X\n", lx_chip_rev(), pConfig->chip);
		//pMJPEGDef->platform = lx_chip_plt();
		//pMJPEGDef->devno = MKDEV(MJPEG_MAJOR, MJPEG_MINOR);

		return &pConfig->def;
	}
	else
	{
		// Default(H13)
		return &g_mjpeg_cfg_table[0].def;
	}
}

/** @} */

