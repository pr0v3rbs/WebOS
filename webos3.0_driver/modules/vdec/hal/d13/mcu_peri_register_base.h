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
#ifndef _MCU_PERI_REGISTER_H_
#define _MCU_PERI_REGISTER_H_

//#include <LG1153/Option.h>
//#include <LG1153/DataType.h>

#define WR32(reg,val) (*(volatile unsigned int *)(reg) = (val))
#define RD32(reg)     (*(volatile unsigned int *)(reg))

#define     DDR_MEM_BASE_ADDR    0x00000000
#define     MCU0_PIF_BASE_ADDR   0x40000000
#define     MCU1_PIF_BASE_ADDR   0x40000000
#define     SC_MEM_BASE_ADDR     0xC0000000  // 128KB
#define     SROM_BASE_ADDR       0xD0000000  //   8KB
#define     SRAM_BASE_ADDR       0xE0002000  //   4KB

// Peripheral
#define     MCU_RC_BASE_ADDR     0xF0000000  //   4KB
#define     WDT_BASE_ADDR        0xF0001000  //   4KB
#define     UART0_BASE_ADDR      0xF0002000  //   4KB
#define     UART1_BASE_ADDR      0xF0003000  //   4KB
#define     SPI_BASE_ADDR        0xF0004000  //   4KB
#define     DMA_BASE_ADDR        0xF0005000  //   4KB
#define     I2C_BASE_ADDR        0xF0006000  //   4KB
#define     GPIO_BASE_ADDR       0xF0007000  //   4KB

// VD0 APB
#define     PDEC_BASE_ADDR       0xF0010000  //   4KB
#define     VD0_PL301_BASE_ADDR  0xF0011000  //   4KB
#define     VD0_BWM_BASE_ADDR    0xF0012000  //   4KB
#define     TE_BASE_ADDR         0xF0020000  //   8KB
#define     ADO_BASE_ADDR        0xE0000000  //   8KB

// VD1 APB
#define     HEVC_BASE_ADDR       0xF0050000  //   4KB
#define     VD1_BWM0_BASE_ADDR   0xF0051000  //   4KB
#define     VD1_BWM1_BASE_ADDR   0xF0052000  //   4KB

// VD2 APB
#define     AVC_BASE_ADDR        0xF0030000  //  64KB
#define     VD2_BWM0_BASE_ADDR   0xF0040000  //   4KB
#define     VD2_BWM1_BASE_ADDR   0xF0040800  //   4KB

// VD3 APB
#define     VDO_BASE_ADDR        0xF0060000  //  16KB
#define     VCP_BASE_ADDR        0xF0061000  //  16KB
#define     ON2_BASE_ADDR        0xF0062000  //  16KB
#define     VD3_BWM_BASE_ADDR    0xF0063000  //
#define     VD3_SLT_BASE_ADDR    0xF0063400  //
#define     VD3_HREG_BASE_ADDR   0xF0063600  //
#define     VD3_HMEM_BASE_ADDR   0xF0063800  //
#define     HDMI_BASE_ADDR       0xF0080000  // 256KB

#define     DDRC_BASE_ADDR       0xF0070000  //   4KB

// H/W Int enable bit
#define IPC_HW_INT_EN      1 <<  0
#define TE_HW_INT_EN       1 <<  1
#define PDEC_HW_INT_EN     1 <<  2
#define VDEC_HW_INT_EN     1 <<  3
#define ADO_HW_INT_EN      1 <<  4
#define VDO_HW_INT_EN      1 <<  5
#define VCP_HW_INT_EN      1 <<  6
#define HDMI_1_HW_INT_EN   1 <<  7
#define HDMI_0_HW_INT_EN   1 <<  8
#define BWM_HW_INT_EN      1 << 10
#define UART_HW_INT_EN     1 << 11
#define SPI_HW_INT_EN      1 << 12
#define I2C_HW_INT_EN      1 << 13
#define DMA_HW_INT_EN      1 << 14
#define GPIO_HW_INT_EN     1 << 15
#define WDT_HW_INT_EN      1 << 16

#define IPC_INT_NUM        0
#define TE_INT_NUM         1
#define PDEC_INT_NUM       2
#define VDEC_INT_NUM       3
#define ADO_INT_NUM        4
#define VDO_INT_NUM        5
#define VCP_INT_NUM        8
#define HDMI_1_INT_NUM     9
#define HDMI_0_INT_NUM     12
#define BWM_INT_NUM        15
#define UART_INT_NUM       16
#define SPI_INT_NUM        17
#define I2C_INT_NUM        18
#define DMA_INT_NUM        19
#define GPIO_INT_NUM       20
#define WDT_INT_NUM        21


#define MCU_BASE 	0xF0000000
#define MCU0_BASE 	0xF0000000
#define MCU1_BASE 	0xF00000C0

#define AD_M1_RUNSTALL             MCU1_BASE + (0x000 << 2)
#define AD_M1_STAT_VECTOR_SEL      MCU1_BASE + (0x001 << 2)
#define AD_M1_PDEBUGENABLE         MCU1_BASE + (0x002 << 2)
#define AD_M1_PDEBUGDATA           MCU1_BASE + (0x003 << 2)
#define AD_M1_PDEBUGPC             MCU1_BASE + (0x004 << 2)
#define AD_M1_PDEBUGSTATUS         MCU1_BASE + (0x005 << 2)
#define AD_M1_ADDR_TR_0            MCU1_BASE + (0x006 << 2)
#define AD_M1_ADDR_TR_1            MCU1_BASE + (0x007 << 2)
#define AD_M1_ADDR_TR_2            MCU1_BASE + (0x008 << 2)
#define AD_M1_ADDR_TR_3            MCU1_BASE + (0x009 << 2)
#define AD_M1_INTR_ENABLE          MCU1_BASE + (0x00A << 2)
#define AD_M1_INTR_MON             MCU1_BASE + (0x00B << 2)
#define AD_M1_IPC_INTR_EN          MCU1_BASE + (0x00C << 2)
#define AD_M1_IPC_INTR_STATUS      MCU1_BASE + (0x00D << 2)
#define AD_M1_IPC_INTR_RAW_STATUS  MCU1_BASE + (0x00E << 2)
#define AD_M1_IPC_INTR_CLR         MCU1_BASE + (0x00F << 2)
#define AD_M1_IPC_00               MCU1_BASE + (0x010 << 2)
#define AD_M1_IPC_01               MCU1_BASE + (0x011 << 2)
#define AD_M1_IPC_02               MCU1_BASE + (0x012 << 2)
#define AD_M1_IPC_03               MCU1_BASE + (0x013 << 2)
#define AD_M1_IPC_04               MCU1_BASE + (0x014 << 2)
#define AD_M1_IPC_05               MCU1_BASE + (0x015 << 2)
#define AD_M1_IPC_06               MCU1_BASE + (0x016 << 2)
#define AD_M1_IPC_07               MCU1_BASE + (0x017 << 2)
#define AD_M1_IPC_08               MCU1_BASE + (0x018 << 2)
#define AD_M1_IPC_09               MCU1_BASE + (0x019 << 2)
#define AD_M1_IPC_10               MCU1_BASE + (0x01A << 2)
#define AD_M1_IPC_11               MCU1_BASE + (0x01B << 2)
#define AD_M1_IPC_12               MCU1_BASE + (0x01C << 2)
#define AD_M1_IPC_13               MCU1_BASE + (0x01D << 2)
#define AD_M1_IPC_14               MCU1_BASE + (0x01E << 2)
#define AD_M1_IPC_15               MCU1_BASE + (0x01F << 2)
#define AD_M1_PERI_INTR_EN         MCU1_BASE + (0x020 << 2)
#define AD_M1_PERI_INTR_STATUS     MCU1_BASE + (0x021 << 2)
#define AD_M1_PERI_INTR_RAW_STATUS MCU1_BASE + (0x022 << 2)
#define AD_M1_CODE_INTRR_MUX       MCU1_BASE + (0x023 << 2)

#define AD_M0_IPC_INTR_EN          MCU0_BASE + (0x00C << 2)
#define AD_M0_IPC_INTR_STATUS      MCU0_BASE + (0x00D << 2)
#define AD_M0_IPC_INTR_RAW_STATUS  MCU0_BASE + (0x00E << 2)
#define AD_M0_IPC_INTR_CLR         MCU0_BASE + (0x00F << 2)
#define AD_M0_IPC_00               MCU0_BASE + (0x010 << 2)
#define AD_M0_IPC_01               MCU0_BASE + (0x011 << 2)
#define AD_M0_IPC_02               MCU0_BASE + (0x012 << 2)
#define AD_M0_IPC_03               MCU0_BASE + (0x013 << 2)
#define AD_M0_IPC_04               MCU0_BASE + (0x014 << 2)
#define AD_M0_IPC_05               MCU0_BASE + (0x015 << 2)
#define AD_M0_IPC_06               MCU0_BASE + (0x016 << 2)
#define AD_M0_IPC_07               MCU0_BASE + (0x017 << 2)
#define AD_M0_IPC_08               MCU0_BASE + (0x018 << 2)
#define AD_M0_IPC_09               MCU0_BASE + (0x019 << 2)
#define AD_M0_IPC_10               MCU0_BASE + (0x01A << 2)
#define AD_M0_IPC_11               MCU0_BASE + (0x01B << 2)
#define AD_M0_IPC_12               MCU0_BASE + (0x01C << 2)
#define AD_M0_IPC_13               MCU0_BASE + (0x01D << 2)
#define AD_M0_IPC_14               MCU0_BASE + (0x01E << 2)
#define AD_M0_IPC_15               MCU0_BASE + (0x01F << 2)

#define AD_VERSION              MCU_BASE + (0x060 << 2)
#define AD_GP_REG0              MCU_BASE + (0x061 << 2)
#define AD_GP_REG1              MCU_BASE + (0x062 << 2)
#define AD_GP_REG2              MCU_BASE + (0x063 << 2)
#define AD_GP_REG3              MCU_BASE + (0x064 << 2)
#define AD_SPI_MODE             MCU_BASE + (0x065 << 2)
#define AD_I2C_DEV_ID           MCU_BASE + (0x066 << 2)
#define AD_I2C_AHB_ADDR         MCU_BASE + (0x067 << 2)
#define AD_I2C_TIME_OUT         MCU_BASE + (0x068 << 2)
#define AD_DMA_INTR_CLR         MCU_BASE + (0x069 << 2)
#define AD_DMA_CRC_CLR          MCU_BASE + (0x06A << 2)
#define AD_DMA_CRC_HEC          MCU_BASE + (0x06B << 2)
#define AD_DMA_CRC_AAL5         MCU_BASE + (0x06C << 2)
#define AD_DDR_PRIORITY         MCU_BASE + (0x06D << 2)
#define AD_FLUSH_EN             MCU_BASE + (0x06E << 2)
#define AD_FLUSH_DONE           MCU_BASE + (0x06F << 2)
#define AD_TOP_CTRL_00          MCU_BASE + (0x100 << 2)
#define AD_TOP_CTRL_01          MCU_BASE + (0x101 << 2)
#define AD_TOP_CTRL_02          MCU_BASE + (0x102 << 2)
#define AD_TOP_CTRL_03          MCU_BASE + (0x103 << 2)
#define AD_TOP_CTRL_04          MCU_BASE + (0x104 << 2)
#define AD_TOP_CTRL_05          MCU_BASE + (0x105 << 2)
#define AD_TOP_CTRL_06          MCU_BASE + (0x106 << 2)
#define AD_TOP_CTRL_07          MCU_BASE + (0x107 << 2)
#define AD_TOP_CTRL_08          MCU_BASE + (0x108 << 2)
#define AD_TOP_CTRL_09          MCU_BASE + (0x109 << 2)
#define AD_TOP_CTRL_10          MCU_BASE + (0x10A << 2)
#define AD_TOP_CTRL_11          MCU_BASE + (0x10B << 2)
#define AD_TOP_CTRL_12          MCU_BASE + (0x10C << 2)
#define AD_TOP_CTRL_13          MCU_BASE + (0x10D << 2)

// WD
#define WD_LOAD     	 	WDT_BASE_ADDR + 0x00
#define WD_CTRL    	  		WDT_BASE_ADDR + 0x08
#define WD_INT_CLR   		WDT_BASE_ADDR + 0x0C

//GPIO
#define GPIO_DR      GPIO_BASE_ADDR + 0x000
#define GPIO_DIR     GPIO_BASE_ADDR + 0x400
#define GPIO_IS		 GPIO_BASE_ADDR + 0x404
#define GPIO_IBE	 GPIO_BASE_ADDR + 0x408
#define GPIO_IEV	 GPIO_BASE_ADDR + 0x40C
#define GPIO_IE      GPIO_BASE_ADDR + 0x410
#define GPIO_RIS     GPIO_BASE_ADDR + 0x414
#define GPIO_MIS     GPIO_BASE_ADDR + 0x418
#define GPIO_IC      GPIO_BASE_ADDR + 0x41C

//SSP0
#define SSP_CR0     SPI_BASE_ADDR + 0x00
#define SSP_CR1     SPI_BASE_ADDR + 0x04
#define SSP_DR      SPI_BASE_ADDR + 0x08
#define SSP_SR      SPI_BASE_ADDR + 0x0C
#define SSP_CPSR    SPI_BASE_ADDR + 0x10
#define SSP_IMSC    SPI_BASE_ADDR + 0x14
#define SSP_RIS     SPI_BASE_ADDR + 0x18
#define SSP_MIS     SPI_BASE_ADDR + 0x1C
#define SSP_ICR     SPI_BASE_ADDR + 0x20

#if 0
//I2C
#define I2CMSMD   		(I2C_BASE_ADDR +0x00) //Master Data Register
#define I2CMSSD   		(I2C_BASE_ADDR +0x04) //Slave  Data Register
#define I2CMSSA   		(I2C_BASE_ADDR +0x08) //Slave  Address Register
#define I2CMSCRT0 		(I2C_BASE_ADDR +0x0C) //Control Register 0
#define I2CMSCRT1 		(I2C_BASE_ADDR +0x10) //Control Register 1
#define I2CMSMS   		(I2C_BASE_ADDR +0x14) //Master Status  Register
#define I2CMSSS   		(I2C_BASE_ADDR +0x18) //Slave  Status  Register

#define I2CMSPREH 		(I2C_BASE_ADDR +0x20) //Clock Prescale Reg. High
#define I2CMSPREL 		(I2C_BASE_ADDR +0x24) //Clock Prescale Reg. Low
#else
//#define I2C_MS1_BASE_ADDR	0xF0002000
#define I2C_MS1_BASE_ADDR	0xF0006000
#define I2C_MS2_BASE_ADDR	0xF0003000
#define I2C_MS3_BASE_ADDR	0xF0004000
#endif
//Register Field: I2CMSCRT0
#define I2C_STA     0x80    //master start   condition generation bit
#define I2C_STO     0x40    //master stop    condition generation bit
#define I2C_RSTA    0x20    //master restart condition generation bit
#define I2C_NTB_1   0x00    //transact 1-Byte
#define I2C_NTB_2   0x08    //transact 2-Byte
#define I2C_NTB_3   0x10    //transact 3-Byte
#define I2C_NTB_4   0x18    //transact 4-Byte
#define I2C_IACK    0x02    //interrupt acknowledge bit
#define I2C_TGO     0x01    //transaction trigger

//Register Field: I2CMSCRT1
#define I2C_TMS_MT  0x00    //master transmitter
#define I2C_TMS_MR  0x40    //master receiver
#define I2C_TMS_SL  0xC0    //slaver transmitter/receiver
#define I2C_CEN     0x20    //core enable bit
#define I2C_IEN     0x10    //interrupt enable bit
#define I2C_CMF     0x08    //clear master fifo
#define I2C_CSF     0x04    //clear slave  fifo
#define I2C_SWR     0x02    //software reset bit

//Register Field: I2CMSMS
#define I2C_MIS     0x04    //Master Interrupt Status, 1: asserted
#define I2C_SNB     0x02    //Master has received NACK
#define I2C_MTB     0x01    //Master Transfer Busy

///////////////////////////////////////////////////////////////////////////////
// register read write interface
///////////////////////////////////////////////////////////////////////////////
// common register file interface
#if 0
UINT32 SetReg(UINT32 addr, UINT32 data);
UINT32 GetReg(UINT32 addr);
#else
#define SetReg(addr,data) (*((volatile UINT32 *)(addr))) = (data)
#define GetReg(addr) (*((volatile UINT32 *)(addr)))
#endif


#endif

