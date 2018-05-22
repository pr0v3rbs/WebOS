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

#ifndef _MHAL_VIF_H_
#define _MHAL_VIF_H_

//#include <asm-mips/types.h>
//#include "mdrv_types.h"
#include "mdrv_vif_st.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


#define _BIT0                        0x00000001
#define _BIT1                        0x00000002
#define _BIT2                        0x00000004
#define _BIT3                        0x00000008
#define _BIT4                        0x00000010
#define _BIT5                        0x00000020
#define _BIT6                        0x00000040
#define _BIT7                        0x00000080
#define _BIT8                        0x00000100
#define _BIT9                        0x00000200
#define _BIT10                       0x00000400
#define _BIT11                       0x00000800
#define _BIT12                       0x00001000
#define _BIT13                       0x00002000
#define _BIT14                       0x00004000
#define _BIT15                       0x00008000

#define _bit0_(val)                (val & _BIT0)
#define _bit1_(val)                (val & _BIT1)
#define _bit2_(val)                (val & _BIT2)
#define _bit3_(val)                (val & _BIT3)
#define _bit4_(val)                (val & _BIT4)
#define _bit5_(val)                (val & _BIT5)
#define _bit6_(val)                (val & _BIT6)
#define _bit7_(val)                (val & _BIT7)
#define _bit8_(val)                (val & _BIT8)
#define _bit9_(val)                (val & _BIT9)
#define _bit10_(val)              (val & _BIT10)
#define _bit11_(val)              (val & _BIT11)
#define _bit12_(val)              (val & _BIT12)
#define _bit13_(val)              (val & _BIT13)
#define _bit14_(val)              (val & _BIT14)
#define _bit15_(val)              (val & _BIT15)

#define AGC_MEAN16_UPBOUND              0x8C     // 1.256*0xE0/2
#define AGC_MEAN16_LOWBOUND             0x13     // (1/4)*0x9A/2
#define AGC_MEAN16_UPBOUND_SECAM        0x28     // 1.256*0x40/2
#define AGC_MEAN16_LOWBOUND_SECAM       0x08     // (1/4)*0x40/2

#define FREQ_STEP_31_25KHz          	0x00
#define FREQ_STEP_50KHz             	0x01
#define FREQ_STEP_62_5KHz           	0x02
#define FREQ_STEP_75KHz           	  0x03

#define IF_FREQ_45_75                   0x01//IF_FREQ_MN
#define IF_FREQ_38_90                   0x02//IF_FREQ_B
#define IF_FREQ_38_00                   0x03//IF_FREQ_PAL_38



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


//Internal
void msVifSetIfFreq(IfFrequencyType ucIfFreq);
void msVifLoad(void);
void msVifInitial(void);
void msVifHandler(BOOL bVifDbbAcq);
void msVifSetSoundSystem(VIFSoundSystem ucSoundSystem);
void msVifAdcInitial(void);
void msVifTopAdjust(void);
void msVifCrKpKiAutoAdjust(BYTE VifCrKpKiAdjustThr1, BYTE VifCrKpKiAdjustThr2);
U8 msVifReadCRFOE(void);
U8 msVifReadLockStatus(void);
void msVifLoadEQCoeff(BYTE VifSoundStandard);
void msVifShiftClk(BYTE VifShiftClk);


//Public
void MHAL_VIF_RegInit (void);
void MHAL_VIF_SetClock(BOOL bEnable);
void MHAL_VIF_BypassDBBAudioFilter(BOOL bEnable);
BOOL MHAL_VIF_GetInputLevelIndicator(void);
void MHAL_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask);
void MHAL_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask);
void MHAL_VIF_WriteByte(U32 u32Reg, U8 u8Val );
U8 MHAL_VIF_ReadByte(U32 u32Reg );



#endif // _MHAL_VIF_H_
