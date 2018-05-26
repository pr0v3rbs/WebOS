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

#ifndef DE_MODEL_h
#define DE_MODEL_h

#define USE_LINUX_KERNEL

#ifdef INCLUDE_KDRV_VER_FPGA
#define USE_VIDEO_FOR_FPGA
#endif

#ifdef USE_QEMU_SYSTEM
#  ifdef USE_VIDEO_IOCTL_CALLING
#    define USE_VIDEO_TEST_MCU_IN_CPU
#  endif
#endif

#undef  USE_DE_TRACE_IPC_MSG
#undef  USE_DE_TRACE_IPC_CHK

#undef  USE_DE_SETTING_SDEC_CLK_FROM_MVI
#define USE_DE_FIRMWARE_DWONLOAD_IN_DRIVER
#undef  USE_DE_FIRMWARE_DOWNLOAD_FROM_FILE
#define USE_DE_FIRMWARE_DOWNLOAD_FROM_KENEL
#undef USE_DE_FIRMWARE_DOWNLOAD_FROM_LBUS
//#define USE_DE_FIRMWARE_DOWNLOAD_FROM_LBUS

#undef  USE_DE_FIRMWARE_RUN_IN_PAK
#define USE_DE_FIRMWARE_RUN_IN_ROM
#undef  USE_DE_FIRMWARE_RUN_IN_DDR
#define USE_DE_FIRMWARE_LOAD_DRM_IRM_EACH
#define USE_DE_FIRMWARE_RUN_IN_PAK_H13
#undef  USE_DE_FIRMWARE_RUN_IN_ROM_H13
#undef  USE_DE_FIRMWARE_RUN_IN_DDR_H13

#define USE_DE_FIRMWARE_RUN_IN_PAK_H14
#undef  USE_DE_FIRMWARE_RUN_IN_ROM_H14
#undef  USE_DE_FIRMWARE_RUN_IN_DDR_H14

#define USE_DE_FIRMWARE_RUN_IN_PAK_M14
#undef  USE_DE_FIRMWARE_RUN_IN_ROM_M14
#undef  USE_DE_FIRMWARE_RUN_IN_DDR_M14

#define USE_DE_FIRMWARE_RUN_IN_PAK_H15
#undef  USE_DE_FIRMWARE_RUN_IN_ROM_H15
#undef  USE_DE_FIRMWARE_RUN_IN_DDR_H15

#define USE_DE_PWM_REVISION
#undef USE_DE_PWM_REVISION

#define USE_PWM_RUN_IN_KDRIVER

#define USE_SUPPORT_EUROPE
#define USE_DE_CVI_DELAY_ON_MCU_PART
#define USE_COLUMN_ALTERNATIVE_HALF_CLOCK
#define USE_SUPPORT_AMAZON
#endif
