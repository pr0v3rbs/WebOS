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
 *  main driver core implementation for sci device.
 *	sci device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.02.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "sci_ctop_cfg.h"

#include "base_types.h"
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
/**
 *	main control block for gpio device.
 *	each minor device has unique control block
 *
 */

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
unsigned int SCI_CTOP_Clock_Config(void)
{
	UINT32 chip = lx_chip_rev();
    UINT32 clock_khz;

	if (0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if (chip >= LX_CHIP_REV(H15, A0))
	{
		CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
    	CTOP_CTRL_H15A0_RdFL(TI, ctr09);

    	CTOP_CTRL_H15A0_Wr01(VIP, ctr36, sc_clk_sel, 1);  // 0 : smart card clock is from core, 1 : smart card clock is from crg(derived from xtal(24Mhz), 27Mhz & 18.432Mhz)

    	CTOP_CTRL_H15A0_Wr01(TI, ctr09, f24_sel, 0);   // 0 : select from xtal(24MHz), 1: select from 27MHz or 18.432MHz
        CTOP_CTRL_H15A0_Wr01(TI, ctr09, f27_18_sel, 0);   // 0 : select 27MHz, 1 : select 18.432MHz
    	CTOP_CTRL_H15A0_Wr01(TI, ctr09, sc_ext_sel, 3);   // 0 : original, 1 : 1/2 divier,  2 : 1/4 divider, 3: 1/6 divier,  4: 1/8 divider

    	CTOP_CTRL_H15A0_WrFL(VIP, ctr36); // sdio enable
    	CTOP_CTRL_H15A0_WrFL(TI, ctr09);

        clock_khz = 4000; // clock supported 4MHZ from CTOP
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (chip >= LX_CHIP_REV(H14, A0))
	{
		CTOP_CTRL_H14A0_RdFL(ctr94);
    	CTOP_CTRL_H14A0_RdFL(ctr01);

    	CTOP_CTRL_H14A0_Wr01(ctr94, sc_clk_sel, 1);  // 0 : smart card clock is from core, 1 : smart card clock is from crg(derived from xtal(24Mhz), 27Mhz & 18.432Mhz)

    	CTOP_CTRL_H14A0_Wr01(ctr01, f24_sel, 0);   // 0 : select from xtal(24MHz), 1: select from 27MHz or 18.432MHz
        CTOP_CTRL_H14A0_Wr01(ctr01, f27_18_sel, 0);   // 0 : select 27MHz, 1 : select 18.432MHz
    	CTOP_CTRL_H14A0_Wr01(ctr01, sc_ext_sel, 3);   // 0 : original, 1 : 1/2 divier,  2 : 1/4 divider, 3: 1/6 divier,  4: 1/8 divider

    	CTOP_CTRL_H14A0_WrFL(ctr94);
    	CTOP_CTRL_H14A0_WrFL(ctr01);

        clock_khz = 4000; // clock supported 4MHZ from CTOP
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (chip >= LX_CHIP_REV(M14, B0))
	{
    	CTOP_CTRL_M14B0_RdFL(TOP, ctr11);
    	CTOP_CTRL_M14B0_RdFL(TOP, ctr06);

    	CTOP_CTRL_M14B0_Wr01(TOP, ctr11, sc_clk_sel, 1);  // 0 : smart card clock is from core, 1 : smart card clock is from crg(derived from xtal(24Mhz), 27Mhz & 18.432Mhz)

    	CTOP_CTRL_M14B0_Wr01(TOP, ctr06, f24_sel, 0);   // 0 : select from xtal(24MHz), 1: select from 27MHz or 18.432MHz
        CTOP_CTRL_M14B0_Wr01(TOP, ctr06, f27_18_sel, 0);   // 0 : select 27MHz, 1 : select 18.432MHz
    	CTOP_CTRL_M14B0_Wr01(TOP, ctr06, sc_ext_sel, 3);   // 0 : original, 1 : 1/2 divier,  2 : 1/4 divider, 3: 1/6 divier,  4: 1/8 divider

    	CTOP_CTRL_M14B0_WrFL(TOP, ctr11);
    	CTOP_CTRL_M14B0_WrFL(TOP, ctr06);

        clock_khz = 4000; // clock supported 4MHZ from CTOP
	}
	else if (chip >= LX_CHIP_REV(M14, A0))
	{
    	CTOP_CTRL_M14_RdFL(ctr94);
    	CTOP_CTRL_M14_RdFL(ctr01);

    	CTOP_CTRL_M14_Wr01(ctr94, sc_clk_sel, 1);  // 0 : smart card clock is from core, 1 : smart card clock is from crg(derived from xtal(24Mhz), 27Mhz & 18.432Mhz)

    	CTOP_CTRL_M14_Wr01(ctr01, f24_sel, 0);   // 0 : select from xtal(24MHz), 1: select from 27MHz or 18.432MHz
        CTOP_CTRL_M14_Wr01(ctr01, f27_18_sel, 0);   // 0 : select 27MHz, 1 : select 18.432MHz
    	CTOP_CTRL_M14_Wr01(ctr01, sc_ext_sel, 3);   // 0 : original, 1 : 1/2 divier,  2 : 1/4 divider, 3: 1/6 divier,  4: 1/8 divider

    	CTOP_CTRL_M14_WrFL(ctr94);
    	CTOP_CTRL_M14_WrFL(ctr01);

        clock_khz = 4000; // clock supported 4MHZ from CTOP
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (chip >= LX_CHIP_REV(H13, A0))
	{
    	CTOP_CTRL_H13_RdFL(ctr94);
    	CTOP_CTRL_H13_RdFL(ctr01);

    	CTOP_CTRL_H13_Wr01(ctr94, sc_clk_sel, 1);  // 0 : smart card clock is from core, 1 : smart card clock is from crg(derived from xtal(24Mhz), 27Mhz & 18.432Mhz)

    	CTOP_CTRL_H13_Wr01(ctr01, f24_sel, 0);   // 0 : select from xtal(24MHz), 1: select from 27MHz or 18.432MHz
        CTOP_CTRL_H13_Wr01(ctr01, f27_18_sel, 0);   // 0 : select 27MHz, 1 : select 18.432MHz
    	CTOP_CTRL_H13_Wr01(ctr01, sc_ext_sel, 3);   // 0 : original, 1 : 1/2 divier,  2 : 1/4 divider, 3: 1/6 divier,  4: 1/8 divider

    	CTOP_CTRL_H13_WrFL(ctr94);
    	CTOP_CTRL_H13_WrFL(ctr01);

        clock_khz = 4000; // clock supported 4MHZ from CTOP
	}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
	else if (chip >= LX_CHIP_REV_L9_B0)
	{
		/* Clock setting */
    	CTOP_CTRL_L9B_RdFL(ctr27_reg_extclk_swrst);
    	CTOP_CTRL_L9B_RdFL(ctr39_reg_gpio_mux_enable);

    	/* SCI clk from crg 8 */
    	CTOP_CTRL_L9B_Wr01(ctr27_reg_extclk_swrst, sc_ext_sel, 4);
    	/* 1/8 */
    	CTOP_CTRL_L9B_Wr01(ctr27_reg_extclk_swrst, f27_18_sel, 0);

    	CTOP_CTRL_L9B_Wr01(ctr39_reg_gpio_mux_enable, sc_clk_sel, 1);

    	CTOP_CTRL_L9B_WrFL(ctr27_reg_extclk_swrst);
    	CTOP_CTRL_L9B_WrFL(ctr39_reg_gpio_mux_enable);

        clock_khz = 3750;
	}
	else if (chip >= LX_CHIP_REV_L9_A0)
	{
		/* Clock setting */
    	CTOP_CTRL_L9_RdFL(ctr27);
    	CTOP_CTRL_L9_RdFL(ctr39);

    	/* SCI clk from crg 8 */
    	CTOP_CTRL_L9_Wr01(ctr39, sc_clk_sel, 1);
    	/* 1/8 */
    	CTOP_CTRL_L9_Wr01(ctr27, sc_ext_sel, 4);

    	CTOP_CTRL_L9_WrFL(ctr27);
    	CTOP_CTRL_L9_WrFL(ctr39);

        clock_khz = 3750;
	}
#endif
    else
	{
        clock_khz = 0;
	}

    return clock_khz;
}

