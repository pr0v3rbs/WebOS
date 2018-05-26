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
 *  main configuration file for de device
 *  de device will teach you how to make device driver with new platform.
 *
 *  author      dongho7.park (dongho7.park@lge.com)
 *  version     1.0
 *  date        2009.12.30
 *  note        Additional information.
 *
 *  @addtogroup lg1150_de
 *  @{
 */

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "os_util.h"
#include "de_ver_def.h"
#include "de_cfg.h"
#include "de_def.h"
#ifdef USE_KDRV_CODES_FOR_H13
#include "h13/de/de_cfg_h13.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14
#include "m14/de/de_cfg_m14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H14
#include "h14/de/de_cfg_h14.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H15
#include "h15/de/de_cfg_h15.h"
#endif

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
LX_DE_MEM_CFG_T gMemCfgDe[] =
{
	// for h13 A0/B0 real chip.
    {
#ifdef USE_KDRV_CODES_FOR_H13
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_H13
            {   "fw_led", VIDEO_H13_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H13_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H13_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H13_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_H13_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H13_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_H13_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H13_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_H13_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H13_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H13_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H13_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_H13_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_H13_MEM_SIZE_OF_FRAME_MEMORY,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	},

	// for m14 A0 real chip.
    {
#ifdef USE_KDRV_CODES_FOR_M14
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_M14
            {   "fw_led", VIDEO_M14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_M14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_M14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_M14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_M14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_M14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_M14_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_M14_MEM_SIZE_OF_FRAME_MEMORY,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	},

	// for m14 B0 real chip.
    {
#ifdef USE_KDRV_CODES_FOR_M14
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_M14
            {   "fw_led", VIDEO_M14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_M14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_M14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_M14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_M14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_M14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_M14_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_M14_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_M14B_MEM_SIZE_OF_DE_BE,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	},

	// for h14 A0/B0 real chip.
    {
#ifdef USE_KDRV_CODES_FOR_H14
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_H14
            {   "fw_led", VIDEO_H14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H14_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_H14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H14_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_H14_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H14_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_H14_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H14_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H14_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H14_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_H14_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_H14_MEM_SIZE_OF_FRAME_MEMORY,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	},

	// for h15 3GB map.
    {
#ifdef USE_KDRV_CODES_FOR_H15
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_H15
            {   "fw_led", VIDEO_H15_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H15_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_H15_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H15_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_H15_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H15_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_H15_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H15_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_H15_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_H15_3G_MEM_SIZE_OF_FRAME_DE,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	},

	// for h15 1GB map.
    {
#ifdef USE_KDRV_CODES_FOR_H15
        {
#ifdef USE_DE_FIRMWARE_RUN_IN_PAK_H15
            {   "fw_led", VIDEO_H15_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H15_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_IPC } ,
            {   "fw_de",  VIDEO_H15_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H15_FIRMWARE_MEM_SIZE_OF_DE  } ,
#else
            {   "fw_de",  VIDEO_H15_FIRMWARE_MEM_BASE_OF_DE,  VIDEO_H15_FIRMWARE_MEM_SIZE_OF_DE  } ,
            {   "fw_led", VIDEO_H15_FIRMWARE_MEM_BASE_OF_LED, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_LED } ,
            {   "fw_ipc", VIDEO_H15_FIRMWARE_MEM_BASE_OF_IPC, VIDEO_H15_FIRMWARE_MEM_SIZE_OF_IPC } ,
#endif
        },
        .frame_name = "DE_FRM",
        .frame_base = VIDEO_H15_MBASE_OF_FRAME_MEMORY,
        .frame_size = VIDEO_H15_1G_MEM_SIZE_OF_FRAME_DE,
#else
	.frame_name = "",
	.frame_base = 0,
	.frame_size = 0
#endif
	}

};

LX_DE_CH_MEM_T gMemCfgDePreW[] =
{
    // for h13 A0/B0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H13
		.fw_name = "DE_PreW",
		.fw_base = 0x0,
		.fw_size = VIDEO_H13_MEM_SIZE_OF_FRAME_PREW,
#endif		
	},
    // for m14 a0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_PreW",
		.fw_base = 0x0,
		.fw_size = VIDEO_M14_MEM_SIZE_OF_FRAME_PREW,
#endif		
	},
    // for m14 b0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_PreW",
		.fw_base = 0x0,
		.fw_size = VIDEO_M14B_MEM_SIZE_OF_FRAME_PREW,
#endif		
	},
	// for h14 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H14
		.fw_name = "DE_PreW",
		.fw_base = 0x0,
		.fw_size = VIDEO_H14_MEM_SIZE_OF_FRAME_PREW,
#endif		
	},
	// for h15 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H15
		.fw_name = "DE_PreW",
		.fw_base = 0x0,
		.fw_size = VIDEO_H15_MEM_SIZE_OF_FRAME_PREW,
#endif		
	}
};

LX_DE_CH_MEM_T gMemCfgDeGrap[] =
{
    // for h13 A0/B0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H13
		.fw_name = "DE_GRAP",
		.fw_base = 0x0,
		.fw_size = VIDEO_H13_MEM_SIZE_OF_FRAME_GRAP + 0x8000, // add 0x8000 for ROW align
#endif
	},
    // for m14 a0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_GRAP",
		.fw_base = 0x0,
		.fw_size = VIDEO_M14_MEM_SIZE_OF_FRAME_GRAP + 0x8000, // add 0x8000 for ROW align
#endif
	},
    // for m14 b0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_GRAP",
		.fw_base = 0x0,
		.fw_size = VIDEO_M14B_MEM_SIZE_OF_FRAME_GRAP,
#endif
	},
	// for h14 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H14
		.fw_name = "DE_GRAP",
		.fw_base = 0x0,
		.fw_size = VIDEO_H14_MEM_SIZE_OF_FRAME_GRAP + 0x8000, // add 0x8000 for ROW align
#endif
	},
	// for h15 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H15
		.fw_name = "DE_GRAP",
		.fw_base = 0x0,
		.fw_size = VIDEO_H15_MEM_SIZE_OF_FRAME_GRAP + 0x8000, // add 0x8000 for ROW align
#endif
	}
};

LX_DE_CH_MEM_T gMemCfgDeFW[] =
{
    // temporal base for h13 B0 real chip.
	{
#ifdef USE_KDRV_CODES_FOR_H13
		.fw_name = "DE_FW_LBUS",
		.fw_base = 0x0,
		.fw_size = (VIDEO_H13_FIRMWARE_MEM_SIZE_OF_DE * 2),
#endif
	},
	// for m14 a0
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_FW_LBUS",
		.fw_base = 0x0,
		.fw_size = (VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE * 2),
#endif
	},
	// for m14 b0
	{
#ifdef USE_KDRV_CODES_FOR_M14
		.fw_name = "DE_FW_LBUS",
		.fw_base = 0x0,
		.fw_size = (VIDEO_M14_FIRMWARE_MEM_SIZE_OF_DE * 2),
#endif
	},
	// for h14 a0
	{
#ifdef USE_KDRV_CODES_FOR_H14
		.fw_name = "DE_FW_LBUS",
		.fw_base = 0x0,
		.fw_size = (VIDEO_H14_FIRMWARE_MEM_SIZE_OF_DE * 2),
#endif
	},
	// for h15 a0
	{
#ifdef USE_KDRV_CODES_FOR_H15
		.fw_name = "DE_FW_LBUS",
		.fw_base = 0x0,
		.fw_size = (VIDEO_H15_FIRMWARE_MEM_SIZE_OF_DE * 2),
#endif
	}
};

//LX_DE_MEM_CFG_T *gpDeMem = &gMemCfgDe[1];   // for FPGA.
LX_DE_MEM_CFG_T *gpDeMem = &gMemCfgDe[0];   // for COSMO.
//LX_DE_MEM_CFG_T *gpDeMem = &gMemCfgDe[2];   // for GP3 L9.

/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
    Implementation Group
========================================================================================*/
int de_cfg_disply(void)
{
	int ret = RET_OK;
	int idx = 0;
	char *fw_name[5] = { NULL }, *frame_name = 0;
	int  fw_base[5] = { 0 },  frame_base = 0;
	int  fw_size[5] = { 0 },  frame_size = 0;

#ifdef USE_KDRV_CODES_FOR_H15
	if (lx_chip_rev( ) >= LX_CHIP_REV(H15,A0) ) {
		idx = 4;
		fw_name[3] = gMemCfgDePreW[4].fw_name;
		fw_base[3] = gMemCfgDePreW[4].fw_base;
		fw_size[3] = gMemCfgDePreW[4].fw_size;
		fw_name[4] = gMemCfgDeGrap[4].fw_name;
		fw_base[4] = gMemCfgDeGrap[4].fw_base;
		fw_size[4] = gMemCfgDeGrap[4].fw_size;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H14
	if (lx_chip_rev( ) >= LX_CHIP_REV(H14,A0) ) {
		idx = 3;
		fw_name[3] = gMemCfgDePreW[3].fw_name;
		fw_base[3] = gMemCfgDePreW[3].fw_base;
		fw_size[3] = gMemCfgDePreW[3].fw_size;
		fw_name[4] = gMemCfgDeGrap[3].fw_name;
		fw_base[4] = gMemCfgDeGrap[3].fw_base;
		fw_size[4] = gMemCfgDeGrap[3].fw_size;
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_M14
	if (lx_chip_rev( ) >= LX_CHIP_REV(M14,A0) ) {
		if (lx_chip_rev( ) >= LX_CHIP_REV(M14,B0) ) {
			idx = 2;
			fw_name[3] = gMemCfgDePreW[2].fw_name;
			fw_base[3] = gMemCfgDePreW[2].fw_base;
			fw_size[3] = gMemCfgDePreW[2].fw_size;
			fw_name[4] = gMemCfgDeGrap[2].fw_name;
			fw_base[4] = gMemCfgDeGrap[2].fw_base;
			fw_size[4] = gMemCfgDeGrap[2].fw_size;
		} else {
			idx = 1;
			fw_name[3] = gMemCfgDePreW[1].fw_name;
			fw_base[3] = gMemCfgDePreW[1].fw_base;
			fw_size[3] = gMemCfgDePreW[1].fw_size;
			fw_name[4] = gMemCfgDeGrap[1].fw_name;
			fw_base[4] = gMemCfgDeGrap[1].fw_base;
			fw_size[4] = gMemCfgDeGrap[1].fw_size;
		}
	} else
#endif
#ifdef USE_KDRV_CODES_FOR_H13
	if (lx_chip_rev( ) >= LX_CHIP_REV(H13,A0) ) {
		idx = 0;
		fw_name[3] = gMemCfgDePreW[0].fw_name;
		fw_base[3] = gMemCfgDePreW[0].fw_base;
		fw_size[3] = gMemCfgDePreW[0].fw_size;
		fw_name[4] = gMemCfgDeGrap[0].fw_name;
		fw_base[4] = gMemCfgDeGrap[0].fw_base;
		fw_size[4] = gMemCfgDeGrap[0].fw_size;
	} else
#endif
	{
	}
		
	fw_name[0] = gMemCfgDe[idx].fw[0].fw_name;
	fw_base[0] = gMemCfgDe[idx].fw[0].fw_base;
	fw_size[0] = gMemCfgDe[idx].fw[0].fw_size;
	fw_name[1] = gMemCfgDe[idx].fw[1].fw_name;
	fw_base[1] = gMemCfgDe[idx].fw[1].fw_base;
	fw_size[1] = gMemCfgDe[idx].fw[1].fw_size;
	fw_name[2] = gMemCfgDe[idx].fw[2].fw_name;
	fw_base[2] = gMemCfgDe[idx].fw[2].fw_base;
	fw_size[2] = gMemCfgDe[idx].fw[2].fw_size;

	frame_name = gMemCfgDe[idx].frame_name;
	frame_base = gMemCfgDe[idx].frame_base;
	frame_size = gMemCfgDe[idx].frame_size;

	DE_PRINT("List of memory map\n");
	DE_PRINT("\tframe name : %s\n", frame_name);
	DE_PRINT("\tframe base : 0x%08x\n", frame_base);
	DE_PRINT("\tframe size : %d\n", frame_size);
	DE_PRINT("\t  fw[0]\n");
	DE_PRINT("\t    name : %s\n", fw_name[0]);
	DE_PRINT("\t    base : 0x%08x\n", fw_base[0]);
	DE_PRINT("\t    size : %d\n", fw_size[0]);
	DE_PRINT("\t  fw[1]\n");
	DE_PRINT("\t    name : %s\n", fw_name[1]);
	DE_PRINT("\t    base : 0x%08x\n", fw_base[1]);
	DE_PRINT("\t    size : %d\n", fw_size[1]);
	DE_PRINT("\t  fw[2]\n");
	DE_PRINT("\t    name : %s\n", fw_name[2]);
	DE_PRINT("\t    base : 0x%08x\n", fw_base[2]);
	DE_PRINT("\t    size : %d\n", fw_size[2]);
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_LBUS
	DE_PRINT("\t  fw is running on L-BUS\n");
#else
	DE_PRINT("\t  fw is running on G-BUS\n");
#endif
	if(fw_size[3])
	{
		DE_PRINT("\tframe name : %s\n", fw_name[3]);
		DE_PRINT("\tframe base : 0x%08x\n", fw_base[3]);
		DE_PRINT("\tframe size : %d\n", fw_size[3]);
	}
	else
	{
		DE_PRINT("\tframe name : %s isn't allocated on external frame memory\n",\
				fw_name[3]);
	}
	if(fw_size[4])
	{
		DE_PRINT("\tframe name : %s\n", fw_name[4]);
		DE_PRINT("\tframe base : 0x%08x\n", fw_base[4]);
		DE_PRINT("\tframe size : %d\n", fw_size[4]);
	}
	else
	{
		DE_PRINT("\tframe name : %s isn't allocated on external frame memory\n",\
				fw_name[3]);
	}

	return ret;
}

int de_cfg_get_fcmem(UINT32 *mem0, UINT32 *mem1, UINT32 *size)
{
	int ret = RET_OK;

	*mem0 = 0;
	*mem1 = 0;
	*size = 2048 * 1080;

	return ret;
}

/** @} */
