////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    reg_sc.h
/// @brief  Smart Card Module Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SC_H_
#define _REG_SC_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SC_REGBASE1                 0xFD205200
#define SC_REGBASE2                 0xFD205280
#define HW_REGBASE                  0xFD200000

// ------------------------------------------------------------------------------------------------
// UART / SC
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }

#define UART1_READ(addr)            READ_BYTE(SC_REGBASE1 + ((addr)<<2))
#define UART2_READ(addr)            READ_BYTE(SC_REGBASE2 + ((addr)<<2))

#define UART1_WRITE(addr, val)      WRITE_BYTE((SC_REGBASE1 + ((addr)<<2)), (val))
#define UART2_WRITE(addr, val)      WRITE_BYTE((SC_REGBASE2 + ((addr)<<2)), (val))

#define UART1_OR(addr, val)         UART1_WRITE(addr, UART1_READ(addr) | (val))
#define UART1_AND(addr, val)        UART1_WRITE(addr, UART1_READ(addr) & (val))
#define UART1_XOR(addr, val)        UART1_WRITE(addr, UART1_READ(addr) ^ (val))
#define UART2_OR(addr, val)         UART2_WRITE(addr, UART2_READ(addr) | (val))
#define UART2_AND(addr, val)        UART2_WRITE(addr, UART2_READ(addr) & (val))
#define UART2_XOR(addr, val)        UART2_WRITE(addr, UART2_READ(addr) ^ (val))


//
// UART Register List
//
#define UART_RX                     0                                   // In:  Receive buffer (DLAB=0), 16-byte FIFO
#define UART_TX                     0                                   // Out: Transmit buffer (DLAB=0), 16-byte FIFO
#define UART_DLL                    0                                   // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM                    1                                   // Out: Divisor Latch High (DLAB=1)
#define UART_IER                    1                                   // Out: Interrupt Enable Register
#define UART_IIR                    2                                   // In:  Interrupt ID Register
#define UART_FCR                    2                                   // Out: FIFO Control Register
#define UART_EFR                    2                                   // I/O: Extended Features Register
                                                                        // (DLAB=1, 16C660 only)
#define UART_LCR                    3                                   // Out: Line Control Register
#define UART_MCR                    4                                   // Out: Modem Control Register
#define UART_LSR                    5                                   // In:  Line Status Register
#define UART_MSR                    6                                   // In:  Modem Status Register
#define UART_SCR                    7                                   // I/O: Scratch Register

#define UART_SCCR                   8                                   // Smartcard Control Register
#define UART_SCSR                   9                                   // Smartcard Status Register

#define UART_SCFC                   10                                  // Smartcard Fifo Count Register
#define UART_SCFI                   11                                  // Smartcard Fifo Index Register
#define UART_SCFR                   12                                  // Smartcard Fifo Read Delay Register

#define UART_SCMR                   13                                  // Smartcard Mode Register

//
// UART_FCR(2)
// FIFO Control Register (16650 only)
//
#define UART_FCR_ENABLE_FIFO        0x01                                // Enable the FIFO
#define UART_FCR_CLEAR_RCVR         0x02                                // Clear the RCVR FIFO
#define UART_FCR_CLEAR_XMIT         0x04                                // Clear the XMIT FIFO
#define UART_FCR_DMA_SELECT         0x08                                // For DMA applications
#define UART_FCR_TRIGGER_MASK       0xC0                                // Mask for the FIFO trigger range
#define UART_FCR_TRIGGER_1          0x00                                // Mask for trigger set at 1
#define UART_FCR_TRIGGER_4          0x40                                // Mask for trigger set at 4
#define UART_FCR_TRIGGER_8          0x80                                // Mask for trigger set at 8
#define UART_FCR_TRIGGER_14         0xC0                                // Mask for trigger set at 14

//
// UART_LCR(3)
// Line Control Register
// Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
// UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
//
#define UART_LCR_WLEN5              0x00                                // Wordlength: 5 bits
#define UART_LCR_WLEN6              0x01                                // Wordlength: 6 bits
#define UART_LCR_WLEN7              0x02                                // Wordlength: 7 bits
#define UART_LCR_WLEN8              0x03                                // Wordlength: 8 bits
#define UART_LCR_STOP1              0x00                                //
#define UART_LCR_STOP2              0x04                                // Stop bits: 0=1 stop bit, 1= 2 stop bits
#define UART_LCR_PARITY             0x08                                // Parity Enable
#define UART_LCR_EPAR               0x10                                // Even parity select
#define UART_LCR_SPAR               0x20                                // Stick parity (?)
#define UART_LCR_SBC                0x40                                // Set break control
#define UART_LCR_DLAB               0x80                                // Divisor latch access bit

//
// UART_LSR(5)
// Line Status Register
//
#define UART_LSR_DR                 0x01                                // Receiver data ready
#define UART_LSR_OE                 0x02                                // Overrun error indicator
#define UART_LSR_PE                 0x04                                // Parity error indicator
#define UART_LSR_FE                 0x08                                // Frame error indicator
#define UART_LSR_BI                 0x10                                // Break interrupt indicator
#define UART_LSR_THRE               0x20                                // Transmit-hold-register empty
#define UART_LSR_TEMT               0x40                                // Transmitter empty

//
// UART_IIR(2)
// Interrupt Identification Register
//
#define UART_IIR_NO_INT             0x01                                // No interrupts pending
#define UART_IIR_ID                 0x06                                // Mask for the interrupt ID

#define UART_IIR_MSI                0x00                                // Modem status interrupt
#define UART_IIR_THRI               0x02                                // Transmitter holding register empty
#define UART_IIR_TOI                0x0c                                // Receive time out interrupt
#define UART_IIR_RDI                0x04                                // Receiver data interrupt
#define UART_IIR_RLSI               0x06                                // Receiver line status interrupt

//
// UART_IER(1)
// Interrupt Enable Register
//
#define UART_IER_RDI                0x01                                // Enable receiver data available interrupt
#define UART_IER_THRI               0x02                                // Enable Transmitter holding reg empty int
#define UART_IER_RLSI               0x04                                // Enable receiver line status interrupt
#define UART_IER_MSI                0x08                                // Enable Modem status interrupt

//
// UART_MCR(4)
// Modem Control Register
//
#define UART_MCR_DTR                0x01                                // DTR complement
#define UART_MCR_RTS                0x02                                // RTS complement
#define UART_MCR_OUT1               0x04                                // Out1 complement
#define UART_MCR_OUT2               0x08                                // Out2 complement
#define UART_MCR_LOOP               0x10                                // Enable loopback test mode

#define UART_MCR_FAST               0x20                                // Slow / Fast baud rate mode

//
// UART_MSR(6)
// Modem Status Register
//
#define UART_MSR_ANY_DELTA          0x0F                                // Any of the delta bits!
#define UART_MSR_DCTS               0x01                                // Delta CTS
#define UART_MSR_DDSR               0x02                                // Delta DSR
#define UART_MSR_TERI               0x04                                // Trailing edge ring indicator
#define UART_MSR_DDCD               0x08                                // Delta DCD
#define UART_MSR_CTS                0x10                                // Clear to Send
#define UART_MSR_DSR                0x20                                // Data Set Ready
#define UART_MSR_RI                 0x40                                // Ring Indicator
#define UART_MSR_DCD                0x80                                // Data Carrier Detect

//
// UART_EFR(2, UART_LCR_DLAB)
// These are the definitions for the Extended Features Register
// (StarTech 16C660 only, when DLAB=1)
//
#define UART_EFR_ENI                0x10                                // Enhanced Interrupt
#define UART_EFR_SCD                0x20                                // Special character detect
#define UART_EFR_RTS                0x40                                // RTS flow control
#define UART_EFR_CTS                0x80                                // CTS flow control

//
// UART_SCCR(8)
// SmartCard Control Register
//
#define UART_SCCR_MASK_CARDIN       0x01                                // Smartcard card in interrupt mask
#define UART_SCCR_MASK_CARDOUT      0x02                                // Smartcard card out interrupt mask
#define UART_SCCR_TX_BINV           0x04                                // Smartcard Tx bit invert
#define UART_SCCR_TX_BSWAP          0x08                                // Smartcard Tx bit swap
#define UART_SCCR_RST               0x10                                // Smartcard reset 0->1, UART Rx enable 1
#define UART_SCCR_RX_BINV           0x20                                // Smartcard Rx bit inverse
#define UART_SCCR_RX_BSWAP          0x40                                // Smartcard Rx bit swap

//
// UART_SCSR(9)
// Smartcard Status Register
//
#define UART_SCSR_CLK               0x01                                // Smartcard clock out
#define UART_SCSR_INT_CARDIN        0x02                                // Smartcard card in interrupt
#define UART_SCSR_INT_CARDOUT       0x04                                // Smartcard card out interrupt
#define UART_SCSR_DETECT            0x08                                // Smartcard detection status

//
// UART_SCFC(10), UART_SCFI(11), UART_SCFR(12)
// Smartcard Fifo Register
//
#define UART_SCFC_MASK              0x07
#define UART_SCFI_MASK              0x0F
#define UART_SCFR_MASK              0x07


//
// UART_SCFR(12)
// Smartcard Fifo Read Delay Register
//
#define UART_SCFR_DELAY_MASK        0x03
#define UART_SCFR_V_HIGH            0x04
#define UART_SCFR_V_ENABLE          0x08                                // Vcc = (Vcc_high ^ (Vcc_en & UART_SCSR_INT_CARDOUT))

//
// UART_SCMR(13)
// SMart Mode Register
//
#define UART_SCMR_RETRY_MASK        0x1F
#define UART_SCMR_SMARTCARD         0x20
#define UART_SCMR_PARITYCHK         0x40


// ------------------------------------------------------------------------------------------------
// TOP

#if defined(READ_WORD) && defined(WRITE_WORD)
#define HW_READ(addr)               READ_WORD(HW_REGBASE + ((addr)<<2))
#define HW_WRITE(addr, val)         WRITE_WORD((HW_REGBASE + ((addr)<<2)), (val))
#else // HAL / HWTEST
#define HW_READ(addr)               (*(volatile MS_U16*)(HW_REGBASE + ((addr)<<2)))
#define HW_WRITE(addr, val)         { (*((volatile MS_U16*)(HW_REGBASE + ((addr)<<2)))) = (MS_U16)(val); }
#endif

#define REG_TOP_SM0                 (0x0F00+0x6E)
    //#define TOP_SM0_OPEN                            BIT(0)
    //#define TOP_SM0_C48                             BIT(4)
    //#define TOP_SM0_EN                              BIT(8)

#define REG_TOP_SM1                 (0x0F00+0x0D)
    #define TOP_SM1_OPEN                            BIT(0)
    #define TOP_SM1_C48                             BIT(4)
    #define TOP_SM1_EN                              BIT(8)

#define REG_TOP_OCP                 (0x0F00+0x0E)
    #define TOP_OCP0_EN                             BIT(0)
    #define TOP_OCP1_EN                             BIT(4)

#define REG_TOP_ALLPAD_IN           (0x0F00+0x50)
    #define TOP_ALLPAD_IN_EN                        BIT(15)

#define REG_TOP_CKG_UART_SRC        (0x0580+0x13)
    #define TOP_CKG_UART_SRC_DFT                    0
    #define TOP_CKG_UART0_SRC_CLK                   BIT(0)              // DFT_LIVE / CLK_UART
    #define TOP_CKG_UART1_SRC_CLK                   BIT(1)
    #define TOP_CKG_UART2_SRC_CLK                   BIT(2)

#define REG_TOP_CKG_UART_CLK        (0x0580+0x6C)
    #define TOP_CKG_UART1_MASK                      BMASK(4:0)
    #define TOP_CKG_UART1_CLK_DIS                   BIT(0)
    #define TOP_CKG_UART1_CLK_INV                   BIT(1)
    #define TOP_CKG_UART1_CLK_MASK                  BMASK(4:2)
    #define TOP_CKG_UART1_CLK_172M                  BITS(4:2, 0)
    #define TOP_CKG_UART1_CLK_160M                  BITS(4:2, 1)
    #define TOP_CKG_UART1_CLK_144M                  BITS(4:2, 2)
    #define TOP_CKG_UART1_CLK_123M                  BITS(4:2, 3)
    #define TOP_CKG_UART1_CLK_108M                  BITS(4:2, 4)
    //#define TOP_CKG_UART1_CLK_PLL                   BITS(4:2, 5)
    //#define TOP_CKG_UART1_CLK_PLL_D2                BITS(4:2, 6)

    #define TOP_CKG_UART2_MASK                      BMASK(12:8)
    #define TOP_CKG_UART2_CLK_DIS                   BIT(8)
    #define TOP_CKG_UART2_CLK_INV                   BIT(9)
    #define TOP_CKG_UART2_CLK_MASK                  BMASK(12:10)
    #define TOP_CKG_UART2_CLK_172M                  BITS(12:10, 0)
    #define TOP_CKG_UART2_CLK_160M                  BITS(12:10, 1)
    #define TOP_CKG_UART2_CLK_144M                  BITS(12:10, 2)
    #define TOP_CKG_UART2_CLK_123M                  BITS(12:10, 3)
    #define TOP_CKG_UART2_CLK_108M                  BITS(12:10, 4)
    #define TOP_CKG_UART2_CLK_PLL                   BITS(12:10, 5)
    #define TOP_CKG_UART2_CLK_PLL_D2                BITS(12:10, 6)

#define REG_TOP_CKG_SM_CA           (0x580+0x6C)
    #define TOP_CKG_SM_CA0_MASK                     BMASK(11:8)
    #define TOP_CKG_SM_CA0_DIS                      BIT(8)
    #define TOP_CKG_SM_CA0_INV                      BIT(9)
    #define TOP_CKG_SM_CA0_CLK_MASK                 BMASK(11:10)
    #define TOP_CKG_SM_CA0_CLK_27M_D6               BITS(11:10, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D2               BITS(11:10, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D4               BITS(11:10, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA0_CLK_27M_D8               BITS(11:10, 3)        // 3.375MHz
    #define TOP_CKG_SM_CA1_MASK                     BMASK(7:4)
    #define TOP_CKG_SM_CA1_DIS                      BIT(4)
    #define TOP_CKG_SM_CA1_INV                      BIT1(5)
    #define TOP_CKG_SM_CA1_CLK_MASK                 BMASK(7:6)
    #define TOP_CKG_SM_CA1_CLK_27M_D6               BITS(7:6, 0)        // 4.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D2               BITS(7:6, 1)        // 13.5MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D4               BITS(7:6, 2)        // 6.75MHz
    #define TOP_CKG_SM_CA1_CLK_27M_D8               BITS(7:6, 3)        // 3.375MHz


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define DLC_FUNCTION  1

#if DLC_FUNCTION
#define BYTE MS_U8
#define WORD MS_U16
#define DWORD MS_U32
#define XDATA
#define extern
#define printf printk
#define DLC_DEBUG(x)        //(x)

#define REG_ADDR_DLC_HANDSHAKE                 L_BK_DLC(0x04)
#define REG_ADDR_DLC_HISTOGRAM_MODE            H_BK_DLC(0x04)
#define REG_ADDR_HISTOGRAM_TOTAL_SUM_L         L_BK_DLC(0x06)
#define REG_ADDR_HISTOGRAM_TOTAL_SUM_H         H_BK_DLC(0x06)
#define REG_ADDR_HISTOGRAM_TOTAL_COUNT_L       L_BK_DLC(0x07)
#define REG_ADDR_HISTOGRAM_TOTAL_COUNT_H       H_BK_DLC(0x07)
#define REG_ADDR_HISTOGRAM_RANGE_ENABLE        L_BK_DLC(0x08)
#define REG_ADDR_BLE_UPPER_BOND                L_BK_DLC(0x09)
#define REG_ADDR_BLE_LOWER_BOND                H_BK_DLC(0x09)
#define REG_ADDR_WLE_UPPER_BOND                L_BK_DLC(0x0A)
#define REG_ADDR_WLE_LOWER_BOND                H_BK_DLC(0x0A)
#define REG_ADDR_MAIN_MAX_VALUE                L_BK_DLC(0x0B)
#define REG_ADDR_MAIN_MIN_VALUE                H_BK_DLC(0x0B)
#define REG_ADDR_SUB_MAX_VALUE                 L_BK_DLC(0x0C)
#define REG_ADDR_SUB_MIN_VALUE                 H_BK_DLC(0x0C)
#define REG_ADDR_MAIN_BLACK_SLOP_LSB_L         L_BK_DLC(0x0D)
#define REG_ADDR_MAIN_BLACK_SLOP_L             L_BK_DLC(0x10)
#define REG_ADDR_MAIN_BLACK_SLOP_H             H_BK_DLC(0x10)
#define REG_ADDR_DLC_MAIN_Y_GAIN               L_BK_DLC(0x14)
#define REG_ADDR_DLC_MAIN_C_GAIN               H_BK_DLC(0x14)
#define REG_ADDR_DLC_SUB_Y_GAIN                L_BK_DLC(0x15)
#define REG_ADDR_DLC_SUB_C_GAIN                H_BK_DLC(0x15)
#define REG_ADDR_HISTOGRAM_DATA_8              L_BK_DLC(0x28)
#define REG_ADDR_DLC_DATA_START_MAIN           L_BK_DLC(0x30)
#define REG_ADDR_DLC_DATA_START_SUB            L_BK_DLC(0x38)
#define REG_ADDR_HISTOGRAM_DATA_32             L_BK_DLC(0x40)
#define REG_ADDR_HISTOGRAM_SUB_TOTAL_COUNT_L   L_BK_DLC(0x72)
#define REG_ADDR_HISTOGRAM_SUB_TOTAL_COUNT_H   H_BK_DLC(0x72)
#define REG_ADDR_HISTOGRAM_SUB_TOTAL_SUM_L     L_BK_DLC(0x73)
#define REG_ADDR_HISTOGRAM_SUB_TOTAL_SUM_H     H_BK_DLC(0x73)
#define REG_ADDR_DLC_DATA_EXTEND_N0_MAIN       L_BK_DLC(0x76)
#define REG_ADDR_DLC_DATA_EXTEND_16_MAIN       L_BK_DLC(0x77)
#define REG_ADDR_DLC_DATA_LSB_START_MAIN       L_BK_DLC(0x78)
#define REG_ADDR_DLC_DATA_LSB_START_SUB        L_BK_DLC(0x7B)
#define REG_ADDR_DLC_DATA_EXTEND_N0_SUB        L_BK_DLC(0x7E)
#define REG_ADDR_DLC_DATA_EXTEND_16_SUB        L_BK_DLC(0x7F)

#define REG_ADDR_VOP_SCREEN_CONTROL           L_BK_VOP(0x19)

#define REG_ADDR_OP_SW_SUB_ENABLE               L_BK_OP(0x10)

#define PRJ_MCNR

#define REG_LPLL_11_L                   (0x103100 | 0x11<< 1)
#define REG_LPLL_13_L                   (0x103100 | 0x13<< 1)

#define REG_SC_BK12_01_L                   (0x131200 | 0x01<< 1)

#define REG_SC_BK0A_02_L                   (0x130A00 | 0x02<< 1)
#define REG_SC_BK0A_05_L                   (0x130A00 | 0x05<< 1)
#define REG_SC_BK0A_06_L                   (0x130A00 | 0x06<< 1)
#define REG_SC_BK0A_07_L                   (0x130A00 | 0x07<< 1)
#define REG_SC_BK0A_0A_L                   (0x130A00 | 0x0A<< 1)
#define REG_SC_BK0A_0B_L                   (0x130A00 | 0x0B<< 1)
#define REG_SC_BK0A_0C_L                   (0x130A00 | 0x0C<< 1)
#define REG_SC_BK0A_0D_L                   (0x130A00 | 0x0D<< 1)
#define REG_SC_BK0A_0F_L                   (0x130A00 | 0x0F<< 1)

#define REG_SC_BK0A_10_L                   (0x130A00 | 0x10<< 1)
#define REG_SC_BK0A_15_L                   (0x130A00 | 0x15<< 1)
#define REG_SC_BK0A_16_L                   (0x130A00 | 0x16<< 1)
#define REG_SC_BK0A_17_L                   (0x130A00 | 0x17<< 1)
#define REG_SC_BK0A_1E_L                   (0x130A00 | 0x1E<< 1)
#define REG_SC_BK0A_1F_L                   (0x130A00 | 0x1F<< 1)

#define REG_SC_BK0A_20_L                   (0x130A00 | 0x20<< 1)
#define REG_SC_BK0A_21_L                   (0x130A00 | 0x21<< 1)
#define REG_SC_BK0A_23_L                   (0x130A00 | 0x23<< 1)
#define REG_SC_BK0A_24_L                   (0x130A00 | 0x24<< 1)
#define REG_SC_BK0A_25_L                   (0x130A00 | 0x25<< 1)
#define REG_SC_BK0A_41_L                   (0x130A00 | 0x41<< 1)
#define REG_SC_BK0A_4A_L                   (0x130A00 | 0x4A<< 1)
#define REG_SC_BK0A_4B_L                   (0x130A00 | 0x4B<< 1)

#define REG_SC_BK0A_57_L                   (0x130A00 | 0x57<< 1)
#define REG_SC_BK0A_5C_L                   (0x130A00 | 0x5C<< 1)
#define REG_SC_BK0A_5E_L                   (0x130A00 | 0x5E<< 1)

#define REG_SC_BK12_03_L                   (0x131200 | 0x03<< 1)
#define REG_SC_BK12_0F_L                   (0x131200 | 0x0F<< 1)
#define REG_SC_BK12_30_L                   (0x131200 | 0x30<< 1)
#define REG_SC_BK12_33_L                   (0x131200 | 0x33<< 1)
#define REG_SC_BK12_3A_L                   (0x131200 | 0x3A<< 1)

#define REG_SC_BK17_61_L                   (0x131700 | 0x61<< 1)
#define REG_SC_BK17_62_L                   (0x131700 | 0x62<< 1)
#define REG_SC_BK17_63_L                   (0x131700 | 0x63<< 1)
#define REG_SC_BK17_64_L                   (0x131700 | 0x64<< 1)
#define REG_SC_BK17_65_L                   (0x131700 | 0x65<< 1)
#define REG_SC_BK17_66_L                   (0x131700 | 0x66<< 1)
#define REG_SC_BK17_67_L                   (0x131700 | 0x67<< 1)


#define REG_SC_BK22_2A_L                   (0x132200 | 0x2A<< 1)
#define REG_SC_BK22_7A_L                   (0x132200 | 0x7A<< 1)
#define REG_SC_BK22_7C_L                   (0x132200 | 0x7C<< 1)

#define REG_SC_BK2A_02_L                   (0x132A00 | 0x02<< 1)
#define REG_SC_BK2A_29_L                   (0x132A00 | 0x29<< 1)
#define REG_SC_BK2A_2A_L                   (0x132A00 | 0x2A<< 1)
#define REG_SC_BK2A_2F_L                   (0x132A00 | 0x2F<< 1)
#define REG_SC_BK2A_7C_L                   (0x132A00 | 0x7C<< 1)
#define REG_SC_BK2A_7D_L                   (0x132A00 | 0x7D<< 1)
#define REG_SC_BK2A_7E_L                   (0x132A00 | 0x7E<< 1)

#define REG_SC_BK30_0C_L                   (0x133000 | 0x0C<< 1)
#define REG_SC_BK30_0D_L                   (0x133000 | 0x0D<< 1)
#define REG_SC_BK30_0E_L                   (0x133000 | 0x0E<< 1)
#define REG_SC_BK30_38_L                   (0x133000 | 0x38<< 1)
#define REG_SC_BK30_39_L                   (0x133000 | 0x39<< 1)
#define REG_SC_BK30_3A_L                   (0x133000 | 0x3A<< 1)

#define REG_SC_BK62_06_L                    (0x136200 | 0x06<< 1)
#define REG_SC_BK62_0E_L                    (0x136200 | 0x0E<< 1)
#define REG_SC_BK62_08_L                    (0x136200 | 0x08<< 1)
#define REG_SC_BK63_61_L                    (0x136300 | 0x61<< 1)

#define _BIT0			(0x0001)
#define _BIT1			(0x0002)
#define _BIT2			(0x0004)
#define _BIT3			(0x0008)
#define _BIT4			(0x0010)
#define _BIT5			(0x0020)
#define _BIT6			(0x0040)
#define _BIT7			(0x0080)

#define L_BK_VOP(_x_)                 (REG_SCALER_BASE | (REG_SC_BK_VOP << 8) | (_x_ << 1))
#define L_BK_DLC(_x_)                 (REG_SCALER_BASE | (REG_SC_BK_DLC << 8) | (_x_ << 1))
#define H_BK_DLC(_x_)                 (REG_SCALER_BASE | (REG_SC_BK_DLC << 8) | ((_x_ << 1)+1))
#define L_BK_OP(_x_)                            (REG_SCALER_BASE | (REG_SC_BK_OP << 8) | (_x_ << 1))

#define REG_SCALER_BASE             0x130000
#define REG_SC_BK_VOP               0x10
#define REG_SC_BK_DLC               0x1A
#define REG_SC_BK_OP                            0x20

#define REG_ADDR(addr)              (*((volatile unsigned short int*)(0xFD000000 + (addr << 1))))

// read 2 byte
#define REG_RR(_reg_)                ({ REG_ADDR(_reg_);})

// write low byte
#define REG_WL(_reg_, _val_)    \
        do{ REG_ADDR(_reg_) = (REG_ADDR(_reg_) & 0xFF00) | ((_val_) & 0x00FF); }while(0)

// write high byte
#define REG_WH(_reg_, _val_)    \
        do{ REG_ADDR(_reg_) = (REG_ADDR(_reg_)  & 0x00FF) | ((_val_) << 8); }while(0)

 // write 2 byte
#define REG_W2B(_reg_, _val_)    \
        do{ REG_ADDR(_reg_) =(_val_) ; }while(0)

#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    /* not used and incomplete typedef:
       unnamed struct/union that defines no instances */
#else
		typedef struct
		{
			// Default luma curve
			BYTE ucLumaCurve[16];
			BYTE ucLumaCurve2_a[16];
			BYTE ucLumaCurve2_b[16];

			BYTE ucDlcPureImageMode; // Compare difference of max and min bright
			BYTE ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
			BYTE ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
			BYTE ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve
			BYTE ucDlcFastAlphaBlending; // min 17 ~ max 32
			BYTE ucDlcSlowEvent; // some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
			BYTE ucDlcTimeOut; // for IsrApp.c
			BYTE ucDlcFlickAlphaStart; // for force to do fast DLC in a while
			BYTE ucDlcYAvgThresholdH; // default value: 128
			BYTE ucDlcYAvgThresholdL; // default value: 0
			BYTE ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
			BYTE ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
			BYTE bCGCCGainCtrl : 1; // 1: enable; 0: disable
			BYTE bEnableBLE : 1; // 1: enable; 0: disable
			BYTE bEnableWLE : 1; // 1: enable; 0: disable
			BYTE ucDlcYAvgThresholdM;
			BYTE ucDlcCurveMode;
			BYTE ucDlcCurveModeMixAlpha;
			BYTE ucDlcAlgorithmMode;
			BYTE ucDlcHistogramLimitCurve[17];
			BYTE ucDlcSepPointH;
			BYTE ucDlcSepPointL;
			WORD uwDlcBleStartPointTH;
			WORD uwDlcBleEndPointTH;
			BYTE ucDlcCurveDiff_L_TH;
			BYTE ucDlcCurveDiff_H_TH;
			WORD uwDlcBLESlopPoint_1;
			WORD uwDlcBLESlopPoint_2;
			WORD uwDlcBLESlopPoint_3;
			WORD uwDlcBLESlopPoint_4;
			WORD uwDlcBLESlopPoint_5;
			WORD uwDlcDark_BLE_Slop_Min;
			BYTE ucDlcCurveDiffCoringTH;
			BYTE ucDlcAlphaBlendingMin;
			BYTE ucDlcAlphaBlendingMax;
			BYTE ucDlcFlicker_alpha;
			BYTE ucDlcYAVG_L_TH;
			BYTE ucDlcYAVG_H_TH;
			BYTE ucDlcDiffBase_L;
			BYTE ucDlcDiffBase_M;
			BYTE ucDlcDiffBase_H;
		};
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */

#endif // _REG_SC_H_
#endif
