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
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2010.07.16
 * note       Additional information.
 *
 * @addtogroup lg1150_de
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include "base_types.h"
#include "os_util.h"
#include "proc_util.h"

#include "de_model.h"
#include "de_ver_def.h"

#ifdef USE_KDRV_CODES_FOR_M14
#include "de_kapi.h"
#include "de_def.h"
#include "de_hal_def.h"
#include "mcu/de_int_def.h"
#include "mcu/de_fw_def.h"
#include "mcu/de_fw.h"
#include "de_ipc_def_m14.h"
#include "de_reg_m14.h"
#if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)
#ifdef USE_VIDEO_FOR_FPGA
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_pak_m14b0f.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_pak_m14a0f.h"
#endif
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_rom_m14b0f.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_rom_m14a0f.h"
#endif
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_ddr_m14b0f.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_ddr_m14a0f.h"
#endif
#else
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_ram_m14b0f.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_ram_m14a0f.h"
#endif
#endif
#endif
#else
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
#ifdef USE_KDRV_CODES_FOR_M14C0
#include "de_fw_pak_m14c0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14B0
#ifdef USE_KDRV_CODES_FOR_M14B2
#include "de_fw_pak_m14b2.h"
#else
#include "de_fw_pak_m14b0.h"
#endif
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_pak_m14a0.h"
#endif
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_rom_m14b0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_rom_m14a0.h"
#endif
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_ddr_m14b0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_ddr_m14a0.h"
#endif
#else
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_fw_ram_m14b0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_fw_ram_m14a0.h"
#endif
#endif
#endif
#endif
#endif // #if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)

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

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
#if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)
#ifdef USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_FILE
static DE_FW_FILE_T sDeFirmwareFromFile_M14A0[] = {
/* [M14] minjun.kim 2010-09-28 아래 값은 수정하지 마시요 */
    //{ 1, "/mnt/nsu/base/res/lglib/kdrv/firmware/led_l8_ddr.bin"},
    { 2, "/mnt/nsu/base/res/lglib/kdrv/firmware/video_asc.bin"},
    //{ 3, "/mnt/nsu/base/res/lglib/kdrv/firmware/video_ddr.bin"},
};
#endif
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
LX_DE_FW_DWLD_T sDeFirmwareFromHeader_M14C0[] = {
#if defined(USE_KDRV_CODES_FOR_M14C0)
#if defined(USE_VIDEO_FOR_FPGA)

#else
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
    { 3, sizeof(gDeFirmwareInDDR_PAK_M14C0),     (char *)gDeFirmwareInDDR_PAK_M14C0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
    { 1, sizeof(gDeFirmwareInIRM_ROM_M14C0),     (char *)gDeFirmwareInIRM_ROM_M14C0  },
    { 2, sizeof(gDeFirmwareInDRM_ROM_M14C0),     (char *)gDeFirmwareInDRM_ROM_M14C0  },
    { 3, sizeof(gDeFirmwareInDDR_ROM_M14C0),     (char *)gDeFirmwareInDDR_ROM_M14C0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
    { 1, sizeof(gDeFirmwareInIRM_DDR_M14C0),     (char *)gDeFirmwareInIRM_DDR_M14C0  },
    { 2, sizeof(gDeFirmwareInDRM_DDR_M14C0),     (char *)gDeFirmwareInDRM_DDR_M14C0  },
    { 3, sizeof(gDeFirmwareInDDR_DDR_M14C0),     (char *)gDeFirmwareInDDR_DDR_M14C0  },
#else
    { 2, sizeof(gDeFirmwareInIRM_RAM_M14C0),    (char *)gDeFirmwareInIRM_RAM_M14C0   },
#endif
#endif
#endif
};
LX_DE_FW_DWLD_T sDeFirmwareFromHeader_M14B0[] = {
#if defined(USE_KDRV_CODES_FOR_M14B0)
#if defined(USE_VIDEO_FOR_FPGA)
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
    { 3, sizeof(gDeFirmwareInDDR_PAK_M14B0F),    (char *)gDeFirmwareInDDR_PAK_M14B0F  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
    { 1, sizeof(gDeFirmwareInIRM_ROM_M14B0F),    (char *)gDeFirmwareInIRM_ROM_M14B0F  },
    { 2, sizeof(gDeFirmwareInDRM_ROM_M14B0F),    (char *)gDeFirmwareInDRM_ROM_M14B0F  },
    { 3, sizeof(gDeFirmwareInDDR_ROM_M14B0F),    (char *)gDeFirmwareInDDR_ROM_M14B0F  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
    { 1, sizeof(gDeFirmwareInIRM_DDR_M14B0F),    (char *)gDeFirmwareInIRM_DDR_M14B0F  },
    { 2, sizeof(gDeFirmwareInDRM_DDR_M14B0F),    (char *)gDeFirmwareInDRM_DDR_M14B0F  },
    { 3, sizeof(gDeFirmwareInDDR_DDR_M14B0F),    (char *)gDeFirmwareInDDR_DDR_M14B0F  },
#else
    { 2, sizeof(gDeFirmwareInIRM_RAM_M14B0F),    (char *)gDeFirmwareInIRM_RAM_M14B0F  },
#endif
#else
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
    { 3, sizeof(gDeFirmwareInDDR_PAK_M14B0),     (char *)gDeFirmwareInDDR_PAK_M14B0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
    { 1, sizeof(gDeFirmwareInIRM_ROM_M14B0),     (char *)gDeFirmwareInIRM_ROM_M14B0  },
    { 2, sizeof(gDeFirmwareInDRM_ROM_M14B0),     (char *)gDeFirmwareInDRM_ROM_M14B0  },
    { 3, sizeof(gDeFirmwareInDDR_ROM_M14B0),     (char *)gDeFirmwareInDDR_ROM_M14B0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
    { 1, sizeof(gDeFirmwareInIRM_DDR_M14B0),     (char *)gDeFirmwareInIRM_DDR_M14B0  },
    { 2, sizeof(gDeFirmwareInDRM_DDR_M14B0),     (char *)gDeFirmwareInDRM_DDR_M14B0  },
    { 3, sizeof(gDeFirmwareInDDR_DDR_M14B0),     (char *)gDeFirmwareInDDR_DDR_M14B0  },
#else
    { 2, sizeof(gDeFirmwareInIRM_RAM_M14B0),    (char *)gDeFirmwareInIRM_RAM_M14B0   },
#endif
#endif
#endif
};
LX_DE_FW_DWLD_T sDeFirmwareFromHeader_M14A0[] = {
#if defined(USE_KDRV_CODES_FOR_M14A0)
#if defined(USE_VIDEO_FOR_FPGA)
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
	{ 3, sizeof(gDeFirmwareInDDR_PAK_M14A0F),    (char *)gDeFirmwareInDDR_PAK_M14A0F  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
    { 1, sizeof(gDeFirmwareInIRM_ROM_M14A0F),    (char *)gDeFirmwareInIRM_ROM_M14A0F  },
    { 2, sizeof(gDeFirmwareInDRM_ROM_M14A0F),    (char *)gDeFirmwareInDRM_ROM_M14A0F  },
    { 3, sizeof(gDeFirmwareInDDR_ROM_M14A0F),    (char *)gDeFirmwareInDDR_ROM_M14A0F  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
    { 1, sizeof(gDeFirmwareInIRM_DDR_M14A0F),    (char *)gDeFirmwareInIRM_DDR_M14A0F  },
    { 2, sizeof(gDeFirmwareInDRM_DDR_M14A0F),    (char *)gDeFirmwareInDRM_DDR_M14A0F  },
    { 3, sizeof(gDeFirmwareInDDR_DDR_M14A0F),    (char *)gDeFirmwareInDDR_DDR_M14A0F  },
#else
    { 1, sizeof(gDeFirmwareInIRM_RAM_M14A0F),    (char *)gDeFirmwareInIRM_RAM_M14A0F  },
    { 2, sizeof(gDeFirmwareInDRM_RAM_M14A0F),    (char *)gDeFirmwareInDRM_RAM_M14A0F  },
#endif
#else
#if defined(USE_DE_FIRMWARE_RUN_IN_PAK_M14)
    { 3, sizeof(gDeFirmwareInDDR_PAK_M14A0),     (char *)gDeFirmwareInDDR_PAK_M14A0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_ROM_M14)
    { 1, sizeof(gDeFirmwareInIRM_ROM_M14A0),     (char *)gDeFirmwareInIRM_ROM_M14A0  },
    { 2, sizeof(gDeFirmwareInDRM_ROM_M14A0),     (char *)gDeFirmwareInDRM_ROM_M14A0  },
    { 3, sizeof(gDeFirmwareInDDR_ROM_M14A0),     (char *)gDeFirmwareInDDR_ROM_M14A0  },
#elif defined(USE_DE_FIRMWARE_RUN_IN_DDR_M14)
    { 1, sizeof(gDeFirmwareInIRM_DDR_M14A0),     (char *)gDeFirmwareInIRM_DDR_M14A0  },
    { 2, sizeof(gDeFirmwareInDRM_DDR_M14A0),     (char *)gDeFirmwareInDRM_DDR_M14A0  },
    { 3, sizeof(gDeFirmwareInDDR_DDR_M14A0),     (char *)gDeFirmwareInDDR_DDR_M14A0  },
#else
    { 1, sizeof(gDeFirmwareInIRM_RAM_M14A0),     (char *)gDeFirmwareInIRM_RAM_M14A0  },
    { 2, sizeof(gDeFirmwareInDRM_RAM_M14A0),     (char *)gDeFirmwareInDRM_RAM_M14A0  },
#endif
#endif
#endif
};
#endif
#endif
#endif // #if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)

/*========================================================================================
 *   Implementation Group
 *=======================================================================================*/

int DE_FW_M14_DownloadFromBin(void)
{
    int ret = RET_OK;
#if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)
#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_FILE
    int i;

    for (i=0;i<ARRAY_SIZE(sDeFirmwareFromFile_M14A0);i++) {
        ret = DE_FW_DownloadFromFile(&sDeFirmwareFromFile_M14A0[i]);
        if (ret) break;
    }
#endif
#endif
    return ret;
}

int DE_FW_M14_DownloadFromHead(void)
{
    int ret = RET_OK;
#if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)
    int i;
	int headerSize;
    LX_DE_FW_DWLD_T *lpDeFirmwareFromHeader = NULL;

#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
	if (lx_chip_rev() >= LX_CHIP_REV(M14, C0)) {
		lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14C0;
		headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14C0);
	} 
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0)) {
			lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14B0;
			headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14B0);
	} 
	else {
        lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14A0;
		headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14A0);
	}

    for (i=0;i<headerSize;i++) {
        ret = DE_REG_M14_FW_Download(&lpDeFirmwareFromHeader[i]);
        if (ret) break;
    }
#endif
#endif
    return ret;
}
#endif

int DE_FW_M14_Verify(void)
{
    int ret = RET_OK;
#if (CONFIG_LX_M14_CHIP_FIRMWARE == 1)
    int i;
	int headerSize;
    LX_DE_FW_DWLD_T *lpDeFirmwareFromHeader = NULL;

#ifdef USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
	if (lx_chip_rev() >= LX_CHIP_REV(M14, C0)) {
		lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14C0;
		headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14C0);
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0)) {
		lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14B0;
		headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14B0);
	}
	else {
        lpDeFirmwareFromHeader = sDeFirmwareFromHeader_M14A0;
		headerSize = ARRAY_SIZE(sDeFirmwareFromHeader_M14A0);
	}

    for (i=0;i<headerSize;i++) {
        ret = DE_REG_M14_FW_Verify(&lpDeFirmwareFromHeader[i]);
        if (ret) break;
    }
#endif
#endif
    return ret;
}

/**  @} */
