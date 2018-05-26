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
 *  main configuration file for i2c device
 *	i2c device will teach you how to make device driver with new platform.
 *
 *  author		ingyu.yang (ingyu.yang@lge.com)
 *  				jun.kong (jun.kong@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_i2c
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>

#include "i2c_cfg.h"
#include "i2c_kapi.h"
#include "i2c_regs.h"
#include "os_util.h"

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

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
#ifdef INCLUDE_L8_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapL8Ax[] =
{
	{L8_I2C0_BASE, 		L8_IRQ_I2C0},
	{L8_I2C1_BASE, 		L8_IRQ_I2C1},
	{L8_I2C2_BASE, 		L8_IRQ_I2C2},
	{L8_I2C3_BASE, 		L8_IRQ_I2C3},
};

static LX_I2C_HW_MAP_T _stI2cHwMapL8Bx[] =
{
	{L8_I2C0_BASE, 		L8_IRQ_I2C0},
	{L8_I2C1_BASE, 		L8_IRQ_I2C1},
	{L8_I2C2_BASE, 		L8_IRQ_I2C2},
	{L8_I2C3_BASE, 		L8_IRQ_I2C3},
	{L8_I2C4_BASE, 		L8_IRQ_I2C4},
	{L8_I2C5_BASE,		L8_IRQ_I2C5},
};
#endif

#ifdef INCLUDE_L9_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapL9Ax[] =
{
	{L9_I2C0_BASE, 		L9_IRQ_I2C0},
	{L9_I2C1_BASE, 		L9_IRQ_I2C1},
	{L9_I2C2_BASE, 		L9_IRQ_I2C2},
	{L9_I2C3_BASE, 		L9_IRQ_I2C3},
	{L9_I2C4_BASE, 		L9_IRQ_I2C4},
	{L9_I2C5_BASE,		L9_IRQ_I2C5},
	{L9_I2C6_BASE,		L9_IRQ_I2C6},
	{L9_I2C7_BASE,		L9_IRQ_I2C7},
	{L9_I2C8_BASE, 		L9_IRQ_I2C8}
};
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapH13Ax[] =
{
	{H13_I2C0_BASE, 	H13_IRQ_I2C0},
	{H13_I2C1_BASE, 	H13_IRQ_I2C1},
	{H13_I2C2_BASE, 	H13_IRQ_I2C2},
	{H13_I2C3_BASE, 	H13_IRQ_I2C3},
	{H13_I2C4_BASE, 	H13_IRQ_I2C4},
	{H13_I2C5_BASE,		H13_IRQ_I2C5},
	{H13_I2C6_BASE,		H13_IRQ_I2C6},
	{H13_I2C7_BASE,		H13_IRQ_I2C7},
	{H13_I2C8_BASE, 	H13_IRQ_I2C8},
	{H13_I2C9_BASE, 	H13_IRQ_I2C9}
};
#endif


#ifdef INCLUDE_M14_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapM14Ax[] =
{
	{M14_I2C0_BASE, 	M14_IRQ_I2C0},
	{M14_I2C1_BASE, 	M14_IRQ_I2C1},
	{M14_I2C2_BASE, 	M14_IRQ_I2C2},
	{M14_I2C3_BASE, 	M14_IRQ_I2C3},
	{M14_I2C4_BASE, 	M14_IRQ_I2C4},
	{M14_I2C5_BASE,		M14_IRQ_I2C5},
	{M14_I2C6_BASE,		M14_IRQ_I2C6},
	{M14_I2C7_BASE,		M14_IRQ_I2C7},
	{M14_I2C8_BASE, 	M14_IRQ_I2C8},
	{M14_I2C9_BASE, 	M14_IRQ_I2C9}
};
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapH14Ax[] =
{
	{H14_I2C0_BASE, 	H14_IRQ_I2C0},
	{H14_I2C1_BASE, 	H14_IRQ_I2C1},
	{H14_I2C2_BASE, 	H14_IRQ_I2C2},
	{H14_I2C3_BASE, 	H14_IRQ_I2C3},
	{H14_I2C4_BASE, 	H14_IRQ_I2C4},
	{H14_I2C5_BASE,		H14_IRQ_I2C5},
	{H14_I2C6_BASE,		H14_IRQ_I2C6},
	{H14_I2C7_BASE,		H14_IRQ_I2C7},
	{H14_I2C8_BASE, 	H14_IRQ_I2C8},
	{H14_I2C9_BASE, 	H14_IRQ_I2C9}
};
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
static LX_I2C_HW_MAP_T _stI2cHwMapH15Ax[] =
{
       {H15_I2C0_BASE,         H15_IRQ_I2C0},
       {H15_I2C1_BASE,         H15_IRQ_I2C1},
       {H15_I2C2_BASE,         H15_IRQ_I2C2},
       {H15_I2C3_BASE,         H15_IRQ_I2C3},
       {H15_I2C4_BASE,         H15_IRQ_I2C4},
       {H15_I2C5_BASE,         H15_IRQ_I2C5},
       {H15_I2C6_BASE,         H15_IRQ_I2C6},
       {H15_I2C7_BASE,         H15_IRQ_I2C7},
       {H15_I2C8_BASE,         H15_IRQ_I2C8},
       {H15_I2C9_BASE,         H15_IRQ_I2C9}
};
#endif


static LX_I2C_CFG_T _stI2cDefaultConfigs[] =
{



#ifdef INCLUDE_L8_CHIP_KDRV
	{
		.chip				= LX_CHIP_REV(L8,A0),
		.num_hw_devices		= 4,
		.clock_freq			= I2C_CLOCK_FREQUENCY_L8,
		.hw_map				= _stI2cHwMapL8Ax,
		.max_gpio_devices	= 3,
	},
	{
		.chip				= LX_CHIP_REV(L8,B0),
		.num_hw_devices		= 6,
		.clock_freq			= I2C_CLOCK_FREQUENCY_L8,
		.hw_map				= _stI2cHwMapL8Bx,
		.max_gpio_devices	= 1,
	},
#endif

#ifdef INCLUDE_L9_CHIP_KDRV
	{
		.chip				= LX_CHIP_REV(L9,A0),
		.num_hw_devices		= 9,
		.clock_freq			= I2C_CLOCK_FREQUENCY_L9,
		.hw_map				= _stI2cHwMapL9Ax,
		.max_gpio_devices	= 1,
	},
	{
		.chip				= LX_CHIP_REV(L9,A1),
		.num_hw_devices		= 9,
		.clock_freq			= I2C_CLOCK_FREQUENCY_L9,
		.hw_map				= _stI2cHwMapL9Ax,
		.max_gpio_devices	= 1,
	},
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
	{
		.chip				= LX_CHIP_REV(H13,A0),
		.num_hw_devices 	= 10,
		.clock_freq 		= I2C_CLOCK_FREQUENCY_H13,
		.hw_map 			= _stI2cHwMapH13Ax,
		.max_gpio_devices	= 8,
	},
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
	{
		.chip				= LX_CHIP_REV(M14,A0),
		.num_hw_devices 	= 10,
		.clock_freq 		= I2C_CLOCK_FREQUENCY_M14,
		.hw_map 			= _stI2cHwMapM14Ax,
		.max_gpio_devices	= 8,
	},
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
	{
		.chip				= LX_CHIP_REV(H14,A0),
		.num_hw_devices 	= 10,
		.clock_freq 		= I2C_CLOCK_FREQUENCY_H14,
		.hw_map 			= _stI2cHwMapH14Ax,
		.max_gpio_devices	= 8,
	},
#endif
#ifdef INCLUDE_H15_CHIP_KDRV
       {
           .chip                   = LX_CHIP_REV(H15,A0),
           .num_hw_devices         = 10,
           .clock_freq             = I2C_CLOCK_FREQUENCY_H15,
           .hw_map                 = _stI2cHwMapH15Ax,
           .max_gpio_devices       = 4,				//i2cgpio
       },
#endif

};


/*========================================================================================
	Implementation Group
========================================================================================*/
LX_I2C_CFG_T* I2C_GetConfig(void)
{
	int i;
	LX_I2C_CFG_T* config = NULL;
	UINT32 chip = lx_chip_rev();

	for(i=0; i < sizeof(_stI2cDefaultConfigs)/sizeof(LX_I2C_CFG_T); i++)
	{
		if(chip >= _stI2cDefaultConfigs[i].chip)
		{
			config = &_stI2cDefaultConfigs[i];
		}
		else
		{
			break;
		}
	}

	return config;
}


/** @} */

