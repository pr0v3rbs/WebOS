////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   devEMAC.c
/// @brief  EMAC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/init.h>
#include <linux/autoconf.h>
#include <linux/mii.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/pci.h>
#include <linux/crc32.h>
#include <linux/ethtool.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

#if defined(CONFIG_MIPS)
#include <asm/mips-boards/prom.h>
#include "mhal_chiptop_reg.h"
#elif defined(CONFIG_ARM)
#include <asm/arm-boards/prom.h>
#include <asm/mach/map.h>
#endif
#include "mdrv_types.h"
#include "mst_platform.h"
#include "mdrv_system.h"
#include "chip_int.h"
#include "mhal_emac.h"
#include "mdrv_emac.h"
#if 0
#include "mdrv_system_st.h"
#endif

#ifdef CHIP_FLUSH_READ
#include "chip_setup.h"
#endif
#ifdef CONFIG_EMAC_SUPPLY_RNG
#include <linux/input.h>
#include <random.h>
#include "mhal_rng_reg.h"
#endif
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------
#define EMAC_RX_TMR         (0)
#define EMAC_LINK_TMR       (1)

#define EMAC_CHECK_LINK_TIME    	(HZ)
#define IER_FOR_INT_JULIAN_D   		(0x0000E4B5UL)

#ifdef TX_SOFTWARE_QUEUE
    #define EMAC_CHECK_CNT              (10)
#else
#define EMAC_CHECK_CNT              (500000)
#endif

#define EMAC_TX_PTK_BASE            (TX_SKB_BASE + RAM_VA_PA_OFFSET)

#define ALBANY_OUI_MSB              (0)
#define RTL_8210                    (0x1CUL)

#define RX_THROUGHPUT_TEST 0
#define TX_THROUGHPUT_TEST 0
#define PACKET_THRESHOLD 260
#define TXCOUNT_THRESHOLD 10

#ifdef CONFIG_MSTAR_STR_TIME
/* PIU timer, for measure STR time on development phase */
#define PM_ADDR_EMAC(addr)			 (*((volatile unsigned short*)(0xFD000000 + ((addr) << 1))))
#define REG_WR_EMAC(_reg_, _val_)        do{ PM_ADDR_EMAC(_reg_) = (_val_);  }while(0)
#define REG_RR_EMAC(_reg_)               ({ PM_ADDR_EMAC(_reg_);})
#define REG_PIU_TIMER0_EMAC(_x_)         (0x003020 | (_x_ << 1))
#endif
//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------
u32 contiROVR = 0;
u32 initstate= 0;
u8 txidx =0;
u32 txcount = 0;
spinlock_t emac_lock;

// 0x78c9: link is down.
static u32 phy_status_register = 0x78c9UL;

struct sk_buff *pseudo_packet;

unsigned char phyaddr = 0;
#if TX_THROUGHPUT_TEST
unsigned char packet_content[] = {
0xa4, 0xba, 0xdb, 0x95, 0x25, 0x29, 0x00, 0x30, 0x1b, 0xba, 0x02, 0xdb, 0x08, 0x00, 0x45, 0x00,
0x05, 0xda, 0x69, 0x0a, 0x40, 0x00, 0x40, 0x11, 0xbe, 0x94, 0xac, 0x10, 0x5a, 0xe3, 0xac, 0x10,
0x5a, 0x70, 0x92, 0x7f, 0x13, 0x89, 0x05, 0xc6, 0x0c, 0x5b, 0x00, 0x00, 0x03, 0x73, 0x00, 0x00,
0x00, 0x65, 0x00, 0x06, 0xe1, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
0x13, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0xff, 0xff, 0xfc, 0x18, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31,
0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};
#endif

//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
static struct timer_list EMAC_timer, Link_timer;
#if RX_THROUGHPUT_TEST
static struct timer_list RX_timer;
#endif
static struct net_device *emac_dev;
//-------------------------------------------------------------------------------------------------
//  EMAC Function
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_tx (struct sk_buff *skb, struct net_device *dev);
static void MDev_EMAC_timer_callback( unsigned long value );
static int MDev_EMAC_SwReset(struct net_device *dev);
static void MDev_EMAC_Send_PausePkt(struct net_device* dev);
unsigned long oldTime;
unsigned long PreLinkStatus;

#ifdef CONFIG_MSTAR_EEE
static int MDev_EMAC_IS_TX_IDLE(void);
#endif //CONFIG_MSTAR_EEE

static unsigned long getCurMs(void)
{
	struct timeval tv;
	unsigned long curMs;

	do_gettimeofday(&tv);
    curMs = tv.tv_usec/1000;
	curMs += tv.tv_sec * 1000;
    return curMs;
}

#if RX_THROUGHPUT_TEST
int receive_bytes = 0;
static void RX_timer_callback( unsigned long value){
    int get_bytes = receive_bytes;
    int cur_speed;
    receive_bytes = 0;

    cur_speed = get_bytes*8/20/1024;
    printk(" %dkbps",cur_speed);
    RX_timer.expires = jiffies + 20*EMAC_CHECK_LINK_TIME;
    add_timer(&RX_timer);
}
#endif

//-------------------------------------------------------------------------------------------------
// PHY MANAGEMENT
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Access the PHY to determine the current Link speed and Mode, and update the
// MAC accordingly.
// If no link or auto-negotiation is busy, then no changes are made.
// Returns:  0 : OK
//              -1 : No link
//              -2 : AutoNegotiation still in progress
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_update_linkspeed (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(dev);
    u32 bmsr, bmcr, LocPtrA, speed, duplex;

    // Link status is latched, so read twice to get current value //
    MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
    MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
    if (!(bmsr & BMSR_LSTATUS)){
    #ifdef CONFIG_MSTAR_EEE
        MHal_EMAC_Disable_EEE();

        if (PreLinkStatus == 1)
        {
            MHal_EMAC_Reset_EEE();
        }
    #endif

        PreLinkStatus = 0;

        return -1;          //no link //
    }

    MHal_EMAC_read_phy (phyaddr, MII_BMCR, &bmcr);

    if (bmcr & BMCR_ANENABLE)
    {   /*** AutoNegotiation is enabled ***/
        if (!(bmsr & BMSR_ANEGCOMPLETE))
        {
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    /* invalid argument; wrong type */
            dev_dbg((const struct device *)LocPtr->dev, "%s ==> AutoNegotiation still in progress\n", __func__);
#else
            dev_dbg(&LocPtr->dev, "%s ==> AutoNegotiation still in progress\n", __func__);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
            return -2;
        }

        MHal_EMAC_read_phy (phyaddr, MII_LPA, &LocPtrA);
        if ((LocPtrA & LPA_100FULL) || (LocPtrA & LPA_100HALF))
        {
            speed = SPEED_100;
        }
        else
        {
            speed = SPEED_10;
        }
        if ((LocPtrA & LPA_100FULL) || (LocPtrA & LPA_10FULL))
            duplex = DUPLEX_FULL;
        else
            duplex = DUPLEX_HALF;
    }
    else
    {
        speed = (bmcr & BMCR_SPEED100) ? SPEED_100 : SPEED_10;
        duplex = (bmcr & BMCR_FULLDPLX) ? DUPLEX_FULL : DUPLEX_HALF;
    }

    // Update the MAC //
    MHal_EMAC_update_speed_duplex(speed,duplex);


#ifdef CONFIG_MSTAR_EEE
    /*TX idle, enable EEE*/
    if((MDev_EMAC_IS_TX_IDLE()) && (speed == SPEED_100) && (duplex == DUPLEX_FULL))
    {
        if (PreLinkStatus == 0)
        {
            MHal_EMAC_Enable_EEE(300);
        }
        else
        {
            MHal_EMAC_Enable_EEE(0);
        }
    }
#endif

    PreLinkStatus = 1;

    return 0;
}

static int MDev_EMAC_get_info(struct net_device *dev)
{
    u32 bmsr, bmcr, LocPtrA;
    u32 uRegStatus =0;

    // Link status is latched, so read twice to get current value //
    MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
    MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
    if (!(bmsr & BMSR_LSTATUS)){
        uRegStatus &= ~ETHERNET_TEST_RESET_STATE;
        uRegStatus |= ETHERNET_TEST_NO_LINK; //no link //
    }
    MHal_EMAC_read_phy (phyaddr, MII_BMCR, &bmcr);

    if (bmcr & BMCR_ANENABLE)
    {
        //AutoNegotiation is enabled //
        if (!(bmsr & BMSR_ANEGCOMPLETE))
        {
            uRegStatus &= ~ETHERNET_TEST_RESET_STATE;
            uRegStatus |= ETHERNET_TEST_AUTO_NEGOTIATION; //AutoNegotiation //
        }
        else
        {
            uRegStatus &= ~ETHERNET_TEST_RESET_STATE;
            uRegStatus |= ETHERNET_TEST_LINK_SUCCESS; //link success //
        }

        MHal_EMAC_read_phy (phyaddr, MII_LPA, &LocPtrA);
        if ((LocPtrA & LPA_100FULL) || (LocPtrA & LPA_100HALF))
        {
            uRegStatus |= ETHERNET_TEST_SPEED_100M; //SPEED_100//
        }
        else
        {
            uRegStatus &= ~ETHERNET_TEST_SPEED_100M; //SPEED_10//
        }

        if ((LocPtrA & LPA_100FULL) || (LocPtrA & LPA_10FULL))
        {
            uRegStatus |= ETHERNET_TEST_DUPLEX_FULL; //DUPLEX_FULL//
        }
        else
        {
            uRegStatus &= ~ETHERNET_TEST_DUPLEX_FULL; //DUPLEX_HALF//
        }
    }
    else
    {
        if(bmcr & BMCR_SPEED100)
        {
            uRegStatus |= ETHERNET_TEST_SPEED_100M; //SPEED_100//
        }
        else
        {
            uRegStatus &= ~ETHERNET_TEST_SPEED_100M; //SPEED_10//
        }

        if(bmcr & BMCR_FULLDPLX)
        {
            uRegStatus |= ETHERNET_TEST_DUPLEX_FULL; //DUPLEX_FULL//
        }
        else
        {
            uRegStatus &= ~ETHERNET_TEST_DUPLEX_FULL; //DUPLEX_HALF//
        }
    }

    return uRegStatus;
}

//-------------------------------------------------------------------------------------------------
//Program the hardware MAC address from dev->dev_addr.
//-------------------------------------------------------------------------------------------------
void MDev_EMAC_update_mac_address (struct net_device *dev)
{
    u32 value;
    value = (dev->dev_addr[3] << 24) | (dev->dev_addr[2] << 16) | (dev->dev_addr[1] << 8) |(dev->dev_addr[0]);
    MHal_EMAC_Write_SA1L(value);
    value = (dev->dev_addr[5] << 8) | (dev->dev_addr[4]);
    MHal_EMAC_Write_SA1H(value);
}

//-------------------------------------------------------------------------------------------------
// ADDRESS MANAGEMENT
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Set the ethernet MAC address in dev->dev_addr
//-------------------------------------------------------------------------------------------------
static void MDev_EMAC_get_mac_address (struct net_device *dev)
{
    char addr[6];
    u32 HiAddr, LoAddr;

    // Check if bootloader set address in Specific-Address 1 //
    HiAddr = MHal_EMAC_get_SA1H_addr();
    LoAddr = MHal_EMAC_get_SA1L_addr();

    addr[0] = (LoAddr & 0xffUL);
    addr[1] = (LoAddr & 0xff00UL) >> 8;
    addr[2] = (LoAddr & 0xff0000UL) >> 16;
    addr[3] = (LoAddr & 0xff000000UL) >> 24;
    addr[4] = (HiAddr & 0xffUL);
    addr[5] = (HiAddr & 0xff00UL) >> 8;
#ifdef CONFIG_LG_CHANGE
//20151130 cyli modify for LMTASKWBS-3766
//    printk("\n\ncylitest %s %d : %x:%x:%x:%x:%x:%x\n\n", __func__, __LINE__, addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
    memcpy (dev->dev_addr, &addr, 6);
    return;
#else
    if (is_valid_ether_addr (addr))
    {
        memcpy (dev->dev_addr, &addr, 6);
        return;
    }
#endif
    // Check if bootloader set address in Specific-Address 2 //
    HiAddr = MHal_EMAC_get_SA2H_addr();
    LoAddr = MHal_EMAC_get_SA2L_addr();
    addr[0] = (LoAddr & 0xffUL);
    addr[1] = (LoAddr & 0xff00UL) >> 8;
    addr[2] = (LoAddr & 0xff0000UL) >> 16;
    addr[3] = (LoAddr & 0xff000000UL) >> 24;
    addr[4] = (HiAddr & 0xffUL);
    addr[5] = (HiAddr & 0xff00UL) >> 8;

    if (is_valid_ether_addr (addr))
    {
        memcpy (dev->dev_addr, &addr, 6);
        return;
    }
}

#ifdef URANUS_ETHER_ADDR_CONFIGURABLE
//-------------------------------------------------------------------------------------------------
// Store the new hardware address in dev->dev_addr, and update the MAC.
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_set_mac_address (struct net_device *dev, void *addr)
{
    struct sockaddr *address = addr;
    if (!is_valid_ether_addr (address->sa_data))
        return -EADDRNOTAVAIL;

    memcpy (dev->dev_addr, address->sa_data, dev->addr_len);
    MDev_EMAC_update_mac_address (dev);
    return 0;
}
#endif

//-------------------------------------------------------------------------------------------------
// Mstar Multicast hash rule
//-------------------------------------------------------------------------------------------------
//Hash_index[5] = da[5] ^ da[11] ^ da[17] ^ da[23] ^ da[29] ^ da[35] ^ da[41] ^ da[47]
//Hash_index[4] = da[4] ^ da[10] ^ da[16] ^ da[22] ^ da[28] ^ da[34] ^ da[40] ^ da[46]
//Hash_index[3] = da[3] ^ da[09] ^ da[15] ^ da[21] ^ da[27] ^ da[33] ^ da[39] ^ da[45]
//Hash_index[2] = da[2] ^ da[08] ^ da[14] ^ da[20] ^ da[26] ^ da[32] ^ da[38] ^ da[44]
//Hash_index[1] = da[1] ^ da[07] ^ da[13] ^ da[19] ^ da[25] ^ da[31] ^ da[37] ^ da[43]
//Hash_index[0] = da[0] ^ da[06] ^ da[12] ^ da[18] ^ da[24] ^ da[30] ^ da[36] ^ da[42]
//-------------------------------------------------------------------------------------------------

static void MDev_EMAC_sethashtable(unsigned char *addr)
{
    u32 mc_filter[2];
    u32 uHashIdxBit;
    u32 uHashValue;
    u32 i;
    u32 tmpcrc;
    u32 uSubIdx;
    u64 macaddr;
    u64 mac[6];

    uHashValue = 0;
    macaddr = 0;

    // Restore mac //
    for(i = 0; i < 6;  i++)
    {
        mac[i] =(u64)addr[i];
    }

    // Truncate mac to u64 container //
    macaddr |=  mac[0] | (mac[1] << 8) | (mac[2] << 16);
    macaddr |=  (mac[3] << 24) | (mac[4] << 32) | (mac[5] << 40);

    // Caculate the hash value //
    for(uHashIdxBit = 0; uHashIdxBit < 6;  uHashIdxBit++)
    {
        tmpcrc = (macaddr & (0x1UL << uHashIdxBit)) >> uHashIdxBit;
        for(i = 1; i < 8;  i++)
        {
            uSubIdx = uHashIdxBit + (i * 6);
            tmpcrc = tmpcrc ^ ((macaddr >> uSubIdx) & 0x1);
        }
        uHashValue |= (tmpcrc << uHashIdxBit);
    }

    mc_filter[0] = MHal_EMAC_ReadReg32( REG_ETH_HSL);
    mc_filter[1] = MHal_EMAC_ReadReg32( REG_ETH_HSH);

    // Set the corrsponding bit according to the hash value //
    if(uHashValue < 32)
    {
        mc_filter[0] |= (0x1UL <<  uHashValue);
        MHal_EMAC_WritReg32( REG_ETH_HSL, mc_filter[0] );
    }
    else
    {
        mc_filter[1] |= (0x1UL <<  (uHashValue - 32));
        MHal_EMAC_WritReg32( REG_ETH_HSH, mc_filter[1] );
    }
}

//-------------------------------------------------------------------------------------------------
//Enable/Disable promiscuous and multicast modes.
//-------------------------------------------------------------------------------------------------
static void MDev_EMAC_set_rx_mode (struct net_device *dev)
{
    u32 uRegVal = 0;
    struct netdev_hw_addr *ha;

    uRegVal  = MHal_EMAC_Read_CFG();

    if (dev->flags & IFF_PROMISC)
    {
        // Enable promiscuous mode //
        uRegVal |= EMAC_CAF;
    }
    else if (dev->flags & (~IFF_PROMISC))
    {
        // Disable promiscuous mode //
        uRegVal &= ~EMAC_CAF;
    }
    MHal_EMAC_Write_CFG(uRegVal);

    if (dev->flags & IFF_ALLMULTI)
    {
        // Enable all multicast mode //
        MHal_EMAC_update_HSH(-1,-1);
        uRegVal |= EMAC_MTI;
    }
    else if (dev->flags & IFF_MULTICAST)
    {
        // Enable specific multicasts//
        MHal_EMAC_update_HSH(0,0);
        netdev_for_each_mc_addr(ha, dev)
        {
            MDev_EMAC_sethashtable(ha->addr);
        }
        uRegVal |= EMAC_MTI;
    }
    else if (dev->flags & ~(IFF_ALLMULTI | IFF_MULTICAST))
    {
        // Disable all multicast mode//
        MHal_EMAC_update_HSH(0,0);
        uRegVal &= ~EMAC_MTI;
    }

    MHal_EMAC_Write_CFG(uRegVal);
}
//-------------------------------------------------------------------------------------------------
// IOCTL
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Enable/Disable MDIO
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_mdio_read (struct net_device *dev, int phy_id, int location)
{
    u32 value;
    MHal_EMAC_read_phy (phy_id, location, &value);
    return value;
}

static void MDev_EMAC_mdio_write (struct net_device *dev, int phy_id, int location, int value)
{
    MHal_EMAC_write_phy (phy_id, location, value);
}

//-------------------------------------------------------------------------------------------------
//ethtool support.
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_ethtool_ioctl (struct net_device *dev, void *useraddr)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    u32 ethcmd;
    int res = 0;

    if (copy_from_user (&ethcmd, useraddr, sizeof (ethcmd)))
        return -EFAULT;

    spin_lock_irq (LocPtr->lock);

    switch (ethcmd)
    {
        case ETHTOOL_GSET:
        {
            struct ethtool_cmd ecmd = { ETHTOOL_GSET };
            res = mii_ethtool_gset (&LocPtr->mii, &ecmd);
            if (LocPtr->phy_media == PORT_FIBRE)
            {   //override media type since mii.c doesn't know //
                ecmd.supported = SUPPORTED_FIBRE;
                ecmd.port = PORT_FIBRE;
            }
            if (copy_to_user (useraddr, &ecmd, sizeof (ecmd)))
                res = -EFAULT;
            break;
        }
        case ETHTOOL_SSET:
        {
            struct ethtool_cmd ecmd;
            if (copy_from_user (&ecmd, useraddr, sizeof (ecmd)))
                res = -EFAULT;
            else
                res = mii_ethtool_sset (&LocPtr->mii, &ecmd);
            break;
        }
        case ETHTOOL_NWAY_RST:
        {
            res = mii_nway_restart (&LocPtr->mii);
            break;
        }
        case ETHTOOL_GLINK:
        {
            struct ethtool_value edata = { ETHTOOL_GLINK };
            edata.data = mii_link_ok (&LocPtr->mii);
            if (copy_to_user (useraddr, &edata, sizeof (edata)))
                res = -EFAULT;
            break;
        }
        default:
            res = -EOPNOTSUPP;
    }
    spin_unlock_irq (LocPtr->lock);
    return res;
}

//-------------------------------------------------------------------------------------------------
// User-space ioctl interface.
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_ioctl (struct net_device *dev, struct ifreq *rq, int cmd)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    struct mii_ioctl_data *data = if_mii(rq);

    if (!netif_running(dev))
    {
        rq->ifr_metric = ETHERNET_TEST_INIT_FAIL;
    }

    switch (cmd)
    {
        case SIOCGMIIPHY:
            data->phy_id = (phyaddr & 0x1FUL);
            return 0;

        case SIOCDEVPRIVATE:
            rq->ifr_metric = (MDev_EMAC_get_info(emac_dev)|initstate);
            return 0;

        case SIOCDEVON:
            MHal_EMAC_Power_On_Clk();
            return 0;

        case SIOCDEVOFF:
            MHal_EMAC_Power_Off_Clk();
            return 0;

        case SIOCGMIIREG:
            // check PHY's register 1.
            if((data->reg_num & 0x1fUL) == 0x1UL)
            {
                // PHY's register 1 value is set by timer callback function.
                spin_lock_irq(LocPtr->lock);
                data->val_out = phy_status_register;
                spin_unlock_irq(LocPtr->lock);
            }
            else
            {
                MHal_EMAC_read_phy((phyaddr & 0x1FUL), (data->reg_num & 0x1fUL), (u32 *)&(data->val_out));
            }
            return 0;

        case SIOCSMIIREG:
            if (!capable(CAP_NET_ADMIN))
                return -EPERM;
            MHal_EMAC_write_phy((phyaddr & 0x1FUL), (data->reg_num & 0x1fUL), data->val_in);
            return 0;

        case SIOCETHTOOL:
            return MDev_EMAC_ethtool_ioctl (dev, (void *) rq->ifr_data);

        default:
            return -EOPNOTSUPP;
    }
}
//-------------------------------------------------------------------------------------------------
// MAC
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//Initialize and start the Receiver and Transmit subsystems
//-------------------------------------------------------------------------------------------------
static void MDev_EMAC_start (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    struct recv_desc_bufs *dlist, *dlist_phys;
#ifndef SOFTWARE_DESCRIPTOR
    int i;
#endif
    u32 uRegVal;

    dlist = LocPtr->dlist;
    dlist_phys = LocPtr->dlist_phys;
#ifdef SOFTWARE_DESCRIPTOR
    dlist->descriptors[MAX_RX_DESCR - 1].addr |= EMAC_DESC_WRAP;
#else
    for(i = 0; i < MAX_RX_DESCR; i++)
    {
        dlist->descriptors[i].addr = 0;
        dlist->descriptors[i].size = 0;
    }
    // Set the Wrap bit on the last descriptor //
    dlist->descriptors[MAX_RX_DESCR - 1].addr = EMAC_DESC_WRAP;
#endif //#ifndef SOFTWARE_DESCRIPTOR
    // set offset of read and write pointers in the receive circular buffer //
    uRegVal = MHal_EMAC_Read_BUFF();
    uRegVal = (RX_BUFFER_BASE|RX_BUFFER_SEL) - MIU0_BUS_BASE;
    MHal_EMAC_Write_BUFF(uRegVal);
    MHal_EMAC_Write_RDPTR(0);
    MHal_EMAC_Write_WRPTR(0);

    // Program address of descriptor list in Rx Buffer Queue register //
    uRegVal = ((phys_addr_t) & dlist_phys->descriptors)- RAM_VA_PA_OFFSET - MIU0_BUS_BASE;
    MHal_EMAC_Write_RBQP(uRegVal);

    //Reset buffer index//
    LocPtr->rxBuffIndex = 0;

    // Enable Receive and Transmit //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= (EMAC_RE | EMAC_TE);
    MHal_EMAC_Write_CTL(uRegVal);
}

//-------------------------------------------------------------------------------------------------
// Open the ethernet interface
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_open (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    u32 uRegVal;
    int ret;

    spin_lock_irq (LocPtr->lock);
    ret = MDev_EMAC_update_linkspeed(dev);
    spin_unlock_irq (LocPtr->lock);

    if (!is_valid_ether_addr (dev->dev_addr))
       return -EADDRNOTAVAIL;

    //ato  EMAC_SYS->PMC_PCER = 1 << EMAC_ID_EMAC;   //Re-enable Peripheral clock //
    MHal_EMAC_Power_On_Clk();
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal |= EMAC_CSR;
    MHal_EMAC_Write_CTL(uRegVal);
    // Enable PHY interrupt //
    MHal_EMAC_enable_phyirq ();

    // Enable MAC interrupts //
#ifndef INT_JULIAN_D
    uRegVal = EMAC_INT_RCOM | IER_FOR_INT_JULIAN_D;
    MHal_EMAC_Write_IER(uRegVal);
#else
    MHal_EMAC_Write_IER(IER_FOR_INT_JULIAN_D);
#endif

    LocPtr->ep_flag |= EP_FLAG_OPEND;

    MDev_EMAC_set_rx_mode(dev);
    MDev_EMAC_start (dev);
    netif_start_queue (dev);

    init_timer( &Link_timer );
    Link_timer.data = EMAC_LINK_TMR;
    Link_timer.function = MDev_EMAC_timer_callback;
    Link_timer.expires = jiffies + EMAC_CHECK_LINK_TIME;
    add_timer(&Link_timer);

    /* check if network linked */
    if (-1 == ret)
    {
        netif_carrier_off(dev);
        ThisBCE.connected = 0;
    }
    else if(0 == ret)
    {
        netif_carrier_on(dev);
        ThisBCE.connected = 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------
// Close the interface
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_close (struct net_device *dev)
{
    u32 uRegVal;
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    //Disable Receiver and Transmitter //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal &= ~(EMAC_TE | EMAC_RE);
    MHal_EMAC_Write_CTL(uRegVal);
    // Disable PHY interrupt //
    MHal_EMAC_disable_phyirq ();
#ifndef INT_JULIAN_D
    //Disable MAC interrupts //
    uRegVal = EMAC_INT_RCOM | IER_FOR_INT_JULIAN_D;
    MHal_EMAC_Write_IDR(uRegVal);
#else
    MHal_EMAC_Write_IDR(IER_FOR_INT_JULIAN_D);
#endif
    netif_stop_queue (dev);
    netif_carrier_off(dev);
    del_timer(&Link_timer);
    MHal_EMAC_Power_Off_Clk();
    ThisBCE.connected = 0;
    LocPtr->ep_flag &= (~EP_FLAG_OPEND);

    return 0;
}

//-------------------------------------------------------------------------------------------------
// Update the current statistics from the internal statistics registers.
//-------------------------------------------------------------------------------------------------
static struct net_device_stats * MDev_EMAC_stats (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    int ale, lenerr, seqe, lcol, ecol;

    spin_lock_irq (LocPtr->lock);

    if (netif_running (dev))
    {
        LocPtr->stats.rx_packets += MHal_EMAC_Read_OK();            /* Good frames received */
        ale = MHal_EMAC_Read_ALE();
        LocPtr->stats.rx_frame_errors += ale;                       /* Alignment errors */
        lenerr = MHal_EMAC_Read_ELR();
        LocPtr->stats.rx_length_errors += lenerr;                   /* Excessive Length or Undersize Frame error */
        seqe = MHal_EMAC_Read_SEQE();
        LocPtr->stats.rx_crc_errors += seqe;                        /* CRC error */
        LocPtr->stats.rx_fifo_errors += MHal_EMAC_Read_ROVR();
        LocPtr->stats.rx_errors += ale + lenerr + seqe + MHal_EMAC_Read_SE() + MHal_EMAC_Read_RJB();
        LocPtr->stats.tx_packets += MHal_EMAC_Read_FRA();           /* Frames successfully transmitted */
        LocPtr->stats.tx_fifo_errors += MHal_EMAC_Read_TUE();       /* Transmit FIFO underruns */
        LocPtr->stats.tx_carrier_errors += MHal_EMAC_Read_CSE();    /* Carrier Sense errors */
        LocPtr->stats.tx_heartbeat_errors += MHal_EMAC_Read_SQEE(); /* Heartbeat error */
        lcol = MHal_EMAC_Read_LCOL();
        ecol = MHal_EMAC_Read_ECOL();
        LocPtr->stats.tx_window_errors += lcol;                     /* Late collisions */
        LocPtr->stats.tx_aborted_errors += ecol;                    /* 16 collisions */
        LocPtr->stats.collisions += MHal_EMAC_Read_SCOL() + MHal_EMAC_Read_MCOL() + lcol + ecol;
    }

    spin_unlock_irq (LocPtr->lock);

    return &LocPtr->stats;
}

static int MDev_EMAC_TxReset(void)
{
    u32 val = MHal_EMAC_Read_CTL() & 0x000001FFUL;

    MHal_EMAC_Write_CTL((val & ~EMAC_TE));
    MHal_EMAC_Write_TCR(0);
    MHal_EMAC_Write_CTL((MHal_EMAC_Read_CTL() | EMAC_TE));
    return 0;
}

static int MDev_EMAC_CheckTSR(void)
{
    u32 check;
    u32 tsrval = 0;

    #ifdef TX_QUEUE_4
    u8  avlfifo[8] = {0};
    u8  avlfifoidx;
    u8  avlfifoval = 0;

    for (check = 0; check < EMAC_CHECK_CNT; check++)
    {
        tsrval = MHal_EMAC_Read_TSR();

        avlfifo[0] = ((tsrval & EMAC_IDLETSR) != 0)? 1 : 0;
        avlfifo[1] = ((tsrval & EMAC_BNQ)!= 0)? 1 : 0;
        avlfifo[2] = ((tsrval & EMAC_TBNQ) != 0)? 1 : 0;
        avlfifo[3] = ((tsrval & EMAC_FBNQ) != 0)? 1 : 0;
        avlfifo[4] = ((tsrval & EMAC_FIFO1IDLE) !=0)? 1 : 0;
        avlfifo[5] = ((tsrval & EMAC_FIFO2IDLE) != 0)? 1 : 0;
        avlfifo[6] = ((tsrval & EMAC_FIFO3IDLE) != 0)? 1 : 0;
        avlfifo[7] = ((tsrval & EMAC_FIFO4IDLE) != 0)? 1 : 0;

        avlfifoval = 0;

        for(avlfifoidx = 0; avlfifoidx < 8; avlfifoidx++)
        {
            avlfifoval += avlfifo[avlfifoidx];
        }

        if (avlfifoval > 4)
            return NETDEV_TX_OK;
    }
    #else
    for (check = 0; check < EMAC_CHECK_CNT; check++)
    {
        tsrval = MHal_EMAC_Read_TSR();
        if ((tsrval & EMAC_IDLETSR) || (tsrval & EMAC_BNQ))
            return NETDEV_TX_OK;
    }
    #endif

#ifndef TX_SOFTWARE_QUEUE
    EMAC_DBG("Err CheckTSR:0x%x\n", tsrval);
    MDev_EMAC_TxReset();
#endif

    return NETDEV_TX_BUSY;
}

#ifdef CONFIG_MSTAR_EEE
static int MDev_EMAC_IS_TX_IDLE(void)
{
    u32 check;
    u32 tsrval = 0;

    u8  avlfifo[8] = {0};
    u8  avlfifoidx;
    u8  avlfifoval = 0;

#ifdef TX_QUEUE_4
    for (check = 0; check < EMAC_CHECK_CNT; check++)
    {
        tsrval = MHal_EMAC_Read_TSR();

        avlfifo[0] = ((tsrval & EMAC_IDLETSR) != 0)? 1 : 0;
        avlfifo[1] = ((tsrval & EMAC_BNQ)!= 0)? 1 : 0;
        avlfifo[2] = ((tsrval & EMAC_TBNQ) != 0)? 1 : 0;
        avlfifo[3] = ((tsrval & EMAC_FBNQ) != 0)? 1 : 0;
        avlfifo[4] = ((tsrval & EMAC_FIFO1IDLE) !=0)? 1 : 0;
        avlfifo[5] = ((tsrval & EMAC_FIFO2IDLE) != 0)? 1 : 0;
        avlfifo[6] = ((tsrval & EMAC_FIFO3IDLE) != 0)? 1 : 0;
        avlfifo[7] = ((tsrval & EMAC_FIFO4IDLE) != 0)? 1 : 0;

        avlfifoval = 0;

        for(avlfifoidx = 0; avlfifoidx < 8; avlfifoidx++)
        {
            avlfifoval += avlfifo[avlfifoidx];
        }

        if (avlfifoval == 8)
            return 1;
    }
#endif

    return 0;
}
#endif //CONFIG_MSTAR_EEE

static u8 pause_pkt[] =
{
    //DA - multicast
    0x01, 0x80, 0xC2, 0x00, 0x00, 0x01,
    //SA
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //Len-Type
    0x88, 0x08,
    //Ctrl code
    0x00, 0x01,
    //Ctrl para 8192
    0x20, 0x00
};

inline static dma_addr_t get_tx_addr(void)
{
    dma_addr_t addr;

    addr = EMAC_TX_PTK_BASE + 2048*txidx;
    txidx ++;
    txidx = txidx % TX_RING_SIZE;
    return addr;
}

void MDrv_EMAC_DumpMem(phys_addr_t addr, u32 len)
{
    u8 *ptr = (u8 *)addr;
    u32 i;

    printk("\n ===== Dump %lx =====\n", (long unsigned int)ptr);
    for (i=0; i<len; i++)
    {
        if ((u32)i%0x10UL ==0)
            printk("%lx: ", (long unsigned int)ptr);
        if (*ptr < 0x10UL)
            printk("0%x ", *ptr);
        else
            printk("%x ", *ptr);
        if ((u32)i%0x10UL == 0x0fUL)
            printk("\n");
	ptr++;
    }
    printk("\n");
}

//Background send
static int MDev_EMAC_BGsend(struct net_device* dev, phys_addr_t addr, int len )
{
	dma_addr_t skb_addr;
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);

    if (NETDEV_TX_OK != MDev_EMAC_CheckTSR())
        return NETDEV_TX_BUSY;

    skb_addr = get_tx_addr();
    memcpy((void*)skb_addr,(void *)addr, len);

    LocPtr->stats.tx_bytes += len;

#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
    if((unsigned int)skb_addr < 0xC0000000UL)
    {
        Chip_Flush_Memory_Range((unsigned int)skb_addr&0x0FFFFFFFUL, len);
    }
    else
    {
        Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
    }
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
    #else
	    #ERROR
    #endif
#endif

    //Set address of the data in the Transmit Address register //
    MHal_EMAC_Write_TAR(skb_addr - RAM_VA_PA_OFFSET - MIU0_BUS_BASE);

    // Set length of the packet in the Transmit Control register //
    MHal_EMAC_Write_TCR(len);

    return NETDEV_TX_OK;
}

static void MDev_EMAC_Send_PausePkt(struct net_device* dev)
{
    u32 val = MHal_EMAC_Read_CTL() & 0x000001FFUL;

    //Disable Rx
    MHal_EMAC_Write_CTL((val & ~EMAC_RE));
    memcpy(&pause_pkt[6], dev->dev_addr, 6);
    MDev_EMAC_BGsend(dev, (phys_addr_t)pause_pkt, sizeof(pause_pkt));
    //Enable Rx
    MHal_EMAC_Write_CTL((MHal_EMAC_Read_CTL() | EMAC_RE));
}

//-------------------------------------------------------------------------------------------------
//Patch for losing small-size packet when running SMARTBIT
//-------------------------------------------------------------------------------------------------
#ifdef CONFIG_MP_ETHERNET_MSTAR_ICMP_ENHANCE
static void MDev_EMAC_Period_Retry(struct sk_buff *skb, struct net_device* dev)
{
    u32 xval;
    u32 uRegVal;

    xval = MHal_EMAC_ReadReg32(REG_ETH_CFG);

    if((skb->len <= PACKET_THRESHOLD) && !(xval & EMAC_SPD) && !(xval & EMAC_FD))
    {
        txcount++;
    }
    else
    {
        txcount = 0;
    }

    if(txcount > TXCOUNT_THRESHOLD)
    {
        uRegVal  = MHal_EMAC_Read_CFG();
        uRegVal  |= 0x00001000UL;
        MHal_EMAC_Write_CFG(uRegVal);
    }
    else
    {
        uRegVal = MHal_EMAC_Read_CFG();
        uRegVal &= ~(0x00001000UL);
        MHal_EMAC_Write_CFG(uRegVal);
    }
}
#endif

int MDev_EMAC_enqueue(struct skb_node **queue_head, struct skb_node **queue_tail, struct sk_buff *skb)
{
    struct skb_node *pskb_node = (struct skb_node *)kmalloc(sizeof(struct skb_node), GFP_ATOMIC);

    if(!pskb_node)
    {
        EMAC_DBG("*pskb_node kmalloc fail\n");
        BUG_ON(1);
        return 1;
    }

    /*Reset the content of pskb_node*/
    memset(pskb_node, 0x0, sizeof(struct skb_node));

    pskb_node->skb = skb;
    pskb_node->next = *queue_head;
    pskb_node->prev = NULL;

    if(!(*queue_tail))
    {
        (*queue_tail) = pskb_node;
    }

    (*queue_tail)->queuesize = (*queue_tail)->queuesize + 1;

    if(!(*queue_head))
    {
        (*queue_head) = pskb_node;
    }
    else
    {
        (*queue_head)->prev = pskb_node;
    }

    (*queue_head) = pskb_node;

    return 0;
}

struct sk_buff* MDev_EMAC_dequeue(struct skb_node **queue_head, struct skb_node **queue_tail)
{
    struct sk_buff *skb;

    if(!(*queue_tail))
    {
        return NULL;
    }
    else
    {
        skb = (*queue_tail)->skb;

        /* If the size of queue is equal to 1*/
        if(*queue_head == *queue_tail)
        {
            kfree(*queue_tail);
            *queue_tail = NULL;
            *queue_head = NULL;
        }
        else
        {
            (*queue_tail)->prev->queuesize = (*queue_tail)->queuesize - 1;
            *queue_tail = (*queue_tail)->prev;
            kfree((*queue_tail)->next);
            (*queue_tail)->next = NULL;
        }
    }

    return skb;
}

void MDev_EMAC_bottom_tx_task(struct work_struct *work)
{
    struct sk_buff *skb;
    unsigned long flags;
    dma_addr_t skb_addr;
    struct EMAC_private *LocPtr = container_of(work, struct EMAC_private, tx_task);
    struct net_device *dev = LocPtr->dev;


    spin_lock_irqsave(LocPtr->lock, flags);

    while(NETDEV_TX_OK == MDev_EMAC_CheckTSR())
    {
        skb = MDev_EMAC_dequeue(&(LocPtr->tx_queue_head), &(LocPtr->tx_queue_tail));

        if(skb == NULL)
        {
            break;
        }
        else
        {

            skb_addr = get_tx_addr();
            memcpy((void*)skb_addr, skb->data, skb->len);

        #ifdef CONFIG_MP_ETHERNET_MSTAR_ICMP_ENHANCE
            MDev_EMAC_Period_Retry(skb, dev);
        #endif
            LocPtr->stats.tx_bytes += skb->len;

        #ifdef CHIP_FLUSH_READ
            #if defined(CONFIG_MIPS)
            if((unsigned int)skb_addr < 0xC0000000UL)
            {
                Chip_Flush_Memory_Range((unsigned int)skb_addr&0x0FFFFFFFUL, skb->len);
            }
            else
            {
                Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
            }
            #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
                Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
            #else
        	    //#ERROR
            #endif
        #endif

            //MDrv_EMAC_DumpMem(tx_fifo->skb_physaddr,skb->len);

            //Set address of the data in the Transmit Address register //
            MHal_EMAC_Write_TAR(skb_addr - RAM_VA_PA_OFFSET - MIU0_BUS_BASE);

            // Set length of the packet in the Transmit Control register //
            MHal_EMAC_Write_TCR(skb->len);

            dev->trans_start = jiffies;
            dev_kfree_skb_irq(skb);
        }
    }
    spin_unlock_irqrestore(LocPtr->lock, flags);
}

#ifdef TX_SOFTWARE_QUEUE
//-------------------------------------------------------------------------------------------------
//Transmit packet.
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_tx (struct sk_buff *skb, struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    unsigned long flags;
    int _ret;

    spin_lock_irqsave(LocPtr->lock, flags);

    /*Add skb to tx software queue*/
    _ret = MDev_EMAC_enqueue(&(LocPtr->tx_queue_head), &(LocPtr->tx_queue_tail), skb);

    /*Triger tx_task*/
    schedule_work(&LocPtr->tx_task);

    spin_unlock_irqrestore(LocPtr->lock, flags);

    if(_ret == 1)
    {
        return NETDEV_TX_BUSY;
    }
    else
    {
        return NETDEV_TX_OK;
    }
}

#else

//-------------------------------------------------------------------------------------------------
//Transmit packet.
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_tx (struct sk_buff *skb, struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    unsigned long flags;
    dma_addr_t skb_addr;

    spin_lock_irqsave(LocPtr->lock, flags);

#ifdef CONFIG_MSTAR_EEE
    MHal_EMAC_Disable_EEE_TX();
#endif

    if (skb->len > EMAC_MTU)
    {
        EMAC_DBG("Wrong Tx len:%u\n", skb->len);
        spin_unlock_irqrestore(LocPtr->lock, flags);
        return NETDEV_TX_BUSY;
    }

    if (NETDEV_TX_OK != MDev_EMAC_CheckTSR())
    {
	    spin_unlock_irqrestore(LocPtr->lock, flags);
	    return NETDEV_TX_BUSY; //check
    }

#ifndef TX_SKB_PTR
    #ifndef TX_QUEUE_4
        skb_addr = get_tx_addr();
        memcpy((void*)skb_addr, skb->data, skb->len);
    #else
        //tx_fifo->skb_physaddr = dma_map_single(NULL, skb->data, skb->len,DMA_TO_DEVICE);
        skb_addr = get_tx_addr();
        memcpy((void*)skb_addr, skb->data, skb->len);
    #endif
#else
    LocPtr->txpkt = dma_map_single(NULL, skb->data, skb->len,DMA_TO_DEVICE);
#endif

#ifdef CONFIG_MP_ETHERNET_MSTAR_ICMP_ENHANCE
    MDev_EMAC_Period_Retry(skb, dev);
#endif

    if (!skb_addr)
    {
        dev_err(NULL,
                "dma map 2 failed (%p, %i). Dropping packet\n",
                skb->data, skb->len);
		spin_unlock_irqrestore(LocPtr->lock, flags);
        return -ENOMEM;
    }

    // Store packet information (to free when Tx completed) //
    //LocPtr->skb = skb;
    //LocPtr->skb_length =(int) skb->len;
    LocPtr->stats.tx_bytes += skb->len;

#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
    if((unsigned int)skb_addr < 0xC0000000UL)
    {
        Chip_Flush_Memory_Range((unsigned int)skb_addr&0x0FFFFFFFUL, skb->len);
    }
    else
    {
        Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
    }
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Flush_Memory_Range(0, 0xFFFFFFFFUL);
    #else
	    #ERROR
    #endif
#endif
    //MDrv_EMAC_DumpMem(tx_fifo->skb_physaddr,skb->len);
    //Set address of the data in the Transmit Address register //
#if defined TX_QUEUE_4
    //MHal_EMAC_Write_TAR(tx_fifo->skb_physaddr - MIU0_BUS_BASE);
    MHal_EMAC_Write_TAR(skb_addr - RAM_VA_PA_OFFSET - MIU0_BUS_BASE);
#else
    MHal_EMAC_Write_TAR(skb_addr - RAM_VA_PA_OFFSET - MIU0_BUS_BASE);
#endif

    // Set length of the packet in the Transmit Control register //
    MHal_EMAC_Write_TCR(skb->len);

    //netif_stop_queue (dev);
    dev->trans_start = jiffies;
    dev_kfree_skb_irq(skb);
    spin_unlock_irqrestore(LocPtr->lock, flags);
    return NETDEV_TX_OK;
}
#endif

#ifdef RX_SOFTWARE_DESCRIPTOR
//-------------------------------------------------------------------------------------------------
// Extract received frame from buffer descriptors and sent to upper layers.
// (Called from interrupt context)
// (Enable RX software discriptor)
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_rx (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    struct recv_desc_bufs *dlist;
    unsigned char *p_recv;
    u32 pktlen;
    u32 retval=0;
#ifdef RX_SOFTWARE_DESCRIPTOR
    u32 uRegVal=0;
    u32 RBQP_offset;
#else
    //u32 wrap_bit;
    struct sk_buff *skb;
#endif

#ifndef INT_JULIAN_D
    u32 uRegVal=0;
    int count = 0;
#endif

    dlist = LocPtr->dlist ;
    // If any Ownership bit is 1, frame received.
    //while ( (dlist->descriptors[LocPtr->rxBuffIndex].addr )& EMAC_DESC_DONE)
    do
    {
#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
        Chip_Read_Memory_Range((unsigned int)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) & 0x0FFFFFFF, sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Inv_Cache_Range_VA_PA((unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)),(unsigned long)__pa((&(dlist->descriptors[LocPtr->rxBuffIndex].addr))), sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
	#else
        #ERROR
    #endif
#endif
        if(!((dlist->descriptors[LocPtr->rxBuffIndex].addr) & EMAC_DESC_DONE))
        {
            break;
        }

        p_recv = (char *) ((dlist->descriptors[LocPtr->rxBuffIndex].addr) & ~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
        pktlen = dlist->descriptors[LocPtr->rxBuffIndex].size & 0x7ffUL;    /* Length of frame including FCS */

    #if RX_THROUGHPUT_TEST
    	receive_bytes += pktlen;
    #endif
#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
        if((unsigned int)p_recv < 0xC0000000UL)
        {
        	Chip_Read_Memory_Range((unsigned int)(p_recv) & 0x0FFFFFFFUL, pktlen);
        }
        else
        {
       		Chip_Read_Memory_Range(0, 0xFFFFFFFFUL);
        }
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Inv_Cache_Range((unsigned int)(p_recv), pktlen);
    #else
	#ERROR
    #endif
#endif
        // the frame is not splitted in two parts //
        if(rx_skb[LocPtr->rxBuffIndex] == rx_skb_dummy)
        {
	        rx_skb[LocPtr->rxBuffIndex] = alloc_skb(SOFTWARE_DESC_LEN, GFP_ATOMIC);
            if(NULL == rx_skb[LocPtr->rxBuffIndex])
		    {
                rx_skb[LocPtr->rxBuffIndex] = rx_skb_dummy;
                LocPtr->stats.rx_dropped += 1;
                EMAC_DBG("Dummy, skb no enough memory!\n");
                goto NOBUF;
            }

            rx_abso_addr[LocPtr->rxBuffIndex] = (u32)rx_skb[LocPtr->rxBuffIndex]->data;
            //copy content of dummy to new skb
            *rx_skb[LocPtr->rxBuffIndex] = *rx_skb_dummy;
            rx_skb[LocPtr->rxBuffIndex]->data = (unsigned char *)rx_abso_addr[LocPtr->rxBuffIndex];
            memcpy(rx_skb[LocPtr->rxBuffIndex]->data, (void *)rx_abso_addr_dummy, pktlen);
	    }
    #ifdef RX_BYTE_ALIGN_OFFSET
        else
        {
            skb_reserve(rx_skb[LocPtr->rxBuffIndex], 2);
        }
    #endif
        skb_put(rx_skb[LocPtr->rxBuffIndex], pktlen);
        // update consumer pointer//
        rx_skb[LocPtr->rxBuffIndex]->dev = dev;
        rx_skb[LocPtr->rxBuffIndex]->protocol = eth_type_trans (rx_skb[LocPtr->rxBuffIndex], dev);
        rx_skb[LocPtr->rxBuffIndex]->len = pktlen;
        dev->last_rx = jiffies;
        LocPtr->stats.rx_bytes += pktlen;

    #ifdef RX_CHECKSUM
		if(((dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_TCP ) || (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_UDP )) \
			&& (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_IP_CSUM) \
			&& (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_TCP_UDP_CSUM) )
        {
            rx_skb[LocPtr->rxBuffIndex]->ip_summed = CHECKSUM_UNNECESSARY;
        }
        else
        {
            rx_skb[LocPtr->rxBuffIndex]->ip_summed = CHECKSUM_NONE;
        }
    #endif


    #if RX_THROUGHPUT_TEST
        kfree_skb(rx_skb[LocPtr->rxBuffIndex]);
    #else
        retval = netif_rx (rx_skb[LocPtr->rxBuffIndex]);
    #endif
        rx_skb[LocPtr->rxBuffIndex] = alloc_skb(SOFTWARE_DESC_LEN, GFP_ATOMIC);
        if (NULL == rx_skb[LocPtr->rxBuffIndex])
        {
            rx_skb[LocPtr->rxBuffIndex] = rx_skb_dummy;
            EMAC_DBG("Skb no enough memory!\n");
        }

NOBUF:
        rx_abso_addr[LocPtr->rxBuffIndex] = (u32)rx_skb[LocPtr->rxBuffIndex]->data;

        RBQP_offset = LocPtr->rxBuffIndex * 8;
        if(LocPtr->rxBuffIndex<(MAX_RX_DESCR-1))
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET,RBQP_BASE + RBQP_offset, rx_abso_addr[LocPtr->rxBuffIndex]);
        }
        else
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET,RBQP_BASE + RBQP_offset, (rx_abso_addr[LocPtr->rxBuffIndex]+EMAC_DESC_WRAP));
        }

        if (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_MULTICAST)
        {
            LocPtr->stats.multicast++;
        }
        dlist->descriptors[LocPtr->rxBuffIndex].addr  &= ~EMAC_DESC_DONE;  /* reset ownership bit */

#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
        Chip_Flush_Memory_Range((unsigned int)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) & 0x0FFFFFFFUL, sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Inv_Cache_Range_VA_PA((unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)),(unsigned long)__pa((&(dlist->descriptors[LocPtr->rxBuffIndex].addr))), sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
#else
        #ERROR
    #endif
#endif

        //wrap after last buffer //
        LocPtr->rxBuffIndex++;
        if (LocPtr->rxBuffIndex == MAX_RX_DESCR)
        {
            LocPtr->rxBuffIndex = 0;
        }

        uRegVal = (u32)rx_skb[LocPtr->rxBuffIndex];
        MHal_EMAC_Write_RDPTR(uRegVal);
    #ifdef CONFIG_EMAC_SUPPLY_RNG
        {
            static unsigned long u32LastInputRNGJiff=0;
            unsigned long u32Jiff=jiffies;

            if ( time_after(u32Jiff, u32LastInputRNGJiff+InputRNGJiffThreshold) )
            {
                unsigned int u32Temp;
                unsigned short u16Temp;

                u32LastInputRNGJiff = u32Jiff;
                u16Temp = MIPS_REG(REG_RNG_OUT);
                memcpy((unsigned char *)&u32Temp+0, &u16Temp, 2);
                u16Temp = MIPS_REG(REG_RNG_OUT);
                memcpy((unsigned char *)&u32Temp+2, &u16Temp, 2);
                add_input_randomness(EV_MSC, MSC_SCAN, u32Temp);
            }
        }
    #endif
    #ifdef INT_JULIAN_D
        if(ThisUVE.flagRBNA == 0)
        {
            xReceiveNum--;
            if(xReceiveNum==0)
                  return 0;
        }
    #else
        if( retval != 0)
        {
            uRegVal  = MHal_EMAC_Read_IDR();
            uRegVal |= (EMAC_INT_RCOM |EMAC_INT_RBNA);
            MHal_EMAC_Write_IDR(uRegVal);
            EMAC_timer.expires = jiffies+10;
            add_timer(&EMAC_timer);
            return 1;
        }

        if( ++count > 5 ) return 0;
    #endif//#ifdef INT_JULIAN_D
    }while(1);
#ifdef INT_JULIAN_D
    xReceiveNum=0;
    ThisUVE.flagRBNA=0;
#endif

    return 0;
}

#else //#ifdef RX_SOFTWARE_DESCRIPTOR

//-------------------------------------------------------------------------------------------------
// Extract received frame from buffer descriptors and sent to upper layers.
// (Called from interrupt context)
// (Disable RX software discriptor)
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_rx (struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    struct recv_desc_bufs *dlist;
    unsigned char *p_recv;
    u32 pktlen;
    u32 retval=0;
    struct sk_buff *skb;
#ifdef CONFIG_K3_RX_SWPATCH
    u32 i;
    u32 ipchecksum;
#endif

#ifndef INT_JULIAN_D
    u32 uRegVal=0;
    int count = 0;
#endif

    dlist = LocPtr->dlist ;
    // If any Ownership bit is 1, frame received.
    //while ( (dlist->descriptors[LocPtr->rxBuffIndex].addr )& EMAC_DESC_DONE)
    do
    {
#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
        Chip_Read_Memory_Range((unsigned int)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) & 0x0FFFFFFFUL, sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Inv_Cache_Range_VA_PA((unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)),(unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) - RAM_VA_PA_OFFSET ,sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
	#else
        #ERROR
    #endif
#endif
        if(!((dlist->descriptors[LocPtr->rxBuffIndex].addr) & EMAC_DESC_DONE))
        {
            break;
        }

        p_recv = (char *) ((((dlist->descriptors[LocPtr->rxBuffIndex].addr) & 0xFFFFFFFFUL) + RAM_VA_PA_OFFSET + MIU0_BUS_BASE) &~(EMAC_DESC_DONE | EMAC_DESC_WRAP));
        pktlen = dlist->descriptors[LocPtr->rxBuffIndex].size & 0x7ffUL;    /* Length of frame including FCS */

    #if RX_THROUGHPUT_TEST
    	receive_bytes += pktlen;
    #endif

        skb = alloc_skb (pktlen + 6, GFP_ATOMIC);

        if (skb != NULL)
        {
            skb_reserve (skb, 2);
    #ifdef CHIP_FLUSH_READ
        #if defined(CONFIG_MIPS)
            if((unsigned int)p_recv < 0xC0000000UL)
            {
                Chip_Read_Memory_Range((unsigned int)(p_recv) & 0x0FFFFFFFUL, pktlen);
            }
            else
            {
                Chip_Read_Memory_Range(0, 0xFFFFFFFFUL);
            }
        #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
            Chip_Inv_Cache_Range_VA_PA((unsigned long)p_recv,(unsigned long)(p_recv - RAM_VA_PA_OFFSET) ,pktlen);
        #else
            #ERROR
        #endif
    #endif

#ifdef CONFIG_K3_RX_SWPATCH
            // compute the checksum of IP
            ipchecksum=0;
            for (i=14;i<34;i+=2)
            {
                if (i==24)
                    continue;
                ipchecksum += ((p_recv[i]<<8) | (p_recv[i+1]));
                if (ipchecksum>=0x10000UL)
                    ipchecksum-=0xFFFFUL;
            }
            if ((((p_recv[24]<<8) | (p_recv[25])) + ipchecksum)==0xFFFFUL)
                memcpy(skb_put(skb, pktlen), p_recv, pktlen);
            else
            {
                //printk("ng\n");
                ipchecksum=0;
                for (i=18;i<38;i+=2)
                {
                    if (i==28)
                        continue;
                    ipchecksum += ((p_recv[i]<<8) | (p_recv[i+1]));
                    if (ipchecksum>=0x10000UL)
                        ipchecksum-=0xFFFFUL;
                }
                if ((((p_recv[28]<<8) | (p_recv[29])) | ipchecksum) == 0xFFFFUL)
                    memcpy(skb_put(skb, pktlen), p_recv+4, pktlen);
                else
                memcpy(skb_put(skb, pktlen), p_recv, pktlen);
            }
#else
            memcpy(skb_put(skb, pktlen), p_recv, pktlen);
#endif
            skb->dev = dev;
            skb->protocol = eth_type_trans (skb, dev);
            skb->len = pktlen;
            dev->last_rx = jiffies;
            LocPtr->stats.rx_bytes += pktlen;
        #if RX_THROUGHPUT_TEST
            kfree_skb(skb);
        #else

        #ifdef RX_CHECKSUM
            if(((dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_TCP ) || (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_UDP )) && \
               (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_IP_CSUM) && \
               (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_DESC_TCP_UDP_CSUM) )
            {
                skb->ip_summed = CHECKSUM_UNNECESSARY;
            }
            else
            {
                skb->ip_summed = CHECKSUM_NONE;
            }
        #endif
            retval = netif_rx (skb);
        #endif
        }
        else
        {
            LocPtr->stats.rx_dropped += 1;
        }

        if (dlist->descriptors[LocPtr->rxBuffIndex].size & EMAC_MULTICAST)
        {
            LocPtr->stats.multicast++;
        }
        dlist->descriptors[LocPtr->rxBuffIndex].addr  &= ~EMAC_DESC_DONE;  /* reset ownership bit */
#ifdef CHIP_FLUSH_READ
    #if defined(CONFIG_MIPS)
        Chip_Flush_Memory_Range((unsigned int)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) & 0x0FFFFFFF, sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
    #elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
        Chip_Inv_Cache_Range_VA_PA((unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)),(unsigned long)(&(dlist->descriptors[LocPtr->rxBuffIndex].addr)) - RAM_VA_PA_OFFSET ,sizeof(dlist->descriptors[LocPtr->rxBuffIndex].addr));
    #else
        #ERROR
    #endif
#endif
        //wrap after last buffer //
        LocPtr->rxBuffIndex++;
        if (LocPtr->rxBuffIndex == MAX_RX_DESCR)
        {
            LocPtr->rxBuffIndex = 0;
        }

    #ifdef CONFIG_EMAC_SUPPLY_RNG
        {
            static unsigned long u32LastInputRNGJiff=0;
            unsigned long u32Jiff=jiffies;

            if ( time_after(u32Jiff, u32LastInputRNGJiff+InputRNGJiffThreshold) )
            {
                unsigned int u32Temp;
                unsigned short u16Temp;

                u32LastInputRNGJiff = u32Jiff;
                u16Temp = MIPS_REG(REG_RNG_OUT);
                memcpy((unsigned char *)&u32Temp+0, &u16Temp, 2);
                u16Temp = MIPS_REG(REG_RNG_OUT);
                memcpy((unsigned char *)&u32Temp+2, &u16Temp, 2);
                add_input_randomness(EV_MSC, MSC_SCAN, u32Temp);
            }
        }
    #endif
    }while(1);
#ifdef INT_JULIAN_D
    xReceiveNum=0;
    ThisUVE.flagRBNA=0;
#endif

    return 0;
}

#endif //#ifdef RX_SOFTWARE_DESCRIPTOR

#ifdef INT_JULIAN_D
//-------------------------------------------------------------------------------------------------
//MAC interrupt handler
//(Interrupt delay enable)
//-------------------------------------------------------------------------------------------------

void MDev_EMAC_bottom_rx_task(struct work_struct *work)
{
    struct EMAC_private *LocPtr = container_of(work, struct EMAC_private, rx_task);
    struct net_device *dev = LocPtr->dev;

    mutex_lock(&(LocPtr->mutex));
    MDev_EMAC_rx(dev);
    mutex_unlock(&(LocPtr->mutex));
}

irqreturn_t MDev_EMAC_interrupt(int irq,void *dev_id)
{
    struct net_device *dev = (struct net_device *) dev_id;
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    u32 intstatus=0;
    unsigned long flags;
#ifndef RX_SOFTWARE_DESCRIPTOR
    u32 wp = 0;
#endif

    spin_lock_irqsave(LocPtr->lock, flags);

    //MAC Interrupt Status register indicates what interrupts are pending.
    //It is automatically cleared once read.
    xoffsetValue = MHal_EMAC_Read_JULIAN_0108() & 0x0000FFFFUL;
    xReceiveNum += xoffsetValue&0xFFUL;

#ifndef RX_SOFTWARE_DESCRIPTOR
    wp = MHal_EMAC_Read_JULIAN_0100() & 0x00100000UL;
    if(wp)
    {
        EMAC_DBG("EMAC HW write invalid address");
    }
#endif

    if(xoffsetValue&0x8000UL)
    {
        LocPtr->xReceiveFlag = 1;
    }
    ThisUVE.flagRBNA = 0;

    oldTime = getCurMs();
    while((LocPtr->xReceiveFlag == 1) || (intstatus = (MHal_EMAC_Read_ISR() & ~MHal_EMAC_Read_IMR() & EMAC_INT_MASK )) )
    {
        if (intstatus & EMAC_INT_RBNA)
        {
            LocPtr->stats.rx_dropped ++;
            ThisUVE.flagRBNA = 1;
            LocPtr->xReceiveFlag = 1;
            //write 1 clear
            MHal_EMAC_Write_RSR(EMAC_BNA);
        }

        // Transmit complete //
        if (intstatus & EMAC_INT_TCOM)
        {

#ifdef TX_SOFTWARE_QUEUE
            /*Triger tx_task*/
            schedule_work(&LocPtr->tx_task);
#endif

            // The TCOM bit is set even if the transmission failed. //
            if (intstatus & (EMAC_INT_TUND | EMAC_INT_RTRY))
            {
                LocPtr->stats.tx_errors += 1;
                if(intstatus & EMAC_INT_TUND)
                {
                    //write 1 clear
                    MHal_EMAC_Write_TSR(EMAC_UND);

                    //Reset TX engine
                    MDev_EMAC_TxReset();
                    EMAC_DBG ("Transmit TUND error, TX reset\n");
                }
            }
            else
            {
                LocPtr->retx_count = 0;
            }

        #if TX_THROUGHPUT_TEST
            MDev_EMAC_tx(pseudo_packet, emac_dev);
        #endif
            if (((LocPtr->ep_flag&EP_FLAG_SUSPENDING)==0) && netif_queue_stopped (dev))
                netif_wake_queue(dev);
        }

        if(intstatus&EMAC_INT_DONE)
        {
            ThisUVE.flagISR_INT_DONE = 0x01UL;
        }

        //Overrun //
        if(intstatus & EMAC_INT_ROVR)
        {
            LocPtr->stats.rx_dropped++;
            contiROVR++;
            //write 1 clear
            MHal_EMAC_Write_RSR(EMAC_RSROVR);
            //EMAC_DBG ("==> %s: ROVR error %u times!\n", dev->name, contiROVR);
            if (contiROVR < 3)
            {
                MDev_EMAC_Send_PausePkt(dev);
            }
            else
            {
                MDev_EMAC_SwReset(dev);
                xReceiveNum = 0;
            }
        }
        else
        {
            contiROVR = 0;
        }

        if(xReceiveNum != 0)
        {
            LocPtr->xReceiveFlag = 1;
        }

        // Receive complete //
        if(LocPtr->xReceiveFlag == 1)
        {
            LocPtr->xReceiveFlag = 0;
        #ifdef ISR_BOTTOM_HALF
            /*Triger rx_task*/
            schedule_work(&LocPtr->rx_task);
        #else
            MDev_EMAC_rx(dev);
        #endif
        }
    }
    spin_unlock_irqrestore(LocPtr->lock, flags);
    return IRQ_HANDLED;
}

#else //#ifdef INT_JULIAN_D

//-------------------------------------------------------------------------------------------------
//MAC interrupt handler with interrupt delay disable
//(Interrupt delay Disable)
//-------------------------------------------------------------------------------------------------
irqreturn_t MDev_EMAC_interrupt(int irq,void *dev_id)
{
    struct net_device *dev = (struct net_device *) dev_id;
    struct EMAC_private *LocPtr = (struct EMAC_private*) netdev_priv(dev);
    unsigned long flags;
    unsigned long intstatus;

    spin_lock_irqsave(LocPtr->lock, flags);

    while(intstatus = (MHal_EMAC_Read_ISR() & ~MHal_EMAC_Read_IMR() & EMAC_INT_MASK))
    {
        //RX buffer not available//
        if (intstatus & EMAC_INT_RBNA)
        {
            LocPtr->stats.rx_dropped ++;
        }
        // Receive complete //
        if (intstatus & EMAC_INT_RCOM)
        {
            if(MDev_EMAC_rx (dev)) goto quit_int;
        }
        // Transmit complete //
        if (intstatus & EMAC_INT_TCOM)
        {
            struct tx_ring* tx_fifo_data = NULL;
            u32 remove_cnt = 1;
            // The TCOM bit is set even if the transmission failed. //
            if (intstatus & (EMAC_INT_TUND | EMAC_INT_RTRY))
            {
                LocPtr->stats.tx_errors += 1;
                if(intstatus &EMAC_INT_TUND)
                {
                    EMAC_DBG ("%s: Transmit TUND error\n", dev->name);
                }
                if(intstatus &EMAC_INT_RTRY)
                {
                    EMAC_DBG ("%s: Transmit RTRY error\n", dev->name);
                }
            }

            if (intstatus & EMAC_INT_TBRE)
                 remove_cnt = 2;
        #if TX_THROUGHPUT_TEST
            MDev_EMAC_tx(pseudo_packet, emac_dev);
        #else
        #ifdef TX_QUEUE_4
           remove_cnt = (intstatus >> EMAC_TX_COUNT);
        #endif
           while (remove_cnt > 0)
           {
                tx_fifo_data = tx_ring_get(LocPtr, 1);
	            if (tx_fifo_data == NULL)
                {
                    break;
                }
                else
                {
                 #ifdef TX_QUEUE_4
	                 dma_unmap_single(NULL, tx_fifo_data->skb_physaddr, tx_fifo_data->skb->len, DMA_FROM_DEVICE);
                 #endif
		             if (tx_fifo_data->skb)
	                 {
                         dev_kfree_skb_irq(tx_fifo_data->skb);
      	                 tx_fifo_data->skb = NULL;
		             }
		             else
		             {
                         EMAC_DBG ("skb is null!\n");
		             }
	            }
	            remove_cnt--;
            }
        #endif
            if (netif_queue_stopped(dev))
                netif_wake_queue(dev);
        }

        if(intstatus&EMAC_INT_DONE)
        {
            ThisUVE.flagISR_INT_DONE = 0x01UL;
        }
        //Overrun //
        if(intstatus & EMAC_INT_ROVR)
        {
            MDev_EMAC_rx(dev);
            LocPtr->stats.rx_dropped++;
        }
    }
quit_int:
    spin_unlock_irqrestore(LocPtr->lock, flags);
    return IRQ_HANDLED;
}
#endif //#ifdef INT_JULIAN_D

//-------------------------------------------------------------------------------------------------
// EMAC Hardware register set
//-------------------------------------------------------------------------------------------------
void MDev_EMAC_HW_init(void)
{
    u32 word_ETH_CTL = 0x00000000UL;
    u32 word_ETH_CFG = 0x00000800UL;
    u32 uJulian104Value = 0;
    u32 uNegPhyVal = 0;
#ifdef SOFTWARE_DESCRIPTOR
    u32 idxRBQP = 0;
    u32 RBQP_offset = 0;
#endif

    // 20140811 cyli modify for emac miu high priority
    *((volatile u32 *) 0xfd202490) = *((volatile u32 *) 0xfd202490) & ~(0x1 << 0xd);

    // (20071026_CHARLES) Disable TX, RX and MDIO:   (If RX still enabled, the RX buffer will be overwrited)
    MHal_EMAC_Write_CTL(word_ETH_CTL);
    // Init RX --------------------------------------------------------------
    memset((u8*)RAM_VA_PA_OFFSET + RX_BUFFER_BASE, 0x00UL, RX_BUFFER_SIZE);

    MHal_EMAC_Write_BUFF((RX_BUFFER_BASE | RX_BUFFER_SEL) - MIU0_BUS_BASE);
    MHal_EMAC_Write_RDPTR(0x00000000UL);
    MHal_EMAC_Write_WRPTR(0x00000000UL);
    // Initialize "Receive Buffer Queue Pointer"
    MHal_EMAC_Write_RBQP(RBQP_BASE - MIU0_BUS_BASE);
	printk("RBQP_BASE = %x\n", RBQP_BASE);
    // Initialize Receive Buffer Descriptors
    memset((u8*)RAM_VA_PA_OFFSET + RBQP_BASE, 0x00UL, RBQP_SIZE);        // Clear for max(8*1024)bytes (max:1024 descriptors)
    MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET, (RBQP_BASE + RBQP_SIZE - 0x08UL), 0x00000002UL);             // (n-1) : Wrap = 1
#ifdef INT_JULIAN_D
    //Reg_rx_frame_cyc[15:8] -0xFF range 1~255
    //Reg_rx_frame_num[7:0]  -0x05 receive frames per INT.
    //0x80 Enable interrupt delay mode.
    //register 0x104 receive counter need to modify smaller for ping
    //Modify bigger(need small than 8) for throughput
    uJulian104Value = JULIAN_104_VAL;//0xFF050080;
    MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
#else
    // Enable Interrupts ----------------------------------------------------
    uJulian104Value = 0x00000000UL;
    MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
#endif
    // Set MAC address ------------------------------------------------------
    MHal_EMAC_Write_SA1_MAC_Address(ThisBCE.sa1[0], ThisBCE.sa1[1], ThisBCE.sa1[2], ThisBCE.sa1[3], ThisBCE.sa1[4], ThisBCE.sa1[5]);
    MHal_EMAC_Write_SA2_MAC_Address(ThisBCE.sa2[0], ThisBCE.sa2[1], ThisBCE.sa2[2], ThisBCE.sa2[3], ThisBCE.sa2[4], ThisBCE.sa2[5]);
    MHal_EMAC_Write_SA3_MAC_Address(ThisBCE.sa3[0], ThisBCE.sa3[1], ThisBCE.sa3[2], ThisBCE.sa3[3], ThisBCE.sa3[4], ThisBCE.sa3[5]);
    MHal_EMAC_Write_SA4_MAC_Address(ThisBCE.sa4[0], ThisBCE.sa4[1], ThisBCE.sa4[2], ThisBCE.sa4[3], ThisBCE.sa4[4], ThisBCE.sa4[5]);
    MHal_EMAC_Write_WOL_MAC_Address(ThisBCE.sa1[0], ThisBCE.sa1[1], ThisBCE.sa1[2], ThisBCE.sa1[3], ThisBCE.sa1[4], ThisBCE.sa1[5]);

#ifdef SOFTWARE_DESCRIPTOR
    #ifdef RX_CHECKSUM
    uJulian104Value=uJulian104Value | (RX_CHECKSUM_ENABLE | SOFTWARE_DESCRIPTOR_ENABLE);
    #else
    uJulian104Value=uJulian104Value | SOFTWARE_DESCRIPTOR_ENABLE;
    #endif

    MHal_EMAC_Write_JULIAN_0104(uJulian104Value);

    //Important : rcu will stall when miu response is faster than riu
    uJulian104Value=MHal_EMAC_Read_JULIAN_0104();
    for(idxRBQP = 0; idxRBQP < RBQP_LENG; idxRBQP++)
    {
    #ifdef RX_SOFTWARE_DESCRIPTOR
        rx_skb[idxRBQP] = alloc_skb(SOFTWARE_DESC_LEN, GFP_ATOMIC);

        rx_abso_addr[idxRBQP] = (u32)rx_skb[idxRBQP]->data;
        RBQP_offset = idxRBQP * 8;
        if(idxRBQP < (RBQP_LENG - 1))
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET, RBQP_BASE + RBQP_offset, rx_abso_addr[idxRBQP]);
        }
        else
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET, RBQP_BASE + RBQP_offset, (rx_abso_addr[idxRBQP] + EMAC_DESC_WRAP));
        }
    #else
            RBQP_offset = idxRBQP * 8;
        if(idxRBQP < (RBQP_LENG - 1))
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET, RBQP_BASE + RBQP_offset, (RX_BUFFER_BASE - MIU0_BUS_BASE + SOFTWARE_DESC_LEN * idxRBQP));
        }
        else
        {
            MHal_EMAC_WritRam32(RAM_VA_PA_OFFSET, RBQP_BASE + RBQP_offset, (RX_BUFFER_BASE - MIU0_BUS_BASE + SOFTWARE_DESC_LEN * idxRBQP + EMAC_DESC_WRAP));
        }
    #endif
    }

    //Important: RIU response can not be earlier than miu
    uJulian104Value=MHal_EMAC_ReadRam32(RAM_VA_PA_OFFSET,RBQP_BASE + RBQP_offset);
#ifdef RX_SOFTWARE_DESCRIPTOR
	rx_skb_dummy = 	alloc_skb(SOFTWARE_DESC_LEN, GFP_ATOMIC);
	if(rx_skb_dummy == NULL)
    {
        EMAC_DBG(KERN_INFO "allocate skb dummy failed\n");
    }
	else
    {
	    rx_abso_addr_dummy = (u32)(rx_skb_dummy->data);
    }
#endif

#endif //#ifdef SOFTWARE_DESCRIPTOR

    if (!ThisUVE.initedEMAC)
    {
        MHal_EMAC_write_phy(phyaddr, MII_BMCR, 0x9000UL);
        MHal_EMAC_write_phy(phyaddr, MII_BMCR, 0x1000UL);
        // IMPORTANT: Run NegotiationPHY() before writing REG_ETH_CFG.
        uNegPhyVal = MHal_EMAC_NegotiationPHY();
        if(uNegPhyVal == 0x01UL)
        {
            ThisUVE.flagMacTxPermit = 0x01UL;
            ThisBCE.duplex = 1;

        }
        else if(uNegPhyVal == 0x02UL)
        {
            ThisUVE.flagMacTxPermit = 0x01UL;
            ThisBCE.duplex = 2;
        }

        // ETH_CFG Register -----------------------------------------------------
        word_ETH_CFG = 0x00000800UL;        // Init: CLK = 0x2
        // (20070808) IMPORTANT: REG_ETH_CFG:bit1(FD), 1:TX will halt running RX frame, 0:TX will not halt running RX frame.
        // If always set FD=0, no CRC error will occur. But throughput maybe need re-evaluate.
        // IMPORTANT: (20070809) NO_MANUAL_SET_DUPLEX : The real duplex is returned by "negotiation"
        if(ThisBCE.speed     == EMAC_SPEED_100) word_ETH_CFG |= 0x00000001UL;
        if(ThisBCE.duplex    == 2)              word_ETH_CFG |= 0x00000002UL;
        if(ThisBCE.cam       == 1)              word_ETH_CFG |= 0x00000200UL;
        if(ThisBCE.rcv_bcast == 0)              word_ETH_CFG |= 0x00000020UL;
        if(ThisBCE.rlf       == 1)              word_ETH_CFG |= 0x00000100UL;

        MHal_EMAC_Write_CFG(word_ETH_CFG);
        // ETH_CTL Register -----------------------------------------------------
        word_ETH_CTL = 0x0000000CUL;                          // Enable transmit and receive : TE + RE = 0x0C (Disable MDIO)
        if(ThisBCE.wes == 1) word_ETH_CTL |= 0x00000080UL;
        MHal_EMAC_Write_CTL(word_ETH_CTL);
        MHal_EMAC_Write_JULIAN_0100(JULIAN_100_VAL);

        ThisUVE.flagPowerOn = 1;
        ThisUVE.initedEMAC  = 1;
    }

    MHal_EMAC_HW_init();

}


//-------------------------------------------------------------------------------------------------
// EMAC init Variable
//-------------------------------------------------------------------------------------------------
extern phys_addr_t memblock_start_of_DRAM(void);
extern phys_addr_t memblock_size_of_first_region(void);

static phys_addr_t MDev_EMAC_VarInit(void)
{
    phys_addr_t alloRAM_PA_BASE = 0;
    phys_addr_t alloRAM_SIZE = 0;

    char addr[6];
    u32 HiAddr, LoAddr;
    phys_addr_t *alloRAM_VA_BASE;

    get_boot_mem_info(EMAC_MEM, &alloRAM_PA_BASE, &alloRAM_SIZE);
#if defined (CONFIG_ARM64)
    alloRAM_PA_BASE = memblock_start_of_DRAM() + memblock_size_of_first_region();
#endif

    alloRAM_VA_BASE = (phys_addr_t *)ioremap(alloRAM_PA_BASE, alloRAM_SIZE); //map buncing buffer from PA to VA

    EMAC_DBG("alloRAM_VA_BASE = 0x%zx alloRAM_PA_BASE= 0x%zx  alloRAM_SIZE= 0x%zx\n", (size_t)alloRAM_VA_BASE, (size_t)alloRAM_PA_BASE, (size_t)alloRAM_SIZE);
    BUG_ON(!alloRAM_VA_BASE);

#ifndef RX_SOFTWARE_DESCRIPTOR
    //Add Write Protect
    //MHal_EMAC_Write_Protect(alloRAM_PA_BASE - MIU0_BUS_BASE, alloRAM_SIZE);
#endif
    memset((phys_addr_t *)alloRAM_VA_BASE,0x00UL,alloRAM_SIZE);

    RAM_VA_BASE       = ((phys_addr_t)alloRAM_VA_BASE + sizeof(struct EMAC_private) + 0x3FFFUL) & ~0x3FFFUL;   // IMPORTANT: Let lowest 14 bits as zero.
    RAM_PA_BASE       = ((phys_addr_t)alloRAM_PA_BASE + sizeof(struct EMAC_private) + 0x3FFFUL) & ~0x3FFFUL;   // IMPORTANT: Let lowest 14 bits as zero.
    RX_BUFFER_BASE    = RAM_PA_BASE + RBQP_SIZE;
    RBQP_BASE         = RAM_PA_BASE;
    TX_BUFFER_BASE    = RAM_PA_BASE + (RX_BUFFER_SIZE + RBQP_SIZE);
    RAM_VA_PA_OFFSET  = RAM_VA_BASE - RAM_PA_BASE;  // IMPORTANT_TRICK_20070512
    TX_SKB_BASE       = TX_BUFFER_BASE + MAX_RX_DESCR * sizeof(struct rbf_t);

    memset(&ThisBCE,0x00UL,sizeof(BasicConfigEMAC));
    memset(&ThisUVE,0x00UL,sizeof(UtilityVarsEMAC));

    ThisBCE.wes       	= 0;             		// 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    ThisBCE.duplex 		= 2;                    // 1:Half-duplex, 2:Full-duplex
    ThisBCE.cam			= 0;                 	// 0:No CAM, 1:Yes
    ThisBCE.rcv_bcast	= 0;                  	// 0:No, 1:Yes
    ThisBCE.rlf  		= 0;                 	// 0:No, 1:Yes receive long frame(1522)
    ThisBCE.rcv_bcast   = 1;
    ThisBCE.speed       = EMAC_SPEED_100;

    // Check if bootloader set address in Specific-Address 1 //
    HiAddr = MHal_EMAC_get_SA1H_addr();
    LoAddr = MHal_EMAC_get_SA1L_addr();

    addr[0] = (LoAddr & 0xffUL);
    addr[1] = (LoAddr & 0xff00UL) >> 8;
    addr[2] = (LoAddr & 0xff0000UL) >> 16;
    addr[3] = (LoAddr & 0xff000000UL) >> 24;
    addr[4] = (HiAddr & 0xffUL);
    addr[5] = (HiAddr & 0xff00UL) >> 8;

#ifdef CONFIG_LG_CHANGE
//20151130 cyli modify for LMTASKWBS-3766
//    printk("\n\ncylitest %s %d : %x:%x:%x:%x:%x:%x\n\n", __func__, __LINE__, addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
    memcpy (ThisBCE.sa1, &addr, 6);
#else
    if (is_valid_ether_addr (addr))
    {
        memcpy (ThisBCE.sa1, &addr, 6);
    }
    else
    {
        // Check if bootloader set address in Specific-Address 2 //
        HiAddr = MHal_EMAC_get_SA2H_addr();
        LoAddr = MHal_EMAC_get_SA2L_addr();
        addr[0] = (LoAddr & 0xffUL);
        addr[1] = (LoAddr & 0xff00UL) >> 8;
        addr[2] = (LoAddr & 0xff0000UL) >> 16;
        addr[3] = (LoAddr & 0xff000000UL) >> 24;
        addr[4] = (HiAddr & 0xffUL);
        addr[5] = (HiAddr & 0xff00UL) >> 8;

        if (is_valid_ether_addr (addr))
        {
            memcpy (ThisBCE.sa1, &addr, 6);
        }
        else
        {
            ThisBCE.sa1[0]      = MY_MAC[0];
            ThisBCE.sa1[1]      = MY_MAC[1];
            ThisBCE.sa1[2]      = MY_MAC[2];
            ThisBCE.sa1[3]      = MY_MAC[3];
            ThisBCE.sa1[4]      = MY_MAC[4];
            ThisBCE.sa1[5]      = MY_MAC[5];
        }
    }
#endif
    ThisBCE.connected = 0;

    return (phys_addr_t)alloRAM_VA_BASE;
}

//-------------------------------------------------------------------------------------------------
// Initialize the ethernet interface
// @return TRUE : Yes
// @return FALSE : FALSE
//-------------------------------------------------------------------------------------------------

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
static const struct net_device_ops mstar_lan_netdev_ops = {
        .ndo_open               = MDev_EMAC_open,
        .ndo_stop               = MDev_EMAC_close,
        .ndo_start_xmit         = MDev_EMAC_tx,
        .ndo_set_mac_address    = MDev_EMAC_set_mac_address,
        .ndo_set_rx_mode        = MDev_EMAC_set_rx_mode,
        .ndo_do_ioctl           = MDev_EMAC_ioctl,
        .ndo_get_stats          = MDev_EMAC_stats,
};

static int MDev_EMAC_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
    struct EMAC_private *LocPtr =(struct EMAC_private *) netdev_priv(dev);

    mii_ethtool_gset (&LocPtr->mii, cmd);

	if (LocPtr->phy_media == PORT_FIBRE)
	{
	    //override media type since mii.c doesn't know //
		cmd->supported = SUPPORTED_FIBRE;
		cmd->port = PORT_FIBRE;
	}

	return 0;
}

static int MDev_EMAC_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
    struct EMAC_private *LocPtr =(struct EMAC_private *) netdev_priv(dev);

    mii_ethtool_sset (&LocPtr->mii, cmd);

    return 0;
}

static int MDev_EMAC_nway_reset(struct net_device *dev)
{
    struct EMAC_private *LocPtr =(struct EMAC_private *) netdev_priv(dev);

    mii_nway_restart (&LocPtr->mii);

    return 0;
}

static u32 MDev_EMAC_get_link(struct net_device *dev)
{
    u32	u32data;
    struct EMAC_private *LocPtr =(struct EMAC_private *) netdev_priv(dev);

    u32data = mii_link_ok (&LocPtr->mii);

    return u32data;
}

static const struct ethtool_ops ethtool_ops = {
    .get_settings = MDev_EMAC_get_settings,
    .set_settings = MDev_EMAC_set_settings,
    .nway_reset   = MDev_EMAC_nway_reset,
    .get_link     = MDev_EMAC_get_link,
};

#endif

static int MDev_EMAC_setup (struct net_device *dev, unsigned long phy_type)
{
    struct EMAC_private *LocPtr;
    static int already_initialized = 0;
    dma_addr_t dmaaddr;
    u32 val;
    phys_addr_t RetAddr;
#ifdef CONFIG_MSTAR_HW_TX_CHECKSUM
    u32 retval;
#endif
    if (already_initialized)
        return FALSE;

    LocPtr = (struct EMAC_private *) netdev_priv(dev);

    /*Init tx and rx tasks*/
    INIT_WORK(&LocPtr->rx_task, MDev_EMAC_bottom_rx_task);
    INIT_WORK(&LocPtr->tx_task, MDev_EMAC_bottom_tx_task);

    LocPtr->dev = dev;
    RetAddr = MDev_EMAC_VarInit();
    if(!RetAddr)
    {
        EMAC_DBG("Var init fail!!\n");
        return FALSE;
    }

    if (LocPtr == NULL)
    {
        free_irq (dev->irq, dev);
        EMAC_DBG("LocPtr fail\n");
        return -ENOMEM;
    }

    dev->base_addr = (long) REG_ADDR_BASE;
    MDev_EMAC_HW_init();
    dev->irq = E_IRQ_EMAC;

    // Allocate memory for DMA Receive descriptors //
    LocPtr->dlist_phys = LocPtr->dlist = (struct recv_desc_bufs *) (RBQP_BASE + RAM_VA_PA_OFFSET);

    if (LocPtr->dlist == NULL)
    {
        dma_free_noncoherent((void *)LocPtr, EMAC_ABSO_MEM_SIZE,&dmaaddr,0);//kfree (dev->priv);
        free_irq (dev->irq, dev);
        return -ENOMEM;
    }

    LocPtr->lock = &emac_lock;
    spin_lock_init (LocPtr->lock);
    mutex_init(&(LocPtr->mutex));
    ether_setup (dev);
#if LINUX_VERSION_CODE == KERNEL_VERSION(2,6,28)
    dev->open = MDev_EMAC_open;
    dev->stop = MDev_EMAC_close;
    dev->hard_start_xmit = MDev_EMAC_tx;
    dev->get_stats = MDev_EMAC_stats;
    dev->set_multicast_list = MDev_EMAC_set_rx_mode;
    dev->do_ioctl = MDev_EMAC_ioctl;
    dev->set_mac_address = MDev_EMAC_set_mac_address;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
    dev->netdev_ops = &mstar_lan_netdev_ops;
#endif
    dev->tx_queue_len = EMAC_MAX_TX_QUEUE;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    /* The SET_ETHTOOL_OPS has been removed from upstream */
    dev->ethtool_ops = &ethtool_ops;
#else
    SET_ETHTOOL_OPS(dev, &ethtool_ops);
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
#endif

    MDev_EMAC_get_mac_address (dev);    // Get ethernet address and store it in dev->dev_addr //
    MDev_EMAC_update_mac_address (dev); // Program ethernet address into MAC //
    spin_lock_irq (LocPtr->lock);
    MHal_EMAC_enable_mdi ();
    MHal_EMAC_read_phy (phyaddr, MII_USCR_REG, &val);
    if ((val & (1 << 10)) == 0)   // DSCR bit 10 is 0 -- fiber mode //
        LocPtr->phy_media = PORT_FIBRE;

    spin_unlock_irq (LocPtr->lock);

    //Support for ethtool //
    LocPtr->mii.dev = dev;
    LocPtr->mii.mdio_read = MDev_EMAC_mdio_read;
    LocPtr->mii.mdio_write = MDev_EMAC_mdio_write;
    already_initialized = 1;
#ifdef CONFIG_MSTAR_HW_TX_CHECKSUM
    retval = MHal_EMAC_Read_JULIAN_0104() | TX_CHECKSUM_ENABLE;
    MHal_EMAC_Write_JULIAN_0104(retval);
    dev->features |= NETIF_F_IP_CSUM;
#endif

    //Install the interrupt handler //
    //Notes: Modify linux/kernel/irq/manage.c  /* interrupt.h */
    if (request_irq(dev->irq, MDev_EMAC_interrupt, SA_INTERRUPT, dev->name, dev))
        return -EBUSY;

    //Determine current link speed //
    spin_lock_irq (LocPtr->lock);
    (void) MDev_EMAC_update_linkspeed (dev);
    spin_unlock_irq (LocPtr->lock);

    return 0;
}

//-------------------------------------------------------------------------------------------------
// Restar the ethernet interface
// @return TRUE : Yes
// @return FALSE : FALSE
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_SwReset(struct net_device *dev)
{
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(dev);
    u32 oldCFG, oldCTL;
    u32 retval;

    MDev_EMAC_get_mac_address (dev);
    oldCFG = MHal_EMAC_Read_CFG();
    oldCTL = MHal_EMAC_Read_CTL() & ~(EMAC_TE | EMAC_RE);

    //free tx skb
    if (LocPtr->retx_count)
    {
        if (LocPtr->skb)
        {
            dev_kfree_skb_irq(LocPtr->skb );
            LocPtr->skb = NULL;
        }
        if (netif_queue_stopped (dev))
            netif_wake_queue (dev);
    }

    netif_stop_queue (dev);

    retval = MHal_EMAC_Read_JULIAN_0100();
    MHal_EMAC_Write_JULIAN_0100(retval & 0x00000FFFUL);
    MHal_EMAC_Write_JULIAN_0100(retval);

    MDev_EMAC_HW_init();
    MHal_EMAC_Write_CFG(oldCFG);
    MHal_EMAC_Write_CTL(oldCTL);
    MHal_EMAC_enable_mdi ();
    MDev_EMAC_update_mac_address (dev); // Program ethernet address into MAC //
    MDev_EMAC_update_linkspeed (dev);
    MHal_EMAC_Write_IER(IER_FOR_INT_JULIAN_D);
    MDev_EMAC_start (dev);
    MDev_EMAC_set_rx_mode(dev);
    netif_start_queue (dev);
    contiROVR = 0;
    LocPtr->retx_count = 0;
#ifdef CONFIG_MSTAR_HW_TX_CHECKSUM
    retval = MHal_EMAC_Read_JULIAN_0104() | TX_CHECKSUM_ENABLE;
    MHal_EMAC_Write_JULIAN_0104(retval);
#endif
    EMAC_DBG("=> Take %lu ms to reset EMAC!\n", (getCurMs() - oldTime));
    return 0;
}

//-------------------------------------------------------------------------------------------------
// Detect MAC and PHY and perform initialization
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_ARM64)
static struct of_device_id mstaremac_of_device_ids[] = {
         {.compatible = "mstar-emac"},
         {},
};
#endif

static int MDev_EMAC_probe (struct net_device *dev)
{
    int detected = -1;
    /* Read the PHY ID registers - try all addresses */
    detected = MDev_EMAC_setup(dev, MII_URANUS_ID);
    return detected;
}

//-------------------------------------------------------------------------------------------------
// EMAC Timer set for Receive function
//-------------------------------------------------------------------------------------------------
static void MDev_EMAC_timer_callback(unsigned long value)
{
    int ret = 0;
    struct EMAC_private *LocPtr = (struct EMAC_private *) netdev_priv(emac_dev);
    static u32 bmsr, time_count = 0;
#ifndef INT_JULIAN_D
    if (EMAC_RX_TMR == value)
    {
        MHal_EMAC_timer_callback(value);
        return;
    }
#endif

    spin_lock_irq (LocPtr->lock);
    ret = MDev_EMAC_update_linkspeed(emac_dev);
    spin_unlock_irq (LocPtr->lock);
    if (0 == ret)
    {
        if (!ThisBCE.connected)
        {
            ThisBCE.connected = 1;
            netif_carrier_on(emac_dev);
        }

        // Link status is latched, so read twice to get current value //
        MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
        MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
        time_count = 0;
        spin_lock_irq (LocPtr->lock);
        phy_status_register = bmsr;
        spin_unlock_irq (LocPtr->lock);
        // Normally, time out sets 1 Sec.
        Link_timer.expires = jiffies + EMAC_CHECK_LINK_TIME;
    }
    else    //no link
    {
        if(ThisBCE.connected) {
            ThisBCE.connected = 0;
        }
        // If disconnected is over 3 Sec, the real value of PHY's status register will report to application.
        if(time_count > CONFIG_DISCONNECT_DELAY_S*10) {
            // Link status is latched, so read twice to get current value //
            MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
            MHal_EMAC_read_phy (phyaddr, MII_BMSR, &bmsr);
            spin_lock_irq (LocPtr->lock);
            phy_status_register = bmsr;
            spin_unlock_irq (LocPtr->lock);
            // Report to kernel.
            netif_carrier_off(emac_dev);
            ThisBCE.connected = 0;
            // Normally, time out is set 1 Sec.
            Link_timer.expires = jiffies + EMAC_CHECK_LINK_TIME;
        }
        else if(time_count <= CONFIG_DISCONNECT_DELAY_S*10){
            time_count++;
            // Time out is set 100ms. Quickly checks next phy status.
            Link_timer.expires = jiffies + (EMAC_CHECK_LINK_TIME / 10);
        }
    }

    add_timer(&Link_timer);
}

//-------------------------------------------------------------------------------------------------
// EMAC MACADDR Setup
//-------------------------------------------------------------------------------------------------

#define MACADDR_FORMAT "XX:XX:XX:XX:XX:XX"

static int __init macaddr_auto_config_setup(char *addrs)
{
    if (strlen(addrs) == strlen(MACADDR_FORMAT)
        && ':' == addrs[2]
        && ':' == addrs[5]
        && ':' == addrs[8]
        && ':' == addrs[11]
        && ':' == addrs[14]
       )
    {
        addrs[2]  = '\0';
        addrs[5]  = '\0';
        addrs[8]  = '\0';
        addrs[11] = '\0';
        addrs[14] = '\0';

        MY_MAC[0] = (u8)simple_strtoul(&(addrs[0]),  NULL, 16);
        MY_MAC[1] = (u8)simple_strtoul(&(addrs[3]),  NULL, 16);
        MY_MAC[2] = (u8)simple_strtoul(&(addrs[6]),  NULL, 16);
        MY_MAC[3] = (u8)simple_strtoul(&(addrs[9]),  NULL, 16);
        MY_MAC[4] = (u8)simple_strtoul(&(addrs[12]), NULL, 16);
        MY_MAC[5] = (u8)simple_strtoul(&(addrs[15]), NULL, 16);

        /* set back to ':' or the environment variable would be destoried */ // REVIEW: this coding style is dangerous
        addrs[2]  = ':';
        addrs[5]  = ':';
        addrs[8]  = ':';
        addrs[11] = ':';
        addrs[14] = ':';
    }

    return 1;
}

__setup("macaddr=", macaddr_auto_config_setup);

//-------------------------------------------------------------------------------------------------
// EMAC init module
//-------------------------------------------------------------------------------------------------
static int MDev_EMAC_ScanPhyAddr(void)
{
    unsigned char addr = 0;
    u32 value = 0;

    MHal_EMAC_Write_JULIAN_0100(JULIAN_100_VAL);

    MHal_EMAC_enable_mdi();
    do
    {
        MHal_EMAC_read_phy(addr, MII_BMSR, &value);
        if (0 != value && 0x0000FFFFUL != value)
        {
            EMAC_DBG("[ PHY Addr ] ==> :%u\n", addr);
            break;
        }
    }while(++addr && addr < 32);
    MHal_EMAC_disable_mdi();
    phyaddr = addr;

	if (phyaddr >= 32)
	{
		EMAC_DBG("Wrong PHY Addr and reset to 0\n");
		phyaddr = 0;
		return -1;
	}
	return 0;
}

static void Rtl_Patch(void)
{
    u32 val = 0;

    MHal_EMAC_read_phy(phyaddr, 25, &val);
    MHal_EMAC_write_phy(phyaddr, 25, 0x400UL);
    MHal_EMAC_read_phy(phyaddr, 25, &val);
}

static void MDev_EMAC_Patch_PHY(void)
{
    u32 val = 0;

    MHal_EMAC_read_phy(phyaddr, 2, &val);
    if (RTL_8210 == val)
        Rtl_Patch();
}

static int MDev_EMAC_init(void)
{
    struct EMAC_private *LocPtr;
#if 0
    if(MDrv_SYS_GetPowerStates()!= SYS_POWER_Supply_Full)
    {
        EMAC_DBG("Power state(%d) does not support ethernet\n",MDrv_SYS_GetPowerStates());
        //return -1;
    }
#endif
    if(emac_dev)
        return -1;

    emac_dev = alloc_etherdev(sizeof(*LocPtr));
    LocPtr = netdev_priv(emac_dev);

    if (!emac_dev)
    {
        EMAC_DBG( KERN_ERR "No EMAC dev mem!\n" );
        return -ENOMEM;
    }

#if TX_THROUGHPUT_TEST
    printk("==========TX_THROUGHPUT_TEST===============");
    pseudo_packet = alloc_skb(SOFTWARE_DESC_LEN, GFP_ATOMIC);
    memcpy(pseudo_packet->data, (void *)packet_content, sizeof(packet_content));
    pseudo_packet->len = sizeof(packet_content);
#endif

#if RX_THROUGHPUT_TEST
    printk("==========RX_THROUGHPUT_TEST===============");
    init_timer(&RX_timer);

    RX_timer.data = EMAC_RX_TMR;
    RX_timer.function = RX_timer_callback;
    RX_timer.expires = jiffies + 20*EMAC_CHECK_LINK_TIME;
    add_timer(&RX_timer);
#endif

    MHal_EMAC_Power_On_Clk();

    init_timer(&EMAC_timer);
    init_timer(&Link_timer);

    EMAC_timer.data = EMAC_RX_TMR;
    EMAC_timer.function = MDev_EMAC_timer_callback;
    EMAC_timer.expires = jiffies;


    MHal_EMAC_Write_JULIAN_0100(JULIAN_100_VAL);

    if (0 > MDev_EMAC_ScanPhyAddr())
        goto end;

    MDev_EMAC_Patch_PHY();
    if (!MDev_EMAC_probe (emac_dev))
        return register_netdev (emac_dev);

end:
    free_netdev(emac_dev);
    emac_dev = 0;
    initstate = ETHERNET_TEST_INIT_FAIL;
    EMAC_DBG( KERN_ERR "Init EMAC error!\n" );
    return -1;
}
//-------------------------------------------------------------------------------------------------
// EMAC exit module
//-------------------------------------------------------------------------------------------------
static void MDev_EMAC_exit(void)
{
    if (emac_dev)
    {
    #ifndef INT_JULIAN_D
        del_timer(&EMAC_timer);
    #endif
        unregister_netdev(emac_dev);
        free_netdev(emac_dev);
    }
}

static int mstar_emac_drv_suspend(struct platform_device *dev, pm_message_t state)
{
#ifdef CONFIG_MSTAR_STR_TIME
	unsigned int u32Timer, u32Timer_us;
#endif
    struct net_device *netdev=(struct net_device*)dev->dev.platform_data;
    struct EMAC_private *LocPtr;
    u32 uRegVal;
    printk(KERN_INFO "mstar_emac_drv_suspend\n");
#ifdef CONFIG_MSTAR_STR_TIME
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x00), (REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x00)) & (~(0x1))));
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x02), 0xFFFF);
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x03), 0xFFFF);
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x00), (REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x00)) | (0x1)));
#endif
    if(!netdev)
    {
        return -1;
    }

    LocPtr = (struct EMAC_private*) netdev_priv(netdev);
    LocPtr->ep_flag |= EP_FLAG_SUSPENDING;
    netif_stop_queue (netdev);

    disable_irq(netdev->irq);
    del_timer(&Link_timer);

    //MHal_EMAC_Power_On_Clk();

    //Disable Receiver and Transmitter //
    uRegVal = MHal_EMAC_Read_CTL();
    uRegVal &= ~(EMAC_TE | EMAC_RE);
    MHal_EMAC_Write_CTL(uRegVal);

    // Disable PHY interrupt //
    MHal_EMAC_disable_phyirq ();
#ifndef INT_JULIAN_D
    //Disable MAC interrupts //
    uRegVal = EMAC_INT_RCOM | IER_FOR_INT_JULIAN_D;
    MHal_EMAC_Write_IDR(uRegVal);
#else
    MHal_EMAC_Write_IDR(IER_FOR_INT_JULIAN_D);
#endif
    MHal_EMAC_Power_Off_Clk();
    MDev_EMAC_SwReset(netdev);

#ifdef CONFIG_MSTAR_STR_TIME
	u32Timer = ((unsigned long)REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x04))) | ((unsigned long)REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x05)) << 16);
	u32Timer_us = ((u32Timer))/12;
	printk("emac suspend time : %d.%06ds\n\n", u32Timer_us/1000000, u32Timer_us%1000000);
#endif
    return 0;
}
static int mstar_emac_drv_resume(struct platform_device *dev)
{
#ifdef CONFIG_MSTAR_STR_TIME
	unsigned int u32Timer, u32Timer_us;
#endif
    struct net_device *netdev=(struct net_device*)dev->dev.platform_data;
    struct EMAC_private *LocPtr;
    phys_addr_t alloRAM_PA_BASE;
    phys_addr_t alloRAM_SIZE;
    u32 retval;
    printk(KERN_INFO "mstar_emac_drv_resume\n");
#ifdef CONFIG_MSTAR_STR_TIME
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x00), (REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x00)) & (~(0x1))));
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x02), 0xFFFF);
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x03), 0xFFFF);
	REG_WR_EMAC(REG_PIU_TIMER0_EMAC(0x00), (REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x00)) | (0x1)));
#endif
    if(!netdev)
    {
        return -1;
    }
    LocPtr = (struct EMAC_private*) netdev_priv(netdev);;
    LocPtr->ep_flag &= ~EP_FLAG_SUSPENDING;

    MHal_EMAC_Power_On_Clk();

    MHal_EMAC_Write_JULIAN_0100(JULIAN_100_VAL);

    if (0 > MDev_EMAC_ScanPhyAddr())
        return -1;

    MDev_EMAC_Patch_PHY();

        get_boot_mem_info(EMAC_MEM, &alloRAM_PA_BASE, &alloRAM_SIZE);
#if defined (CONFIG_ARM64)
        alloRAM_PA_BASE = memblock_start_of_DRAM() + memblock_size_of_first_region();
#endif

#ifndef RX_SOFTWARE_DESCRIPTOR
    //Add Write Protect
    //MHal_EMAC_Write_Protect(alloRAM_PA_BASE - MIU0_BUS_BASE, alloRAM_SIZE);
#endif

    ThisUVE.initedEMAC = 0;
    MDev_EMAC_HW_init();

    MDev_EMAC_update_mac_address (netdev); // Program ethernet address into MAC //
    spin_lock_irq (LocPtr->lock);
    MHal_EMAC_enable_mdi ();
    MHal_EMAC_read_phy (phyaddr, MII_USCR_REG, &retval);
    if ((retval & (1 << 10)) == 0)   // DSCR bit 10 is 0 -- fiber mode //
        LocPtr->phy_media = PORT_FIBRE;

    spin_unlock_irq (LocPtr->lock);

#ifdef CONFIG_MSTAR_HW_TX_CHECKSUM
    retval = MHal_EMAC_Read_JULIAN_0104() | TX_CHECKSUM_ENABLE;
    MHal_EMAC_Write_JULIAN_0104(retval);
#endif

    enable_irq(netdev->irq);
    if(LocPtr->ep_flag & EP_FLAG_OPEND)
    {
        if(0>MDev_EMAC_open(netdev))
        {
            printk(KERN_WARNING "Driver Emac: open failed after resume\n");
        }
    }
#ifdef CONFIG_MSTAR_STR_TIME
	u32Timer = ((unsigned long)REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x04))) | ((unsigned long)REG_RR_EMAC(REG_PIU_TIMER0_EMAC(0x05)) << 16);
	u32Timer_us = ((u32Timer))/12;
	printk("emac resume time : %d.%06ds\n\n", u32Timer_us/1000000, u32Timer_us%1000000);
#endif
    return 0;
}

static int mstar_emac_drv_probe(struct platform_device *pdev)
{
    int retval=0;

    if( !(pdev->name) || strcmp(pdev->name,"Mstar-emac")
        || pdev->id!=0)
    {
        retval = -ENXIO;
    }
    retval = MDev_EMAC_init();
    if(!retval)
    {
        pdev->dev.platform_data=emac_dev;
    }
	return retval;
}

static int mstar_emac_drv_remove(struct platform_device *pdev)
{
    if( !(pdev->name) || strcmp(pdev->name,"Mstar-emac")
        || pdev->id!=0)
    {
        return -1;
    }
    MDev_EMAC_exit();
    pdev->dev.platform_data=NULL;
    return 0;
}



static struct platform_driver Mstar_emac_driver = {
	.probe 		= mstar_emac_drv_probe,
	.remove 	= mstar_emac_drv_remove,
    .suspend    = mstar_emac_drv_suspend,
    .resume     = mstar_emac_drv_resume,

	.driver = {
		.name	= "Mstar-emac",
#if defined(CONFIG_ARM64)
		.of_match_table = mstaremac_of_device_ids,
#endif
        .owner  = THIS_MODULE,
	}
};

static int __init mstar_emac_drv_init_module(void)
{
    int retval=0;

    emac_dev=NULL;
    retval = platform_driver_register(&Mstar_emac_driver);
    return retval;
}

static void __exit mstar_emac_drv_exit_module(void)
{
    platform_driver_unregister(&Mstar_emac_driver);
    emac_dev=NULL;
}



module_init(mstar_emac_drv_init_module);
module_exit(mstar_emac_drv_exit_module);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("EMAC Ethernet driver");
MODULE_LICENSE("GPL");