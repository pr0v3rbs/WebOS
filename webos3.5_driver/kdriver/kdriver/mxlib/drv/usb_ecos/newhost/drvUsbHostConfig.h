//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
#ifndef DRV_USBHOST_CONFIG_H
#define DRV_USBHOST_CONFIG_H

#include <MsIRQ.h>

//#define URB_TIMEOUT_BY_WAIT_EVENT

#define ROOTHUB_TIMER_INTERVAL  500

#define NUM_OF_ROOT_HUB 4

#define ENABLE_LEGACY_CACHE_SETTING

//#define CPU_TYPE_AEON
#if defined(CHIP_KAISER)
#define CPU_TYPE_ARM
#else
#define CPU_TYPE_MIPS
#endif

#if defined(CPU_TYPE_AEON)
#define OS_BASE_ADDR        0xa0000000
#elif defined(CPU_TYPE_MIPS) // U4, K1, K2
    #if defined(TITANIA2_SERIAL_USE) 
    #define OS_BASE_ADDR    0xbf800000
    #else
    #define OS_BASE_ADDR    0xbf200000
    #endif
    #define MIPS_L1_CACHE_SIZE 32
#elif defined(CPU_TYPE_ARM) // K3
    #define OS_BASE_ADDR        0xfd200000
#else
#No CPU type for USB
#endif

#ifdef ENABLE_LEGACY_CACHE_SETTING
#define CPU_L1_CACHE_BOUND (15)
#else
#define CPU_L1_CACHE_BOUND (MIPS_L1_CACHE_SIZE-1)
#endif

#define KAISERIN_CHIP_TOP_BASE (OS_BASE_ADDR+(0x1E00*2))
 
#define BASE_USBBC_NULL     (0)

/****** USB port RIU base address ******/
#define BASE_UTMI0          (OS_BASE_ADDR+(0x3A80*2))
#define BASE_UHC0           (OS_BASE_ADDR+(0x2400*2))
#define BASE_USBC0          (OS_BASE_ADDR+(0x0700*2))
#define BASE_USBBC0_KAPPA   (OS_BASE_ADDR+(0x20500*2))
#define BASE_USBBC0_KELTIC  (OS_BASE_ADDR+(0x11700*2))
#define BASE_USBBC0_KENYA   (OS_BASE_ADDR+(0x205E0*2))
#define BASE_USBBC0_KAISER  (OS_BASE_ADDR+(0x22F00*2))
#define BASE_USBBC0_KERES   (OS_BASE_ADDR+(0x13700*2))
#define E_IRQ_UHC          (E_INT_IRQ_UHC)
#define E_IRQ_USBC          (E_INT_IRQ_USB)

#define BASE_UTMI1          (OS_BASE_ADDR+(0x3A00*2))
#define BASE_UHC1           (OS_BASE_ADDR+(0x0D00*2))
#define BASE_USBC1          (OS_BASE_ADDR+(0x0780*2))
#define BASE_USBBC1_KENYA   (OS_BASE_ADDR+(0x205C0*2))
#define BASE_USBBC1_KAISER  (OS_BASE_ADDR+(0x22F40*2))
#define BASE_USBBC1_KERES   (OS_BASE_ADDR+(0x13740*2))
#define E_IRQ_UHC1          (E_INT_IRQ_UHC1)
#define E_IRQ_USBC1          (E_INT_IRQ_USB1)

#define BASE_UTMI2          (OS_BASE_ADDR+(0x2A00*2))
#define BASE_UHC2           (OS_BASE_ADDR+(0x10300*2))
#define BASE_USBC2          (OS_BASE_ADDR+(0x10200*2))
#define BASE_USBBC2_KAISER  (OS_BASE_ADDR+(0x22F80*2))
#define E_IRQ_UHC2          (E_INT_IRQ_UHC2)
#define E_IRQ_USBC2          (E_INT_IRQ_USB2)

#define BASE_UTMI3      (OS_BASE_ADDR+(0x20A00*2))
#define BASE_UHC3          (OS_BASE_ADDR+(0x20900*2))
#define BASE_USBC3          (OS_BASE_ADDR+(0x20800*2))
#define E_IRQ_UHC3         (E_INT_IRQ_UHC3)
#define E_IRQ_USBC3          (E_INT_IRQ_USB3)

//------ Hardware ECO enable switch ----------------------------------
//---- 1. fix pv2mi bridge mis-behavior
#if defined(CHIP_KAPPA) || \
	defined(CHIP_KELTIC) || \
	defined(CHIP_KENYA) || \
	defined(CHIP_KRITI) || \
	defined(CHIP_KERES)
        #define ENABLE_PV2MI_BRIDGE_ECO
#endif

//---- 2. Reduce EOF1 to 16us for performance imporvement
#if !defined(CHIP_U4) && !defined(CHIP_K1) && !defined(CHIP_K2)
/* Enlarge EOF1 from 12us to 16us for babble prvention under hub case.
 * However, we keep the "old config name". 20130121
 */
        #define ENABLE_16US_EOF1
#endif

//---- 3. Enable UTMI 240 as 120 phase
#if defined(CHIP_KAISER)
        #define ENABLE_UTMI_240_AS_120_PHASE_ECO
#endif

//---- 4. Change to 55 interface
#if defined(CHIP_KAISER)
        #define ENABLE_UTMI_55_INTERFACE
#endif

//---- 5. tx/rx reset clock gating cause XIU timeout
#if defined(CHIP_KAISER)
        #define ENABLE_TX_RX_RESET_CLK_GATING_ECO
#endif

//---- 6. Setting PV2MI bridge read/write burst size to minimum
#if 0 // every chip must apply it, so far
        #define _USB_MINI_PV2MI_BURST_SIZE 0
#else
        #define _USB_MINI_PV2MI_BURST_SIZE 1
#endif

//---- 7. HS connection fail problem (Gate into VFALL state)
#if defined(CHIP_KELTIC) || \
	defined(CHIP_KERES)
	#define ENABLE_HS_CONNECTION_FAIL_INTO_VFALL_ECO
#endif

//------ Software patch enable switch ----------------------------------
//---- 1. Monkey Test software Patch
#if defined(CHIP_U4) || defined(CHIP_K1) || defined(CHIP_K2)
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    1
#else
        #define _USB_HS_CUR_DRIVE_DM_ALLWAYS_HIGH_PATCH    0
#endif

//---- 2. Clock phase adjustment software Patch
#if defined(CHIP_KERES)
        #define _USB_CLOCK_PHASE_ADJ_PATCH    1
#else
        #define _USB_CLOCK_PHASE_ADJ_PATCH    0
#endif

//-----------------------------------------
// U4_series_usb_init flag:
// Use low word as flag
#define EHCFLAG_NONE             0x0
#define EHCFLAG_DPDM_SWAP        0x1
#define EHCFLAG_TESTPKG          0x2
#define EHCFLAG_DOUBLE_DATARATE  0x4
#define EHCFLAG_USBBC_OFF        0x8
// Use high word as data
#define EHCFLAG_DDR_MASK     0xF0000000
#define EHCFLAG_DDR_x15      0x10000000 //480MHz x1.5
#define EHCFLAG_DDR_x18      0x20000000 //480MHz x1.8
//-----------------------------------------

#define HUB_STACK_SIZE  0x2000
#define ms_usbhost_err(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)


//------ UTMI eye diagram parameters ---------------------------------
#if 0
        // for 40nm
        #define UTMI_EYE_SETTING_2C     (0x98)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x10)
        #define UTMI_EYE_SETTING_2F     (0x01)
#elif 0
        // for 40nm after Agate, use 55nm setting7
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting6
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting5
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x30)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting4
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif 0
        // for 40nm after Agate, use 55nm setting3
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x03)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#elif defined(CHIP_KENYA) || \
    defined(CHIP_KAISER)
        // for 40nm after Agate, use 55nm setting2
        #define UTMI_EYE_SETTING_2C     (0x90)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#else
        // for 40nm after Agate, use 55nm setting1, the default
        #define UTMI_EYE_SETTING_2C     (0x10)
        #define UTMI_EYE_SETTING_2D     (0x02)
        #define UTMI_EYE_SETTING_2E     (0x00)
        #define UTMI_EYE_SETTING_2F     (0x81)
#endif

#endif

