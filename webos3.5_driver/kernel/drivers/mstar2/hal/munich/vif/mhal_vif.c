////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
//
// You can redistribute it and/or modify it under the terms of the GNU General Public
// License version 2 as published by the Free Foundation. This program is distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MHAL_VIF_C_
#define _MHAL_VIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <asm/io.h>

#include "mhal_vif.h"
#include "mhal_vif_reg.h"
#include "halVIF_Customer.h"


//-------------------------------------------------------------------------------------------------
//  Variables
//-------------------------------------------------------------------------------------------------

BYTE g_ucVifStatusStep;
BYTE g_ucVifSoundSystemType;
BOOL g_bCheckModulationType;  // 0: negative; 1: positive
BOOL g_bCheckIFFreq;          // 0: 38.9 MHz (PAL/SECAM L); 1: 33.9 MHz (SECAM L')
DWORD g_VifCrLfFfMaxDiff;
DWORD g_VifCrLfFfMaxInstDiff;
BOOL g_bCrSeriousDrift;
BYTE g_VifCrKp;
BYTE g_VifCrKi;
FrequencyBand g_FreqBand;
IfFrequencyType g_FreqType;
BOOL g_VifZeroDetFlag;
WORD g_VifCrKpKiAdjLoopCnt;
BYTE g_VifKpKiCnt1;
BYTE g_VifKpKiCnt2;
BYTE g_VifCrKpKiGearCnt1;
BYTE g_VifCrKpKiGearCnt2;
BOOL g_VifHWKpKiFlag;
BOOL g_VifChannelScanFlag;
BYTE g_VifVDStableFlag;
BYTE g_VifVideoStandard;


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define HALVIFDBG(x)
#define HALVIFDBG_BIT       (DBB1_REG_BASE+0x06)  // Bit 4~7
#define HALVIFDBG1_BIT       (DBB1_REG_BASE+0x04)  // Bit 1
#define HALVIFDBG2_BIT       (DBB1_REG_BASE+0xF6)  // Bit 0/4
//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
#define HAL_VIF_Delay1ms(x)                mdelay(x)
#define HAL_VIF_Delay1us(x)                HAL_VIF_Delay1ms(1)//MAsm_CPU_DelayUs(x) // no us delay support now.
#define HAL_VIF_GetSystemTime()            ((unsigned long)((jiffies)*(1000/HZ)))


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define __CHIP_VERSION 0x1ECF

#ifndef _END_OF_TBL_
#define _END_OF_TBL_        0xFFFF
#endif

#define msRead2Bytes(x)                 RIU_Read2Byte(x)
#define msReadByte(x)                   RIU_ReadByte(x)


// Base address should be initial.
#if defined (__aeon__)            // Non-OS
    #define BASEADDR_RIU 0xA0000000UL
#else                                       // ecos
    #define BASEADDR_RIU 0xBF800000UL
#endif

#define RIU_MACRO_START     do {
#define RIU_MACRO_END       } while (0)


/*
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
*/

#define READ_BYTE(_reg)             (*(volatile U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile U8*)(_reg))) = (U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile U16*)(_reg))) = (U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile U32*)(_reg))) = (U32)(_val); }


// Address bus of RIU is 16 bits.
#define RIU_READ_BYTE(addr)         ( READ_BYTE( _hal_VIF.u32VIFBaseAddr + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( _hal_VIF.u32VIFBaseAddr + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _hal_VIF.u32VIFBaseAddr + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _hal_VIF.u32VIFBaseAddr + (addr), val) }

// Standard Form

#define RIU_ReadByte( u32Reg )   RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define RIU_Read2Byte( u32Reg )    (RIU_READ_2BYTE((u32Reg)<<1))

#define RIU_ReadRegBit( u32Reg, u8Mask )   (RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define RIU_WriteByte( u32Reg, u8Val )                                                  \
    RIU_MACRO_START                                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);            \
    RIU_MACRO_END

#define RIU_Write2Byte( u32Reg, u16Val )                                               \
    RIU_MACRO_START                                                                                     \
    if ( ((u32Reg) & 0x01) )                                                                            \
    {                                                                                                                \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (U8)((u16Val)));                                     \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    RIU_MACRO_END

#define RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    DWORD u32VIFBaseAddr;
    DWORD bBaseAddrInitialized;
} hal_VIF_t;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_VIF_t _hal_VIF ={BASEADDR_RIU, 0};
extern VIFInitialIn VIFInitialIn_inst; //luke_vif
//VIFInitialIn VIFInitialIn_inst;
BYTE g_VifShiftClk = 0;

//-------------------------------------------------------------------------------------------------
//  Local code data
//-------------------------------------------------------------------------------------------------
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_1dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x08},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x70},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_2dB[]=
{
    {SOS21_C0_L,0xa4},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x8e},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x12},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x5c},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x60},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_3dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x1c},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x5d},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_4dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x28},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x51},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_3dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x07},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xd5},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_4dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x0a},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xd2},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_5dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x0e},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xcf},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_PEAKING_NULL[]=
{
    {SOS21_C0_L,0x00},  // SOS21
    {SOS21_C0_H,0x00},
    {SOS21_C1_L,0x00},
    {SOS21_C1_H,0x00},
    {SOS21_C2_L,0x00},
    {SOS21_C2_H,0x00},
    {SOS21_C3_L,0x00},
    {SOS21_C3_H,0x00},
    {SOS21_C4_L,0x00},
    {SOS21_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_YCDelay_VSB[]=
{
    {SOS22_C0_L,0x15},  // SOS22 Y/C delay
    {SOS22_C0_H,0x02},
    {SOS22_C1_L,0x84},
    {SOS22_C1_H,0x06},
    {SOS22_C2_L,0x7c},
    {SOS22_C2_H,0x01},
    {SOS22_C3_L,0xeb},
    {SOS22_C3_H,0x05},
    {SOS22_C4_L,0x00},
    {SOS22_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_YCDelay_NULL[]=
{
    {SOS22_C0_L,0x00},  // SOS22
    {SOS22_C0_H,0x00},
    {SOS22_C1_L,0x00},
    {SOS22_C1_H,0x00},
    {SOS22_C2_L,0x00},
    {SOS22_C2_H,0x00},
    {SOS22_C3_L,0x00},
    {SOS22_C3_H,0x00},
    {SOS22_C4_L,0x00},
    {SOS22_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_Low_R[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xb4},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf8},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x08},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x4c},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_Low_L[]=
{
    {SOS31_C0_L,0x3c},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0xb8},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x48},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0xc4},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xd9},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf7},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x0a},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x28},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_High_R[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xc7},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xd8},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x28},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x39},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_High_L[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xb0},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0x13},
    {SOS32_C1_H,0x07},
    {SOS32_C2_L,0xed},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x50},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_VSB_LG[]=
{
    {SOS31_C0_L,0xab},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0x9b},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x65},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0x55},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xe1},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf7},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x0a},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x1f},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_VSB_Philips[]=
{
    {SOS31_C0_L,0x9f},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0xa8},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x58},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0x62},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xcd},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0x05},
    {SOS32_C1_H,0x07},
    {SOS32_C2_L,0xfb},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x33},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_GDE_GroupDelay_NULL[]=
{
    {SOS31_C0_L,0x00},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x00},
    {SOS31_C1_H,0x00},
    {SOS31_C2_L,0x00},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x00},
    {SOS31_C3_H,0x00},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0x00},  // SOS32
    {SOS32_C0_H,0x00},
    {SOS32_C1_L,0x00},
    {SOS32_C1_H,0x00},
    {SOS32_C2_L,0x00},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x00},
    {SOS32_C3_H,0x00},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_B_A2[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};
const MS_VIF_REG_TYPE VIF_PAL_B_NICAM[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};
const MS_VIF_REG_TYPE VIF_PAL_GH_A2[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};
const MS_VIF_REG_TYPE VIF_PAL_GH_NICAM[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};
const MS_VIF_REG_TYPE VIF_SECAM_L_NICAM[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // SECAM L NICAM
};

const MS_VIF_REG_TYPE VIF_PAL_I_NICAM[]=
{
    {N_A1_C0_L,0x6b}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x6e},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x2e}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xaf},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x6b}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x6e},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL I NICAM
};
const MS_VIF_REG_TYPE VIF_SECAM_DK1_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x4f}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x8c},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK1 A2
};
const MS_VIF_REG_TYPE VIF_PAL_DK2_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x18}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xc6},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.96
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK2 A2
};

const MS_VIF_REG_TYPE VIF_PAL_DK2_NICAM[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK NICAM
};
const MS_VIF_REG_TYPE VIF_SECAM_DK3_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK3 A2
};

const MS_VIF_REG_TYPE VIF_GDE_BG_A2_NOTCH[]=
{
    {N_A3_C0_L,0x9f}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x37},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x86}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x51},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0xad}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0x37},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};

const MS_VIF_REG_TYPE VIF_GDE_SECAM_BG_NOTCH[]=
{
    {N_A3_C0_L,0xAD}, // Notch_A3, R = 0.96
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x28},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x37},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x94}, // Notch_A4, R = 0.96
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x28},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x51},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0xad}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0x37},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // SECAM B/G
};

const MS_VIF_REG_TYPE VIF_GDE_SECAM_DK_NOTCH[]=
{
    {N_A3_C0_L,0x40}, // Notch_A3, R = 0.96
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x28},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x23}, // Notch_A4, R = 0.96
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x28},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xc6},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // SECAM DK
};

const MS_VIF_REG_TYPE VIF_GDE_BG_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x9f}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x37},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0xad}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0x37},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};

const MS_VIF_REG_TYPE VIF_GDE_I_NOTCH[]=
{
    {N_A3_C0_L,0x6b}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x6e},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x2e}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xaf},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x78}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0x6e},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL I NICAM
};

const MS_VIF_REG_TYPE VIF_GDE_DK1_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x4f}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x8c},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL DK1 A2
};

const MS_VIF_REG_TYPE VIF_GDE_DK2_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x18}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xc6},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL DK2 A2
};

const MS_VIF_REG_TYPE VIF_GDE_DK3_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x86}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x51},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL DK3 A2
};

const MS_VIF_REG_TYPE VIF_GDE_DK_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // PAL DK NICAM
};

const MS_VIF_REG_TYPE VIF_GDE_L_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x40}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x02},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xa9},
    {N_A5_C2_H,0x05},
    {_END_OF_TBL_,0x00}, // SECAM L NICAM
};

const MS_VIF_REG_TYPE VIF_GDE_MN_NOTCH[]=
{
    {N_A3_C0_L,0xfc}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xd4},
    {N_A3_C2_H,0x04},
    {N_A4_C0_L,0xe8}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xe8},
    {N_A4_C2_H,0x04},
    {N_A5_C0_L,0x0c}, // Notch_A5, R = 0.96
    {N_A5_C0_H,0x03},
    {N_A5_C1_L,0x28},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xd4},
    {N_A5_C2_H,0x04},
    {_END_OF_TBL_,0x00}, // NTSC M/N
};
const MS_VIF_REG_TYPE VIF_NTSC_MN_A2[]=
{
    {N_A1_C0_L,0xfc}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xd4},
    {N_A1_C2_H,0x04},
    {N_A2_C0_L,0xe8}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xe8},
    {N_A2_C2_H,0x04},
    {SOS12_C0_L,0xfc}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xd4},
    {SOS12_C3_H,0x04},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // NTSC M/N
};
const MS_VIF_REG_TYPE VIF_PAL_DK_LOWER_ACI[]=
{
    {SOS11_C0_L,0x3a},  // SOS11 notch at 16.5MHz (0.94)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xf3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_I_LOWER_ACI[]=
{
    {SOS11_C0_L,0x0d},  // SOS11 notch at 17MHz (0.94)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0x23},
    {SOS11_C3_H,0x03},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_GH_LOWER_ACI[]=
{
    {SOS11_C0_L,0xe4},  // SOS11 notch at 17.5MHz (0.94)
    {SOS11_C0_H,0x04},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0x4f},
    {SOS11_C3_H,0x03},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_B_LOWER_ACI[]=
{
    {SOS11_C0_L,0x1C},  // SOS11 notch at 16.5MHz (0.98)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x14},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xF3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_NTSC_MN_LOWER_ACI[]=
{
    {SOS11_C0_L,0x15},  // SOS11 notch at 16.5MHz
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x0A},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xF3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_DK_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0x0D},  // SOS12 notch at 8.5MHz
    {SOS12_C0_H,0x01},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xB0},
    {SOS12_C3_H,0x06},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_GH_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0x9A},  // SOS12 notch at 9.5MHz
    {SOS12_C0_H,0x00},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x40},
    {SOS12_C3_H,0x07},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
const MS_VIF_REG_TYPE VIF_PAL_I_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0xD4},  // SOS12 notch at 9MHz
    {SOS12_C0_H,0x00},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xF7},
    {SOS12_C3_H,0x06},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};

const MS_VIF_REG_TYPE VIF_NULL_LOWER_ACI[]=
{
    {SOS11_C0_L,0x00},  // SOS11
    {SOS11_C0_H,0x00},
    {SOS11_C1_L,0x00},
    {SOS11_C1_H,0x00},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x00},
    {SOS11_C3_L,0x00},
    {SOS11_C3_H,0x00},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};

const MS_VIF_REG_TYPE VIF_CR_IIR_LPF1[]=
{
/*
    {CR_IIR_COEF_G,0x02},
    {CR_IIR_COEF_G+1,0x00},
    {CR_IIR_COEF_A1,0xfc},
    {CR_IIR_COEF_A1+1,0x01},
    {CR_IIR_COEF_A2,0x00},
    {CR_IIR_COEF_A2+1,0x00},
    {CR_IIR_COEF_B1,0x00},
    {CR_IIR_COEF_B1+1,0x02},
    {CR_IIR_COEF_B2,0x00},
    {CR_IIR_COEF_B2+1,0x00},
    {_END_OF_TBL_,0x00},
*/

    {CR_IIR_COEF_G,0x10},
    {CR_IIR_COEF_G+1,0x00},
    {CR_IIR_COEF_A1,0xc1},
    {CR_IIR_COEF_A1+1,0x03},
    {CR_IIR_COEF_A2,0x3b},
    {CR_IIR_COEF_A2+1,0x06},
    {CR_IIR_COEF_B1,0x78},
    {CR_IIR_COEF_B1+1,0x04},
    {CR_IIR_COEF_B2,0x00},
    {CR_IIR_COEF_B2+1,0x02},
    {_END_OF_TBL_,0x00},

};
const MS_VIF_REG_TYPE VIF_CR_IIR_LPF2[]=
{
    {CR_IIR_COEF_G,0x02},
    {CR_IIR_COEF_G+1,0x00},
    {CR_IIR_COEF_A1,0xd6},
    {CR_IIR_COEF_A1+1,0x03},
    {CR_IIR_COEF_A2,0x27},
    {CR_IIR_COEF_A2+1,0x06},
    {CR_IIR_COEF_B1,0x5b},
    {CR_IIR_COEF_B1+1,0x06},
    {CR_IIR_COEF_B2,0x00},
    {CR_IIR_COEF_B2+1,0x02},
    {_END_OF_TBL_,0x00},
};

U16 VIF_PAL_EQ_CO_A_REJ[]=
{
		0x0009,
		0x0003,
		0x000B,
		0x1FE1,
		0x002F,
		0x1FD0,
		0x001F,
		0x1FF2,
		0x001A,
		0x1FB4,
		0x008A,
		0x1F5E,
		0x0077,
		0x1FDC,
		0x1FFC,
		0x1FBB,
		0x00F1,
		0x1E79,
		0x0165,
		0x1FC9,
		0x1E4C,
		0x037C,
		0x0BCA,
		0x037C,
		0x1E4C,
		0x1FC9,
		0x0165,
		0x1E79,
		0x00F1,
		0x1FBB,
		0x1FFC,
		0x1FDC,
		0x0077,
		0x1F5E,
		0x008A,
		0x1FB4,
		0x001A,
		0x1FF2,
		0x001F,
		0x1FD0,
		0x002F,
		0x1FE1,
		0x000B,
		0x0003,
		0x0009,
		0x0000
};
U16 VIF_NTSC_EQ_CO_A_REJ[]=
{
            0x001B,
            0x1FEB,
            0x0001,
            0x0000,
            0x0024,
            0x1FBF,
            0x0027,
            0x1FFE,
            0x0025,
            0x1F87,
            0x0083,
            0x1FD6,
            0x0009,
            0x1F78,
            0x0106,
            0x1F5A,
            0x1FD9,
            0x1FEF,
            0x017E,
            0x1DF8,
            0x1FB4,
            0x044E,
            0x09AF,
            0x044E,
            0x1FB4,
            0x1DF8,
            0x017E,
            0x1FEF,
            0x1FD9,
            0x1F5A,
            0x0106,
            0x1F78,
            0x0009,
            0x1FD6,
            0x0083,
            0x1F87,
            0x0025,
            0x1FFE,
            0x0027,
            0x1FBF,
            0x0024,
            0x0000,
            0x0001,
            0x1FEB,
            0x001B,
            0x0000
};
//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------
void msWriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    RIU_WriteByteMask(u32Reg, u8Val, u8Mask);
    msVifLoad();
}

void msWriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    RIU_WriteRegBit(u32Reg, bEnable, u8Mask);
    msVifLoad();
}

void msVifWriteByte(U32 u32Reg, U8 u8Val )
{
    RIU_WriteByte(u32Reg,u8Val);
    msVifLoad();
}

void MHAL_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    msWriteByteMask(u32Reg, u8Val, u8Mask);
}

void MHAL_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    msWriteBit(u32Reg, bEnable, u8Mask);
}

void MHAL_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    msVifWriteByte(u32Reg, u8Val);
}

U8 MHAL_VIF_ReadByte(U32 u32Reg )
{
    return msReadByte(u32Reg);
}

void msWriteRegsTbl(MS_VIF_REG_TYPE *pRegTable)
{
    U16 u16Dummy;
    U32 u32Address;
    U8  u8Value;

    u16Dummy = 2000;

    do
    {
        u32Address = pRegTable->u32Address;
        u8Value = pRegTable->u8Value;
        if (u32Address == 0xFFFF)       // check end of table
            break;
        RIU_WriteByte(u32Address, u8Value);
        pRegTable++;
    } while (--u16Dummy > 0);
    msVifLoad();
}



void MHAL_VIF_RegInit (void)
{
    //DWORD u32NonPMBank, u32NonPMBankSize;

    //printk("\r\nMHAL_VIF_RegInit()");

#if 0  //to be check.

    HALVIFDBG(printk("\r\nMHAL_VIF_RegInit()"));
    if (!MDrv_MMIO_GetBASE( &u32NonPMBank, &u32NonPMBankSize, MS_MODULE_VIF))
    {
        printk("\r\nIOMap failure to get MAP_NONPM_BANK");
        u32NonPMBank = BASEADDR_RIU; // TODO what to do if failed??
    }
    else
    {
        HALVIFDBG(printk("\r\nMS_MODULE_VIF base = 0x%lX, length = %lu", u32NonPMBank, u32NonPMBankSize));
    }
#endif

    _hal_VIF.u32VIFBaseAddr=NONPM_RIU_BASEADDR;//u32NonPMBank; //TODO: To check it!!!!!! 20110127
    _hal_VIF.bBaseAddrInitialized = 1;
}

void MHAL_VIF_SetClock(BOOL bEnable)
{
    HALVIFDBG(printk("\r\nMHAL_VIF_SetClock=%d",bEnable));

    //bEnable = (bEnable) ? 0:1; // 0 means enable
}

void msVifAdcInitial(void)
{
    HALVIFDBG(printk("\r\nmsVifAdcInitial()"));

    if (!_hal_VIF.bBaseAddrInitialized)
    {
     printk("!!! [VIF]nmsVifAdcInitial returned !!!()");
     return ;
    }
    RIU_WriteByteMask(0x1E39L, 0x00, 0x03);  // DMDTOP/DMDANA_controlled by HK_MCU (0) or DMD_MCU (1)

    RIU_WriteByte(0x11F0AL, 0x00);   // ADC_CLK
    RIU_WriteByte(0x11F0BL, 0x00);
    RIU_WriteByte(0x3314L, 0x04);     // ADC clock
    RIU_WriteByte(0x3315L, 0x00);     // CLK_DVBTC_INNC clock
    RIU_WriteByte(0x11F18L, 0x01);   // SRAM
    RIU_WriteByte(0x11F19L, 0x40);
    RIU_WriteByte(0x331AL, 0x04);   // MPLLDIV10/2=43MHz
    RIU_WriteByte(0x331BL, 0x04);   // MPLLDIV10/2=43MHz
    RIU_WriteByte(0x11F1CL, 0x00);
    RIU_WriteByte(0x11F1DL, 0x00);
    RIU_WriteByte(0x11F2AL, 0x00);   // DAGC1/2 SRAM MUX
    RIU_WriteByte(0x11F2BL, 0x00);
    RIU_WriteByte(0x12000L, 0x10);   // ISDBT SRAM share enable, ISDBT and DTMB reset
    RIU_WriteByte(0x12001L, 0x01);
    RIU_WriteByte(0x12002L, 0x74);   // Enable VIF, DVBT, ATSC and VIF reset
    RIU_WriteByte(0x12003L, 0x00);
    RIU_WriteByte(0x12000L, 0x00);   // ISDBT SRAM share enable
    RIU_WriteByte(0x12001L, 0x01);
    RIU_WriteByte(0x12002L, 0x14);   // Enable VIF
    RIU_WriteByte(0x12003L, 0x00);
    RIU_WriteByte(0x12060L, 0x04);   // Disable ADC sign bit
    RIU_WriteByte(0x12061L, 0x00);
    RIU_WriteByte(0x12064L, 0x00);
    RIU_WriteByte(0x12065L, 0x00);
    RIU_WriteByte(0x12066L, 0x00);
    RIU_WriteByte(0x12067L, 0x00);
    RIU_WriteByte(0x120A0L, 0x01);    // VIF use DVB SRAM and FIR
    RIU_WriteByte(0x11F22L, 0x01);
    RIU_WriteByte(0x11F23L, 0x18);

    RIU_WriteByteMask(0x12879L, 0x00, 0xF0);  // Enable LDOs
    RIU_WriteRegBit(0x12840L, 0, _BIT4);
    RIU_WriteRegBit(0x12834L, 1, _BIT4);           // EN_VCO_DIG
    RIU_WriteByte(0x12802L, 0x40);                   // VIF path, Bypass PGA
    RIU_WriteByte(0x12803L, 0x04);

    RIU_WriteByte(0x12816L, 0x05);	 // Set ADC gain is 1
    RIU_WriteByte(0x12817L, 0x05);
    RIU_WriteByte(0x1281EL, 0x80);
    RIU_WriteByte(0x1281FL, 0x00);
    RIU_WriteByte(0x12860L, 0x00);   // Set MPLL_ADC_DIV_SEL
    RIU_WriteByte(0x12861L, 0x06);
    RIU_WriteByte(0x12866L, 0x01);   // Set MPLL_LOOP_DIV_FIRST and SECOND
    RIU_WriteByte(0x12867L, 0x12);
    RIU_WriteByte(0x1286AL, 0x83);   // Disable MPLL reset
    RIU_WriteByte(0x1286BL, 0x00);
    RIU_WriteByte(0x1286AL, 0x03);   // Disable MPLL reset
    RIU_WriteByte(0x1286BL, 0x00);
    RIU_WriteByte(0x1286CL, 0x10);  // AGC enable
    RIU_WriteByte(0x1286DL, 0x00);
    RIU_WriteByte(0x12818L, 0x00);
    RIU_WriteByte(0x12819L, 0x00);
    RIU_WriteByte(0x1287EL, 0x00);  // SIF_CLK_SEL:0x1128_7E[4]
    RIU_WriteByte(0x1287FL, 0x00);
    RIU_WriteByte(0x12864L, 0x00);
    RIU_WriteByte(0x12865L, 0x00); // OUTPUT_DIV_SECOND

    HAL_VIF_Delay1ms(1);

    // RFAGC and IFAGC control (ADC)
    RIU_WriteByteMask(RFAGC_DATA_SEL, 0, 0x0C);		// RFAGC
    RIU_WriteByteMask(IFAGC_DATA_SEL, 0, 0xC0);		// IFAGC
    RIU_WriteRegBit(RFAGC_ODMODE, 0, _BIT1);
    RIU_WriteRegBit(IFAGC_ODMODE, 0, _BIT5);
    if (VIFInitialIn_inst.VifSawArch == SILICON_TUNER)
        RIU_WriteRegBit(RFAGC_ENABLE, 0, _BIT0);         // RFAGC disable
    else
        RIU_WriteRegBit(RFAGC_ENABLE, 1, _BIT0);         // RFAGC enable
    RIU_WriteRegBit(IFAGC_ENABLE, 1, _BIT4);		 // IFAGC enable

    // RFAGC and IFAGC control (RF)
    msWriteBit(RFAGC_SEL_SECONDER, 1, _BIT6);	        // 0: 1st order; 1: 2nd order
    msWriteBit(RFAGC_DITHER_EN, 1, _BIT7);	            // dither disable
    msWriteBit(RFAGC_POLARITY, 1, _BIT4);                // RFAGC polarity 0: negative logic
    msWriteBit(OREN_RFAGC, 0, _BIT5);		            // RFAGC 0: BB control; 1: I2C control

    msWriteBit(IFAGC_SEL_SECONDER, 1, _BIT6);	        // 0: 1st order; 1: 2nd order
    msWriteBit(IFAGC_DITHER_EN, 1, _BIT7);	            // dither disable
    msWriteBit(IFAGC_POLARITY, 1, _BIT4);                // RFAGC polarity 0: negative logic
    msWriteBit(OREN_IFAGC, 0, _BIT6);		            // RFAGC 0: BB control; 1: I2C control

    msWriteBit(OREN_PGA1_V, 0, _BIT3);                  // Video PGA1 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA2_V, 0, _BIT2);                  // Video PGA2 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA1_S, 0, _BIT1);                  // Audio PGA1 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA2_S, 0, _BIT0);                  // Audio PGA2 0: BB control; 1: I2C control

    // EQ BYPASS
    msWriteBit(BYPASS_EQFIR, 1, _BIT0);
    printk("!!! [VIF]nmsVifAdcInitial done !!!()");
}

// For API
void msVifSetIfFreq(IfFrequencyType ucIfFreq)
{
    HALVIFDBG(printk("\r\nmsVifSetIfFreq() ucIfFreq=%d",ucIfFreq));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    g_FreqType=ucIfFreq;

    // cvbs output
    msWriteBit(VIFDAC_ENABLE, 1, _BIT3);                // enable vifdac

   // for China descrambler box
    if (VIFInitialIn_inst.ChinaDescramblerBox == 0)
    {
        msWriteByteMask(VIFDAC_OUT_SEL, 0x00, 0x07);    // 0: cvbs output; 4: debug bus
        msWriteBit(N_A1_IN_SEL, 1, _BIT4);                         // 0:from dvga, 1:from image_rej_iir
    }
    else
    {
        msWriteByteMask(VIFDAC_OUT_SEL, 0x04, 0x07);    // 0: cvbs output; 4: debug bus
        msWriteBit(DEBUG2_EN, 1, _BIT7);                // select debug2 data
        msWriteByteMask(DEBUG_MODULE, 0x00, 0x0F);      // select filter debug bus
        msWriteBit(N_A1_IN_SEL, 0, _BIT4);                         // 0:from dvga, 1:from image_rej_iir

        if (VIFInitialIn_inst.ChinaDescramblerBox == 2)
            msVifWriteByte(DEBUG_PORT, 0x8A);                                     // select CVBS output after IMAGE_REJ1
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 3)
            msVifWriteByte(DEBUG_PORT, 0x88);                                     // select CVBS output after ACI_REJ
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 4)
            msVifWriteByte(DEBUG_PORT, 0x86);                                     // select CVBS output after MIXER_OUT_I
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 5)
            msVifWriteByte(DEBUG_PORT, 0x8B);                                     // select CVBS output after IMAGE_REJ2
        else
            msVifWriteByte(DEBUG_PORT, 0x98);                                     // select CVBS output after IMAGE_IIR
     }

    if (VIFInitialIn_inst.VifTunerType == 1)
    {
        // silicon tuner
        msVifWriteByte(IF_RATE, 0x00);                         // IF rate for 0 MHz
        msVifWriteByte(IF_RATE+1, 0x00);
        msWriteByteMask(IF_RATE+2, 0x00, 0x3F);
    }
    else if (VIFInitialIn_inst.VifTunerType == 2)
    {
        // FM tuner
        if(VIFInitialIn_inst.VifSawArch == SILICON_TUNER)
        {
            msVifWriteByte(IF_RATE, 0x00);                         // IF rate for 27 MHz
            msVifWriteByte(IF_RATE+1, 0x00);
            msWriteByteMask(IF_RATE+2, 0x00, 0x3F);
        }
    }
    else
    {
        switch(ucIfFreq)
        {
            case IF_FREQ_3395:
                msVifWriteByte(IF_RATE, 0x44);                 // IF rate for -48.9 MHz  // HEX [ (IF/144) * 2^22]
                msVifWriteByte(IF_RATE+1, 0x44);
                msWriteByteMask(IF_RATE+2, 0x2A, 0x3F);
                break;
            case IF_FREQ_3800:
		if (g_VifShiftClk == 1)
		{
                    msVifWriteByte(0x12866L, 0x00);//loop divider
                    msVifWriteByte(0x12867L, 0x23);

                    msVifWriteByte(IF_RATE, 0xA8);                 // IF rate for 23 MHz
                    msVifWriteByte(IF_RATE+1, 0x83);
                    msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
		}
              else if(g_VifShiftClk == 2)
              {
                    msVifWriteByte(0x12866L, 0x00);//loop divider
                    msVifWriteByte(0x12867L, 0x25);

                    msVifWriteByte(IF_RATE, 0x29);                 // IF rate for 23 MHz
                    msVifWriteByte(IF_RATE+1, 0xF2);
                    msWriteByteMask(IF_RATE+2, 0x09, 0x3F);
              }
		else
		{
                    msVifWriteByte(0x12866L, 0x00);//loop divider
                    msVifWriteByte(0x12867L, 0x24);

                    msVifWriteByte(IF_RATE, 0xE3);                 // IF rate for 23 MHz
                    msVifWriteByte(IF_RATE+1, 0x38);
                    msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                }
                break;
            case IF_FREQ_3890:
                msVifWriteByte(IF_RATE, 0x49);                 // IF rate for 23.9 MHz
                msVifWriteByte(IF_RATE+1, 0x9F);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                break;
            case IF_FREQ_3950:
                msVifWriteByte(IF_RATE, 0x8E);                 // IF rate for 24.5 MHz
                msVifWriteByte(IF_RATE+1, 0xE3);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                break;
            case IF_FREQ_4575:
                msVifWriteByte(IF_RATE, 0xAA);                 // IF rate for 30.75 MHz
                msVifWriteByte(IF_RATE+1, 0xAA);
                msWriteByteMask(IF_RATE+2, 0x0D, 0x3F);
                break;
            case IF_FREQ_5875:
                msVifWriteByte(IF_RATE, 0xC7);                 // IF rate for 43.75 MHz
                msVifWriteByte(IF_RATE+1, 0x71);
                msWriteByteMask(IF_RATE+2, 0x13, 0x3F);
                break;
            default:
                break;
        }
    }
}

void msVifGroupDelayFilter(VIFSoundSystem ucSoundSystem, FrequencyBand frequencyRange)
{
    BYTE VifPeakingFilter=0, VifYcDelayFilter=0, VifGroupDelayFilter=0;

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    switch (ucSoundSystem)
    {
        case VIF_SOUND_B:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter B frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_GH:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter GH frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_I:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
{
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter I frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_DK1:
        case VIF_SOUND_DK2:
        case VIF_SOUND_DK3:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter DK frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_L:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter L frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_LL:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter LL frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_MN:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_UHF;
            }
            else
                printk("\r\n ERROR msVifGroupDelayFilter MN frequencyRange=%d", frequencyRange);
            break;

        default:
            printk("\r\n Error msVifGroupDelayFilter ucSoundSystem=%d",ucSoundSystem);
            break;
    }

    HALVIFDBG(printk("\r\nmsVifGroupDelayFilter() VifPeakingFilter=%d",VifPeakingFilter));
    HALVIFDBG(printk(" VifYcDelayFilter=%d VifGroupDelayFilter=%d",VifYcDelayFilter, VifGroupDelayFilter));

    if (VifPeakingFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_NULL);
    }
    else if (VifPeakingFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_1dB);
    }
    else if (VifPeakingFilter == 0x02)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_2dB);
    }
    else if (VifPeakingFilter == 0x03)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_3dB);
    }
    else if (VifPeakingFilter == 0x04)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_4dB);
    }
    else if (VifPeakingFilter == 0x05)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_3dB_VSB);
    }
    else if (VifPeakingFilter == 0x06)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_4dB_VSB);
    }
    else if (VifPeakingFilter == 0x07)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_5dB_VSB);
    }
    else if (VifPeakingFilter == 0x80)
    {
        msVifWriteByte(SOS21_C0_L, VIFInitialIn_inst.VifSos21FilterC0);    // SOS21 (user define)
        msWriteByteMask(SOS21_C0_H, VIFInitialIn_inst.VifSos21FilterC0>>8, 0x07);
        msVifWriteByte(SOS21_C1_L, VIFInitialIn_inst.VifSos21FilterC1);
        msWriteByteMask(SOS21_C1_H, VIFInitialIn_inst.VifSos21FilterC1>>8, 0x07);
        msVifWriteByte(SOS21_C2_L, VIFInitialIn_inst.VifSos21FilterC2);
        msWriteByteMask(SOS21_C2_H, VIFInitialIn_inst.VifSos21FilterC2>>8, 0x07);
        msVifWriteByte(SOS21_C3_L, VIFInitialIn_inst.VifSos21FilterC3);
        msWriteByteMask(SOS21_C3_H, VIFInitialIn_inst.VifSos21FilterC3>>8, 0x07);
        msVifWriteByte(SOS21_C4_L, VIFInitialIn_inst.VifSos21FilterC4);
        msWriteByteMask(SOS21_C4_H, VIFInitialIn_inst.VifSos21FilterC4>>8, 0x07);
    }

    if (VifYcDelayFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_YCDelay_NULL);
    }
    else if (VifYcDelayFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_YCDelay_VSB);
    }
    else if (VifYcDelayFilter == 0x80)
    {
        msVifWriteByte(SOS22_C0_L, VIFInitialIn_inst.VifSos22FilterC0);    // SOS22 (user define)
        msWriteByteMask(SOS22_C0_H, VIFInitialIn_inst.VifSos22FilterC0>>8, 0x07);
        msVifWriteByte(SOS22_C1_L, VIFInitialIn_inst.VifSos22FilterC1);
        msWriteByteMask(SOS22_C1_H, VIFInitialIn_inst.VifSos22FilterC1>>8, 0x07);
        msVifWriteByte(SOS22_C2_L, VIFInitialIn_inst.VifSos22FilterC2);
        msWriteByteMask(SOS22_C2_H, VIFInitialIn_inst.VifSos22FilterC2>>8, 0x07);
        msVifWriteByte(SOS22_C3_L, VIFInitialIn_inst.VifSos22FilterC3);
        msWriteByteMask(SOS22_C3_H, VIFInitialIn_inst.VifSos22FilterC3>>8, 0x07);
        msVifWriteByte(SOS22_C4_L, VIFInitialIn_inst.VifSos22FilterC4);
        msWriteByteMask(SOS22_C4_H, VIFInitialIn_inst.VifSos22FilterC4>>8, 0x07);
    }

    if (VifGroupDelayFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_NULL);
    }
    else if (VifGroupDelayFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_VSB_LG);
    }
    else if (VifGroupDelayFilter == 0x02)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_VSB_Philips);
    }
    else if (VifGroupDelayFilter == 0x03)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_Low_R);
    }
    else if (VifGroupDelayFilter == 0x04)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_Low_L);
    }
    else if (VifGroupDelayFilter == 0x05)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_High_R);
    }
    else if (VifGroupDelayFilter == 0x06)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_High_L);
    }
    else if (VifGroupDelayFilter == 0x80)
    {
        msVifWriteByte(SOS31_C0_L, VIFInitialIn_inst.VifSos31FilterC0);    // SOS31 (user define)
        msWriteByteMask(SOS31_C0_H, VIFInitialIn_inst.VifSos31FilterC0>>8, 0x07);
        msVifWriteByte(SOS31_C1_L, VIFInitialIn_inst.VifSos31FilterC1);
        msWriteByteMask(SOS31_C1_H, VIFInitialIn_inst.VifSos31FilterC1>>8, 0x07);
        msVifWriteByte(SOS31_C2_L, VIFInitialIn_inst.VifSos31FilterC2);
        msWriteByteMask(SOS31_C2_H, VIFInitialIn_inst.VifSos31FilterC2>>8, 0x07);
        msVifWriteByte(SOS31_C3_L, VIFInitialIn_inst.VifSos31FilterC3);
        msWriteByteMask(SOS31_C3_H, VIFInitialIn_inst.VifSos31FilterC3>>8, 0x07);
        msVifWriteByte(SOS31_C4_L, VIFInitialIn_inst.VifSos31FilterC4);
        msWriteByteMask(SOS31_C4_H, VIFInitialIn_inst.VifSos31FilterC4>>8, 0x07);
        msVifWriteByte(SOS32_C0_L, VIFInitialIn_inst.VifSos32FilterC0);    // SOS32 (user define)
        msWriteByteMask(SOS32_C0_H, VIFInitialIn_inst.VifSos32FilterC0>>8, 0x07);
        msVifWriteByte(SOS32_C1_L, VIFInitialIn_inst.VifSos32FilterC1);
        msWriteByteMask(SOS32_C1_H, VIFInitialIn_inst.VifSos32FilterC1>>8, 0x07);
        msVifWriteByte(SOS32_C2_L, VIFInitialIn_inst.VifSos32FilterC2);
        msWriteByteMask(SOS32_C2_H, VIFInitialIn_inst.VifSos32FilterC2>>8, 0x07);
        msVifWriteByte(SOS32_C3_L, VIFInitialIn_inst.VifSos32FilterC3);
        msWriteByteMask(SOS32_C3_H, VIFInitialIn_inst.VifSos32FilterC3>>8, 0x07);
        msVifWriteByte(SOS32_C4_L, VIFInitialIn_inst.VifSos32FilterC4);
        msWriteByteMask(SOS32_C4_H, VIFInitialIn_inst.VifSos32FilterC4>>8, 0x07);
    }
}


// For API
void msVifSetSoundSystem(VIFSoundSystem ucSoundSystem)
{
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    DWORD VifCrRateTemp;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
    HALVIFDBG(printk("\r\nmsVifSetSoundSystem() ucSoundSystem=%d",ucSoundSystem));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
#else
    DWORD VifCrRateTemp;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
    g_ucVifSoundSystemType = ucSoundSystem;

    switch(ucSoundSystem)
    {
        case VIF_SOUND_B:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_B & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_B, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_A2);
            msVifGroupDelayFilter(VIF_SOUND_B, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_A2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_GH:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_GH & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_GH, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_A2);
            msVifGroupDelayFilter(VIF_SOUND_GH, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_A2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_I:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_I;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_I;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_I & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_I>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_I>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_I, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_I_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_I, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_I_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_I_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_DK1:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);  // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_DK1_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK1, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK1_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_DK2:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK2_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK2, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_DK3:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_DK3_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK3, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK3_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_L:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_L;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_L;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_L & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_L>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_L>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_L, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_L_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_L, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_L_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_LL:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_LL;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_LL;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_LL & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_LL>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_LL>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_LL, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_L_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_LL, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_L_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        case VIF_SOUND_MN:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if(g_VifShiftClk == 1)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_MN;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if(g_VifShiftClk == 2)
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_MN;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msVifWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msVifWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_MN & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msVifWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_MN>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_MN>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_MN, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_NTSC_MN_A2);
	     msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_MN_NOTCH);
            msVifGroupDelayFilter(VIF_SOUND_MN, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_NTSC_MN_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 1, _BIT6);                     // A_LPF_BG_SEL = 1 (NTSC)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 0 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 0 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            break;

        default:
            break;
    }
    msVifLoadEQCoeff(ucSoundSystem);
}

// For API
void msVifTopAdjust(void)
{
    HALVIFDBG(printk("\r\nmsVifTopAdjust() "));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        msWriteByteMask(AGC_PGA2_MIN, VIFInitialIn_inst.VifTop, 0x1F); 	// pga2 min
        msWriteByteMask(AGC_PGA2_OV, VIFInitialIn_inst.VifTop, 0x1F);
        msWriteBit(AGC_PGA2_OREN, 1, _BIT1);
        msWriteBit(AGC_PGA2_OREN, 0, _BIT1);
    }
}

void msVifDynamicTopAdjust(void)
{
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
    BYTE mean256=0, agc_pga2=0, ref=0, diff=0;
    WORD vga=0;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
    HALVIFDBG(printk("\r\nmsVifDynamicTopAdjust() "));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
#else
    BYTE mean256=0, agc_pga2=0, ref=0, diff=0;
    WORD vga=0;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        vga = msRead2Bytes(AGC_VGA);
        agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
        mean256 = (BYTE)(msRead2Bytes(AGC_MEAN256)>>1);              // AGC mean256
        ref = msReadByte(AGC_REF);                      // AGC ref

        if (g_bCheckModulationType == 0)
        {
            diff = 0x15;                                // negative modulation
        }
        else
        {
            diff = 0x0A;                                // positive modulation
        }

        if ((vga == VIFInitialIn_inst.VifVgaMinimum) && (mean256 >= (ref+diff)) && (agc_pga2 == VIFInitialIn_inst.VifTop))
        {
            msWriteByteMask(AGC_PGA2_MIN, VIFInitialIn_inst.VifDynamicTopMin, 0x1F);  // pga2 min
        }
        else if (((agc_pga2) < VIFInitialIn_inst.VifTop) && ((vga >= 0xF000) || (vga <= VIFInitialIn_inst.VifVgaMaximum)))
        {
            msVifTopAdjust();
        }
    }
}

void msVifLoad(void)
{
    if (!_hal_VIF.bBaseAddrInitialized) return ;

    RIU_WriteRegBit(RF_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB1_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB2_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB2_LOAD , 0, _BIT0);
}

// For API
void msVifInitial(void)
{
    HALVIFDBG(printk("\r\nmsVifInitial()"));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    msWriteByteMask(VIF_SOFT_RSTZ, 0x00, 0x7F);                     // VIF software reset
    msWriteBit(CLAMPGAIN_RSTZ, 0, _BIT0);                           // clampgain software reset
    msWriteBit(VSYNC_RSTZ, 0, _BIT0);                               // vsync software reset

    g_VifZeroDetFlag = 0;
    g_ucVifStatusStep = VIF_START;

    if ((g_ucVifSoundSystemType == VIF_SOUND_L) || (g_ucVifSoundSystemType == VIF_SOUND_LL))
    {
        g_bCheckModulationType = 1;     // positive modulation
        g_bCheckIFFreq = (g_ucVifSoundSystemType == VIF_SOUND_L) ? 0 : 1;   // 0: 38.9 MHz; 1: 33.9 MHz
    }
    else
    {
        g_bCheckModulationType = 0;     // negative modulation
        g_bCheckIFFreq = 0;             // 38.9 MHz
    }

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(MODULATION_TYPE, 0x0F, 0x0F);               // positive modulation
    }
    else
    {
        msWriteByteMask(MODULATION_TYPE, 0x00, 0x0F);               // negative modulation
    }

    // AGC
    if (g_bCheckModulationType == 1)
    {
	    msVifWriteByte(AGC_PEAK_CNT_L, 0x00);                          // AGC peak cnt
	    msWriteByteMask(AGC_PEAK_CNT_H, 0x0B, 0x0F);
	    msVifWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefPositive);  // AGC ref
    }
    else
    {
	    msVifWriteByte(AGC_PEAK_CNT_L, 0x00);                          // AGC peak cnt
	    msWriteByteMask(AGC_PEAK_CNT_H, 0x0C, 0x0F);
	    msVifWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefNegative);  // AGC ref
    }
    msWriteByteMask(AGC_MEAN_SEL, _BIT2, _BIT2|_BIT3);              // mean16
    msVifWriteByte(AGC_LINE_CNT_L, 0x01);                              // AGC line cnt = 1
    msVifWriteByte(AGC_LINE_CNT_H, 0x00);
    if (VIFInitialIn_inst.VifTunerType == 1)
        msWriteByteMask(AGC_K, 0x03, _BIT0|_BIT1|_BIT2);                // k
    else
        msWriteByteMask(AGC_K, 0x02, _BIT0|_BIT1|_BIT2);                // k
    msWriteByteMask(AGC_PGA2_OREN, 0x00, 0x03);
    msVifWriteByte(AGC_VGA_MAX_L, VIFInitialIn_inst.VifVgaMaximum);    // vga max
    msVifWriteByte(AGC_VGA_MAX_H, VIFInitialIn_inst.VifVgaMaximum>>8);
    msVifWriteByte(AGC_VGA_MIN_L, VIFInitialIn_inst.VifVgaMinimum);    // vga min
    msVifWriteByte(AGC_VGA_MIN_H, VIFInitialIn_inst.VifVgaMinimum>>8);
    msWriteByteMask(AGC_PGA1_MAX, 0x00, 0x0F); 		                // pga1 max
    if (VIFInitialIn_inst.VifTunerType == 1)
        msWriteByteMask(AGC_PGA2_MAX, 0x00, 0x1F); 		                // pga2 max
    else
        msWriteByteMask(AGC_PGA2_MAX, 0x1F, 0x1F); 		                // pga2 max

    msVifWriteByte(VAGC_VGA2_OV_L, 0x00);  		    			// VGA2(IFAGC) output minimun
    msVifWriteByte(VAGC_VGA2_OV_H, 0x80);
    msWriteBit(VAGC_VGA2_OREN, 1, _BIT2);

    if (VIFInitialIn_inst.VifSawArch == SILICON_TUNER)
        msWriteBit(VAGC_VGA_OUT_SEL, 1, _BIT0);				// VGA1 -> IFAGC

    msWriteBit(LEVEL_SENSE_BYPASS, 1, _BIT0);             // Level_Sense bypass
    msWriteBit(LEVEL_SENSE_OUT_SEL, 0, _BIT4);           // DVGA input: 0: from LEVEL_SENSE out(can be bypassed); 1: ACI_BPF out(cannot be bypassed)
    msWriteBit(BYPASS_V_ACI_BPF4AGC, 1, _BIT0);        // bypass ACI_BPF before AGC input: 0:not bypass; 1: bypass
    msWriteBit(AGC_IN_SEL, 1, _BIT1);                           // AGC input 0: from SOS_out ; 1:from ACI_BPF out(can be bypassed)

    // AGC gain distribution
    msWriteBit(AGC_DBB_VVGA_SEL, 0, _BIT3);                         // Vga gain force x1
    msWriteBit(AGC_DBB_AVGA_SEL, 0, _BIT4);                         // Avga gain force x1

    msVifWriteByte(AGC_VGA_THR, VIFInitialIn_inst.VifVgaMaximum);      // vga threshold
    msVifWriteByte(AGC_VGA_THR+1, (VIFInitialIn_inst.VifVgaMaximum - 0x1000)>>8);
    msVifWriteByte(AGC_VGA_BASE, (VIFInitialIn_inst.VifAgcVgaBase - 0x14));     // vga base
    msVifWriteByte(AGC_VGA_OFFS, VIFInitialIn_inst.VifAgcVgaOffs);     // vga offset

    msWriteBit(AGC_ENABLE, 1, _BIT0);	                            // AGC enable

    // CR
    msVifWriteByte(CR_DL_A, 0x16);	            	    	            // CR audio delay line
    msVifWriteByte(CR_PD_ERR_MAX_L, 0xFF);	                            // CR pd error max
    msWriteByteMask(CR_PD_ERR_MAX_H, 0x3F, 0x3F);
    msVifWriteByte(CR_NOTCH_A1_L, 0x41);	            	            // CR notch filter coefficient
    msVifWriteByte(CR_NOTCH_A1_H, 0x0C);
    msVifWriteByte(CR_NOTCH_A2_L, 0xE9);	            	            // CR notch filter coefficient
    msVifWriteByte(CR_NOTCH_A2_H, 0x0B);
    msVifWriteByte(CR_NOTCH_B1_L, 0x58);	            	            // CR notch filter coefficient
    msVifWriteByte(CR_NOTCH_B1_H, 0x00);
    msWriteBit(CR_ANCO_SEL, 1, _BIT0);	            	            // audio nco select
    if (VIFInitialIn_inst.VifTunerType == 2)
    {
        msWriteByteMask(CR_KF1_HW, 0x00, 0x0F);   // kf1 hardware mode
        msWriteByteMask(CR_KP1_HW, 0x00, 0x0F);   // kp1 hardware mode
        msWriteByteMask(CR_KI1_HW, 0x00, 0xF0);// ki1 hardware mode
        msWriteByteMask(CR_KP2_HW, 0x00, 0x0F);   // kp2 hardware mode
        msWriteByteMask(CR_KI2_HW, 0x00, 0xF0);// ki2 hardware mode
        msWriteBit(CR_K_SEL, 0, _BIT6);	          			            // hw mode
    }
    else
    {
        msWriteByteMask(CR_KF1_HW, VIFInitialIn_inst.VifCrKf1, 0x0F);   // kf1 hardware mode
        msWriteByteMask(CR_KP1_HW, VIFInitialIn_inst.VifCrKp1, 0x0F);   // kp1 hardware mode
        msWriteByteMask(CR_KI1_HW, VIFInitialIn_inst.VifCrKi1<<4, 0xF0);// ki1 hardware mode
        msWriteByteMask(CR_KP2_HW, VIFInitialIn_inst.VifCrKp2, 0x0F);   // kp2 hardware mode
        msWriteByteMask(CR_KI2_HW, VIFInitialIn_inst.VifCrKi2<<4, 0xF0);// ki2 hardware mode
	  msWriteBit(CR_K_SEL, 0, _BIT6);			// kp,ki,kf
	  msWriteBit(CR_PD_IMAG_INV, 1, _BIT1);                             // for > 150% overmodulation
    }
    msWriteByteMask(CR_KF_SW, 0x00, 0x0F);                          // kf software mode
    msWriteByteMask(CR_KP_SW, VIFInitialIn_inst.VifCrKp, 0x0F);     // kp software mode
    msWriteByteMask(CR_KI_SW, VIFInitialIn_inst.VifCrKi<<4, 0xF0);  // ki software mode
    msWriteBit(CR_JTRDET_IN_SEL, 1, _BIT4);                         // carrier jitter detector input select CR_LF1
    msWriteBit(VNCO_INV_OREN, 0, _BIT1);

    //locking range setting
    msWriteBit(CR_FD_IN_SEL, 0 , _BIT0);                                  //0:IIR LPF2; 1:FIR
    msWriteBit(CR_IIR_SEL, 1 , _BIT1);                                   //0:IIR LPF1; 1:IIR LPF2

    if(VIFInitialIn_inst.VifCrPdModeSel == 0)                      // 0: imaginary part; 1: cordic
      msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF1);     // IIR LPF1 coefficients
    else
      msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF2);    // IIR LPF2 coefficients

    msWriteBit(CR_LPF_SEL, VIFInitialIn_inst.VifCrLpfSel, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF
    msWriteBit(CR_PD_MODE, VIFInitialIn_inst.VifCrPdModeSel, _BIT1);    // 0: imaginary part; 1: cordic
    msWriteBit(LOCK_LEAKY_SEL, VIFInitialIn_inst.VifCrLockLeakySel, _BIT0);
    msWriteBit(CR_PD_X2, VIFInitialIn_inst.VifCrPdX2, _BIT2);       // CR X2 0: lock 0 degree; 1: lock 0 or 180 degree
    msVifWriteByte(CR_LOCK_TH_L, VIFInitialIn_inst.VifCrLockThr);      // CR lock threshold
    msWriteByteMask(CR_LOCK_TH_H, VIFInitialIn_inst.VifCrLockThr>>8, 0x03);
    msVifWriteByte(CR_UNLOCK_NUM, 0x00);                               // CR unlock num
    msVifWriteByte(CR_UNLOCK_NUM+1, 0x40);
    msWriteByteMask(CR_UNLOCK_NUM+2, 0x00, 0x0F);
    msVifWriteByte(CR_LOCK_NUM, VIFInitialIn_inst.VifCrLockNum);       // CR lock num
    msVifWriteByte(CR_LOCK_NUM+1, VIFInitialIn_inst.VifCrLockNum>>8);
    msWriteByteMask(CR_LOCK_NUM+2, VIFInitialIn_inst.VifCrLockNum>>16, 0x0F);
    msVifWriteByte(CR_CODIC_TH, VIFInitialIn_inst.VifCrThr);           // CR cordic threshold
    msWriteByteMask(CR_CODIC_TH+1, VIFInitialIn_inst.VifCrThr>>8, 0x3F);

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        if (g_VifShiftClk == 1)
        {
            msVifWriteByte(CR_RATE, 0x6D);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0xDB);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert
        }
        else if(g_VifShiftClk == 2)
        {
            msVifWriteByte(CR_RATE, 0x22);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0x9F);
            msWriteByteMask(CR_RATE+2, 0x15, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert
        }
	else
        {
            msVifWriteByte(CR_RATE, 0xE3);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0x38);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                              // cr_rate not invert
        }
    }

    // tuner step size
    VIFInitialIn_inst.VifTunerStepSize = FREQ_STEP_75KHz;
    if (VIFInitialIn_inst.VifTunerStepSize == FREQ_STEP_75KHz)    // 62.5KHz
    {
        if (g_bCheckIFFreq == 0)
        {
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0x40);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x02, 0x0F);
        }
        else
        {
            // SECAM L'
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0xC0);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x0D, 0x0F);
        }
    }
    else if (VIFInitialIn_inst.VifTunerStepSize == FREQ_STEP_62_5KHz)    // 62.5KHz
    {
        if (g_bCheckIFFreq == 0)
        {
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0xB3);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x02, 0x0F);
        }
        else
        {
            // SECAM L'
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0x4D);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x0D, 0x0F);
        }
    }
    else if (VIFInitialIn_inst.VifTunerStepSize == FREQ_STEP_50KHz) // 50KHz
    {
        if (g_bCheckIFFreq == 0)
        {
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0x60);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x03, 0x0F);
        }
        else
        {
            // SECAM L'
            msVifWriteByte(CR_FOE_SCAL_FACTOR_L, 0xA0);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x0C, 0x0F);
        }
    }

    // Filter
    msWriteBit(DEBUG_V_A, 1, _BIT5);                            // single ADC
    if (VIFInitialIn_inst.ChinaDescramblerBox !=0)
    {
        msWriteByteMask(IMAGE_REJ_IIR_SEL, _BIT3, _BIT2|_BIT3);      // 0: aci_rej_out; 1: nyq_slp_out1; 2: nyq_slp_out2; 3: mixer_out_i
    }
    msWriteByteMask(IMAGE_REJ1_SEL, _BIT0, _BIT0|_BIT1);      // 0: aci_rej_out; 1: nyq_slp_out1; 2: nyq_slp_out2; 3: mixer_out_i
    msWriteBit(BYPASS_CO_A_REJ, 0, _BIT4);                      // CO_A_REJ not bypass

    msWriteBit(IMAGE_REJ_OUT_SEL, 0, _BIT7);                     // 0: IMAGE_REJ1; 1: IMAGE_REJ_IIR
    msWriteBit(A_BP_OUT_X2, 1, _BIT7);                              // A_BP output x2
    msWriteBit(A_DAGC_SEL, 1, _BIT7);                               // 0: input from a_sos; 1: input from a_lpf_up
    msWriteBit(BYPASS_A_NOTCH, 1, _BIT6);                           // A_NOTCH bypass
    msWriteBit(BYPASS_A_SOS, 1, _BIT7);                             // A_SOS bypass
    msWriteBit(BYPASS_SOS21, 0, _BIT2);                             // SOS21 not bypass
    msWriteBit(BYPASS_SOS22, 0, _BIT3);                             // SOS22 not bypass
    msWriteBit(BYPASS_SOS31, 0, _BIT4);             	            // SOS31 not bypass
    msWriteBit(BYPASS_SOS32, 0, _BIT5);             	            // SOS32 not bypass

    // silicon tuner
    if (VIFInitialIn_inst.VifTunerType == 1)
    {
        msWriteBit(BYPASS_V_ACI_BPF4LS, 1, _BIT5);             	 // VACI_BPF bypass
        msWriteBit(BYPASS_A_ACI_BPF, 0, _BIT1);             	        // AACI_BPF not bypass
        msWriteBit(BYPASS_VSPUR_REJ, 1, _BIT2);             	        // VSPUR_REJ bypass
        msWriteBit(BYPASS_ASPUR_REJ, 1, _BIT3);             	        // ASPUR_REJ bypass
    }
    else
    {
        msWriteBit(BYPASS_A_ACI_BPF, 0, _BIT1);             	        // AACI_BPF  not bypass
    }

    // DAGC1
    if (_bit1_(VIFInitialIn_inst.VifDelayReduce))
    {
        msWriteBit(DAGC1_DL_BYPASS, 1, _BIT3);                      // DAGC1 delay line bypass
    }
    else
    {
        msWriteBit(DAGC1_DL_BYPASS, 0, _BIT3);                      // DAGC1 delay line not bypass
    }
    msWriteBit(DAGC1_BYPASS, 0, _BIT1);                             // DAGC1 not bypass

    msWriteBit(DAGC1_OREN, 1, _BIT6);	                            // DAGC1 gain_overwrite = 1
    msWriteBit(DAGC1_OREN, 0, _BIT6);	                            // DAGC1 gain_overwrite = 0

    msWriteBit(DAGC1_GAIN0_FB_EN, 0, _BIT2);	                    // DAGC1 gain_update = 1

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(DAGC1_REF, 0x0B, 0x3F);		                // DAGC1 ref
        msWriteByteMask(DAGC1_RATIO, 0x03, 0x07);		            // DAGC1 ratio
        msVifWriteByte(DAGC1_PEAK_CNT_L, 0x00);	    	            // DAGC1 peak cnt
        msWriteByteMask(DAGC1_PEAK_CNT_H, 0x0B, 0x0F);
        msVifWriteByte(DAGC1_GAIN_OVERWRITE_L, VIFInitialIn_inst.VifDagc1GainOv);
        msWriteByteMask(DAGC1_GAIN_OVERWRITE_H, VIFInitialIn_inst.VifDagc1GainOv>>8, 0x3F);
    }
    else
    {
        msWriteByteMask(DAGC1_REF, VIFInitialIn_inst.VifDagc1Ref, 0x3F);    // DAGC1 ref
        msWriteByteMask(DAGC1_RATIO, 0x00, 0x07);		            // DAGC1 ratio
        msVifWriteByte(DAGC1_PEAK_CNT_L, 0x00);	                    // DAGC1 peak cnt
        msWriteByteMask(DAGC1_PEAK_CNT_H, 0x0C, 0x0F);
    }
    msWriteBit(DAGC1_ENABLE, 1, _BIT0);                             // DAGC1 enable

    // DAGC2
    if (_bit2_(VIFInitialIn_inst.VifDelayReduce))
    {
        msWriteBit(DAGC2_DL_BYPASS, 1, _BIT3);                      // DAGC2 delay line bypass
    }
    else
    {
        msWriteBit(DAGC2_DL_BYPASS, 0, _BIT3);                      // DAGC2 delay line not bypass
    }
    msWriteBit(DAGC2_BYPASS, 0, _BIT1);                             // DAGC2 not bypass

    msWriteBit(DAGC2_OREN, 1, _BIT6);	                            // DAGC2 gain_overwrite = 1
    msWriteBit(DAGC2_OREN, 0, _BIT6);	                            // DAGC2 gain_overwrite = 0

    msWriteBit(DAGC2_GAIN0_FB_EN, 0, _BIT2);	                    // DAGC2 gain_update = 1

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(DAGC2_REF, 0x0B, 0x3F);		                // DAGC2 ref
        msWriteByteMask(DAGC2_RATIO, 0x03, 0x07);		            // DAGC2 ratio
        msVifWriteByte(DAGC2_PEAK_CNT_L, 0x00);	                    // DAGC2 peak cnt
        msWriteByteMask(DAGC2_PEAK_CNT_H, 0x0B, 0x0F);
        msVifWriteByte(DAGC2_GAIN_OVERWRITE_L, VIFInitialIn_inst.VifDagc2GainOv);
        msWriteByteMask(DAGC2_GAIN_OVERWRITE_H, VIFInitialIn_inst.VifDagc2GainOv>>8, 0x3F);
    }
    else
    {
        msWriteByteMask(DAGC2_REF, VIFInitialIn_inst.VifDagc2Ref, 0x3F);    // DAGC2 ref
        msWriteByteMask(DAGC2_RATIO, 0x00, 0x07);		            // DAGC2 ratio
        msVifWriteByte(DAGC2_PEAK_CNT_L, 0x00);	    	            // DAGC2 peak cnt
        msWriteByteMask(DAGC2_PEAK_CNT_H, 0x0C, 0x0F);
    }
    msWriteBit(DAGC2_ENABLE, 1, _BIT0);                             // DAGC2 enable

	// clampgain
    if (g_bCheckModulationType == 1)
    {
        //RIU_WriteRegBit(0x3D20L, 1, _BIT3);  // IDAC x1.33 mode
        RIU_WriteRegBit(0x3D20L, 1, _BIT7);  // IDAC x1.33 mode change register for monaco
        msVifWriteByte(CLAMPGAIN_CLAMP_OVERWRITE, VIFInitialIn_inst.VifClampgainClampOvPositive);  // clampgain clamp overwrite value
        msWriteByteMask(CLAMPGAIN_CLAMP_OVERWRITE+1, VIFInitialIn_inst.VifClampgainClampOvPositive>>8, 0x07);
        msVifWriteByte(CLAMPGAIN_GAIN_OVERWRITE, VIFInitialIn_inst.VifClampgainGainOvPositive);    // clampgain gain overwrite value
        msWriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, VIFInitialIn_inst.VifClampgainGainOvPositive>>8, 0x07);
    }
    else
    {
        //RIU_WriteRegBit(0x3D20L, 0, _BIT3);  // IDAC x1 mode
        RIU_WriteRegBit(0x3D20L, 0, _BIT7);  // IDAC x1 mode change register setting for monaco
        msVifWriteByte(CLAMPGAIN_CLAMP_OVERWRITE, VIFInitialIn_inst.VifClampgainClampOvNegative);  // clampgain clamp overwrite value
        msWriteByteMask(CLAMPGAIN_CLAMP_OVERWRITE+1, VIFInitialIn_inst.VifClampgainClampOvNegative>>8, 0x07);
        msVifWriteByte(CLAMPGAIN_GAIN_OVERWRITE, VIFInitialIn_inst.VifClampgainGainOvNegative);    // clampgain gain overwrite value
        msWriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, VIFInitialIn_inst.VifClampgainGainOvNegative>>8, 0x07);
    }
    msWriteBit(CLAMPGAIN_BYPASS, 0, _BIT1);				                                        // clampgain not bypass
    msWriteBit(CLAMPGAIN_SEL, VIFInitialIn_inst.VifClampgainClampSel, _BIT3);                   // 0: clamp select sync bottom; 1: clamp select porch
    msVifWriteByte(CLAMPGAIN_SYNCBOTT_REF, VIFInitialIn_inst.VifClampgainSyncbottRef);	            // porch or syncbottom ref
    msVifWriteByte(CLAMPGAIN_SYNCHEIGHT_REF, VIFInitialIn_inst.VifClampgainSyncheightRef);         // syncheight ref
    msWriteByteMask(CLAMPGAIN_KC, VIFInitialIn_inst.VifClampgainKc, 0x07);			            // kc
    msWriteByteMask(CLAMPGAIN_KG, VIFInitialIn_inst.VifClampgainKg<<4, 0x70);			        // kg
    msVifWriteByte(CLAMPGAIN_PORCH_CNT, VIFInitialIn_inst.VifClampgainPorchCnt);                   // clampgain porch cnt for NTSC
    msWriteByteMask(CLAMPGAIN_PORCH_CNT+1, VIFInitialIn_inst.VifClampgainPorchCnt>>8, 0x01);
    msVifWriteByte(CLAMPGAIN_CLAMP_MIN, VIFInitialIn_inst.VifClampgainClampMin);                   // clampgain clamp min
    msVifWriteByte(CLAMPGAIN_CLAMP_MAX, VIFInitialIn_inst.VifClampgainClampMax);                   // clampgain clamp max
    msVifWriteByte(CLAMPGAIN_GAIN_MIN, VIFInitialIn_inst.VifClampgainGainMin);                     // clampgain gain min
    msVifWriteByte(CLAMPGAIN_GAIN_MAX, VIFInitialIn_inst.VifClampgainGainMax);                     // clampgain gain max
    msWriteBit(CLAMPGAIN_CLAMP_OREN, VIFInitialIn_inst.VifClampgainClampOren, _BIT0);           // clampgain clamp overwrite enable
    msWriteBit(CLAMPGAIN_CLAMP_OREN, VIFInitialIn_inst.VifClampgainGainOren, _BIT1);            // clampgain gain overwrite enable
    msWriteBit(CLAMPGAIN_EN, 1, _BIT2);					                                        // clampgain enable

    // vsync
    msWriteBit(VSYNC_ENABLE, 1, _BIT1);                             // vsync enable

    // ADAGC
    if (g_bCheckModulationType == 1)
    {
        msWriteBit(ADAGC_BYPASS, 1, _BIT1);                         // ADAGC bypass
	 msWriteByteMask(ADAGC_K, 0x00, 0x07);			            // ADAGC k
        msWriteBit(ADAGC_ENABLE, 0, _BIT0);                         // ADAGC disable
    }
    else
    {
        msWriteBit(ADAGC_BYPASS, 0, _BIT1);                         // ADAGC not bypass
        msWriteByteMask(ADAGC_K, 0x04, 0x07);			            // ADAGC k
        msWriteBit(ADAGC_ENABLE, 1, _BIT0);                         // ADAGC enable
    }

    // zero detector
    msWriteBit(ZERO_IN_SEL, 1 , _BIT1);                             // 0:dagc_in, 1:dagc_out
    msWriteByteMask(ZERO_TH, 0x20, 0x7F);
    msVifWriteByte(ZERO_CNT_NUM, 0x0A);
    msWriteByteMask(ZERO_CNT_NUM+1, 0x00, 0x0F);
    msVifWriteByte(ZERO_ZERO_NUM, 0x20);
    msVifWriteByte(ZERO_ZERO_NUM+1, 0x00);
    msWriteBit(ZERO_ENABLE, 0 , _BIT0);                            // zero detector disable

    //AM Hum detection setting
    msWriteByteMask(AGC_HUM_CNT_MAX , _BIT5 , _BIT4|_BIT5|_BIT6);   // 0->128 ,1->256, 2->512 samples
    msVifWriteByte(AGC_HUM_ERR_THR , 0x20);                                             // format <8,8> => 0.125 = 0x20
    msVifWriteByte(AGC_HUM_DET_LIM , 0x20);                                              // format <8,-2> => 128 samples

    //CR_Ki/Kp speed up setting
    msWriteBit(CR_KPKI_SPEEDUP_EN , 0 , _BIT0);                                     //0:disable , 1:enable
    msWriteBit(CR_INV2_EN , 0 , _BIT4);                                                    //0:disable , 1:enable
    msWriteByteMask(CR_KP_SPEED, _BIT2 , _BIT0|_BIT1|_BIT2|_BIT3);
    msWriteByteMask(CR_KI_SPEED, _BIT6 , _BIT4|_BIT5|_BIT6|_BIT7);

    // locking range +/- 500KHz  ->  +/- 1MHz  setting
    msWriteByteMask(CR_KF1_HW, 0x02, 0x0F);   // kf1 hardware mode
    msWriteByteMask(CR_KP1_HW, 0x43, 0x0F);   // kp1 hardware mode
    msWriteByteMask(CR_KI1_HW, 0x43, 0xF0);   // ki1 hardware mode
    msWriteByteMask(CR_FD_DELAY_SEL, _BIT5, _BIT4|_BIT5);   // fd_delay = 8
    msWriteByteMask(CR_FD_MU, _BIT5, _BIT4|_BIT5);          // fd_mu = 2^-4

    msWriteBit(BYPASS_SOS33, 1, _BIT6);

    // real HW_KPKI_THR1_L
    msVifWriteByte(KPKI_ADJ_TH1_L, 0x50);
    msVifWriteByte(KPKI_ADJ_TH1_L+1, 0x00);

     // real HW_KPKI_THR1_H
    msVifWriteByte(KPKI_ADJ_TH1_H, 0x50);
    msVifWriteByte(KPKI_ADJ_TH1_H+1, 0x00);

    // real HW_KPKI_THR2_L
    msVifWriteByte(KPKI_ADJ_TH2_L, 0x00);
    msVifWriteByte(KPKI_ADJ_TH2_L+1, 0x01);

    // real HW_KPKI_THR2_H
    msVifWriteByte(KPKI_ADJ_TH2_H, 0x00);
    msVifWriteByte(KPKI_ADJ_TH2_H+1, 0x01);

    // real HW_KPKI_THR3_L
    msVifWriteByte(KPKI_ADJ_TH3_L, 0xFF);
    msVifWriteByte(KPKI_ADJ_TH3_L+1, 0xFF);

    // real HW_KPKI_THR3_H
    msVifWriteByte(KPKI_ADJ_TH3_H, 0xFF);
    msVifWriteByte(KPKI_ADJ_TH3_H+1, 0xFF);

    // real HW_KPKI setting
    msWriteByteMask(CR_KP_ADJ1, 0x05, 0x0F);
    msWriteByteMask(CR_KI_ADJ1, 0x80, 0xF0);
    msWriteByteMask(CR_KP_ADJ2, 0x04, 0x0F);
    msWriteByteMask(CR_KI_ADJ2, 0x70, 0xF0);
    msWriteByteMask(CR_KP_ADJ3, 0x03, 0x0F);
    msWriteByteMask(CR_KI_ADJ3, 0x60, 0xF0);

    // real HW_KPKI_disable
    msWriteBit(KPKI_ADJ_EN, 0, _BIT0);
    g_VifHWKpKiFlag = 1; // 0: SW_Kp/Ki ; 1: Real HW_Kp/Ki
    msWriteBit(HALVIFDBG2_BIT, g_VifHWKpKiFlag, _BIT0);

    msWriteBit(BYPASS_N_A1, 1, _BIT2);
    msWriteBit(BYPASS_N_A2, 1, _BIT3);
    msWriteBit(BYPASS_SOS11, 1, _BIT0);
    msWriteBit(BYPASS_SOS12, 1, _BIT1);

    msWriteBit(N_A1_IN_SEL, 1, _BIT4);                         // 0:from dvga, 1:from image_rej_iir

    msWriteByteMask(VIF_SOFT_RSTZ, 0x7F, 0x7D);                     // VIF software reset
    msWriteBit(CLAMPGAIN_RSTZ, 1, _BIT0);                           // clampgain software reset
    msWriteBit(VSYNC_RSTZ, 1, _BIT0);                               // vsync software reset

    // TOP
    msVifTopAdjust();

    // version control
    msVifWriteByte(FIRMWARE_VERSION_L, 0x02);                          // Version 2
    msVifWriteByte(FIRMWARE_VERSION_H, 0x10);                          // Formal version
    msVifWriteByte(FIRMWARE_DATECODE_L, 0x10);                       // 16(dd)
    msVifWriteByte(FIRMWARE_DATECODE_H, 0x4F);                       // 04/15 (mm/yy)  firmware version control

    HAL_VIF_Delay1ms(1);
    msWriteByteMask(VIF_SOFT_RSTZ, 0x7F, 0x7F);
}

// For API
void msVifHandler(BOOL bVifDbbAcq)
{
    BYTE afc_foe;
    BYTE mean16;
    BYTE agc_pga2;
    WORD agc_vga;
    BYTE dagc1_var;
    BYTE KPKI_GEAR;
    WORD crjtr_det_avgcnt;
    BYTE KpKi_THR1 = 100;
    BYTE KpKi_THR2 = 20;
    BYTE KPKI_GEAR_CNT_THR1 = 100;
    BYTE KPKI_GEAR_CNT_THR2 = 20;
    static WORD CNT_IDX = 0;

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    //HAL_VIF_Delay1ms(1);

    g_VifChannelScanFlag = bVifDbbAcq;

    switch(g_ucVifStatusStep)
    {
        case VIF_START:
        case VIF_AGC_STATUS:
            g_VifCrKpKiAdjLoopCnt = 0;
            CNT_IDX = 0;
            mean16 = (BYTE)(msRead2Bytes(AGC_MEAN16)>>1);           // AGC mean16
            agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
            agc_vga = msRead2Bytes(AGC_VGA);
            if (g_bCheckModulationType == 0)
            {
	          if (((mean16 < AGC_MEAN16_UPBOUND) && (mean16 > AGC_MEAN16_LOWBOUND)) || (agc_pga2 == 0x1F) || (agc_vga == VIFInitialIn_inst.VifVgaMinimum))
                {
                    msVifWriteByte(AGC_LINE_CNT_L, 0x04);              // AGC line cnt = 4
                    msVifWriteByte(AGC_LINE_CNT_H, 0x00);

                    msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1; kp2,ki2,kf2
                    msWriteBit(CR_K_SEL2, 0, _BIT0);
                    g_ucVifStatusStep = VIF_AFC_STATUS;
		  }
            }
            else
            {
                if (((mean16 < AGC_MEAN16_UPBOUND_SECAM) && (mean16 > AGC_MEAN16_LOWBOUND_SECAM)) || (agc_pga2 == 0x1F) || (agc_vga == VIFInitialIn_inst.VifVgaMinimum))
                {
                    msVifWriteByte(AGC_LINE_CNT_L, 0x04);              // AGC line cnt = 4
                    msVifWriteByte(AGC_LINE_CNT_H, 0x00);

                    msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1,kp2,ki2,kf2
                    g_ucVifStatusStep = VIF_AFC_STATUS;
                }
            }
            break;

        case VIF_AFC_STATUS:
            if (_bit0_(msReadByte(CR_LOCK_STATUS)))
            {
                // DAGC
                if (g_bCheckModulationType == 1)
                {
                    msWriteBit(DAGC1_OREN, 1, _BIT6);	            // DAGC1 gain_overwrite = 1
                    msWriteBit(DAGC2_OREN, 1, _BIT6);	            // DAGC2 gain_overwrite = 1
                }
                g_ucVifStatusStep = VIF_AFC_STATUS2;
            }
            else
            {
                    msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1,kp2,ki2,kf2
                    HAL_VIF_Delay1us(1);
                    msWriteBit(CR_NCO_FF_RSTZ, 0, _BIT2);          // reset NCO_FF
                    msWriteBit(CR_LF_FF_RSTZ, 0, _BIT5);            // reset AFC integral part
                    HAL_VIF_Delay1us(5);
                    msWriteBit(CR_NCO_FF_RSTZ, 1, _BIT2);
                    HAL_VIF_Delay1us(1);
                    msWriteBit(CR_LF_FF_RSTZ, 1, _BIT5);
            }
            break;

        case VIF_AFC_STATUS2:
            afc_foe = msReadByte(CR_FOE);                           // AFC_FOE
            if ((afc_foe <= 0x04) || (afc_foe >= 0xFC))             // |AFC_FOE|<=4
            {
                // AGC
                msVifWriteByte(AGC_VGA_THR, VIFInitialIn_inst.GainDistributionThr);    // vga threshold
                msVifWriteByte(AGC_VGA_THR+1, VIFInitialIn_inst.GainDistributionThr>>8);
                msVifWriteByte(AGC_VGA_BASE, VIFInitialIn_inst.VifAgcVgaBase);         // vga base
                if (bVifDbbAcq == 0)        // 0: not channel scan; 1: channel scan
                {
                    // AGC
                    msWriteByteMask(AGC_K, 0x04, _BIT0|_BIT1|_BIT2);// k
                    msVifWriteByte(AGC_LINE_CNT_L, 0x10);              // AGC line cnt = 16
                    msVifWriteByte(AGC_LINE_CNT_H, 0x00);

                    // CR
                    msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF2);    // IIR LPF2 coefficients

                    if (VIFInitialIn_inst.VifCrKpKiAdjust && ((msReadByte(HALVIFDBG2_BIT) & 0x01) == 0))
                    {
                        msWriteBit(KPKI_ADJ_EN, 0, _BIT0);                 // real HW_KPKI_disable
                        g_VifCrKp = VIFInitialIn_inst.VifCrKp;
                        g_VifCrKi = VIFInitialIn_inst.VifCrKi;
                        for(crjtr_det_avgcnt = 0; crjtr_det_avgcnt < 1024; crjtr_det_avgcnt++)
                            msVifCrKpKiAutoAdjust(0x28, 0x50);

                        msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                        msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);
                    }
                    else
                    {
                        msWriteByteMask(CR_KP_SW, VIFInitialIn_inst.VifCrKp, 0x0F);     // kp software mode
                        msWriteByteMask(CR_KI_SW, VIFInitialIn_inst.VifCrKi<<4, 0xF0);  // ki software mode
                    }
                    msWriteByteMask(CR_KF_SW, 0x00, 0x0F);                              // kf software mode
                    msWriteBit(CR_K_SEL, 1, _BIT6);	                                    // kp,ki,kf

                    if (VIFInitialIn_inst.VifCrKpKiAdjust)
                    {
                         //if(g_VifHWKpKiFlag == 1)
                         if((msReadByte(HALVIFDBG2_BIT) & 0x01) != 0)
                         {
                             msWriteBit(KPKI_ADJ_EN, 1, _BIT0);                 // real HW_KPKI_enable
                             KPKI_GEAR = msReadByte(CR_KPKI_GEAR) & 0x30;
                         }
                    }

                    msVifWriteByte(CR_PD_ERR_MAX_L, VIFInitialIn_inst.VifCrPdErrMax);      // CR pd error max
                    msWriteByteMask(CR_PD_ERR_MAX_H, VIFInitialIn_inst.VifCrPdErrMax>>8, 0x3F);
                    msVifWriteByte(CR_UNLOCK_NUM, VIFInitialIn_inst.VifCrUnlockNum);       // CR unlock num
                    msVifWriteByte(CR_UNLOCK_NUM+1, VIFInitialIn_inst.VifCrUnlockNum>>8);
                    msWriteByteMask(CR_UNLOCK_NUM+2, VIFInitialIn_inst.VifCrUnlockNum>>16, 0x0F);

                    // over modulation
                    if ((VIFInitialIn_inst.VifOverModulation == 1) && (g_bCheckModulationType == 0))
                    {
                        msWriteBit(VNCO_INV_OREN, 1, _BIT1);
                        msWriteBit(VNCO_INV_OV, 0, _BIT2);
                    }

                    g_ucVifStatusStep = VIF_STEADY_STATUS;
                }
            }

            if (!(_bit0_(msReadByte(CR_LOCK_STATUS))))
                msVifInitial();
            break;

        case VIF_STEADY_STATUS:
            // Dynamic TOP adjust for strong signal
            if (VIFInitialIn_inst.VifDynamicTopAdjust)
            {
                msVifDynamicTopAdjust();
            }

            // AM hum detector
            agc_vga = msRead2Bytes(AGC_VGA);
            dagc1_var = msReadByte(DAGC1_VAR+1);
            if ((VIFInitialIn_inst.VifAmHumDetection == 1) && ((agc_vga > VIFInitialIn_inst.VifVgaMinimum) || (agc_vga < (VIFInitialIn_inst.GainDistributionThr - 0x1000))))
            {
                if ((dagc1_var >= 0x18) && (g_bCheckModulationType == 0))
                {
                    // 20% AM modulation
                    msVifWriteByte(AGC_REF, 0x43);                                     // AGC ref
                }
                else if ((dagc1_var <= 0x05) && (g_bCheckModulationType == 0))
                {
                    // 10% AM modulation
                    msVifWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefNegative);      // AGC ref
                }
            }

            // AGC
            mean16 = (BYTE)(msRead2Bytes(AGC_MEAN16)>>1);                // AGC mean16
            if (g_bCheckModulationType == 0)
            {
	          if ((mean16 < AGC_MEAN16_UPBOUND) && (mean16 > AGC_MEAN16_LOWBOUND))
                {
                    msWriteByteMask(AGC_K, 0x04, _BIT0|_BIT1|_BIT2);    // k
		  }
                else
                {
                    if (VIFInitialIn_inst.VifTunerType == 1)
                        msWriteByteMask(AGC_K, 0x03, _BIT0|_BIT1|_BIT2);    // k
                    else
                        msWriteByteMask(AGC_K, 0x02, _BIT0|_BIT1|_BIT2);    // k
		  }
            }

            // CR monitor
            agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
            if ((agc_pga2 >= 0x0F) && (VIFInitialIn_inst.VifCrPdModeSel == 1))
            {
                msWriteByteMask(CR_KP_SW,  (VIFInitialIn_inst.VifCrKp)+0x01, 0x0F);     // kp software mode
                msWriteByteMask(CR_KI_SW, (VIFInitialIn_inst.VifCrKi<<4)+0x10, 0xF0);  // ki software mode
            }
            else
            {
                if (VIFInitialIn_inst.VifCrKpKiAdjust)
                {
                    //if(g_VifHWKpKiFlag == 1)
                    if((msReadByte(HALVIFDBG2_BIT) & 0x01) != 0)
                    {
                        g_VifCrKp = VIFInitialIn_inst.VifCrKp;
                        g_VifCrKi = VIFInitialIn_inst.VifCrKi;
                        msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                        msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);

                        msWriteBit(KPKI_ADJ_EN, 1, _BIT0);                 // real HW_KPKI_enable
                        KPKI_GEAR = msReadByte(CR_KPKI_GEAR) & 0x30;

                        if(KPKI_GEAR != 0)                                          // If carrier drift
                        {
                            g_VifCrKpKiGearCnt1 = 0;
                            g_VifCrKpKiGearCnt2++;

                            if(g_VifCrKpKiGearCnt2 == KPKI_GEAR_CNT_THR2)
                            {
                                msWriteBit(CR_LPF_SEL, 0, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF

                                if(KPKI_GEAR == 0x20)
                                {
                                   msWriteByteMask(CR_KP_SCALE, _BIT0 |_BIT2, _BIT0 |_BIT1 |_BIT2);
                                   msWriteByteMask(CR_KI_SCALE, _BIT4 |_BIT6, _BIT4 |_BIT5 |_BIT6);
                                }

                                g_VifCrKpKiGearCnt2 = 0;
                            }
                         }
                         else
                         {
                             g_VifCrKpKiGearCnt1++;
                             g_VifCrKpKiGearCnt2 = 0;

                             if(g_VifCrKpKiGearCnt1 == KPKI_GEAR_CNT_THR1)
                             {
                                 msWriteBit(CR_LPF_SEL, 0, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF
                                 msWriteByteMask(CR_KP_SCALE, _BIT0 |_BIT1, _BIT0 |_BIT1 |_BIT2);
                                 msWriteByteMask(CR_KI_SCALE, _BIT4 |_BIT5, _BIT4 |_BIT5 |_BIT6);

                                 g_VifCrKpKiGearCnt1 = 0;
                             }
                         }

                         if(KPKI_GEAR == 0)
                         {
                             CNT_IDX = 0;
                         }
                         else
                         {
                             if(CNT_IDX == 1500)
                             {
                                 msWriteBit(CR_PD_IMAG_INV, 0, _BIT1);             // for > 150% overmodulation
                                 CNT_IDX = 0;
                             }
                             CNT_IDX++;
                         }
                    }
                    else
                    {
                        msWriteBit(KPKI_ADJ_EN, 0, _BIT0);                 // real HW_KPKI_disable

                        for(crjtr_det_avgcnt = 0; crjtr_det_avgcnt < 1024; crjtr_det_avgcnt++)
                           msVifCrKpKiAutoAdjust(VIFInitialIn_inst.VifCrKpKiAdjustThr1, VIFInitialIn_inst.VifCrKpKiAdjustThr2);

                        if(g_VifCrKp == VIFInitialIn_inst.VifCrKp)
                        {
			  g_VifKpKiCnt1++;
			  g_VifKpKiCnt2 = 0;

			  if(g_VifKpKiCnt1 == KpKi_THR1)
			  {
				g_VifCrKp = VIFInitialIn_inst.VifCrKp;
                                   g_VifCrKi = VIFInitialIn_inst.VifCrKi;
				msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                                   msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);
                                   msWriteBit(CR_LPF_SEL, 0, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF
                                   msWriteByteMask(CR_KP_SCALE, _BIT0 |_BIT1, _BIT0 |_BIT1 |_BIT2);
                                   msWriteByteMask(CR_KI_SCALE, _BIT4 |_BIT5, _BIT4 |_BIT5 |_BIT6);

                                   g_VifKpKiCnt1 = 0;
                                   msWriteBit(CR_PD_IMAG_INV, 1, _BIT1);
			  }
                         }
                         else
                         {
			  g_VifKpKiCnt1 = 0;
			  g_VifKpKiCnt2++;

			  if(g_VifKpKiCnt2 == KpKi_THR2)
			  {
			      msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                                  msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);
                                  msWriteBit(CR_LPF_SEL, 0, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF
                                  msWriteByteMask(CR_KP_SCALE, _BIT0 |_BIT2, _BIT0 |_BIT1 |_BIT2);
                                  msWriteByteMask(CR_KI_SCALE, _BIT4 |_BIT6, _BIT4 |_BIT5 |_BIT6);

                                  g_VifKpKiCnt2 = 0;
                                  msWriteBit(CR_PD_IMAG_INV, 0, _BIT1);
                              }
                         }
                     }
                }
                else
                {
                    msWriteByteMask(CR_KP_SW, VIFInitialIn_inst.VifCrKp, 0x0F);         // kp software mode
                    msWriteByteMask(CR_KI_SW, VIFInitialIn_inst.VifCrKi<<4, 0xF0);      // ki software mode
                }
            }


            if(g_VifChannelScanFlag == 0)  // not channel Scan
            {
                 g_VifVDStableFlag = RIU_ReadByte(0x35CCL)&0x01;
                 g_VifVideoStandard = RIU_ReadByte(0x35CCL)&0xE0;

                 if(g_bCheckModulationType == 0)  // negative modulation
                 {
                     if(g_VifVDStableFlag == 0x01)    // VD stable
                     {
                          if(g_VifVideoStandard == 0x20) // SECAM
                          {
                              msVifWriteByte(CLAMPGAIN_GAIN_OVERWRITE, 0x00);    // clampgain gain overwrite value
                              msWriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, 0x04, 0x07);

                              msWriteBit(BYPASS_N_A5, 1, _BIT3);                        //Notch_A5 filter bypass
                              msWriteBit(BYPASS_CO_A_REJ, 0, _BIT4);                // CO_A_REJ bypass
                              //msWriteBit(BYPASS_CO_A_REJ, 1, _BIT4);

                              if((g_ucVifSoundSystemType == VIF_SOUND_B) || (g_ucVifSoundSystemType == VIF_SOUND_GH))
                              {
                                  msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_SECAM_BG_NOTCH);
                              }
                              else if((g_ucVifSoundSystemType >= VIF_SOUND_DK1)&&(g_ucVifSoundSystemType <= VIF_SOUND_DK3))
                              {
                                  msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_SECAM_DK_NOTCH);
                              }
                          }
                      }
                  }
              }

            if(!(_bit0_(msReadByte(CR_LOCK_STATUS))))
            {
                HAL_VIF_Delay1ms(50);                           // for Fluke 54200 50dBuV <-> 51dBuV switch
                if(!(_bit0_(msReadByte(CR_LOCK_STATUS))))
                {
                    // for debug
                    if (msReadByte(HALVIFDBG_BIT) & 0x08)
                    {
                        printk("VIF msVifInitial!!!");
                    }
                    msVifInitial();
                }
            }

            // for debug
            if (msReadByte(HALVIFDBG_BIT) & 0x40)
            {
                if (VIFInitialIn_inst.VifCrKpKiAdjust==1 )
                {
                    VIFInitialIn_inst.VifCrKpKiAdjust=0;
                }
                printk("\r\n Disable VIF KpKi auto adjust");
            }

            // for debug
            if (msReadByte(HALVIFDBG_BIT) & 0x80)
            {
                U8 ir_rate;

                // IR Rate
                ir_rate = msReadByte(IF_RATE);
                if (ir_rate==0x49)
                    printk("\r\n IF_FREQ_3395 IF_FREQ_3890");
                else if (ir_rate==0xE3)
                    printk("\r\n IF_FREQ_3800");
                else if (ir_rate==0x8E)
                    printk("\r\n IF_FREQ_3950");
                else if (ir_rate==0xAA)
                    printk("\r\n IF_FREQ_4575");
                else if (ir_rate==0xC7)
                    printk("\r\n IF_FREQ_5875");
                else
                    printk("\r\n unknown");

                printk(" IR_RATE=0x%x ", ir_rate);

                // sound system
                if (g_ucVifSoundSystemType==0)
                    printk("\r\n VIF_SOUND_B");
                else if (g_ucVifSoundSystemType==1)
                    printk("\r\n VIF_SOUND_B_NICAM");
                else if (g_ucVifSoundSystemType==2)
                    printk("\r\n VIF_SOUND_GH");
                else if (g_ucVifSoundSystemType==3)
                    printk("\r\n VIF_SOUND_GH_NICAM");
                else if (g_ucVifSoundSystemType==4)
                    printk("\r\n VIF_SOUND_I");
                else if (g_ucVifSoundSystemType==5)
                    printk("\r\n VIF_SOUND_DK1");
                else if (g_ucVifSoundSystemType==6)
                    printk("\r\n VIF_SOUND_DK2");
                else if (g_ucVifSoundSystemType==7)
                    printk("\r\n VIF_SOUND_DK3");
                else if (g_ucVifSoundSystemType==8)
                    printk("\r\n VIF_SOUND_DK_NICAM");
                else if (g_ucVifSoundSystemType==9)
                    printk("\r\n VIF_SOUND_L");
                else if (g_ucVifSoundSystemType==10)
                    printk("\r\n VIF_SOUND_LL");
                else if (g_ucVifSoundSystemType==11)
                    printk("\r\n VIF_SOUND_MN");
                else
                    printk("\r\n unknown");

                printk(" sound system=%d", (BYTE)g_ucVifSoundSystemType);


                // freq band select
                printk("\r\n band=%d", (BYTE)VIFInitialIn_inst.VifFreqBand);
            }
            break;

        default:
            g_ucVifStatusStep++;
            break;
    }

    if ((g_ucVifSoundSystemType == VIF_SOUND_L) || (g_ucVifSoundSystemType == VIF_SOUND_LL))
    {
        if (g_bCheckModulationType == 0)
            msVifInitial();
        if ((g_ucVifSoundSystemType == VIF_SOUND_L) && (g_bCheckIFFreq == 1))
            msVifInitial();
        if ((g_ucVifSoundSystemType == VIF_SOUND_LL) && (g_bCheckIFFreq == 0))
            msVifInitial();
    }
    else
    {
        if (g_bCheckModulationType == 1)
            msVifInitial();
    }
}

void msVifCrKpKiAutoAdjust(BYTE VifCrKpKiAdjustThr1, BYTE VifCrKpKiAdjustThr2)
{
    S16 CrJtrMax, CrJtrMin;
    static DWORD CrJtrDelta;

    HALVIFDBG(printk("\r\msVifCrKpKiAutoAdjust()"));

    msWriteBit(CR_STATUS_LATCH_EN, 1, _BIT4);                  // latch CR loop-filter

    msWriteByteMask(CR_JTR_SEL, 0, _BIT3|_BIT2|_BIT1|_BIT0);         // 0: max
    CrJtrMax = msRead2Bytes(CR_JTR_OUT);

    msWriteByteMask(CR_JTR_SEL, _BIT0, _BIT3|_BIT2|_BIT1|_BIT0);     // 1: min
    CrJtrMin = msRead2Bytes(CR_JTR_OUT);

    msWriteBit(CR_STATUS_LATCH_EN, 0, _BIT4);                  // un-latch CR loop-filter status

    if(g_VifCrKpKiAdjLoopCnt == 0)                           // reset delta value
        CrJtrDelta = 0;

    CrJtrDelta += (DWORD)(CrJtrMax - CrJtrMin);

    if (++g_VifCrKpKiAdjLoopCnt == 1024)                       // 1024 samples
    {
        CrJtrDelta = CrJtrDelta >> 10;                       // divided by 1024
        CrJtrDelta = CrJtrDelta >> 7;
        if (g_VifCrKp >= VIFInitialIn_inst.VifCrKp)
        {
            if (CrJtrDelta >= VifCrKpKiAdjustThr2)
            {
                g_VifCrKp -= 0x02;
                g_VifCrKi -= 0x02;
            }
	      else if ((CrJtrDelta < VifCrKpKiAdjustThr2) && (CrJtrDelta >= VifCrKpKiAdjustThr1))
            {
                g_VifCrKp -= 0x01;
                g_VifCrKi -= 0x01;
            }
        }
        else if (g_VifCrKp == VIFInitialIn_inst.VifCrKp - 1)
	 {
            if (CrJtrDelta >= VifCrKpKiAdjustThr2)
	     {
                g_VifCrKp -= 0x01;
                g_VifCrKi -= 0x01;
            }
            else if (CrJtrDelta < VifCrKpKiAdjustThr1 - 1)
	     {
                g_VifCrKp += 0x01;
                g_VifCrKi += 0x01;
	     }
	  }
	  else if (g_VifCrKp == VIFInitialIn_inst.VifCrKp - 2)
	  {
            if (CrJtrDelta < VifCrKpKiAdjustThr1 - 1)
	     {
                g_VifCrKp += 0x02;
                g_VifCrKi += 0x02;
	     }
            else if (CrJtrDelta < VifCrKpKiAdjustThr2 - 3)
	     {
                g_VifCrKp += 0x01;
                g_VifCrKi += 0x01;
	     }
        }

        g_VifCrKpKiAdjLoopCnt = 0;
        if (msReadByte(HALVIFDBG_BIT) & 0x20)
        {
            printk("\r\ng_ucVifStatusStep = %d", g_ucVifStatusStep);
            printk("\r\n Ki/Kp = %x%x", g_VifCrKi, g_VifCrKp);
            printk("\r\n CrJtrMax = %x", CrJtrMax >> 7);
            printk("\r\n CrJtrMin = %x", CrJtrMin >> 7);
            printk("\r\nCrJtrDelta = %x", (WORD)((CrJtrDelta & 0xFFFF0000) >> 16));
            printk("%x\r\n",(WORD)(CrJtrDelta & 0x0000FFFF));
	 }
    }

 }

void msVifLoadEQCoeff(BYTE VifSoundStandard)
{
    U8 u8index;

    HALVIFDBG(printf("\r\n msVifLoadEQCoeff()"));

    // set coef
    RIU_WriteByte(0x120A0L, 0x01);                      // VIF use DVB SRAM and FIR
    RIU_WriteByte(0x120A1L, 0x00);                      // VIF use DVB SRAM and FIR
    RIU_WriteByteMask(0x120A2L, 0x01, 0x0F);    // reg_vif_fir_coef_ctrl
    RIU_WriteByteMask(0x120A2L, 0x03, 0x0F);    // reg_vif_fir_coef_ctrl
    msWriteBit(BYPASS_EQFIR , 1 , _BIT0);     // Bypass EQ before loading coeff

    if((VifSoundStandard >= VIF_SOUND_B)&&(VifSoundStandard <= VIF_SOUND_LL))
    {
        for(u8index = 0; u8index < 46; ++u8index)
        {
            RIU_Write2Byte(0x120A4, VIF_PAL_EQ_CO_A_REJ[u8index]+0x8000);
            RIU_Write2Byte(0x120A4, VIF_PAL_EQ_CO_A_REJ[u8index]);
        }
    }
    else
    {
        for(u8index = 0; u8index < 46; ++u8index)
        {
            RIU_Write2Byte(0x120A4, VIF_NTSC_EQ_CO_A_REJ[u8index]+0x8000);
            RIU_Write2Byte(0x120A4, VIF_NTSC_EQ_CO_A_REJ[u8index]);
        }
    }
    msWriteBit(BYPASS_EQFIR , 0 , _BIT0);     // EQ not BYPASS
}

void msVifShiftClk(BYTE VifShiftClk)
{
   if (VifShiftClk == 1)
   {
	 g_VifShiftClk = 1;

        msVifWriteByte(0x12866L, 0x00);//loop divider
        msVifWriteByte(0x12867L, 0x23);
        if (VIFInitialIn_inst.VifTunerType == 0)
        {
	     // move to clk 42 Mhz
            msVifWriteByte(CR_RATE, 0x6D);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0xDB);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

	     // move to clk 140 Mhz
            msVifWriteByte(IF_RATE, 0xA8);                 			  // IF rate for 23 MHz
            msVifWriteByte(IF_RATE+1, 0x83);
            msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
        }
    }
    else if(VifShiftClk == 2)
    {
        g_VifShiftClk = 2;

        msVifWriteByte(0x12866L, 0x00);//loop divider
        msVifWriteByte(0x12867L, 0x25);
        if (VIFInitialIn_inst.VifTunerType == 0)
        {
			// move to clk 44.4 Mhz
            msVifWriteByte(CR_RATE, 0x22);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0x9F);
            msWriteByteMask(CR_RATE+2, 0x15, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

			// move to clk 148 Mhz
            msVifWriteByte(IF_RATE, 0x29);                 									// IF rate for 23 MHz
            msVifWriteByte(IF_RATE+1, 0xF2);
            msWriteByteMask(IF_RATE+2, 0x09, 0x3F);
       }
    }
    else
    {
	 g_VifShiftClk = 0;

        msVifWriteByte(0x12866L, 0x00);//loop divider
        msVifWriteByte(0x12867L, 0x24);
        if (VIFInitialIn_inst.VifTunerType == 0)
        {
	     // move to clk 43.2 Mhz
            msVifWriteByte(CR_RATE, 0xE3);                                     // cr_rate for 15 MHz
            msVifWriteByte(CR_RATE+1, 0x38);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

	      // move to clk 142 Mhz
            msVifWriteByte(IF_RATE, 0xE3);                 									// IF rate for 23 MHz
            msVifWriteByte(IF_RATE+1, 0x38);
            msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
        }
     }
}

U8 msVifReadCRFOE(void)
{
    HALVIFDBG(printk("\r\nmsVifReadCRFOE()"));

    if (!_hal_VIF.bBaseAddrInitialized) return 0;

    return msReadByte(CR_FOE);
}

U8 msVifReadLockStatus(void)
{
    HALVIFDBG(printk("\r\nmsVifReadLockStatus()"));

    if (!_hal_VIF.bBaseAddrInitialized) return 0;

    return msReadByte(CR_LOCK_STATUS);
}

void MHAL_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    HALVIFDBG(printk("HAL_VIF_BypassDBBAudioFilter() bEnableq=%d\n",bEnable));
    msWriteBit(A_DAGC_SEL, (!bEnable), _BIT7);  // 0: input from a_sos; 1: input from a_lpf_up
}

BOOL MHAL_VIF_GetInputLevelIndicator(void)
{
    BYTE ref, mean256, diff;

    HALVIFDBG(printk("\r\nHAL_VIF_GetInputLevelIndicator()"));

    ref = msReadByte(AGC_REF);                         // AGC ref
    mean256 = (BYTE)(msRead2Bytes(AGC_MEAN256)>>1);     // AGC mean256

    if (g_bCheckModulationType == 0)
        diff = 0x15;                                // negative modulation
    else
        diff = 0x0A;                                // positive modulation

    if (mean256 >= (ref-diff))
        return 1;
    else
        return 0;
}

#endif //_MHAL_VIF_C_
