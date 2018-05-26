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
 *  Header file of defining possible of output modes of H13Bx 	
 *
 *  author      won.hur@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */
 
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_kapi.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/


static LX_OVI_REG_T ovi_h13b0_output_mode_epi_tcon_test_pattern[] =
{
	{0xc00400d4, 0x00000013},
	{0xc0040090, 0x00000025},
	{0xc0040094, 0x0000000b},
	{0xc0040098, 0x00000016},
	{0xc004009c, 0x00000004},
	{0xc00400a0, 0x00000005},
	{0xc00400a4, 0x00000024},
	{0xc00400a8, 0x00000001},
	{0xc00400ac, 0x000000e0},
	{0xc00400b0, 0x00000004},
	{0xc00400b4, 0x00000038},
	{0xc00400d4, 0x0000000b},
	{0xc00400e4, 0x00000003},
	{0xc00400e8, 0x000000ff},
	{0xc00400ec, 0x00000003},
	{0xc00400f0, 0x000000ff},
	{0xc00400f4, 0x00000003},
	{0xc00400f8, 0x000000ff},
};

static LX_OVI_REG_T	ovi_h13b0_output_format_hslvds_2port_60hz_480p[] =
{
//deB Sync Parameter
	{0xC0014008 ,0x00060021},
	{0xC001400c ,0x000601E0},
	{0xC0014010 ,0x0010003A},
	{0xC0014014 ,0x004002D0},
	{0xC0014018 ,0x00000027},
//DPPA Sync Paramter
	{0xC0035090 ,0x00050006},
	{0xC0035094 ,0x001E01E0},
	{0xC0035098 ,0x0010003E},
	{0xC003509C ,0x003A02D0},
	{0xC00350A0 ,0x00000012},
	{0xC00350A4 ,0x000C01B5},

//CTOP Setting
	{0xc001b114 ,0x11400000},
	{0xc001b05c ,0x00000000},
	{0xc001b060 ,0x41115014},
	{0xc001b198 ,0x80000000},
	{0xc001b19c ,0x02100414},
	{0xc001b14c ,0xC8290000},
	{0xc001b148 ,0x09000000},
	{0xc001b060 ,0x41115013},

//Detour
//H3D detour
	{0xC002c010 ,0x01000000},
//FRC detour
	{0xC0028100 ,0x0},
	{0xC0028104 ,0x11},
//PE1 detour
	{0xC0035308 ,0x01000A00},
//SRE detour
	{0xC0025004 ,0x1},
//DPPB detour
	{0xC0030010 ,0x1},
	{0xC0030768 ,0x10100},
//1080p TCON},
	/* NEW ADDED */
	{0xC0040004, 0x0},
	{0xC0040008, 0x80},
	{0xC004001C, 0x0},
	/* END OF NEWLY ADDED */
	{0xC0049000 ,0x042D0004},
	{0xC0049018 ,0x0000721A},
};

static LX_OVI_REG_T	ovi_h13b0_output_format_hslvds_2port_60hz_720p[] =
{
//deB Sync Parameter
	{0xC0014008 ,0x00050014},
	{0xC001400c ,0x000502D0},
	{0xC0014010 ,0x004600DC},
	{0xC0014014 ,0x00500500},
	{0xC0014018 ,0x00000027},
////DPPA Sync Paramter
	{0xC0035090 ,0x00050005},
	{0xC0035094 ,0x001402D0},
	{0xC0035098 ,0x006E0028},
	{0xC003509C ,0x00DC0500},
	{0xC00350A0 ,0x0000006C},
	{0xC00350A4 ,0x000C01B3},
////CTOP Setting
	{0xc001b114 ,0x11400000},
	{0xc001b05c ,0x00000000},
	{0xc001b060 ,0x41115014},
	{0xc001b198 ,0x80000000},
	{0xc001b19c ,0x02100414},
	{0xc001b14c ,0xC8290000},
	{0xc001b148 ,0x0D000000},
	{0xc001b060 ,0x41115013},
//Detour
//H3D detour
	{0xC002c010 ,0x01000000},
//
//FRC detour
	{0xC0028100 ,0x0},
	{0xC0028104 ,0x11},
//
////PE1 detour
	{0xC0035308 ,0x01000A00},
//
////SRE detour
	{0xC0025004 ,0x1},
//
////DPPB detour
	{0xC0030010 ,0x1},
	{0xC0030768 ,0x10100},
//
////1080p TCON},
	/* NEW ADDED */
	{0xC0040004, 0x0},
	{0xC0040008, 0x80},
	{0xC004001C, 0x0},
	/* END OF NEWLY ADDED */
	{0xC0049000 ,0x04500004},
	{0xC0049018 ,0x0000721A},
};

static LX_OVI_REG_T	ovi_h13b0_output_format_hslvds_2port_60hz_1080p[] =
{
//deB Sync Parameter
	{0xC0014008 ,0x00040024},
	{0xC001400c ,0x00050438},
	{0xC0014010 ,0x00580094},
	{0xC0014014 ,0x002C0780},
	{0xC0014018 ,0x00000027},
//
////DPPA Sync Paramter
	{0xC0035090 ,0x00010005},
	{0xC0035094 ,0x00270438},
	{0xC0035098 ,0x002B0059},
	{0xC003509C ,0x00940780},
	{0xC00350A0 ,0x00000054},
	{0xC00350A4 ,0x000D0183},
//
////CTOP Setting
	{0xc001b114 ,0x11400000},
	{0xc001b05c ,0x00000000},
	{0xc001b060 ,0x41115014},
	{0xc001b198 ,0x80000000},
	{0xc001b19c ,0x02100414},
	{0xc001b14c ,0xC8290000},
	{0xc001b148 ,0x09000000},
	{0xc001b060 ,0x41115014},

//                 
////Detour
////H3D detour
	{0xC002c010 ,0x01000000},
//
////FRC detour
	{0xC0028100 ,0x0},
	{0xC0028104 ,0x11},
//
////PE1 detour
	{0xC0035308 ,0x01000A00},
//
////SRE detour
	{0xC0025004 ,0x1},
//
////DPPB detour
	{0xC0030010 ,0x1},
	{0xC0030768 ,0x10100},
//
////1080p TCON},
	/* NEW ADDED */
	{0xC0040004, 0x0},
	{0xC0040008, 0x80},
	{0xC004001C, 0x0},
	/* END OF NEWLY ADDED */
	{0xC0049000 ,0x053C0004},
	{0xC0049018 ,0x0000721A},
};

static LX_OVI_REG_T	ovi_h13b0_output_format_hslvds_2port_2x_60hz_1080p[] =
{
	// de
	{ 0xC0014008 , 0x00040024 },
	{ 0xC001400C , 0x00050438 },
	{ 0xC0014010 , 0x00580094 },
	{ 0xC0014014 , 0x002C0780 },

	//frc
	{ 0xc002b000 , 0x00001130 }, //sc_htotal
	{ 0xc002b004 , 0x00000058 }, //sg_hsyncwi
	{ 0xc002b008 , 0x00000000 }, //sg_hsyncst
	{ 0xc002b00c , 0x00000780 }, //sg_hactive
	{ 0xc002b010 , 0x00000180 }, //sg_hactive
	{ 0xc002b014 , 0x00000465 }, //sg_vtotal
	{ 0xc002b018 , 0x00000005 }, //sg_vsyncwi
	{ 0xc002b01c , 0x00000000 }, //sg_vsyncvs
	{ 0xc002b020 , 0x00000000 }, //sg_vsynchs
	{ 0xc002b024 , 0x00000438 }, //sg_vactive
	{ 0xc002b028 , 0x0000002B }, //sg_vactive
	{ 0xc002b080 , 0x00000001 }, //disp_start

	// dppa
	{ 0xc0035010 , 0x00000000 },
	{ 0xc0035090 , 0x00010005 },
	{ 0xc0035094 , 0x00270438 },
	{ 0xc0035098 , 0x002b0059 },
	{ 0xc003509C , 0x00940780 },
	{ 0xc00350a4 , 0x000d0183 },
	{ 0xc00350a0 , 0x80000054 },
	{ 0xc00350a0 , 0x00000054 },
	{ 0xc0035308 , 0x01000a01 },

	// Spread Spectrum off
	{0xC001B13C, 0x981D4000},

	// Register case7 in boot loader
	{0xC0040090, 0x00000025},
	{0xC0040094, 0x0000000B},
	{0xC0040098, 0x00000016},
	{0xC004009C, 0x00000024},
	{0xC00400a0, 0x00000005},
	{0xC00400a4, 0x00000004},
	{0xC0040004, 0x00000000},
	{0xC0040008, 0x00000080},
	{0xC004000C, 0x00000000},
	{0xC004001C, 0x00000000},
	{0xC00400E4, 0x00000003},
	{0xC00400E8, 0x000000FF},
	{0xC00400EC, 0x00000003},
	{0xC00400F0, 0x000000FF},
	{0xC00400F4, 0x00000003},
	{0xC00400F8, 0x000000FF},
	{0xC0049000, 0x2D3C005D},
	{0xC0049018, 0x0000721A},
	{0xC001B060, 0x00000004},
	{0xC001B1BC, 0x00000FFF},

	// Added 20130206 : Set clock divider for BE modules(H3D to DPPA)
	{0xc001b114 ,0x0},	// If value is 0x11400000, it will reduce clock input of H3D to DPPA in half of display PLL
};

static LX_OVI_REG_T	ovi_h13b0_output_format_hslvds_4port_120hz_1080p[] =
{

	/* Step 0. TCON/EPI SW reset - workaround solution for boot sequence problem */
	{0xC001B05C, 0x4000},
	{0xC001B05C, 0x0},
	
	// de
	{ 0xC0014008 , 0x00040024 },
	{ 0xC001400C , 0x00050438 },
	{ 0xC0014010 , 0x00580094 },
	{ 0xC0014014 , 0x002C0780 },

	//frc
	{ 0xc002b000 , 0x00000898 }, //sc_htotal
	{ 0xc002b004 , 0x00000058 }, //sg_hsyncwi
	{ 0xc002b008 , 0x00000000 }, //sg_hsyncst
	{ 0xc002b00c , 0x00000780 }, //sg_hactive
	{ 0xc002b010 , 0x000000EC }, //sg_hactive
	{ 0xc002b014 , 0x00000465 }, //sg_vtotal
	{ 0xc002b018 , 0x00000004 }, //sg_vsyncwi
	{ 0xc002b01c , 0x00000000 }, //sg_vsyncvs
	{ 0xc002b020 , 0x00000000 }, //sg_vsynchs
	{ 0xc002b024 , 0x00000438 }, //sg_vactive
	{ 0xc002b028 , 0x0000002B }, //sg_vactive
	{ 0xc002b080 , 0x00000001 }, //disp_start

	// dppa
	{ 0xc0035010 , 0x00000000 },
	{ 0xc0035090 , 0x00010005 },
	{ 0xc0035094 , 0x00270438 },
	{ 0xc0035098 , 0x002b0059 },
	{ 0xc003509C , 0x00940780 },
	{ 0xc00350a4 , 0x000d0183 },
	{ 0xc00350a0 , 0x80000054 },
	{ 0xc00350a0 , 0x00000054 },
	{ 0xc0035308 , 0x01000a01 },

	// Register case6 in boot loader
	{0xC0040004, 0x0000000E},
	{0xC0040008, 0x00000000},
	{0xC004000C, 0x00000000},
	{0xC004001C, 0x00000080},
	{0xC004002C, 0x00000009},
	{0xC0040030, 0x00000003},
	{0xC0040034, 0x000000FF},
	{0xC004003C, 0x00000020},
	{0xC0040040, 0x00000020},
	{0xC0040044, 0x00000020},
	{0xC0040090, 0x00000025},
	{0xC0040094, 0x0000000B},
	{0xC0040098, 0x00000016},
	{0xC004009C, 0x00000024},
	{0xC00400a0, 0x00000005},
	{0xC00400a4, 0x00000004},
	{0xC00400E4, 0x00000003},
	{0xC00400E8, 0x000000FF},
	{0xC00400EC, 0x00000003},
	{0xC00400F0, 0x000000FF},
	{0xC00400F4, 0x00000003},
	{0xC00400F8, 0x000000FF},
	{0xC0040154, 0x00000000},
	{0xC0049000, 0x0F1E0004},
	{0xC0049018, 0x0000F21A},
	{0xC001B060, 0x00000004},

	// Added 20130206 : Set clock divider for BE modules(H3D to DPPA)
	{0xc001b114 ,0x0},	// If value is 0x11400000, it will reduce clock input of H3D to DPPA in half of display PLL
};

static LX_OVI_REG_T	ovi_h13b0_output_format_epi_6lane_120hz_1080p[] =
{
	/* Step 0. TCON/EPI SW reset - workaround solution for boot sequence problem */
	{0xC001B05C, 0x6000},
	{0xC001B05C, 0x0},

	// de
	{ 0xC0014008 , 0x00040024 },
	{ 0xC001400C , 0x00050438 },
	{ 0xC0014010 , 0x00580094 },
	{ 0xC0014014 , 0x002C0780 },

	//frc
	{ 0xc002b000 , 0x00000898 }, //sc_htotal
	{ 0xc002b004 , 0x00000058 }, //sg_hsyncwi
	{ 0xc002b008 , 0x00000000 }, //sg_hsyncst
	{ 0xc002b00c , 0x00000780 }, //sg_hactive
	{ 0xc002b010 , 0x000000EC }, //sg_hactive
	{ 0xc002b014 , 0x00000465 }, //sg_vtotal
	{ 0xc002b018 , 0x00000004 }, //sg_vsyncwi
	{ 0xc002b01c , 0x00000000 }, //sg_vsyncvs
	{ 0xc002b020 , 0x00000000 }, //sg_vsynchs
	{ 0xc002b024 , 0x00000438 }, //sg_vactive
	{ 0xc002b028 , 0x0000002B }, //sg_vactive
	{ 0xc002b080 , 0x00000001 }, //disp_start

	// dppa
	{ 0xc0035010 , 0x00000000 },
	{ 0xc0035090 , 0x00010005 },
	{ 0xc0035094 , 0x00270438 },
	{ 0xc0035098 , 0x002b0059 },
	{ 0xc003509C , 0x00940780 },
	{ 0xc00350a4 , 0x000d0183 },
	{ 0xc00350a0 , 0x80000054 },
	{ 0xc00350a0 , 0x00000054 },
	{ 0xc0035308 , 0x01000a01 },
	
	// Register case3 in boot loader
	{0xc001b194, 0x03fc0000},
	{0xc001b198, 0x00000000},
	{0xc001b19c, 0x0011945c},
	{0xc001b1ac, 0x01000000},
	{0xc001b1b0, 0x00000004},
	{0xc001b1bc, 0x0003f000},
	{0xc00400d4, 0x00000000},

	// Added 20130206 : Set clock divider for BE modules(H3D to DPPA)
	{0xc001b114 ,0x0},	// If value is 0x11400000, it will reduce clock input of H3D to DPPA in half of display PLL
};

/**  @} */

