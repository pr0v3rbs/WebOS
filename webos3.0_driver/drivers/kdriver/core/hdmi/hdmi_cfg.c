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
 *  main configuration file for hdmi device
 *	hdmi device will teach you how to make device driver with new platform.
 *
 *  author		sh.myoung (sh.myoung@lge.com)
 *  version		1.0
 *  date		2010.01.06
 *  note		Additional information.
 *
 *  @addtogroup lg1150_hdmi
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "hdmi_cfg.h"
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


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
HDMI Register configuration.
*/
LX_HDMI_REG_T gRegCfgHdmi[] =
{
	// for LG1154(H13) A0 & B0
	{
		.chip_name	= "H13",
		.link_reg_base_addr 	= 0xc000c000,
		.link_reg_size			= 0x00000320,
		.link1_reg_base_addr 	= 0x0,
		.link1_reg_size			= 0x0,
		.link_top_reg_base_addr = 0x0,
		.link_top_reg_size		= 0x0,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0x0,
		.hdcp22_prt0_reg_size		= 0x0,
		.hdcp22_prt1_reg_base_addr 	= 0x0,
		.hdcp22_prt1_reg_size		= 0x0,
	},


	{
		.chip_name	= "H14",
		.link_reg_base_addr 	= 0xc000c000,
		.link_reg_size			= 0x00000320,
		.link1_reg_base_addr 	= 0x0,
		.link1_reg_size			= 0x0,
		.link_top_reg_base_addr = 0x0,
		.link_top_reg_size		= 0x0,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0x0,
		.hdcp22_prt0_reg_size		= 0x0,
		.hdcp22_prt1_reg_base_addr 	= 0x0,
		.hdcp22_prt1_reg_size		= 0x0,
	},

	{
		.chip_name	= "M14",
		.link_reg_base_addr 	= 0xc000c000,
		.link_reg_size			= 0x00000800,
		.link1_reg_base_addr 	= 0x0,
		.link1_reg_size			= 0x0,
		.link_top_reg_base_addr = 0x0,
		.link_top_reg_size		= 0x0,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0x0,
		.hdcp22_prt0_reg_size		= 0x0,
		.hdcp22_prt1_reg_base_addr 	= 0x0,
		.hdcp22_prt1_reg_size		= 0x0,
	},

	{
		.chip_name	= "M14B0",
		.link_reg_base_addr 	= 0xc000e000,
		.link_reg_size			= 0x00000828,
		.link1_reg_base_addr 	= 0x0,
		.link1_reg_size			= 0x0,
		.link_top_reg_base_addr = 0x0,
		.link_top_reg_size		= 0x0,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0x0,
		.hdcp22_prt0_reg_size		= 0x0,
		.hdcp22_prt1_reg_base_addr 	= 0x0,
		.hdcp22_prt1_reg_size		= 0x0,
	},
	{
		.chip_name	= "M14C0",
		.link_reg_base_addr 	= 0xc000e000,
		.link_reg_size			= 0x00000828,
		.link1_reg_base_addr 	= 0x0,
		.link1_reg_size			= 0x0,
		.link_top_reg_base_addr = 0x0,
		.link_top_reg_size		= 0x0,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0x0,
		.hdcp22_prt0_reg_size		= 0x0,
		.hdcp22_prt1_reg_base_addr 	= 0x0,
		.hdcp22_prt1_reg_size		= 0x0,
	},


	{
		.chip_name	= "H15A0",
		.link_reg_base_addr 	= 0xc8210000,
		.link_reg_size			= 0x0000039c,
		.link1_reg_base_addr 	= 0xc8210400,
		.link1_reg_size			= 0x0000039c,
		.link_top_reg_base_addr = 0xc8210c00,
		.link_top_reg_size		= 0x00000130,
		.link_qemu_base_addr	= 0x04007000,
		.hdcp22_prt0_reg_base_addr 	= 0xc8211100,
		.hdcp22_prt0_reg_size		= 0x100,
		.hdcp22_prt1_reg_base_addr 	= 0xc8211900,
		.hdcp22_prt1_reg_size		= 0x100,
	},
};

/* for HDMI Switch */
HDMI_DATA_T gCfgBootData[] =
{
	 // for LG1151(L9) A0/1 & B0
	 {
	 	1,			// hdmi_switch
		0,			// SWport
		{ 1},	// LX_HDMI_HPD_T

		0,			// mode
		0,			// port
		0,			// Pixel Repetition
		{4,	4},		// connectCn[2]
		{21, 8},	// timingCn[2]
		{20, 4},	// hdmiCn[2]
	    	{3,	2}		// dviCn[2]
	  }
};

LX_HDMI_REG_T *pH13HdmiRegCfg = &gRegCfgHdmi[0];	// for Real.
HDMI_DATA_T *gH13BootData = &gCfgBootData[0];	// for Real.

LX_HDMI_REG_T *pH14HdmiRegCfg = &gRegCfgHdmi[1];	// for Real.
HDMI_DATA_T *gH14BootData = &gCfgBootData[0];	// for Real.

LX_HDMI_REG_T *pM14A0HdmiRegCfg = &gRegCfgHdmi[2];	// for Real.
HDMI_DATA_T *gM14BootData = &gCfgBootData[0];	// for Real.

LX_HDMI_REG_T *pM14B0HdmiRegCfg = &gRegCfgHdmi[3];	// for Real.
HDMI_DATA_T *gM14B0BootData = &gCfgBootData[0];	// for Real.

LX_HDMI_REG_T *pM14C0HdmiRegCfg = &gRegCfgHdmi[4];	// for Real.
HDMI_DATA_T *gM14C0BootData = &gCfgBootData[0];	// for Real.

LX_HDMI_REG_T *pH15A0HdmiRegCfg = &gRegCfgHdmi[5];	// for Real.
HDMI_DATA_T *gH15BootData = &gCfgBootData[0];	// for Real.



/** @} */

