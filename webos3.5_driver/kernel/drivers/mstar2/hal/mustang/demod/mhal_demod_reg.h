
#ifndef _MHAL_DEMOD_REG_H_
#define _MHAL_DEMOD_REG_H_

#include "chip_setup.h"

//----------------------------------------------------------------------
#define _BIT0                       0x00000001
#define _BIT1                       0x00000002
#define _BIT2                       0x00000004
#define _BIT3                       0x00000008
#define _BIT4                       0x00000010
#define _BIT5                       0x00000020
#define _BIT6                       0x00000040
#define _BIT7                       0x00000080
#define _BIT8                       0x00000100
#define _BIT9                       0x00000200
#define _BIT10                      0x00000400
#define _BIT11                      0x00000800
#define _BIT12                      0x00001000
#define _BIT13                      0x00002000
#define _BIT14                      0x00004000
#define _BIT15                      0x00008000

#define _BIT31                      0x80000000
#define RIU_ARM_BASE	   (0xFD200000UL)
/*
RIU Byte address to Titania IO address
RIU address in titania is 4 byte alignment and high word address is reserved.
*/
#define BYTE2REAL(B)                (((B)>>1<<2)+((B)&0x01))
#define WORD2REAL(W)                ((W)<<2)

/* BITMASK */
#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
#define BITFLAG(loc) (1U << (loc))
#define IS_BITS_SET(val, bits)      (((val)&(bits)) == (bits))

#define MAKEWORD(value1, value2)    (((U16)(value1)) * 0x100) + (value2)


#define XBYTE(addr)             X1BYTE(addr)
#define X1BYTE(addr)            *(volatile U8*)(RIU_ARM_BASE + (addr))
#define X2BYTE(addr)            *(volatile U16*)(RIU_ARM_BASE + (addr))
/* Write/Read method invalid */
#define _MHal_R1B( u32Reg )        X1BYTE(u32Reg)
#define _MHal_R2B( u32Reg )        X2BYTE(u32Reg)

#define _MHal_W1B( u32Reg, u08Val )   (X1BYTE(u32Reg) = u08Val)
#define _MHal_W2B( u32Reg, u16Val )   (X2BYTE(u32Reg) = u16Val)

#define _MHal_W1BM( u32Reg, u08Val, u08Mask ) \
    (X1BYTE(u32Reg) = (X1BYTE(u32Reg) & ~(u08Mask)) | ((u08Val) & (u08Mask)))

#define _MHal_W1Bb( u32Reg, bBit, u08BitPos ) \
    (X1BYTE(u32Reg) = (bBit) ? (X1BYTE(u32Reg) | (u08BitPos)) : (X1BYTE(u32Reg) & ~(u08BitPos)))

#define _MHal_R1Bb( u32Reg, u08BitPos ) \
    (X1BYTE(u32Reg) & (u08BitPos))

#define _MHal_W2BM( u32Reg, u08Val, u08Mask ) \
    (X2BYTE(u32Reg) = (X2BYTE(u32Reg) & ~(u08Mask)) | ((u08Val) & (u08Mask)))

#define _MHal_W2Bb( u32Reg, bBit, u08BitPos ) \
    (X2BYTE(u32Reg) = (bBit) ? (X2BYTE(u32Reg) | (u08BitPos)) : (X2BYTE(u32Reg) & ~(u08BitPos)))

#define _MHal_R2Bb( u32Reg, u08BitPos ) \
    (X2BYTE(u32Reg) & (u08BitPos))
//----------------------------------------------------------------------


#define BK_REG_L( x, y )                        ((x) | (((y) << 1)))
#define BK_REG_H( x, y )                        (((x) | (((y) << 1))) + 1)

/* Demod */
#define MBRegBase	                            0x112600UL
#define VDMcuBase                            	0x103400UL

/* Chip Top */
#define CHIP_REG_BASE                           (0x1E00)
#define L_BK_CHIPTOP(y)                         BYTE2REAL(BK_REG_L(CHIP_REG_BASE, y))
#define H_BK_CHIPTOP(y)                         BYTE2REAL(BK_REG_H(CHIP_REG_BASE, y))

/* VD MCU Interface */
#define VD_MCU_REG_BASE                         0x3480
#define VD_MCU_REG(x)                           BYTE2REAL(VD_MCU_REG_BASE + (x) )

#define VD_MCU_RST                              BYTE2REAL(VD_MCU_REG_BASE + 0x00)
#define VD_MCU_SRAM_EN                          BYTE2REAL(VD_MCU_REG_BASE + 0x01)
#define VD_MCU_ADDR_AUTO_INC                    BYTE2REAL(VD_MCU_REG_BASE + 0x03)
#define VD_MCU_KEY                              BYTE2REAL(VD_MCU_REG_BASE + 0x03)
#define VD_MCU_ADDR_L                           BYTE2REAL(VD_MCU_REG_BASE + 0x04)
#define VD_MCU_ADDR_H                           BYTE2REAL(VD_MCU_REG_BASE + 0x05)
#define VD_MCU_SRAM_WD                          BYTE2REAL(VD_MCU_REG_BASE + 0x0C)
#define VD_MCU_SRAM_RD                          BYTE2REAL(VD_MCU_REG_BASE + 0x10)

#endif
