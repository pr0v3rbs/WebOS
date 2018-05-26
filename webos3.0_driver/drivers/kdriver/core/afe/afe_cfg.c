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
 *  CVD LDrv file for afe device
 *	afe device will teach you how to make device driver with new platform.
 *
 *  author		wonsik.do (wonsik.do@lge.com)
 *  version		1.0
 *  date		2009.12.30
 *  note		Additional information.
 *
 *  @addtogroup lg1150_afe
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "afe_cfg.h"
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

/**
Vport Register configuration.
*/
LX_CVD_REG_T gRegCfgVport[] =
{
	// for LG1151(L9) A0 & LG1152(L9B0)
	{
		.chip_name	= "CVD_L9",
		.vport_reg_base_addr 	= 0xc0024100,
		.vport_reg_size			= 0x00000800,
		.vport_qemu_base_addr	= 0x04008000,
		.chbcvd_reg_base_addr	= 0xc000af00,
		.chbcvd_reg_size	= 	0xf4,
	},
	// for LG1154(H13Ax)
	{
		.chip_name	= "CVD_H13Ax",
		.vport_reg_base_addr 	= 0xc0017100,
		.vport_reg_size			= 0x00000808,
		.vport_qemu_base_addr	= 0x04008000,
		.chbcvd_reg_base_addr	= 0x0,
		.chbcvd_reg_size	= 	0x0,
	},
	// for LG1154(H13Bx) and M14Ax, H14Ax
	{
		.chip_name	= "CVD_H13Bx",
		.vport_reg_base_addr 	= 0xc0017100,
		.vport_reg_size			= 0x00000814,
		.vport_qemu_base_addr	= 0x04008000,
		.chbcvd_reg_base_addr	= 0x0,
		.chbcvd_reg_size	= 	0x0,
	},
	// for LG1131(M14Bx)
	{
		.chip_name	= "CVD_M14Bx",
		.vport_reg_base_addr 	= 0xc0021100,
		.vport_reg_size			= 0x00000848,
		.vport_qemu_base_addr	= 0x04008000,
		.chbcvd_reg_base_addr	= 0x0,
		.chbcvd_reg_size	= 	0x0,
	},
	// for H15A0
	{
		.chip_name	= "CVD_H15Ax",
		.vport_reg_base_addr 	= 0xc8000600,
		.vport_reg_size			= 0x00000848,
		.vport_qemu_base_addr	= 0x04008000,
		.chbcvd_reg_base_addr	= 0x0,
		.chbcvd_reg_size	= 	0x0,
	},
};

LX_CVD_REG_T *pVportRegCfg = NULL;

// CVD 3D Comb Filter Memory
LX_CVD_MEM_CFG_T gMemCfgCvd[] =
{
	{
	.memory_name = "cvd_mem_L8",
	.memory_base = 0x27ce0000,
	.memory_size = 0x00320000,
	},
	{
	.memory_name = "cvd_mem_L9",
	.memory_base = 0x0,	//7bb00000,
	.memory_size = 0x00500000,
	},
	{
	.memory_name = "cvd_mem_H13Ax",
	.memory_base = 0x88000000,	// USE G-Bus Fixed Address
	.memory_size = 0x00500000,
	},
	{
	.memory_name = "cvd_mem_H13Bx",
	.memory_base = 0x0,	
	.memory_size = 0x00600000,
	}
};

LX_CVD_MEM_CFG_T*	gpCvdMemCfg = NULL;
LX_CVD_MEM_CFG_T	gCvdMemCfg = {0,};
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/


/*========================================================================================
	Implementation Group
========================================================================================*/
void    AFE_InitCfg ( void )
{

	printk("Main CVD Initialize Start... DTVSoC Chip Verstion [0x%x]\n", lx_chip_rev() );
	/*-----------------------------------------------------------------------------------
	 * [H13] CVD Memory configuration
	 *----------------------------------------------------------------------------------*/
    if( lx_chip_rev( ) >= LX_CHIP_REV(H15,A0) )
	{
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[3];
		gCvdMemCfg = gMemCfgCvd[3];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[4];	// for Real.
	}
	else if( lx_chip_rev( ) >= LX_CHIP_REV(H14,A0) )
	{
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[3];
		gCvdMemCfg = gMemCfgCvd[3];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[2];	// for Real.
	}
    else if( lx_chip_rev( ) >= LX_CHIP_REV(M14,B0) )
	{
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[3];
		gCvdMemCfg = gMemCfgCvd[3];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[3];	// for Real.
	}
    else if( lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) )
	{
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[3];
		gCvdMemCfg = gMemCfgCvd[3];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[2];	// for Real.
	}
	else if( lx_chip_rev( ) >= LX_CHIP_REV(H13,A0) )
	{
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[2];
		gCvdMemCfg = gMemCfgCvd[2];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[1];	// for Real.
	}
	/*-----------------------------------------------------------------------------------
	 * [L9] CVD Memory configuration
	 *----------------------------------------------------------------------------------*/
	else if( lx_chip_rev( ) >= LX_CHIP_REV(L9,A0) )
    {
		// 3D Comb Buffer Memory Address
		//gpCvdMemCfg = &gMemCfgCvd[1];
		gCvdMemCfg = gMemCfgCvd[1];
		gpCvdMemCfg = &gCvdMemCfg;
		// CVD Register Base Address
		pVportRegCfg = &gRegCfgVport[0];	// for Real.
    }
    else
    {
    }

}

void 	AFE_GetMemCfg( UINT32* mem_base_comb_buffer, UINT32* mem_size_comb_buffer, UINT32* mem_base_cvd_reg, UINT32* mem_size_cvd_reg)
{
	*mem_base_comb_buffer = gpCvdMemCfg->memory_base;
	*mem_size_comb_buffer = gpCvdMemCfg->memory_size;

	*mem_base_cvd_reg = pVportRegCfg->vport_reg_base_addr;
	*mem_size_cvd_reg = pVportRegCfg->vport_reg_size;

	return;
}



/** @} */

