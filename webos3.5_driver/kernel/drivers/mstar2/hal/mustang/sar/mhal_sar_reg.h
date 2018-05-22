////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
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

#ifndef _SAR_REG_H_
#define _SAR_REG_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_CH1					0
#define HAL_SAR_CH2					1
#define HAL_SAR_CH3					2
#define HAL_SAR_CH4					3
#define HAL_SAR_CH5					4
#define HAL_SAR_CH6					5
#define HAL_SAR_CH7					6
#define HAL_SAR_CH8					7
#define HAL_SAR_MAX_CHANNELS			(HAL_SAR_CH8+1)
#define HAL_SAR_ADC_DATA_MAX			0xFF

#define HAL_SAR_LEVEL					1
#define HAL_SAR_EDGE					0
#define HAL_SAR_FREERUN				1
#define HAL_SAR_ONESHOT				0
#define HAL_SAR_ADC_POWERDOWN			1
#define HAL_SAR_ADC_POWERUP			0

// defination for DISABLE
#define DISABLE							0
// defination for ENABLE
#define ENABLE							1

// base address
#define REG_MIPS_BASE					0xFD000000
#define REG_SAR_BASE					(0xA00<<2)

// mask bits
#define MASK_SAR_ADCOUT				0xFF    //sar data resilotion 8 bits
#define MASK_SAR_SINGLE_CH			0x07

// register definition
#define REG_SAR_CTRL0					(REG_SAR_BASE + (0x00<<2))
#define _SAR_SINGLE_CH					(BIT2|BIT1|BIT0)//[2:0] //select channel for single channel modech0~ch7
#define _SAR_LEVEL_TRIGGER			BIT3 //keypad level trigger enable. 0:  use edge trigger, 1:  use level trigger
#define _SAR_SINGLE_CH_EN				BIT4 //enable single channel mode. 0: disable1: enable
#define _SAR_MODE						BIT5 //select sar digital operation mode. 0: one-shot, 1: freerun
#define _SAR_PD							BIT6 //sar digital power down
#define _SAR_START						BIT7 //sar start signal
#define _SAR_CH1_UPB					(BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)

#define REG_SAR_CTRL1					(REG_SAR_BASE + (0x00<<2) +1)
#define _SAR_ADC_PD					BIT0
#define _SAR_FREERUN				BIT1
#define _SAR_SEL					BIT2
#define _SAR_8CH_EN					BIT3
#define _SAR_LOAD_EN                BIT6

#define REG_SAR_CKSAMP_PRD			(REG_SAR_BASE + (0x01<<2))

#define REG_SAR_CH1_UPB				(REG_SAR_BASE + (0x20<<2))
#define REG_SAR_CH1_LOB				(REG_SAR_BASE + (0x30<<2))
#define REG_SAR_CH2_UPB				(REG_SAR_BASE + (0x21<<2))
#define REG_SAR_CH2_LOB				(REG_SAR_BASE + (0x31<<2))
#define REG_SAR_CH3_UPB				(REG_SAR_BASE + (0x22<<2))
#define REG_SAR_CH3_LOB				(REG_SAR_BASE + (0x32<<2) + 1)
#define REG_SAR_CH4_UPB				(REG_SAR_BASE + (0x23<<2))
#define REG_SAR_CH4_LOB				(REG_SAR_BASE + (0x33<<2) + 1)
#define REG_SAR_CH5_UPB				(REG_SAR_BASE + (0x24<<2))
#define REG_SAR_CH5_LOB				(REG_SAR_BASE + (0x34<<2) + 1)
#define REG_SAR_CH6_UPB				(REG_SAR_BASE + (0x25<<2))
#define REG_SAR_CH6_LOB				(REG_SAR_BASE + (0x35<<2) + 1)
#define REG_SAR_CH7_UPB				(REG_SAR_BASE + (0x26<<2))
#define REG_SAR_CH7_LOB				(REG_SAR_BASE + (0x36<<2) + 1)
#define REG_SAR_CH8_UPB				(REG_SAR_BASE + (0x27<<2))
#define REG_SAR_CH8_LOB				(REG_SAR_BASE + (0x37<<2) + 1)
#define _SAR_CHN_UPB_MSK                    (0xFF)
#define _SAR_CHN_LOB_MSK                    (0xFF)


#define REG_SAR_ADC_CH1_DATA			(REG_SAR_BASE + (0x40<<2))
#define REG_SAR_ADC_CH2_DATA			(REG_SAR_BASE + (0x41<<2))
#define REG_SAR_ADC_CH3_DATA			(REG_SAR_BASE + (0x42<<2))
#define REG_SAR_ADC_CH4_DATA			(REG_SAR_BASE + (0x43<<2))
#define REG_SAR_ADC_CH5_DATA			(REG_SAR_BASE + (0x44<<2))
#define REG_SAR_ADC_CH6_DATA			(REG_SAR_BASE + (0x45<<2))
#define REG_SAR_ADC_CH7_DATA			(REG_SAR_BASE + (0x46<<2))
#define REG_SAR_ADC_CH8_DATA			(REG_SAR_BASE + (0x47<<2))
    #define _SAR_ADC_OUT_10BITMSK               (0x3FF)
    #define _SAR_ADC_OUT_8BITMSK                (0x3FF)



#define REG_SAR_INT_MASK                        ((0x00*0x10000)+ (REG_SAR_BASE + (0x14<<2))) //Bit8~Bit0
    #define _SAR_INT_MASK_CH                    (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_INT_MASK_CH1                   (BIT0)
    #define _SAR_INT_MASK_CH2                   (BIT1)
    #define _SAR_INT_MASK_CH3                   (BIT2)
    #define _SAR_INT_MASK_CH4                   (BIT3)
    #define _SAR_INT_MASK_CH5                   (BIT4)

#define REG_SAR_INT_CLR                         ((0x00*0x10000)+ (REG_SAR_BASE + (0x15<<2))) //Bit8~Bit0
    #define _SAR_INT_CLR_CH                     (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_INT_CLR_CH1                    (BIT0)
    #define _SAR_INT_CLR_CH2                    (BIT1)
    #define _SAR_INT_CLR_CH3                    (BIT2)
    #define _SAR_INT_CLR_CH4                    (BIT3)
    #define _SAR_INT_CLR_CH5                    (BIT4)

#define REG_SAR_INT_FORCE                       ((0x00*0x10000)+ (REG_SAR_BASE + (0x16<<2))) //Bit8~Bit0
    #define _SAR_INT_FORCE_CH                   (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_INT_FORCE_CH1                  (BIT0)
    #define _SAR_INT_FORCE_CH2                  (BIT1)
    #define _SAR_INT_FORCE_CH3                  (BIT2)
    #define _SAR_INT_FORCE_CH4                  (BIT3)
    #define _SAR_INT_FORCE_CH5                  (BIT4)

#define REG_SAR_INT_STATUS                      ((0x00*0x10000)+ (REG_SAR_BASE + 0x17<<2)) //Bit8~Bit0
    #define _SAR_INT_STATUS_CH                  (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_INT_STATUS_CH1                 (BIT0)
    #define _SAR_INT_STATUS_CH2                 (BIT1)
    #define _SAR_INT_STATUS_CH3                 (BIT2)
    #define _SAR_INT_STATUS_CH4                 (BIT3)
    #define _SAR_INT_STATUS_CH5                 (BIT4)



//#define REG_SAR_SW_PACK_EN				(REG_SAR_BASE + (0x19<<2) + 1)
//#define _SAR_SW_PACK_EN                     (BIT0)


#define REG_SAR_AISEL					(REG_SAR_BASE + (0x11<<2))
#define _SAR_AISEL						(BIT4|BIT3|BIT2|BIT1|BIT0)
#define _SAR_AISEL_CH1					(BIT0)
#define _SAR_AISEL_CH2					(BIT1)
#define _SAR_AISEL_CH3					(BIT2)
#define _SAR_AISEL_CH4					(BIT3)
#define _SAR_AISEL_CH5					(BIT4)
#define REG_SAR_OEN_SAR_GPIO			(REG_SAR_BASE + (0x11<<2) + 1)
#define _SAR_OEN_SAR_GPIO				(BIT4|BIT3|BIT2|BIT1|BIT0)
#define _SAR_OEN_GPIO_CH1				(BIT0)
#define _SAR_OEN_GPIO_CH2				(BIT1)
#define _SAR_OEN_GPIO_CH3				(BIT2)
#define _SAR_OEN_GPIO_CH4				(BIT3)
#define _SAR_OEN_GPIO_CH5				(BIT4)
#define REG_SAR_I_SAR_GPIO			(REG_SAR_BASE + (0x12<<2))
#define REG_SAR_C_SAR_GPIO			(REG_SAR_BASE + (0x12<<2) + 1)
//#define REG_SAR_TEST0					(REG_SAR_BASE + (0x13<<2))
//#define REG_SAR_TEST1					(REG_SAR_BASE + (0x13<<2) + 1)

#define REG_SAR_INT					(REG_SAR_BASE + (0x14<<2))
#define _SAR_INT_MASK					BIT0
#define _SAR_INT_CLR					BIT1
#define _SAR_INT_FORCE					BIT2
#define _SAR_INT_STATUS				BIT3

#define MHal_SAR_REG(addr)			(*(volatile U8*)((REG_MIPS_BASE) + (addr)))
#define MHal_SAR_REG_2Byte(addr)			(*(volatile U16*)((REG_MIPS_BASE) + (addr)))

// write mask
#define HAL_SAR_WriteByte(_reg_, _val_, _msk_)    \
		do{ rmb();                         \
        MHal_SAR_REG(_reg_) = (MHal_SAR_REG(_reg_) & ~(_msk_)) | ((_val_) & (_msk_)); wmb(); }while(0)

#endif // _SAR_REG_H_
