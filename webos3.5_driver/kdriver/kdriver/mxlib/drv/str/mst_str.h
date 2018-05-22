#ifndef __MST_STR_H
#define __MST_STR_H

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/io.h>

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//#define STR_DEBUG
//#define STR_REG_RW_DEBUG
#define STR_MEASURE_TIME

#ifdef STR_DEBUG
#define STR_PRINT(fmt, args...)		printk("[%s][%06d]     " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define STR_PRINT(fmt, args...)
#endif

#ifdef STR_REG_RW_DEBUG
#define STR_REG_RW_PRINT(fmt, args...)		printk("[%s][%06d]     " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define STR_REG_RW_PRINT(fmt, args...)
#endif

typedef unsigned char   MS_BOOL;
typedef unsigned char	MS_U8;
typedef unsigned short	MS_U16;
typedef unsigned long	MS_U32;
typedef unsigned long long MS_U64;   
typedef signed long long MS_S64; 
typedef unsigned long   MS_PHYADDR;     

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#define RIU_MAP 0xFD000000
#define RIU_ADDR 0xFD000000

#define RIU		((unsigned short volatile *) RIU_MAP)
#define RIU8		((unsigned char  volatile *) RIU_MAP)

#define MST_MACRO_START	do {
#define MST_MACRO_END		} while (0)

#define CPU_BUS_INTERVAL                       0x20000000

/* PIU timer */
#define PM_ADDR(addr)              (*((volatile MS_U16*)(0xFD000000 + ((addr) << 1))))
#define REG_WR(_reg_, _val_)        do{ PM_ADDR(_reg_) = (_val_);  }while(0)
#define REG_RR(_reg_)               ({ PM_ADDR(_reg_);})
#define REG_PIU_TIMER0(_x_)         (0x003020 | (_x_ << 1))

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MDrv_STR_WriteByte(u32Bank, u32Reg, u8Val)					\
    MST_MACRO_START												\
    STR_REG_RW_PRINT("WRITE: Bank=0X%x, 8bit Reg Addr=0X%x, Val=0X%x\n", (u32Bank), (u32Reg), (u8Val));		\
    writeb((MS_U8)(u8Val), (void *)(RIU_ADDR + ((u32Bank+(u32Reg << 0)) << 1) - (u32Reg & 1)));	\
    MST_MACRO_END

#define MDrv_STR_Write2Byte(u32Bank, u32Reg, u16Val)					\
    MST_MACRO_START												\
    STR_REG_RW_PRINT("WRITE: Bank=0X%x, 16bit Reg Addr=0X%x, Val=0X%x\n", (u32Bank), (u32Reg), (u16Val));	\
    writew((MS_U16)(u16Val), (void *)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1)));	\
    MST_MACRO_END

#define MDrv_STR_WriteByteMsk(u32Bank, u32Reg, u8Val, u8MasK)			\
    MST_MACRO_START                                                                     			\
    STR_REG_RW_PRINT("WRITE: Bank=0X%x, 16bit Reg Addr=0X%x, Val0X%x, Mask=0X%x, ", (u32Bank), (u32Reg), (u8Val), (u8MasK));                   \
    STR_REG_RW_PRINT("Val&Mask=0X%x\n", (u8Val)& (u8MasK));                   	\
    writeb((MS_U8)(readb((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1))) & ~(u8MasK)) | ((u8Val) & (u8MasK)), (void *)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1)));	\
    MST_MACRO_END

#define MDrv_STR_Write2ByteMsk(u32Bank, u32Reg, u16Val, u16Mask)		\
    MST_MACRO_START												\
    STR_REG_RW_PRINT("WRITE: Bank=0X%x, 16bit Reg Addr=0X%x, Val=0X%x, Mask=0X%x, ", (u32Bank), (u32Reg), (u16Val), (u16Mask));                   \
    STR_REG_RW_PRINT("Val&Mask=0X%x\n", (u16Val) & (u16Mask));			\
    writew((MS_U16)(readw((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1))) & ~(u16Mask)) | ((u16Val) & (u16Mask)), (void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1)));  \
    MST_MACRO_END

#define MDrv_STR_ReadByte(u32Bank, u32Reg)							\
    readb((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1)))

#define MDrv_STR_Read2Byte(u32Bank, u32Reg)						\
    readw((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1)))

#define MDrv_STR_ReadByteMask(u32Bank, u32Reg, u8MaSK)				\
    readb((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1))) & (u8MaSK)

#define MDrv_STR_Read2ByteMsk(u32Bank, u32Reg, u16Mask)				\
    readw((void*)(RIU_ADDR + ((u32Bank+(u32Reg << 1)) << 1))) & (u16Mask)

#endif //End of  __MST_STR_H

