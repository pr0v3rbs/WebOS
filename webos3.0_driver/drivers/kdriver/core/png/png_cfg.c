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
 *  main configuration file for png device
 *	png device will teach you how to make device driver with new platform.
 *
 *  author		Juhee Park
 *  version		1.0
 *  date		2011.12.02
 *  note		Additional information.
 *
 *  @addtogroup lg_png
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/slab.h>

#include <asm/io.h>

#include "os_util.h"
#include "base_device.h"

#include "png_cfg.h"
#include "png_kapi.h"

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
	global Variables
----------------------------------------------------------------------------------------*/
// Default configuration is for L8
LX_PNG_MEM_CFG_T gMemCfgPng[] =
{
	[0] = /* L9 */
	{
		.memory_name = "PNG_L9_buffer",
		.memory_base = 0x0,
		.memory_size = 0x00800000,
	},

	[1] = /* H13 */
	{
		.memory_name = "PNG_H13_HW_buffer",
		.memory_base = 0x0, //0x7C000000,
		.memory_size = 0x00400000,
	},
};

PNG_CFG_TBL_T	g_png_cfg_table[] =
{
	[0] = { .chip = LX_CHIP_REV( H13, A0 ),
            .png_reg_base = H13_IOCD_BASE + 0x0400,
		    .icod_reg_base = H13_IOCD_BASE + 0x0C00,
            .irq_num = H13_IRQ_ICOD0,
            .num_device = 1,
	},

	[1] = { .chip = LX_CHIP_REV( H13, B0 ),
            .png_reg_base = H13_IOCD_BASE + 0x0400,
		    .icod_reg_base = H13_IOCD_BASE + 0x0C00,
            .irq_num = H13_IRQ_ICOD0,
            .num_device = 1,
	},

    [2] = { .chip = LX_CHIP_REV( M14, A0 ),
            .png_reg_base = M14_IOCD_BASE + 0x0400,
		    .icod_reg_base = M14_IOCD_BASE + 0x0C00,
            .irq_num = M14_A0_IRQ_ICOD0,
            .num_device = 1,
    },

    [3] = { .chip = LX_CHIP_REV( M14, B0 ),
            .png_reg_base = M14_IOCD_BASE + 0x0400,
		    .icod_reg_base = M14_IOCD_BASE + 0x0C00,
            .irq_num = M14_B0_IRQ_ICOD0,
            .num_device = 1,
    },

    [4] = { .chip = LX_CHIP_REV( H14, A0 ),
            .png_reg_base = H14_IOCD_BASE + 0x0400,
		    .icod_reg_base = H14_IOCD_BASE + 0x0C00,
            .irq_num = H14_IRQ_ICOD0,
            .num_device = 1,
    },
	[5] = { .chip = LX_CHIP_REV( H15, A0 ),
			.png_reg_base = H15_ICOD_BASE + 0x0400,
			.icod_reg_base = H15_ICOD_BASE + 0x0C00,
			.irq_num = H15_IRQ_ICOD0,
			.num_device = 1,
	},
};

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
int PNG_Config(PNG_CFG_TBL_T *pPNG)
{
	int ret = RET_ERROR;
    int i, flag = 0;
    unsigned int chip_rev = lx_chip_rev();

	//printk("PNG_Config\n" );

    //printk("chip_rev : 0x%X\n", chip_rev);

    for(i = 0; i < sizeof(g_png_cfg_table)/sizeof(PNG_CFG_TBL_T); i++)
    {
        //printk("g_png_cfg_table chip_rev : 0x%X\n", g_png_cfg_table[i].chip);
        if(chip_rev == g_png_cfg_table[i].chip)
        {
            flag = 1;
            break;
        }
    }

	if (flag)
	{
        memcpy(pPNG, &g_png_cfg_table[i], sizeof(PNG_CFG_TBL_T));
        memcpy(&pPNG->mem_cfg, &gMemCfgPng[1], sizeof(LX_PNG_MEM_CFG_T));
    	pPNG->platform = lx_chip_plt();
    	pPNG->devno = MKDEV(PNG_MAJOR, PNG_MINOR);

		printk("found chip rev 0x%x\n", chip_rev);
        ret = RET_OK;
	}
	else
	{
		int shift = 0;
		while(!flag)
		{
			shift += 4;

			if (shift > 8)
				break;

			chip_rev = lx_chip_rev() >> shift;
			//printk("chip_rev : 0x%X\n", chip_rev);

			for(i = 0; i < sizeof(g_png_cfg_table)/sizeof(PNG_CFG_TBL_T); i++)
			{
				//printk("g_png_cfg_table chip_rev : 0x%X\n", g_png_cfg_table[i].chip >> shift);
				if(chip_rev == g_png_cfg_table[i].chip >> shift)
				{
					memcpy(pPNG, &g_png_cfg_table[i], sizeof(PNG_CFG_TBL_T));
					memcpy(&pPNG->mem_cfg, &gMemCfgPng[1], sizeof(LX_PNG_MEM_CFG_T));
					pPNG->platform = lx_chip_plt();
					pPNG->devno = MKDEV(PNG_MAJOR, PNG_MINOR);
					flag = 1;
					break;
				}
				else if(chip_rev < g_png_cfg_table[i].chip >> shift)
				{
					memcpy(pPNG, &g_png_cfg_table[i-1], sizeof(PNG_CFG_TBL_T));
					memcpy(&pPNG->mem_cfg, &gMemCfgPng[1], sizeof(LX_PNG_MEM_CFG_T));
					pPNG->platform = lx_chip_plt();
					pPNG->devno = MKDEV(PNG_MAJOR, PNG_MINOR);
					flag = 1;
					break;
				}
			}
			if(flag)
			{
				printk("matched chip rev 0x%0X to 0x%X\n", lx_chip_rev(), pPNG->chip);
				ret = RET_OK;
				break;
			}
		}
	}

	if (!flag)
	{
        memcpy(pPNG, &g_png_cfg_table[0], sizeof(PNG_CFG_TBL_T));
        memcpy(&pPNG->mem_cfg, &gMemCfgPng[1], sizeof(LX_PNG_MEM_CFG_T));
    	pPNG->platform = lx_chip_plt();
    	pPNG->devno = MKDEV(PNG_MAJOR, PNG_MINOR);

		printk("can't found chip rev 0x%x\n", lx_chip_rev());
        ret = RET_OK;
	}

	return ret;
}

/** @} */

