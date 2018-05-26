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
 *	sys device will teach you how to make device driver with new platform.
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
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "sys_drv.h"
#include "reg_ctrl.h"
#ifndef PLATFORM_FPGA
#include "i2c_drv.h"
#include "i2c_core.h"
#endif

#include "ctop_regs.h"


#include "sys_regs.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct{
	UINT8	ch;
	UINT8	clock;
} LX_I2C_INTERNAL_T;

typedef struct
{
	UINT32				chip;
	UINT32				num_internals;
	LX_I2C_INTERNAL_T*	internal;		/* channels should be inited at initial time */
} LX_REG_CTRL_CFG_T;

typedef struct
{
	UINT8				ch;
	UINT8				inited;
} LX_I2C_INFO_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 global Variables
 *---------------------------------------------------------------------------------------*/
#ifdef INCLUDE_L8_CHIP_KDRV
CTOP_CTRL_T				gCTOP_CTRL;				// CTOP for L8
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
CTOP_CTRL_REG_L9_T		gCTOP_CTRL_L9;	// CTOP for L9 A0
CTOP_CTRL_REG_L9B_T		gCTOP_CTRL_L9B;	// CTOP for L9 B0
ACE_REG_CTRL_L9_T		gACE_REG_CTRL_L9;
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
CTOP_CTRL_REG_H13_T		gCTOP_CTRL_H13;

// ACE Registers
ACE_REG_CTRL_H13_T		gACE_REG_CTRL_H13;

// CPU TOP Control Registers
CPU_TOP_REG_H13_T		gCPU_TOP_REG_H13;
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
CTOP_CTRL_REG_M14_T		gCTOP_CTRL_M14;
EXPORT_SYMBOL(gCTOP_CTRL_M14);
CTOP_CTRL_REG_M14B0_T	gCTOP_CTRL_M14B0;
EXPORT_SYMBOL(gCTOP_CTRL_M14B0);

// Analog TOP Registers
ATOP_REG_CTRL_M14_T		gATOP_REG_CTRL_M14;

// CPU TOP Control Registers
CPU_TOP_REG_M14_T		gCPU_TOP_REG_M14;
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
CTOP_CTRL_REG_H14_T		gCTOP_CTRL_H14;

// Analog TOP Registers
ATOP_REG_CTRL_H14_T		gATOP_REG_CTRL_H14;

// CPU TOP Control Registers
CPU_TOP_REG_H14_T		gCPU_TOP_REG_H14;
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
CTOP_CTRL_REG_H15_T		gCTOP_CTRL_H15;
EXPORT_SYMBOL(gCTOP_CTRL_H15);

// Analog TOP Registers
ATOP_REG_CTRL_H15_T		gATOP_REG_CTRL_H15;

// CPU TOP Control Registers
//CPU_TOP_REG_H15_T		gCPU_TOP_REG_H15;
#endif

/*----------------------------------------------------------------------------------------
 *	 Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Static Variables
 *---------------------------------------------------------------------------------------*/

#ifndef PLATFORM_FPGA
#ifdef INCLUDE_L9_CHIP_KDRV
static LX_I2C_INTERNAL_T _stI2cInternalL9A0[] ={
	{4, I2C_CLOCK_400KHZ},
};
static LX_I2C_INTERNAL_T _stI2cInternalL9A1[] = {
	{8, I2C_CLOCK_2MHZ}
};
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
static LX_I2C_INTERNAL_T _stI2cInternalH13A0[] = {
	{8, I2C_CLOCK_400KHZ},	// ACE
	{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
};
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
static LX_I2C_INTERNAL_T _stI2cInternalM14A0[] = {
	{8, I2C_CLOCK_400KHZ},	// ACE
	{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
};

static LX_I2C_INTERNAL_T _stI2cInternalM14B0[] = {
	{8, I2C_CLOCK_400KHZ},	// ACE
	{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
};


#endif

#ifdef INCLUDE_H14_CHIP_KDRV
static LX_I2C_INTERNAL_T _stI2cInternalH14A0[] = {
	{8, I2C_CLOCK_400KHZ},	// ACE			// h14bringup
	{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
};
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
static LX_I2C_INTERNAL_T _stI2cInternalH15A0[] = {
	{8, I2C_CLOCK_400KHZ},	// ACE
	{9, I2C_CLOCK_400KHZ}	// HIDMI PHY
};
#endif

static LX_REG_CTRL_CFG_T _stRegCtrlConfigs[] =
{
#ifdef INCLUDE_L9_CHIP_KDRV
	{
		.chip = LX_CHIP_REV(L9,A0),
		.num_internals = 1,
		.internal = _stI2cInternalL9A0,
	},
	{
		.chip = LX_CHIP_REV(L9,A1),
		.num_internals = 1,
		.internal = _stI2cInternalL9A1,
	},
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	{
		.chip = LX_CHIP_REV(H13,A0),
		.num_internals = 2,
		.internal = _stI2cInternalH13A0,
	},
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	{
		.chip = LX_CHIP_REV(M14,A0),
		.num_internals = 2,
		.internal = _stI2cInternalM14A0,
	},
	{
		.chip = LX_CHIP_REV(M14,B0),
		.num_internals = 2,
		.internal = _stI2cInternalM14B0,
	},
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	{
		.chip = LX_CHIP_REV(H14,A0),
		.num_internals = 2,			// h14bringup
		.internal = _stI2cInternalH14A0,
	},
#endif
#ifdef INCLUDE_H15_CHIP_KDRV
	{
		.chip = LX_CHIP_REV(H15,A0),
		.num_internals = 2,			// h14bringup
		.internal = _stI2cInternalH15A0,
	},
#endif

};

static LX_REG_CTRL_CFG_T _stDummyConfig =
{
	.num_internals = 0,
};

static LX_I2C_DEV_HANDLE *_i2c_handle;
static LX_REG_CTRL_CFG_T *_config = &_stDummyConfig;

#endif // PLATFORM_FPGA

static void InitConfig(void)
{
	int i;
	UINT32 chip = lx_chip_rev();

#ifndef PLATFORM_FPGA
	for(i=0; i < sizeof(_stRegCtrlConfigs)/sizeof(LX_REG_CTRL_CFG_T); i++)
	{
		if(chip >= _stRegCtrlConfigs[i].chip) _config = &_stRegCtrlConfigs[i];
		else break;
	}
#endif	// PLATFORM_FPGA
}


int REG_CTRL_Init(void)
{
	int regs_size;

	InitConfig();


	if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if (lx_chip() == LX_CHIP_H15)
	{
#define CTOP_H15A0_INIT(_m)	\
	do { \
		gCTOP_CTRL_H15.phys.a0._m = (H15_A0_CTOP_##_m##_TYPE *)ioremap(H15_A0_CTOP_##_m##_BASE, sizeof(H15_A0_CTOP_##_m##_TYPE)); \
		gCTOP_CTRL_H15.shdw.a0._m = (H15_A0_CTOP_##_m##_TYPE *)OS_KMalloc(sizeof(H15_A0_CTOP_##_m##_TYPE));	\
	} while(0)

		CTOP_H15A0_INIT(CVI);
		CTOP_H15A0_INIT(IMX);
		CTOP_H15A0_INIT(ND0);
		CTOP_H15A0_INIT(ND1);
		CTOP_H15A0_INIT(SUB);
		CTOP_H15A0_INIT(GSC);
		CTOP_H15A0_INIT(SR);
		CTOP_H15A0_INIT(VSD);
		CTOP_H15A0_INIT(DPE);
		CTOP_H15A0_INIT(CCO);
		CTOP_H15A0_INIT(MCU);
		CTOP_H15A0_INIT(M1);
		CTOP_H15A0_INIT(GFX);
		CTOP_H15A0_INIT(VDEC0);
		CTOP_H15A0_INIT(VDEC1);
		CTOP_H15A0_INIT(M2);
		CTOP_H15A0_INIT(VDEC2);
		CTOP_H15A0_INIT(M0);
		CTOP_H15A0_INIT(TI);
		CTOP_H15A0_INIT(AUD);
		CTOP_H15A0_INIT(VENC);
		CTOP_H15A0_INIT(VIP);
		CTOP_H15A0_INIT(GPU);
		CTOP_H15A0_INIT(CPU);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		regs_size = sizeof(CTOP_CTRL_REG_H14A0_T);
		gCTOP_CTRL_H14.phys.addr = (volatile UINT32 *)ioremap(H14_CTOP_REG_BASE, regs_size);
		gCTOP_CTRL_H14.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);

		regs_size = sizeof(CPU_TOP_REG_M14A0_T);
		gCPU_TOP_REG_H14.phys.addr = (volatile UINT32 *)ioremap(H14_TOPCTRL_BASE, regs_size);
		gCPU_TOP_REG_H14.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14,A0))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
#define CTOP_M14B0_INIT(_m)	\
	do { \
		gCTOP_CTRL_M14B0.phys._m = (M14_B0_CTOP_##_m##_TYPE *)ioremap(M14_B0_CTOP_##_m##_BASE, sizeof(M14_B0_CTOP_##_m##_TYPE)); \
		gCTOP_CTRL_M14B0.shdw._m = (M14_B0_CTOP_##_m##_TYPE *)OS_KMalloc(sizeof(M14_B0_CTOP_##_m##_TYPE));	\
	} while(0)

			CTOP_M14B0_INIT(TOP);
			CTOP_M14B0_INIT(LEFT);
			CTOP_M14B0_INIT(RIGHT);
			CTOP_M14B0_INIT(GFX);
			CTOP_M14B0_INIT(VDEC);
			CTOP_M14B0_INIT(HEVC);
			CTOP_M14B0_INIT(TE);
			CTOP_M14B0_INIT(VENC);
			CTOP_M14B0_INIT(AUD);
			CTOP_M14B0_INIT(DEI);
			CTOP_M14B0_INIT(GPU);
			CTOP_M14B0_INIT(IDSMCU);
			CTOP_M14B0_INIT(DEM);
			CTOP_M14B0_INIT(DEO);
			CTOP_M14B0_INIT(DPP);
			CTOP_M14B0_INIT(FDA);
			CTOP_M14B0_INIT(FDB);
			CTOP_M14B0_INIT(FDC);
			CTOP_M14B0_INIT(FDD);
			CTOP_M14B0_INIT(DPE);
			CTOP_M14B0_INIT(CPU_PERI);

			gCPU_TOP_REG_M14.phys.addr = (volatile UINT32 *)ioremap(M14_TOPCTRL_BASE, sizeof(CPU_TOP_REG_M14B0_T));
			gCPU_TOP_REG_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(CPU_TOP_REG_M14B0_T));
		}
		else
		{
			gCTOP_CTRL_M14.phys.addr = (volatile UINT32 *)ioremap(M14_CTOP_REG_BASE, sizeof(CTOP_CTRL_REG_M14A0_T));
			gCTOP_CTRL_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(CTOP_CTRL_REG_M14A0_T));

			gCPU_TOP_REG_M14.phys.addr = (volatile UINT32 *)ioremap(M14_TOPCTRL_BASE, sizeof(CPU_TOP_REG_M14A0_T));
			gCPU_TOP_REG_M14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(CPU_TOP_REG_M14A0_T));
		}


	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13,B0))
	{
		regs_size = sizeof(CTOP_CTRL_REG_H13B0_T);
		gCTOP_CTRL_H13.phys.addr = (volatile UINT32 *)ioremap(H13_CTOP_REG_BASE, regs_size);
		gCTOP_CTRL_H13.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);

		regs_size = sizeof(CPU_TOP_REG_H13A0_T);
		gCPU_TOP_REG_H13.phys.addr = (volatile UINT32 *)ioremap(H13_TOPCTRL_BASE, regs_size);
		gCPU_TOP_REG_H13.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H13,A0))
	{
		regs_size = sizeof(CTOP_CTRL_REG_H13A0_T);
		gCTOP_CTRL_H13.phys.addr = (volatile UINT32 *)ioremap(H13_CTOP_REG_BASE, regs_size);
		gCTOP_CTRL_H13.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);

		regs_size = sizeof(CPU_TOP_REG_H13A0_T);
		gCPU_TOP_REG_H13.phys.addr = (volatile UINT32 *)ioremap(H13_TOPCTRL_BASE, regs_size);
		gCPU_TOP_REG_H13.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(L9,A0))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(L9,B0))
		{
			regs_size = sizeof(CTOP_CTRL_REG_L9B0_T);

			gCTOP_CTRL_L9B.phys.addr = (volatile UINT32 *)ioremap(L9_TOPCTRL_BASE, regs_size);
			gCTOP_CTRL_L9B.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
		}

		/* A1 & B0 ctop are used at same time */
		regs_size = sizeof(CTOP_CTRL_REG_L9A0_T);

		gCTOP_CTRL_L9.phys.addr = (volatile UINT32 *)ioremap(L9_TOPCTRL_BASE, regs_size);
		gCTOP_CTRL_L9.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
#endif
#ifdef INCLUDE_L8_CHIP_KDRV
 	else if (lx_chip_rev() >= LX_CHIP_REV(L8,B0))
	{
		regs_size = sizeof(CTOP_CTRL_REG_B0_T);

		gCTOP_CTRL.phys.addr = (volatile UINT32 *)ioremap(L8_TOP_CTRL, regs_size);
		gCTOP_CTRL.shadow.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
	else
	{
		regs_size = sizeof(CTOP_CTRL_REG_A0_T);

		gCTOP_CTRL.phys.addr = (volatile UINT32 *)ioremap(L8_TOP_CTRL, regs_size);
		gCTOP_CTRL.shadow.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
#endif

#ifndef PLATFORM_FPGA
	// Init ACE registers and etc...
	if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H15)	/* H15 */
	{
		regs_size = sizeof(ATOP_REG_H15A0_T);	// max size
		gATOP_REG_CTRL_H15.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
		/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
		CTOP_CTRL_H15A0_RdFL(CVI,ctr00);
		CTOP_CTRL_H15A0_Wr01(CVI,ctr00,phy_i2c_sel,1);
		CTOP_CTRL_H15A0_WrFL(CVI,ctr00);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H14) 		/* H14 */
	{
		regs_size = sizeof(ATOP_REG_H14A0_T);	// max size
		gATOP_REG_CTRL_H14.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
		/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
		CTOP_CTRL_H14A0_RdFL(ctr01);
		CTOP_CTRL_H14A0_Wr01(ctr01,phy_i2c_clk_sel,1);
		CTOP_CTRL_H14A0_WrFL(ctr01);
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_M14) 		/* M14 */
	{

		if (lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
			regs_size = sizeof(ATOP_REG_M14B0_T);	// max size
			gATOP_REG_CTRL_M14.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);

			/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
			CTOP_CTRL_M14B0_RdFL(TOP,ctr06);
			CTOP_CTRL_M14B0_Wr01(TOP,ctr06, phy_i2c_clk_sel, 1);
			CTOP_CTRL_M14B0_WrFL(TOP,ctr06);

		    /* enable spi interface through disabling jtag1 interface */
			CTOP_CTRL_M14B0_RdFL(LEFT,ctr95);
			CTOP_CTRL_M14B0_Wr01(LEFT,ctr95, jtag1_disable, 1);
			CTOP_CTRL_M14B0_WrFL(LEFT,ctr95);
		}
		else
		{
			regs_size = sizeof(ATOP_REG_M14A0_T);	// max size
			gATOP_REG_CTRL_M14.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
			/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
			CTOP_CTRL_M14A0_RdFL(ctr01);
			CTOP_CTRL_M14A0_Wr01(ctr01,phy_i2c_clk_sel,1);
			CTOP_CTRL_M14A0_WrFL(ctr01);

			/* enable spi interface through disabling jtag1 interface */
			CTOP_CTRL_M14A0_RdFL(ctr96);
			CTOP_CTRL_M14A0_Wr01(ctr96,jtag1_disable,1);
			CTOP_CTRL_M14A0_WrFL(ctr96);
		}
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H13) 	/* H13 */
	{
		regs_size = sizeof(ACE_REG_H13A0_T);	// max size
		gACE_REG_CTRL_H13.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);

		/* select HDMI PHY i2c clock : 0(3Mhz), 1(24Mhz) */
		CTOP_CTRL_H13A0_RdFL(ctr01);
		CTOP_CTRL_H13A0_Wr01(ctr01,phy_i2c_clk_sel,1);
		CTOP_CTRL_H13A0_WrFL(ctr01);
	}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_L9)	/* L9 */
	{
		regs_size = sizeof(ACE_REG_L9B0_T);		// max size
		gACE_REG_CTRL_L9.shdw.addr = (UINT32 *)OS_KMalloc(regs_size);
	}
#endif

	if(_config->num_internals > 0)
	{
		int i;

		I2C_Init();

		_i2c_handle = (LX_I2C_DEV_HANDLE*)OS_KMalloc(sizeof(LX_I2C_DEV_HANDLE) * _config->num_internals);
		for(i=0; i<_config->num_internals; i++)
		{
			_i2c_handle[i] = I2C_DevOpenPriv(_config->internal[i].ch, 1);

			if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
			else if(lx_chip() == LX_CHIP_H15) 	/* H15 */
			{
				I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
				if(i == 0)
				{
					ACE_REG_H15A0_RdFL(h13a_version_0);
					SYS_DEBUG("H15A VER : 0x%02x\n", ACE_REG_H15A0_Rd(h13a_version_0));

					//ACE 24MHz Clock Reset Release.
					ACE_REG_H15A0_RdFL(soft_reset_0);

					if(ACE_REG_H15A0_RdFd(soft_reset_0,swrst_f24m) == 1)
					{
						ACE_REG_H15A0_Wr01(soft_reset_0,swrst_f24m,0);
						ACE_REG_H15A0_WrFL(soft_reset_0);
					}

					/* Workaround code for AAD clock latchup. */
					ACE_REG_H15A0_RdFL(main_pll_4);
					ACE_REG_H15A0_Wr01(main_pll_4, dr3p_pdb, 0);
					ACE_REG_H15A0_WrFL(main_pll_4);

					ACE_REG_H15A0_RdFL(main_pll_4);
					ACE_REG_H15A0_Wr01(main_pll_4, dr3p_pdb, 1);
					ACE_REG_H15A0_WrFL(main_pll_4);
					/* ****************************** */
				}
			}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
			else if(lx_chip() == LX_CHIP_H14) 	/* H14 */
			{
				I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
				if(i == 0)
				{
					ACE_REG_H14A0_RdFL(h14a_version_0);
					SYS_DEBUG("H14A VER : 0x%02x\n", ACE_REG_H14A0_Rd(h14a_version_0));

					//ACE 24MHz Clock Reset Release.
					ACE_REG_H14A0_RdFL(soft_reset_0);

					if(ACE_REG_H14A0_RdFd(soft_reset_0,swrst_f24m) == 1)
					{
						ACE_REG_H14A0_Wr01(soft_reset_0,swrst_f24m,0);
						ACE_REG_H14A0_WrFL(soft_reset_0);
					}

					/* Workaround code for AAD clock latchup. */
					ACE_REG_H14A0_RdFL(main_pll_4);
					ACE_REG_H14A0_Wr01(main_pll_4, dr3p_pdb, 0);
					ACE_REG_H14A0_WrFL(main_pll_4);

					ACE_REG_H14A0_RdFL(main_pll_4);
					ACE_REG_H14A0_Wr01(main_pll_4, dr3p_pdb, 1);
					ACE_REG_H14A0_WrFL(main_pll_4);
					/* ****************************** */
				}
			}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
			else if(lx_chip() == LX_CHIP_M14) 	/* M14 */
			{
				I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
				if(i == 0)
				{
					if (lx_chip_rev() >= LX_CHIP_REV(M14,B0))
					{

						ACE_REG_M14B0_RdFL(m14b_version_0);
						SYS_DEBUG("M14B VER : 0x%02x\n", ACE_REG_M14B0_Rd(m14b_version_0));

						//ACE 24MHz Clock Reset Release.
						ACE_REG_M14B0_RdFL(soft_reset_0);

						if(ACE_REG_M14B0_RdFd(soft_reset_0,swrst_f24m) == 1)
						{
							ACE_REG_M14B0_Wr01(soft_reset_0,swrst_f24m,0);
							ACE_REG_M14B0_WrFL(soft_reset_0);
						}
						/* Workaround code for AAD clock latchup. */
						ACE_REG_M14B0_RdFL(main_pll_4);
						ACE_REG_M14B0_Wr01(main_pll_4, dr3p_pdb, 0);
						ACE_REG_M14B0_WrFL(main_pll_4);

						ACE_REG_M14B0_RdFL(main_pll_4);
						ACE_REG_M14B0_Wr01(main_pll_4, dr3p_pdb, 1);
						ACE_REG_M14B0_WrFL(main_pll_4);
						/* ****************************** */

					}
					else
					{
						ACE_REG_M14A0_RdFL(m14a_version_0);
						SYS_DEBUG("M14A VER : 0x%02x\n", ACE_REG_M14A0_Rd(m14a_version_0));

						//ACE 24MHz Clock Reset Release.
						ACE_REG_M14A0_RdFL(soft_reset_0);

						if(ACE_REG_M14A0_RdFd(soft_reset_0,swrst_f24m) == 1)
						{
							ACE_REG_M14A0_Wr01(soft_reset_0,swrst_f24m,0);
							ACE_REG_M14A0_WrFL(soft_reset_0);
						}
						/* Workaround code for AAD clock latchup. */
						ACE_REG_M14A0_RdFL(main_pll_4);
						ACE_REG_M14A0_Wr01(main_pll_4, dr3p_pdb, 0);
						ACE_REG_M14A0_WrFL(main_pll_4);

						ACE_REG_M14A0_RdFL(main_pll_4);
						ACE_REG_M14A0_Wr01(main_pll_4, dr3p_pdb, 1);
						ACE_REG_M14A0_WrFL(main_pll_4);
						/* ****************************** */
					}

				}
			}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
			else if(lx_chip() == LX_CHIP_H13) 	/* H13 */
			{
				I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
				if(i == 0)
				{
					ACE_REG_H13A0_RdFL(h13a_version_0);
					SYS_DEBUG("H13A VER : 0x%02x\n", ACE_REG_H13A0_Rd(h13a_version_0));

					//ACE 24MHz Clock Reset Release.
					ACE_REG_H13A0_RdFL(soft_reset_0);

					if(ACE_REG_H13A0_RdFd(soft_reset_0,swrst_f24m) == 1)
					{
						ACE_REG_H13A0_Wr01(soft_reset_0,swrst_f24m,0);
						ACE_REG_H13A0_WrFL(soft_reset_0);
					}

					/* Workaround code for AAD clock latchup. */
					ACE_REG_H13A0_RdFL(main_pll_4);
					ACE_REG_H13A0_Wr01(main_pll_4, dr3p_pdb, 0);
					ACE_REG_H13A0_WrFL(main_pll_4);

					ACE_REG_H13A0_RdFL(main_pll_4);
					ACE_REG_H13A0_Wr01(main_pll_4, dr3p_pdb, 1);
					ACE_REG_H13A0_WrFL(main_pll_4);
					/* ****************************** */
				}
			}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
			else if(lx_chip() == LX_CHIP_L9)	/* L9 */
			{
				UINT32 initial_clock = (_config->internal[i].clock > I2C_CLOCK_2MHZ) ?
													I2C_CLOCK_2MHZ : _config->internal[i].clock;
				I2C_DevSetClock(_i2c_handle[i], initial_clock);
				if ( lx_chip_rev() >= LX_CHIP_REV_L9_A0)
				{
					ACE_REG_L9B0_RdFL(soft_reset_0);
					ACE_REG_L9B0_Wr01(soft_reset_0,swrst_f24m,0);
					ACE_REG_L9B0_WrFL(soft_reset_0);

					ACE_REG_L9B0_RdFL(main_pll_4);
					ACE_REG_L9B0_Wr01(main_pll_4,dr3p_pdb,0);
					ACE_REG_L9B0_WrFL(main_pll_4);

					ACE_REG_L9B0_RdFL(main_pll_4);
					ACE_REG_L9B0_Wr01(main_pll_4,dr3p_pdb,1);
					ACE_REG_L9B0_WrFL(main_pll_4);

					if(_config->internal[i].clock > I2C_CLOCK_2MHZ)
					{
						mdelay(1);
						// Internal I2C support upto 8Mhz
						ACE_REG_L9B0_RdFL(hostif_control_0);
						ACE_REG_L9B0_Wr01(hostif_control_0,reg_sel_hostif_clock,1);
						ACE_REG_L9B0_WrFL(hostif_control_0);

						mdelay(1);
						ACE_REG_L9B0_RdFL(hostif_control_0);
						if(ACE_REG_L9B0_RdFd(hostif_control_0,ro_hostif_is_162mhz) == 1)
							SYS_DEBUG("Set Internal I2C 162MHZ Clock !!!!\n");

						I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
					}
				}
			}
#endif
			else
			{
				I2C_DevSetClock(_i2c_handle[i], _config->internal[i].clock);
			}
		}
	}


#endif //PLATFORM_FPGA

#if 0	// MACRO TEST
	ACE_REG_H13A0_RdFL(h13a_version_0);

	CTOP_CTRL_H13A0_RdFL(ctr00_reg_rst_sel);
	CTOP_CTRL_H13A0_Wr01(ctr00_reg_rst_sel,reg_afifo_r,0);
	CTOP_CTRL_H13A0_WrFL(ctr00_reg_rst_sel);

	CPU_TOP_H13A0_RdFL(cpu0_addr_sw_reg_0);
	CPU_TOP_H13A0_Wr01(cpu0_addr_sw_reg_0,cpu0_paddr_app_nor_boot,2);
	CPU_TOP_H13A0_WrFL(cpu0_addr_sw_reg_0);
#endif

	return 0;
}

int REG_CTRL_Free(void)
{

	if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H15)
	{
#define CTOP_H15A0_DEINIT(_m)	\
		do { \
			if(gCTOP_CTRL_H15.shdw.a0._m) OS_Free((void*)gCTOP_CTRL_H15.shdw.a0._m); \
			if(gCTOP_CTRL_H15.phys.a0._m) iounmap((void *)gCTOP_CTRL_H15.phys.a0._m); \
		} while(0)

		CTOP_H15A0_DEINIT(CVI);
		CTOP_H15A0_DEINIT(IMX);
		CTOP_H15A0_DEINIT(ND0);
		CTOP_H15A0_DEINIT(ND1);
		CTOP_H15A0_DEINIT(SUB);
		CTOP_H15A0_DEINIT(GSC);
		CTOP_H15A0_DEINIT(SR);
		CTOP_H15A0_DEINIT(VSD);
		CTOP_H15A0_DEINIT(DPE);
		CTOP_H15A0_DEINIT(CCO);
		CTOP_H15A0_DEINIT(MCU);
		CTOP_H15A0_DEINIT(M1);
		CTOP_H15A0_DEINIT(GFX);
		CTOP_H15A0_DEINIT(VDEC0);
		CTOP_H15A0_DEINIT(VDEC1);
		CTOP_H15A0_DEINIT(M2);
		CTOP_H15A0_DEINIT(VDEC2);
		CTOP_H15A0_DEINIT(M0);
		CTOP_H15A0_DEINIT(TI);
		CTOP_H15A0_DEINIT(AUD);
		CTOP_H15A0_DEINIT(VENC);
		CTOP_H15A0_DEINIT(VIP);
		CTOP_H15A0_DEINIT(GPU);
		CTOP_H15A0_DEINIT(CPU);

	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		if(gCTOP_CTRL_H14.shdw.addr) OS_Free((void *)gCTOP_CTRL_H14.shdw.addr);
		if(gCTOP_CTRL_H14.phys.addr) iounmap((void *)gCTOP_CTRL_H14.phys.addr);

		if(gCPU_TOP_REG_H14.shdw.addr) OS_Free((void *)gCPU_TOP_REG_H14.shdw.addr);
		if(gCPU_TOP_REG_H14.phys.addr) iounmap((void *)gCPU_TOP_REG_H14.phys.addr);
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14,A0))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(M14,B0))
		{
#define CTOP_M14B0_DEINIT(_m)	\
			do { \
				if(gCTOP_CTRL_M14B0.shdw._m) OS_Free((void*)gCTOP_CTRL_M14B0.shdw._m);	\
				if(gCTOP_CTRL_M14B0.phys._m) iounmap((void *)gCTOP_CTRL_M14B0.phys._m);	\
			} while(0)

			CTOP_M14B0_DEINIT(TOP);
			CTOP_M14B0_DEINIT(LEFT);
			CTOP_M14B0_DEINIT(RIGHT);
			CTOP_M14B0_DEINIT(GFX);
			CTOP_M14B0_DEINIT(VDEC);
			CTOP_M14B0_DEINIT(HEVC);
			CTOP_M14B0_DEINIT(TE);
			CTOP_M14B0_DEINIT(VENC);
			CTOP_M14B0_DEINIT(AUD);
			CTOP_M14B0_DEINIT(DEI);
			CTOP_M14B0_DEINIT(GPU);
			CTOP_M14B0_DEINIT(IDSMCU);
			CTOP_M14B0_DEINIT(DEM);
			CTOP_M14B0_DEINIT(DEO);
			CTOP_M14B0_DEINIT(DPP);
			CTOP_M14B0_DEINIT(FDA);
			CTOP_M14B0_DEINIT(FDB);
			CTOP_M14B0_DEINIT(FDC);
			CTOP_M14B0_DEINIT(FDD);
			CTOP_M14B0_DEINIT(DPE);
			CTOP_M14B0_DEINIT(CPU_PERI);
		}
		else
		{
			if(gCTOP_CTRL_M14.shdw.addr) OS_Free((void *)gCTOP_CTRL_M14.shdw.addr);
			if(gCTOP_CTRL_M14.phys.addr) iounmap((void *)gCTOP_CTRL_M14.phys.addr);
		}

		if(gCPU_TOP_REG_M14.shdw.addr) OS_Free((void *)gCPU_TOP_REG_M14.shdw.addr);
		if(gCPU_TOP_REG_M14.phys.addr) iounmap((void *)gCPU_TOP_REG_M14.phys.addr);
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13,A0))
	{
		if(gCTOP_CTRL_H13.shdw.addr) OS_Free((void *)gCTOP_CTRL_H13.shdw.addr);
		if(gCTOP_CTRL_H13.phys.addr) iounmap((void *)gCTOP_CTRL_H13.phys.addr);

		if(gCPU_TOP_REG_H13.shdw.addr) OS_Free((void *)gCPU_TOP_REG_H13.shdw.addr);
		if(gCPU_TOP_REG_H13.phys.addr) iounmap((void *)gCPU_TOP_REG_H13.phys.addr);
	}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(L9,A0))
	{
		if (lx_chip_rev() >= LX_CHIP_REV(L9,B0))
		{
			OS_Free((void *)gCTOP_CTRL_L9B.shdw.addr);
			if(gCTOP_CTRL_L9B.phys.addr) iounmap((void *)gCTOP_CTRL_L9B.phys.addr);
		}

		/* A1 & B0 ctop are used at same time */
		OS_Free((void *)gCTOP_CTRL_L9.shdw.addr);
		if (gCTOP_CTRL_L9.phys.addr) iounmap((void *)gCTOP_CTRL_L9.phys.addr);
	}
#endif
#ifdef INCLUDE_L8_CHIP_KDRV
	else
	{
		OS_Free((void *)gCTOP_CTRL.shadow.addr);
		if (gCTOP_CTRL.phys.addr) iounmap((void *)gCTOP_CTRL.phys.addr);
	}
#endif

	// ACE Registers
	if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if (lx_chip() == LX_CHIP_H15)
	{
		OS_Free((void *)gATOP_REG_CTRL_H15.shdw.addr);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		OS_Free((void *)gATOP_REG_CTRL_H14.shdw.addr);
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14,A0))
	{
		OS_Free((void *)gATOP_REG_CTRL_M14.shdw.addr);
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13,A0))
	{
		OS_Free((void *)gACE_REG_CTRL_H13.shdw.addr);
	}
#endif
#ifdef INCLUDE_L9_CHIP_KDRV
	else if(lx_chip_rev() >= LX_CHIP_REV(L9,B0))
	{
		OS_Free((void *)gACE_REG_CTRL_L9.shdw.addr);
	}
#endif

	return 0;
}


int REG_WriteI2C(UINT32 idx, UINT8 slave, UINT8 reg, UINT8 data)
{
#ifndef PLATFORM_FPGA
	UINT32 i;
	int rc = -1;
	LX_I2C_RW_DATA_T param;
	LX_I2C_INFO_T *pInfo =NULL;

	UINT32 retry = 1;

	if(idx >= _config->num_internals)
	{
		SYS_ERROR("Not supported !!!\n");
		return -1;
	}

	param.slaveAddr		= slave;
	param.subAddrSize	= 1;
	param.subAddr[0]	= reg;
	param.buf			= &data;
	param.bufSize		= 1;
	param.clock			= I2C_CLOCK_INVALID;	//use initial clock
	param.flag			= 0;

	for(i=0; i<retry; i++)
	{
		rc = I2C_DevTransfer(_i2c_handle[idx], &param, I2C_WRITE_MODE);
		if(rc >= 0) break;
	}


	if(rc < 0)
	{
		pInfo = (LX_I2C_INFO_T *)(_i2c_handle[idx]);
		SYS_ERROR("Internal i2c write failed. ch[%d:%d] slave=0x%02x, reg=0x%02x\n",
			pInfo->ch,pInfo->inited,slave, reg);
	}

	return rc;
#else
	return -1;
#endif
}

int REG_ReadI2C(UINT32 idx, UINT8 slave, UINT8 reg, UINT8* data)
{
#ifndef PLATFORM_FPGA
	UINT32 i;
	int rc = -1;
	LX_I2C_RW_DATA_T param;
	UINT32 retry = 1;
	LX_I2C_INFO_T *pInfo = NULL;
//printk("REG_ReadI2C....idx:%d,slave:0x%2x,reg:0x%02x,data:%x\n",idx,slave,reg,data);

	if(idx >= _config->num_internals)
	{
		SYS_ERROR("Not supported !!!\n");
		return -1;
	}

	param.slaveAddr		= slave;
	param.subAddrSize	= 1;
	param.subAddr[0]	= reg;
	param.buf			= data;
	param.bufSize		= 1;
	param.clock			= I2C_CLOCK_INVALID;	//use initial clock
	param.flag			= 0;

	for(i=0; i<retry; i++)
	{
		rc = I2C_DevTransfer(_i2c_handle[idx], &param, I2C_READ_MODE);
		if(rc >= 0) break;
	}

	if(rc < 0)
	{
		pInfo = (LX_I2C_INFO_T *)(_i2c_handle[idx]);
		SYS_ERROR("Internal i2c read failed. ch[%d:%d] slave=0x%02x, reg=0x%02x\n",
			pInfo->ch,pInfo->inited,slave, reg);
	}

	return rc;
#else
	return -1;
#endif
}


int ACE_RegWrite(UINT8 slave, UINT8 reg, UINT8 data)
{
#ifndef PLATFORM_FPGA
	return REG_WriteI2C(0, slave, reg, data);
#else
	return -1;
#endif
}

int ACE_RegRead(UINT8 slave, UINT8 reg, UINT8* data)
{
#ifndef PLATFORM_FPGA
	return REG_ReadI2C(0, slave, reg, data);
#else
	return -1;
#endif
}

