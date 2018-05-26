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
 *  VDOGDI register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _VDOGDI_REG_H_
#define _VDOGDI_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
// Control Flag
#define VDO_GDI_CBCR_INTLV			(0x1<<16)
#define VDO_GDI_COLOR_MODE_420		(0x0<<17)
#define VDO_GDI_COLOR_MODE_422		(0x1<<17)
#define VDO_GDI_COLOR_MODE_224		(0x2<<17)
#define VDO_GDI_COLOR_MODE_444		(0x3<<17)
#define VDO_GDI_COLOR_MODE_400		(0x4<<17)
#define VDO_GDI_MAP_RASTER			(0<<20)
#define VDO_GDI_MAP_MIXED			(2<<20)
#define VDO_GDI_MAP_TILED_MB_RASTER	(3<<20)


#define VDEC_VDO_GDI_BASE	(VDEC_BASE + 0x60000)

// VDO GDI Register
/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_offset [31:29] = 0x0
/////////////////////////////////////////////////////////
#define VDO_GDI_PRI_RD_PRIO_L            (*(volatile UINT32 *)(stpVdoGdiReg + 0x000))
#define VDO_GDI_PRI_RD_PRIO_H            (*(volatile UINT32 *)(stpVdoGdiReg + 0x004))

#define VDO_GDI_PRI_RD_LOCK_CNT          (*(volatile UINT32 *)(stpVdoGdiReg + 0x010))

#define VDO_GDI_PIC_INIT_HOST            (*(volatile UINT32 *)(stpVdoGdiReg + 0x038))

#define VDO_GDI_CONFIG_REPORT_0          (*(volatile UINT32 *)(stpVdoGdiReg + 0x040))
#define VDO_GDI_CONFIG_REPORT_1          (*(volatile UINT32 *)(stpVdoGdiReg + 0x044))
#define VDO_GDI_CONFIG_REPORT_2          (*(volatile UINT32 *)(stpVdoGdiReg + 0x048))
#define VDO_GDI_CONFIG_REPORT_3          (*(volatile UINT32 *)(stpVdoGdiReg + 0x04C))

#define VDO_GDI_CONFIG_REPORT_4          (*(volatile UINT32 *)(stpVdoGdiReg + 0x050))
#define VDO_GDI_CONFIG_REPORT_5          (*(volatile UINT32 *)(stpVdoGdiReg + 0x054))
#define VDO_GDI_CONFIG_REPORT_6          (*(volatile UINT32 *)(stpVdoGdiReg + 0x058))
#define VDO_GDI_CONFIG_REPORT_7          (*(volatile UINT32 *)(stpVdoGdiReg + 0x05C))

#define VDO_GDI_PINFO_REQ                (*(volatile UINT32 *)(stpVdoGdiReg + 0x060))
#define VDO_GDI_PINFO_ACK                (*(volatile UINT32 *)(stpVdoGdiReg + 0x064))
#define VDO_GDI_PINFO_ADDR               (*(volatile UINT32 *)(stpVdoGdiReg + 0x068))
#define VDO_GDI_PINFO_DATA               (*(volatile UINT32 *)(stpVdoGdiReg + 0x06C))

/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_offset [31:29] = 0x4
/////////////////////////////////////////////////////////
#define VDO_GDI_INFO_CONTROL(frame_num)  (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x400)) // 0x400
#define VDO_GDI_INFO_PIC_SIZE(frame_num) (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x404)) // 0x400
#define VDO_GDI_INFO_BASE_Y(frame_num)   (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x408)) // 0x400
#define VDO_GDI_INFO_BASE_CB(frame_num)  (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x40C)) // 0x400
#define VDO_GDI_INFO_BASE_CR(frame_num)  (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x410)) // 0x400
#define VDO_GDI_INFO_BASE_Y_BOT(frame_num)   (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x414)) // 0x400
#define VDO_GDI_INFO_BASE_CB_BOT(frame_num)  (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x418)) // 0x400
#define VDO_GDI_INFO_BASE_CR_BOT(frame_num)  (*(volatile UINT32 *)(stpVdoGdiReg + frame_num*0x20 + 0x41C)) // 0x400


/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_offset [31:29] = 0x8
/////////////////////////////////////////////////////////
#define VDO_GDI_XY2_CAS_0                (*(volatile UINT32 *)(stpVdoGdiReg + 0x000))  //800
#define VDO_GDI_XY2_CAS_1                (*(volatile UINT32 *)(stpVdoGdiReg + 0x004))  //800
#define VDO_GDI_XY2_CAS_2                (*(volatile UINT32 *)(stpVdoGdiReg + 0x008))  //800
#define VDO_GDI_XY2_CAS_3                (*(volatile UINT32 *)(stpVdoGdiReg + 0x00C))  //800
#define VDO_GDI_XY2_CAS_4                (*(volatile UINT32 *)(stpVdoGdiReg + 0x010))  //800
#define VDO_GDI_XY2_CAS_5                (*(volatile UINT32 *)(stpVdoGdiReg + 0x014))  //800
#define VDO_GDI_XY2_CAS_6                (*(volatile UINT32 *)(stpVdoGdiReg + 0x018))  //800
#define VDO_GDI_XY2_CAS_7                (*(volatile UINT32 *)(stpVdoGdiReg + 0x01C))  //800
#define VDO_GDI_XY2_CAS_8                (*(volatile UINT32 *)(stpVdoGdiReg + 0x020))  //800
#define VDO_GDI_XY2_CAS_9                (*(volatile UINT32 *)(stpVdoGdiReg + 0x024))  //800
#define VDO_GDI_XY2_CAS_A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x028))  //800
#define VDO_GDI_XY2_CAS_B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x02C))  //800
#define VDO_GDI_XY2_CAS_C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x030))  //800
#define VDO_GDI_XY2_CAS_D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x034))  //800
#define VDO_GDI_XY2_CAS_E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x038))  //800
#define VDO_GDI_XY2_CAS_F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x03C))  //800

#define VDO_GDI_XY2_BA_0                 (*(volatile UINT32 *)(stpVdoGdiReg + 0x040))  //800
#define VDO_GDI_XY2_BA_1                 (*(volatile UINT32 *)(stpVdoGdiReg + 0x044))  //800
#define VDO_GDI_XY2_BA_2                 (*(volatile UINT32 *)(stpVdoGdiReg + 0x048))  //800
#define VDO_GDI_XY2_BA_3                 (*(volatile UINT32 *)(stpVdoGdiReg + 0x04C))  //800

#define VDO_GDI_XY2_RAS_0                (*(volatile UINT32 *)(stpVdoGdiReg + 0x050))  //800
#define VDO_GDI_XY2_RAS_1                (*(volatile UINT32 *)(stpVdoGdiReg + 0x054))  //800
#define VDO_GDI_XY2_RAS_2                (*(volatile UINT32 *)(stpVdoGdiReg + 0x058))  //800
#define VDO_GDI_XY2_RAS_3                (*(volatile UINT32 *)(stpVdoGdiReg + 0x05C))  //800
#define VDO_GDI_XY2_RAS_4                (*(volatile UINT32 *)(stpVdoGdiReg + 0x060))  //800
#define VDO_GDI_XY2_RAS_5                (*(volatile UINT32 *)(stpVdoGdiReg + 0x064))  //800
#define VDO_GDI_XY2_RAS_6                (*(volatile UINT32 *)(stpVdoGdiReg + 0x068))  //800
#define VDO_GDI_XY2_RAS_7                (*(volatile UINT32 *)(stpVdoGdiReg + 0x06C))  //800
#define VDO_GDI_XY2_RAS_8                (*(volatile UINT32 *)(stpVdoGdiReg + 0x070))  //800
#define VDO_GDI_XY2_RAS_9                (*(volatile UINT32 *)(stpVdoGdiReg + 0x074))  //800
#define VDO_GDI_XY2_RAS_A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x078))  //800
#define VDO_GDI_XY2_RAS_B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x07C))  //800
#define VDO_GDI_XY2_RAS_C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x080))  //800
#define VDO_GDI_XY2_RAS_D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x084))  //800
#define VDO_GDI_XY2_RAS_E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x088))  //800
#define VDO_GDI_XY2_RAS_F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x08C))  //800

#define VDO_GDI_XY2_RBC_CONFIG           (*(volatile UINT32 *)(stpVdoGdiReg + 0x090))  //800

#define VDO_GDI_RBC2_AXI_0               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A0))  //800
#define VDO_GDI_RBC2_AXI_1               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A4))  //800
#define VDO_GDI_RBC2_AXI_2               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A8))  //800
#define VDO_GDI_RBC2_AXI_3               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0AC))  //800
#define VDO_GDI_RBC2_AXI_4               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B0))  //800
#define VDO_GDI_RBC2_AXI_5               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B4))  //800
#define VDO_GDI_RBC2_AXI_6               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B8))  //800
#define VDO_GDI_RBC2_AXI_7               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0BC))  //800
#define VDO_GDI_RBC2_AXI_8               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C0))  //800
#define VDO_GDI_RBC2_AXI_9               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C4))  //800
#define VDO_GDI_RBC2_AXI_A               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C8))  //800
#define VDO_GDI_RBC2_AXI_B               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0CC))  //800
#define VDO_GDI_RBC2_AXI_C               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D0))  //800
#define VDO_GDI_RBC2_AXI_D               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D4))  //800
#define VDO_GDI_RBC2_AXI_E               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D8))  //800
#define VDO_GDI_RBC2_AXI_F               (*(volatile UINT32 *)(stpVdoGdiReg + 0x0DC))  //800
#define VDO_GDI_RBC2_AXI_10              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E0))  //800
#define VDO_GDI_RBC2_AXI_11              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E4))  //800
#define VDO_GDI_RBC2_AXI_12              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E8))  //800
#define VDO_GDI_RBC2_AXI_13              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0EC))  //800
#define VDO_GDI_RBC2_AXI_14              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F0))  //800
#define VDO_GDI_RBC2_AXI_15              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F4))  //800
#define VDO_GDI_RBC2_AXI_16              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F8))  //800
#define VDO_GDI_RBC2_AXI_17              (*(volatile UINT32 *)(stpVdoGdiReg + 0x0FC))  //800
#define VDO_GDI_RBC2_AXI_18              (*(volatile UINT32 *)(stpVdoGdiReg + 0x100))  //900
#define VDO_GDI_RBC2_AXI_19              (*(volatile UINT32 *)(stpVdoGdiReg + 0x104))  //900
#define VDO_GDI_RBC2_AXI_1A              (*(volatile UINT32 *)(stpVdoGdiReg + 0x108))  //900
#define VDO_GDI_RBC2_AXI_1B              (*(volatile UINT32 *)(stpVdoGdiReg + 0x10C))  //900
#define VDO_GDI_RBC2_AXI_1C              (*(volatile UINT32 *)(stpVdoGdiReg + 0x110))  //900
#define VDO_GDI_RBC2_AXI_1D              (*(volatile UINT32 *)(stpVdoGdiReg + 0x114))  //900
#define VDO_GDI_RBC2_AXI_1E              (*(volatile UINT32 *)(stpVdoGdiReg + 0x118))  //900
#define VDO_GDI_RBC2_AXI_1F              (*(volatile UINT32 *)(stpVdoGdiReg + 0x11C))  //900
#define VDO_GDI_TILEDBUF_BASE            (*(volatile UINT32 *)(stpVdoGdiReg + 0x120))  //900

/////////////////////////////////////////////////////////
// Must set RC0 APB_CONF register
// reg_apb_vdo_offset [31:29] = 0xA
/////////////////////////////////////////////////////////
#define VDO_GDI_CH1_XY2_CAS_0            (*(volatile UINT32 *)(stpVdoGdiReg + 0x000))  //800
#define VDO_GDI_CH1_XY2_CAS_1            (*(volatile UINT32 *)(stpVdoGdiReg + 0x004))  //800
#define VDO_GDI_CH1_XY2_CAS_2            (*(volatile UINT32 *)(stpVdoGdiReg + 0x008))  //800
#define VDO_GDI_CH1_XY2_CAS_3            (*(volatile UINT32 *)(stpVdoGdiReg + 0x00C))  //800
#define VDO_GDI_CH1_XY2_CAS_4            (*(volatile UINT32 *)(stpVdoGdiReg + 0x010))  //800
#define VDO_GDI_CH1_XY2_CAS_5            (*(volatile UINT32 *)(stpVdoGdiReg + 0x014))  //800
#define VDO_GDI_CH1_XY2_CAS_6            (*(volatile UINT32 *)(stpVdoGdiReg + 0x018))  //800
#define VDO_GDI_CH1_XY2_CAS_7            (*(volatile UINT32 *)(stpVdoGdiReg + 0x01C))  //800
#define VDO_GDI_CH1_XY2_CAS_8            (*(volatile UINT32 *)(stpVdoGdiReg + 0x020))  //800
#define VDO_GDI_CH1_XY2_CAS_9            (*(volatile UINT32 *)(stpVdoGdiReg + 0x024))  //800
#define VDO_GDI_CH1_XY2_CAS_A            (*(volatile UINT32 *)(stpVdoGdiReg + 0x028))  //800
#define VDO_GDI_CH1_XY2_CAS_B            (*(volatile UINT32 *)(stpVdoGdiReg + 0x02C))  //800
#define VDO_GDI_CH1_XY2_CAS_C            (*(volatile UINT32 *)(stpVdoGdiReg + 0x030))  //800
#define VDO_GDI_CH1_XY2_CAS_D            (*(volatile UINT32 *)(stpVdoGdiReg + 0x034))  //800
#define VDO_GDI_CH1_XY2_CAS_E            (*(volatile UINT32 *)(stpVdoGdiReg + 0x038))  //800
#define VDO_GDI_CH1_XY2_CAS_F            (*(volatile UINT32 *)(stpVdoGdiReg + 0x03C))  //800

#define VDO_GDI_CH1_XY2_BA_0             (*(volatile UINT32 *)(stpVdoGdiReg + 0x040))  //800
#define VDO_GDI_CH1_XY2_BA_1             (*(volatile UINT32 *)(stpVdoGdiReg + 0x044))  //800
#define VDO_GDI_CH1_XY2_BA_2             (*(volatile UINT32 *)(stpVdoGdiReg + 0x048))  //800
#define VDO_GDI_CH1_XY2_BA_3             (*(volatile UINT32 *)(stpVdoGdiReg + 0x04C))  //800

#define VDO_GDI_CH1_XY2_RAS_0            (*(volatile UINT32 *)(stpVdoGdiReg + 0x050))  //800
#define VDO_GDI_CH1_XY2_RAS_1            (*(volatile UINT32 *)(stpVdoGdiReg + 0x054))  //800
#define VDO_GDI_CH1_XY2_RAS_2            (*(volatile UINT32 *)(stpVdoGdiReg + 0x058))  //800
#define VDO_GDI_CH1_XY2_RAS_3            (*(volatile UINT32 *)(stpVdoGdiReg + 0x05C))  //800
#define VDO_GDI_CH1_XY2_RAS_4            (*(volatile UINT32 *)(stpVdoGdiReg + 0x060))  //800
#define VDO_GDI_CH1_XY2_RAS_5            (*(volatile UINT32 *)(stpVdoGdiReg + 0x064))  //800
#define VDO_GDI_CH1_XY2_RAS_6            (*(volatile UINT32 *)(stpVdoGdiReg + 0x068))  //800
#define VDO_GDI_CH1_XY2_RAS_7            (*(volatile UINT32 *)(stpVdoGdiReg + 0x06C))  //800
#define VDO_GDI_CH1_XY2_RAS_8            (*(volatile UINT32 *)(stpVdoGdiReg + 0x070))  //800
#define VDO_GDI_CH1_XY2_RAS_9            (*(volatile UINT32 *)(stpVdoGdiReg + 0x074))  //800
#define VDO_GDI_CH1_XY2_RAS_A            (*(volatile UINT32 *)(stpVdoGdiReg + 0x078))  //800
#define VDO_GDI_CH1_XY2_RAS_B            (*(volatile UINT32 *)(stpVdoGdiReg + 0x07C))  //800
#define VDO_GDI_CH1_XY2_RAS_C            (*(volatile UINT32 *)(stpVdoGdiReg + 0x080))  //800
#define VDO_GDI_CH1_XY2_RAS_D            (*(volatile UINT32 *)(stpVdoGdiReg + 0x084))  //800
#define VDO_GDI_CH1_XY2_RAS_E            (*(volatile UINT32 *)(stpVdoGdiReg + 0x088))  //800
#define VDO_GDI_CH1_XY2_RAS_F            (*(volatile UINT32 *)(stpVdoGdiReg + 0x08C))  //800

#define VDO_GDI_CH1_XY2_RBC_CONFIG       (*(volatile UINT32 *)(stpVdoGdiReg + 0x090))  //800

#define VDO_GDI_CH1_RBC2_AXI_0           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A0))  //800
#define VDO_GDI_CH1_RBC2_AXI_1           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A4))  //800
#define VDO_GDI_CH1_RBC2_AXI_2           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0A8))  //800
#define VDO_GDI_CH1_RBC2_AXI_3           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0AC))  //800
#define VDO_GDI_CH1_RBC2_AXI_4           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B0))  //800
#define VDO_GDI_CH1_RBC2_AXI_5           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B4))  //800
#define VDO_GDI_CH1_RBC2_AXI_6           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0B8))  //800
#define VDO_GDI_CH1_RBC2_AXI_7           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0BC))  //800
#define VDO_GDI_CH1_RBC2_AXI_8           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C0))  //800
#define VDO_GDI_CH1_RBC2_AXI_9           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C4))  //800
#define VDO_GDI_CH1_RBC2_AXI_A           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0C8))  //800
#define VDO_GDI_CH1_RBC2_AXI_B           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0CC))  //800
#define VDO_GDI_CH1_RBC2_AXI_C           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D0))  //800
#define VDO_GDI_CH1_RBC2_AXI_D           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D4))  //800
#define VDO_GDI_CH1_RBC2_AXI_E           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0D8))  //800
#define VDO_GDI_CH1_RBC2_AXI_F           (*(volatile UINT32 *)(stpVdoGdiReg + 0x0DC))  //800
#define VDO_GDI_CH1_RBC2_AXI_10          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E0))  //800
#define VDO_GDI_CH1_RBC2_AXI_11          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E4))  //800
#define VDO_GDI_CH1_RBC2_AXI_12          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0E8))  //800
#define VDO_GDI_CH1_RBC2_AXI_13          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0EC))  //800
#define VDO_GDI_CH1_RBC2_AXI_14          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F0))  //800
#define VDO_GDI_CH1_RBC2_AXI_15          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F4))  //800
#define VDO_GDI_CH1_RBC2_AXI_16          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0F8))  //800
#define VDO_GDI_CH1_RBC2_AXI_17          (*(volatile UINT32 *)(stpVdoGdiReg + 0x0FC))  //800
#define VDO_GDI_CH1_RBC2_AXI_18          (*(volatile UINT32 *)(stpVdoGdiReg + 0x100))  //900
#define VDO_GDI_CH1_RBC2_AXI_19          (*(volatile UINT32 *)(stpVdoGdiReg + 0x104))  //900
#define VDO_GDI_CH1_RBC2_AXI_1A          (*(volatile UINT32 *)(stpVdoGdiReg + 0x108))  //900
#define VDO_GDI_CH1_RBC2_AXI_1B          (*(volatile UINT32 *)(stpVdoGdiReg + 0x10C))  //900
#define VDO_GDI_CH1_RBC2_AXI_1C          (*(volatile UINT32 *)(stpVdoGdiReg + 0x110))  //900
#define VDO_GDI_CH1_RBC2_AXI_1D          (*(volatile UINT32 *)(stpVdoGdiReg + 0x114))  //900
#define VDO_GDI_CH1_RBC2_AXI_1E          (*(volatile UINT32 *)(stpVdoGdiReg + 0x118))  //900
#define VDO_GDI_CH1_RBC2_AXI_1F          (*(volatile UINT32 *)(stpVdoGdiReg + 0x11C))  //900
#define VDO_GDI_CH1_TILEDBUF_BASE        (*(volatile UINT32 *)(stpVdoGdiReg + 0x120))  //900

// D13 VDO GDI
#define VDO_GDI_CH0_XY2_AXI_LUM_00                (*(volatile UINT32 *)(stpVdoGdiReg + 0x800))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_01                (*(volatile UINT32 *)(stpVdoGdiReg + 0x804))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_02                (*(volatile UINT32 *)(stpVdoGdiReg + 0x808))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_03                (*(volatile UINT32 *)(stpVdoGdiReg + 0x80C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_04                (*(volatile UINT32 *)(stpVdoGdiReg + 0x810))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_05                (*(volatile UINT32 *)(stpVdoGdiReg + 0x814))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_06                (*(volatile UINT32 *)(stpVdoGdiReg + 0x818))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_07                (*(volatile UINT32 *)(stpVdoGdiReg + 0x81C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_08                (*(volatile UINT32 *)(stpVdoGdiReg + 0x820))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_09                (*(volatile UINT32 *)(stpVdoGdiReg + 0x824))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x828))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x82C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x830))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x834))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x838))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_0F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x83C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_10                (*(volatile UINT32 *)(stpVdoGdiReg + 0x840))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_11                (*(volatile UINT32 *)(stpVdoGdiReg + 0x844))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_12                (*(volatile UINT32 *)(stpVdoGdiReg + 0x848))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_13                (*(volatile UINT32 *)(stpVdoGdiReg + 0x84C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_14                (*(volatile UINT32 *)(stpVdoGdiReg + 0x850))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_15                (*(volatile UINT32 *)(stpVdoGdiReg + 0x854))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_16                (*(volatile UINT32 *)(stpVdoGdiReg + 0x858))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_17                (*(volatile UINT32 *)(stpVdoGdiReg + 0x85C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_18                (*(volatile UINT32 *)(stpVdoGdiReg + 0x860))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_19                (*(volatile UINT32 *)(stpVdoGdiReg + 0x864))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x868))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x86C))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x870))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x874))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x878))  //800
#define VDO_GDI_CH0_XY2_AXI_LUM_1F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x87C))  //800

#define VDO_GDI_CH0_XY2_AXI_CHR_00                (*(volatile UINT32 *)(stpVdoGdiReg + 0x880))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_01                (*(volatile UINT32 *)(stpVdoGdiReg + 0x884))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_02                (*(volatile UINT32 *)(stpVdoGdiReg + 0x888))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_03                (*(volatile UINT32 *)(stpVdoGdiReg + 0x88C))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_04                (*(volatile UINT32 *)(stpVdoGdiReg + 0x890))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_05                (*(volatile UINT32 *)(stpVdoGdiReg + 0x894))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_06                (*(volatile UINT32 *)(stpVdoGdiReg + 0x898))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_07                (*(volatile UINT32 *)(stpVdoGdiReg + 0x89C))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_08                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8A0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_09                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8A4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8A8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8AC))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8B0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8B4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8B8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_0F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8BC))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_10                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8C0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_11                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8C4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_12                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8C8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_13                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8CC))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_14                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8D0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_15                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8D4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_16                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8D8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_17                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8DC))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_18                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8E0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_19                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8E4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1A                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8E8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1B                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8EC))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1C                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8F0))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1D                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8F4))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1E                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8F8))  //800
#define VDO_GDI_CH0_XY2_AXI_CHR_1F                (*(volatile UINT32 *)(stpVdoGdiReg + 0x8FC))  //800

#define VDO_GDI_CH0_XY2_AXI_CONFIG           	  (*(volatile UINT32 *)(stpVdoGdiReg + 0x900))  //900
/* Reserved */
#define VDO_GDI_CH1_XY2_AXI_LUM_00                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA00))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_01                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA04))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_02                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA08))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_03                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA0C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_04                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA10))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_05                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA14))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_06                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA18))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_07                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA1C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_08                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA20))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_09                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA24))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0A                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA28))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0B                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA2C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0C                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA30))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0D                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA34))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0E                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA38))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_0F                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA3C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_10                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA40))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_11                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA44))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_12                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA48))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_13                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA4C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_14                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA50))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_15                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA54))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_16                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA58))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_17                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA5C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_18                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA60))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_19                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA64))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1A                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA68))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1B                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA6C))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1C                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA70))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1D                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA74))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1E                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA78))  //A00
#define VDO_GDI_CH1_XY2_AXI_LUM_1F                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA7C))  //A00

#define VDO_GDI_CH1_XY2_AXI_CHR_00                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA80))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_01                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA84))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_02                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA88))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_03                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA8C))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_04                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA90))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_05                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA94))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_06                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA98))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_07                (*(volatile UINT32 *)(stpVdoGdiReg + 0xA9C))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_08                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAA0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_09                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAA4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0A                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAA8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0B                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAAC))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0C                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAB0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0D                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAB4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0E                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAB8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_0F                (*(volatile UINT32 *)(stpVdoGdiReg + 0xABC))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_10                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAC0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_11                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAC4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_12                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAC8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_13                (*(volatile UINT32 *)(stpVdoGdiReg + 0xACC))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_14                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAD0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_15                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAD4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_16                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAD8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_17                (*(volatile UINT32 *)(stpVdoGdiReg + 0xADC))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_18                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAE0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_19                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAE4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1A                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAE8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1B                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAEC))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1C                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAF0))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1D                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAF4))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1E                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAF8))  //A00
#define VDO_GDI_CH1_XY2_AXI_CHR_1F                (*(volatile UINT32 *)(stpVdoGdiReg + 0xAFC))  //A00

#define VDO_GDI_CH1_XY2_AXI_CONFIG           	  (*(volatile UINT32 *)(stpVdoGdiReg + 0xB00))  //B00


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif
#if 0
/*-----------------------------------------------------------------------------
	0x0000 vdo_gdi_pri_rd_prio_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pri_rd_prio_l                   ;   	// 31: 0
} VDO_GDI_PRI_RD_PRIO_L;

/*-----------------------------------------------------------------------------
	0x0004 vdo_gdi_pri_rd_prio_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pri_rd_prio_h                   ;   	// 31: 0
} VDO_GDI_PRI_RD_PRIO_H;

/*-----------------------------------------------------------------------------
	0x0010 vdo_gdi_pri_rd_lock_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pri_rd_lock_cnt                 ;   	// 31: 0
} VDO_GDI_PRI_RD_LOCK_CNT;

/*-----------------------------------------------------------------------------
	0x0038 vdo_gdi_pic_init_host ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_init                        : 1;	//     0
} VDO_GDI_PIC_INIT_HOST;

/*-----------------------------------------------------------------------------
	0x0040 vdo_gdi_config_report_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	product_name                    ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_0;

/*-----------------------------------------------------------------------------
	0x0044 vdo_gdi_config_report_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	product_version                 ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_1;

/*-----------------------------------------------------------------------------
	0x0048 vdo_gdi_config_report_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	axi1k                           : 1,	//     7
	axi2axi_async                   : 1,	//     8
	axi64bit                        : 1,	//     9
	dcu_if                          : 1,	//    10
	axi_2nd                         : 1,	//    11
	ref_clk                         : 1,	//    12
	                                : 1,	//    13  reserved
	map_linear                      : 1,	//    14
	map_tiled                       : 1,	//    15
	                                : 8;	// 16:23 reserved
} VDO_GDI_CONFIG_REPORT_2;

/*-----------------------------------------------------------------------------
	0x004c vdo_gdi_config_report_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mp2_mp                          : 1,	//     0
	mp4_sp                          : 1,	//     1
	mp4_asp                         : 1,	//     2
	mp4_263                         : 1,	//     3
	mp4_dvx                         : 1,	//     4
	mp4_xvid                        : 1,	//     5
	mp4_spk                         : 1,	//     6
	rv8                             : 1,	//     7
	rv9                             : 1,	//     8
	rv10                            : 1,	//     9
	jpg_bp                          : 1,	//    10
	std_h264                        : 1,	//    11
	std_vc1                         : 1,	//    12
	std_mp2                         : 1,	//    13
	std_mp4                         : 1,	//    14
	std_rvx                         : 1,	//    15
	std_avs                         : 1,	//    16
	std_jpg                         : 1,	//    17
	x264_bp                         : 1,	//    18
	x264_mp                         : 1,	//    19
	x264_hp                         : 1,	//    20
	vc1_sp                          : 1,	//    21
	vc1_mp                          : 1,	//    22
	vc1_ap                          : 1,	//    23
	vc1_wmv                         : 1;	//    24
} VDO_GDI_CONFIG_REPORT_3;

/*-----------------------------------------------------------------------------
	0x0050 vdo_gdi_config_report_4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	release_ver                     ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_4;

/*-----------------------------------------------------------------------------
	0x0054 vdo_gdi_config_report_5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	config_data                     ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_5;

/*-----------------------------------------------------------------------------
	0x0058 vdo_gdi_config_report_6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	config_rev                      ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_6;

/*-----------------------------------------------------------------------------
	0x005c vdo_gdi_config_report_7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	config_type                     ;   	// 31: 0
} VDO_GDI_CONFIG_REPORT_7;

/*-----------------------------------------------------------------------------
	0x0060 vdo_gdi_pinfo_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pinfo_req                       : 1;	//     0
} VDO_GDI_PINFO_REQ;

/*-----------------------------------------------------------------------------
	0x0064 vdo_gdi_pinfo_ack ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pinfo_ack                       : 1;	//     0
} VDO_GDI_PINFO_ACK;

/*-----------------------------------------------------------------------------
	0x0068 vdo_gdi_pinfo_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	pinfo_addr                      : 8;	//  2: 9
} VDO_GDI_PINFO_ADDR;

/*-----------------------------------------------------------------------------
	0x006c vdo_gdi_pinfo_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pinfo_rdata                     ;   	// 31: 0
} VDO_GDI_PINFO_DATA;

/*-----------------------------------------------------------------------------
	0x0400 vdo_gdi_info_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stride                          :16,	//  0:15
	cbcrinterleaved                 : 1,	//    16
	colormode                       : 3,	// 17:19
	framemap                        : 2;	// 20:21
} VDO_GDI_INFO_CONTROL;

/*-----------------------------------------------------------------------------
	0x0404 vdo_gdi_info_pic_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	picversizeluma                  :16,	//  0:15
	pichorsizeluma                  :16;	// 16:31
} VDO_GDI_INFO_PIC_SIZE;

/*-----------------------------------------------------------------------------
	0x0408 vdo_gdi_info_base_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lumabaseaddr                    ;   	// 31: 0
} VDO_GDI_INFO_BASE_Y;

/*-----------------------------------------------------------------------------
	0x040c vdo_gdi_info_base_cb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cbbaseaddr                      ;   	// 31: 0
} VDO_GDI_INFO_BASE_CB;

/*-----------------------------------------------------------------------------
	0x0410 vdo_gdi_info_base_cr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	crbaseaddr                      ;   	// 31: 0
} VDO_GDI_INFO_BASE_CR;

/*-----------------------------------------------------------------------------
	0x0800~0x083c vdo_gdi_ch_xy2_cas_n (n<16) ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bitselcr                        : 4,	//  0: 3
	xyselcr                         : 1,	//     4
	topbotcr                        : 1,	//     5
	zerocr                          : 1,	//     6
	invertcr                        : 1,	//     7
	xysellu                         : 4,	//  8:11
	bitsellu                        : 1,	//    12
	topbotlu                        : 1,	//    13
	zerolu                          : 1,	//    14
	invertlu                        : 1;	//    15
} VDO_GDI_CH_XY2_CAS;

/*-----------------------------------------------------------------------------
	0x0840~0x084c vdo_gdi_ch_xy2_ba_n (n<4) ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bitselcr                        : 4,	//  0: 3
	xyselcr                         : 1,	//     4
	topbotcr                        : 1,	//     5
	zerocr                          : 1,	//     6
	invertcr                        : 1,	//     7
	xysellu                         : 4,	//  8:11
	bitsellu                        : 1,	//    12
	topbotlu                        : 1,	//    13
	zerolu                          : 1,	//    14
	invertlu                        : 1;	//    15
} VDO_GDI_CH_XY2_BA;

/*-----------------------------------------------------------------------------
	0x0850~0x088c vdo_gdi_ch_xy2_ras_n (n<16) ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bitselcr                        : 4,	//  0: 3
	xyselcr                         : 1,	//     4
	topbotcr                        : 1,	//     5
	zerocr                          : 1,	//     6
	invertcr                        : 1,	//     7
	xysellu                         : 4,	//  8:11
	bitsellu                        : 1,	//    12
	topbotlu                        : 1,	//    13
	zerolu                          : 1,	//    14
	invertlu                        : 1;	//    15
} VDO_GDI_CH_XY2_RAS;

/*-----------------------------------------------------------------------------
	0x0890 vdo_gdi_ch_xy2_rbc_config ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	burstlenfieldcr                 : 4,	//  0: 3
	burstlenframecr                 : 4,	//  4: 7
	burstlenfieldlu                 : 4,	//  8:11
	burstlenframelu                 : 4,	// 12:15
	addrincdir                      : 1,	//    16
	tiledmap                        : 1,	//    17
	topbotsplit                     : 1,	//    18
	fieldmemmap                     : 1,	//    19
	maxburstlinear                  : 4;	// 20:23
} VDO_GDI_CH_XY2_RBC_CONFIG;

/*-----------------------------------------------------------------------------
	0x08a0~0x091c vdo_gdi_ch_rbc2_axi_n (n<32) ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bitsel                          : 4,	//  0: 3
	rbcsel                          : 2;	//  4: 5
} VDO_GDI_CH_RBC2_AXI;

/*-----------------------------------------------------------------------------
	0x0920 vdo_gdi_ch_tiledbuf_base ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tiledbaseaddr                   ;   	// 31: 0
} VDO_GDI_CH_TILEDBUF_BASE;

typedef struct {
	VDO_GDI_PRI_RD_PRIO_L           	vdo_gdi_pri_rd_prio_l           ;	// 0x0000 : ''
	VDO_GDI_PRI_RD_PRIO_H           	vdo_gdi_pri_rd_prio_h           ;	// 0x0004 : ''
	UINT32                          	                 __rsvd_00[   2];	// 0x0008 ~ 0x000c
	VDO_GDI_PRI_RD_LOCK_CNT         	vdo_gdi_pri_rd_lock_cnt         ;	// 0x0010 : ''
	UINT32                          	                 __rsvd_01[   9];	// 0x0014 ~ 0x0034
	VDO_GDI_PIC_INIT_HOST           	vdo_gdi_pic_init_host           ;	// 0x0038 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x003c
	VDO_GDI_CONFIG_REPORT_0         	vdo_gdi_config_report_0         ;	// 0x0040 : ''
	VDO_GDI_CONFIG_REPORT_1         	vdo_gdi_config_report_1         ;	// 0x0044 : ''
	VDO_GDI_CONFIG_REPORT_2         	vdo_gdi_config_report_2         ;	// 0x0048 : ''
	VDO_GDI_CONFIG_REPORT_3         	vdo_gdi_config_report_3         ;	// 0x004c : ''
	VDO_GDI_CONFIG_REPORT_4         	vdo_gdi_config_report_4         ;	// 0x0050 : ''
	VDO_GDI_CONFIG_REPORT_5         	vdo_gdi_config_report_5         ;	// 0x0054 : ''
	VDO_GDI_CONFIG_REPORT_6         	vdo_gdi_config_report_6         ;	// 0x0058 : ''
	VDO_GDI_CONFIG_REPORT_7         	vdo_gdi_config_report_7         ;	// 0x005c : ''
	VDO_GDI_PINFO_REQ               	vdo_gdi_pinfo_req               ;	// 0x0060 : ''
	VDO_GDI_PINFO_ACK               	vdo_gdi_pinfo_ack               ;	// 0x0064 : ''
	VDO_GDI_PINFO_ADDR              	vdo_gdi_pinfo_addr              ;	// 0x0068 : ''
	VDO_GDI_PINFO_DATA              	vdo_gdi_pinfo_data              ;	// 0x006c : ''
	UINT32                          	                 __rsvd_03[ 228];	// 0x0070 ~ 0x03fc
	VDO_GDI_INFO_CONTROL            	vdo_gdi_info_control            ;	// 0x0400 : ''
	VDO_GDI_INFO_PIC_SIZE           	vdo_gdi_info_pic_size           ;	// 0x0404 : ''
	VDO_GDI_INFO_BASE_Y             	vdo_gdi_info_base_y             ;	// 0x0408 : ''
	VDO_GDI_INFO_BASE_CB            	vdo_gdi_info_base_cb            ;	// 0x040c : ''
	VDO_GDI_INFO_BASE_CR            	vdo_gdi_info_base_cr            ;	// 0x0410 : ''
	UINT32                          	                 __rsvd_04[ 251];	// 0x0414 ~ 0x07fc
	struct {
	VDO_GDI_CH_XY2_CAS              	vdo_gdi_ch_xy2_cas[16]          ;	// 0x0800 ~ 0x083c
	VDO_GDI_CH_XY2_BA               	vdo_gdi_ch_xy2_ba[4]            ;	// 0x0840 ~ 0x084c
	VDO_GDI_CH_XY2_RAS              	vdo_gdi_ch_xy2_ras[16]          ;	// 0x0850 ~ 0x088c
	VDO_GDI_CH_XY2_RBC_CONFIG       	vdo_gdi_ch_xy2_rbc_config       ;	// 0x0890 : ''
	VDO_GDI_CH_RBC2_AXI             	vdo_gdi_ch_rbc2_axi[32]         ;	// 0x08a0 ~ 0x0891
	VDO_GDI_CH_TILEDBUF_BASE        	vdo_gdi_ch_tiledbuf_base        ;	// 0x0920 : ''
	UINT32                          	                 __rsvd_05[  55];	// 0x0924 ~ 0x09fc
	} vdo_gdi_ch_tiledmap[2];
} VDOGDI_REG_T;
/* 58 regs, 58 types */

/* 59 regs, 59 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVDOGDI : real register of VDOGDI.
 * gpRegVDOGDI     : shadow register.
 *
 * @def VDOGDI_RdFL: Read  FLushing : Shadow <- Real.
 * @def VDOGDI_WrFL: Write FLushing : Shadow -> Real.
 * @def VDOGDI_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VDOGDI_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VDOGDI_Rd01 ~ VDOGDI_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VDOGDI_Wr01 ~ VDOGDI_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VDOGDI_RdFL(_r)			((gpRegVDOGDI->_r)=(gpRealRegVDOGDI->_r))
#define VDOGDI_WrFL(_r)			((gpRealRegVDOGDI->_r)=(gpRegVDOGDI->_r))

#define VDOGDI_Rd(_r)			*((UINT32*)(&(gpRegVDOGDI->_r)))
#define VDOGDI_Wr(_r,_v)			((VDOGDI_Rd(_r))=((UINT32)(_v)))

#define VDOGDI_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
								} while(0)

#define VDOGDI_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
								} while(0)

#define VDOGDI_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
								} while(0)

#define VDOGDI_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
								} while(0)

#define VDOGDI_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
								} while(0)

#define VDOGDI_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
								} while(0)

#define VDOGDI_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
								} while(0)

#define VDOGDI_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
								} while(0)

#define VDOGDI_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
								} while(0)

#define VDOGDI_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
								} while(0)

#define VDOGDI_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
								} while(0)

#define VDOGDI_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
									(_v12) = (gpRegVDOGDI->_r._f12);				\
								} while(0)

#define VDOGDI_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
									(_v12) = (gpRegVDOGDI->_r._f12);				\
									(_v13) = (gpRegVDOGDI->_r._f13);				\
								} while(0)

#define VDOGDI_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
									(_v12) = (gpRegVDOGDI->_r._f12);				\
									(_v13) = (gpRegVDOGDI->_r._f13);				\
									(_v14) = (gpRegVDOGDI->_r._f14);				\
								} while(0)

#define VDOGDI_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
									(_v12) = (gpRegVDOGDI->_r._f12);				\
									(_v13) = (gpRegVDOGDI->_r._f13);				\
									(_v14) = (gpRegVDOGDI->_r._f14);				\
									(_v15) = (gpRegVDOGDI->_r._f15);				\
								} while(0)

#define VDOGDI_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVDOGDI->_r._f01);				\
									(_v02) = (gpRegVDOGDI->_r._f02);				\
									(_v03) = (gpRegVDOGDI->_r._f03);				\
									(_v04) = (gpRegVDOGDI->_r._f04);				\
									(_v05) = (gpRegVDOGDI->_r._f05);				\
									(_v06) = (gpRegVDOGDI->_r._f06);				\
									(_v07) = (gpRegVDOGDI->_r._f07);				\
									(_v08) = (gpRegVDOGDI->_r._f08);				\
									(_v09) = (gpRegVDOGDI->_r._f09);				\
									(_v10) = (gpRegVDOGDI->_r._f10);				\
									(_v11) = (gpRegVDOGDI->_r._f11);				\
									(_v12) = (gpRegVDOGDI->_r._f12);				\
									(_v13) = (gpRegVDOGDI->_r._f13);				\
									(_v14) = (gpRegVDOGDI->_r._f14);				\
									(_v15) = (gpRegVDOGDI->_r._f15);				\
									(_v16) = (gpRegVDOGDI->_r._f16);				\
								} while(0)


#define VDOGDI_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
								} while(0)

#define VDOGDI_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
								} while(0)

#define VDOGDI_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
								} while(0)

#define VDOGDI_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
								} while(0)

#define VDOGDI_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
								} while(0)

#define VDOGDI_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
								} while(0)

#define VDOGDI_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
								} while(0)

#define VDOGDI_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
								} while(0)

#define VDOGDI_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
								} while(0)

#define VDOGDI_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
								} while(0)

#define VDOGDI_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
								} while(0)

#define VDOGDI_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
									(gpRegVDOGDI->_r._f12) = (_v12);				\
								} while(0)

#define VDOGDI_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
									(gpRegVDOGDI->_r._f12) = (_v12);				\
									(gpRegVDOGDI->_r._f13) = (_v13);				\
								} while(0)

#define VDOGDI_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
									(gpRegVDOGDI->_r._f12) = (_v12);				\
									(gpRegVDOGDI->_r._f13) = (_v13);				\
									(gpRegVDOGDI->_r._f14) = (_v14);				\
								} while(0)

#define VDOGDI_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
									(gpRegVDOGDI->_r._f12) = (_v12);				\
									(gpRegVDOGDI->_r._f13) = (_v13);				\
									(gpRegVDOGDI->_r._f14) = (_v14);				\
									(gpRegVDOGDI->_r._f15) = (_v15);				\
								} while(0)

#define VDOGDI_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVDOGDI->_r._f01) = (_v01);				\
									(gpRegVDOGDI->_r._f02) = (_v02);				\
									(gpRegVDOGDI->_r._f03) = (_v03);				\
									(gpRegVDOGDI->_r._f04) = (_v04);				\
									(gpRegVDOGDI->_r._f05) = (_v05);				\
									(gpRegVDOGDI->_r._f06) = (_v06);				\
									(gpRegVDOGDI->_r._f07) = (_v07);				\
									(gpRegVDOGDI->_r._f08) = (_v08);				\
									(gpRegVDOGDI->_r._f09) = (_v09);				\
									(gpRegVDOGDI->_r._f10) = (_v10);				\
									(gpRegVDOGDI->_r._f11) = (_v11);				\
									(gpRegVDOGDI->_r._f12) = (_v12);				\
									(gpRegVDOGDI->_r._f13) = (_v13);				\
									(gpRegVDOGDI->_r._f14) = (_v14);				\
									(gpRegVDOGDI->_r._f15) = (_v15);				\
									(gpRegVDOGDI->_r._f16) = (_v16);				\
								} while(0)

/* Indexed Register Access.
 *
 * There is in-direct field specified by 'index' field within a register.
 * Normally a register has only one meaning for a 'field_name', but indexed register
 * can hold several data for a 'field_name' specifed by 'index' field of indexed register.
 * When writing an 3rd data for given 'field_name' register, you need to set 'rw' = 0, 'index' = 2,
 * and 'load' = 0.
 *
 * ASSUMPTION
 * For Writing indexed register load bit
 *
 * parameter list
 * _r     : name of register
 * _lname : name of load  bit field	: shall be 0 after macro executed.
 * _rwname: name of rw    bit field : shall be 0 after VDOGDI_Wind(), 1 for VDOGDI_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VDOGDI_Rind : General indexed register Read.(
 * VDOGDI_Wind : General indexed register Read.
 *
 * VDOGDI_Ridx : For 'index', 'rw', 'load' field name
 * VDOGDI_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VDOGDI_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VDOGDI_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VDOGDI_WrFL(_r);									\
								VDOGDI_RdFL(_r);									\
								VDOGDI_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VDOGDI_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VDOGDI_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VDOGDI_Ridx(_r, _ival, _fname, _fval)	VDOGDI_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VDOGDI_Widx(_r, _ival, _fname, _fval)	VDOGDI_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif
#endif
#endif	/* _VDOGDI_REG_H_ */

/* from 'vdogdi-REG.csv' 20120326 19:08:28   대한민국 표준시 by getregs v2.7 */
