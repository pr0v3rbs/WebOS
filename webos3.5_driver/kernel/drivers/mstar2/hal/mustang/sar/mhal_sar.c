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

#include "mhal_sar.h"
#include "mhal_sar_reg.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define CH_MAX_ID   HAL_SAR_CH7

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void MHal_SAR_Init(void)
{

}

U8 MHal_SAR_GetChannelMaxId(void)
{
    return CH_MAX_ID;
}

B16 MHal_SAR_Config_ChannelBound(U8 u8Channel, SAR_BND_t *psarBndCfg)
{
    U16  wChannelAdcValue = 0;

    if(u8Channel >= CH_MAX_ID)
        return FALSE;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH1_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH1_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH1_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH1_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH1_UPB) = MHal_SAR_REG(REG_SAR_CH1_UPB) & ~_SAR_CH1_UPB;
            //MHal_SAR_REG(REG_SAR_CH1_UPB) = MHal_SAR_REG(REG_SAR_CH1_UPB) | (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH1_LOB) = (psarBndCfg->u8LoBnd);
            break;
        case HAL_SAR_CH2:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH2_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH2_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH2_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH3_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH2_UPB) = (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH2_LOB) = (psarBndCfg->u8LoBnd);
            break;
        case HAL_SAR_CH3:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH3_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH3_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH3_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH3_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH3_UPB) = (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH3_LOB) = (psarBndCfg->u8LoBnd);
            break;
        case HAL_SAR_CH4:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH4_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH4_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH4_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH4_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH4_UPB) = (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH4_LOB) = (psarBndCfg->u8LoBnd);
            break;
        case HAL_SAR_CH5:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH5_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH5_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH5_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH5_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH5_UPB) = (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH5_LOB) = (psarBndCfg->u8LoBnd);
            break;
        case HAL_SAR_CH6:

            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH6_UPB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH6_UPB, (U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByte((REG_SAR_CH6_LOB + 1), (U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByte(REG_SAR_CH6_LOB, (U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //MHal_SAR_REG(REG_SAR_CH6_UPB) = (psarBndCfg->u8UpBnd);
            //MHal_SAR_REG(REG_SAR_CH6_LOB) = (psarBndCfg->u8LoBnd);
            break;

    }

    return TRUE;
}

void MHal_SAR_Config_SingleChannel(U8 u8Channel)
{
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8Channel,_SAR_SINGLE_CH);
}

void MHal_SAR_Config_TriggerMode(B16 bMode)
{
    U8 u8Trigger = (bMode)? _SAR_LEVEL_TRIGGER : (~_SAR_LEVEL_TRIGGER);
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8Trigger,_SAR_LEVEL_TRIGGER);
}

void MHal_SAR_Config_SingleChannelEn(B16 bEnable)
{
    U8 u8ChEn = (bEnable)? _SAR_SINGLE_CH_EN : (~_SAR_SINGLE_CH_EN);
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8ChEn,_SAR_SINGLE_CH_EN);
}

void MHal_SAR_Config_ShotMode(U8 u8Mode)
{
    U8 u8ShotMode = (u8Mode)? _SAR_MODE : (~_SAR_MODE); // 1: Freerun, 0: One-Shot
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8ShotMode,_SAR_MODE);
}

void MHal_SAR_Config_Powerdown(B16 bEnable)
{
    U8 u8PDEn = (bEnable)? _SAR_PD : (~_SAR_PD);
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8PDEn,_SAR_PD);
}

void MHal_SAR_Config_Start(B16 bEnable)
{
    U8 u8Start = (bEnable)? _SAR_START : (~_SAR_START);
    HAL_SAR_WriteByte(REG_SAR_CTRL0,u8Start,_SAR_START);
}

void MHal_SAR_Config_ADCPowerdown(B16 bEnable)
{
    U8 u8ADCPDEn = (bEnable)? _SAR_ADC_PD : (~_SAR_ADC_PD); //sar atop power down. 1: power down, 0: enable sar atop
    HAL_SAR_WriteByte(REG_SAR_CTRL1,u8ADCPDEn,_SAR_ADC_PD);
}

void MHal_SAR_Config_FreeRun(B16 bEnable)
{
    U8 u8Freerun = (bEnable)? _SAR_FREERUN : (~_SAR_FREERUN); //sar atop freerun mode. 0: controlled by digital (default), 1: freerun
    HAL_SAR_WriteByte(REG_SAR_CTRL1,u8Freerun,_SAR_FREERUN);
}

void MHal_SAR_Config_Selection(B16 bEnable)
{
    U8 u8Sel = (bEnable)? _SAR_SEL : (~_SAR_SEL);
    HAL_SAR_WriteByte(REG_SAR_CTRL1,u8Sel,_SAR_SEL);
}

void MHal_SAR_Config_8Channel(B16 bEnable)
{
    U8 u8En8Ch = (bEnable)? _SAR_8CH_EN : (~_SAR_8CH_EN);     // 1: sar 8 channel, 0: sar 4 channel
    HAL_SAR_WriteByte(REG_SAR_CTRL1,u8En8Ch,_SAR_8CH_EN);
}

void MHal_SAR_Config_ClockSamplePeriod(U8 u8ClkSmpPrd)
{
    MHal_SAR_REG(REG_SAR_CKSAMP_PRD) = u8ClkSmpPrd;
}

void MHal_SAR_Config_IntMask(B16 bEnable)
{
    U8 u8IntMask = (bEnable)? _SAR_INT_MASK : (~_SAR_INT_MASK);
    HAL_SAR_WriteByte(REG_SAR_INT,u8IntMask,_SAR_INT_MASK);
}

void MHal_SAR_Config_IntClear(B16 bEnable)
{
    U8 u8IntClr = (bEnable)? _SAR_INT_CLR : (~_SAR_INT_CLR);
    HAL_SAR_WriteByte(REG_SAR_INT,u8IntClr,_SAR_INT_CLR);
}

void MHal_SAR_Config_IntForce(B16 bEnable)
{
    U8 u8IntForce = (bEnable)? _SAR_INT_FORCE : (~_SAR_INT_FORCE);
    HAL_SAR_WriteByte(REG_SAR_INT,u8IntForce,_SAR_INT_FORCE);
}

B16 MHal_SAR_GetIntStatus(void)
{
    U8 u8IntStatus;

    u8IntStatus = MHal_SAR_REG(REG_SAR_INT) & _SAR_INT_STATUS;
    return (u8IntStatus)? ENABLE : DISABLE;
}

U8 MHal_SAR_GetChannelADC(U8 u8Channel)
{
    U32 u32RegAddr = 0;
    U16  wChannelAdcValue = 0;


    if(u8Channel >= CH_MAX_ID)
        return HAL_SAR_ADC_DATA_MAX;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u32RegAddr = REG_SAR_ADC_CH1_DATA;
            break;
        case HAL_SAR_CH2:
            u32RegAddr = REG_SAR_ADC_CH2_DATA;
            break;
        case HAL_SAR_CH3:
            u32RegAddr = REG_SAR_ADC_CH3_DATA;
            break;
        case HAL_SAR_CH4:
            u32RegAddr = REG_SAR_ADC_CH4_DATA;
            break;
        case HAL_SAR_CH5:
            u32RegAddr = REG_SAR_ADC_CH5_DATA;
            break;
        case HAL_SAR_CH6:
            u32RegAddr = REG_SAR_ADC_CH6_DATA;
            break;
		default:
			 u32RegAddr = REG_SAR_ADC_CH1_DATA;
            break;
    }


    MHal_SAR_REG(REG_SAR_CTRL1) = MHal_SAR_REG(REG_SAR_CTRL1)| _SAR_LOAD_EN;
    wChannelAdcValue = (MHal_SAR_REG_2Byte(u32RegAddr) & _SAR_ADC_OUT_10BITMSK);
    wChannelAdcValue = ((wChannelAdcValue >> 2) & _SAR_ADC_OUT_8BITMSK); //return [9:2]

    return (U8) wChannelAdcValue;

}


void HAL_SAR_Config_AanlogInputSelect(U8 u8Channel, BOOL bEnable)
{
    U8 u8CtrlData=0,u8Mask=0;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH1 : (~_SAR_AISEL_CH1);
            u8Mask=_SAR_AISEL_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH2 : (~_SAR_AISEL_CH2);
            u8Mask=_SAR_AISEL_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH3 : (~_SAR_AISEL_CH3);
            u8Mask=_SAR_AISEL_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH4 : (~_SAR_AISEL_CH4);
            u8Mask=_SAR_AISEL_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH5 : (~_SAR_AISEL_CH5);
            u8Mask=_SAR_AISEL_CH5;
            break;
        default: return;
    }
	//printf("why enter here ? u8Channel= 0x%02bX  -> 0x%02bX Mask :0x%02bX \n",u8Channel,u8CtrlData,u8Mask);
    HAL_SAR_WriteByte(REG_SAR_AISEL, u8CtrlData, u8Mask);
}

void HAL_SAR_Config_OptputEnable(U8 u8Channel, BOOL bEnable)
{
    U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH1 : _SAR_OEN_GPIO_CH1;
            u8Mask = _SAR_OEN_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH2 : _SAR_OEN_GPIO_CH2;
            u8Mask = _SAR_OEN_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH3 : _SAR_OEN_GPIO_CH3;
            u8Mask = _SAR_OEN_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH4 : _SAR_OEN_GPIO_CH4;
            u8Mask = _SAR_OEN_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH5 : _SAR_OEN_GPIO_CH5;
            u8Mask = _SAR_OEN_GPIO_CH5;
            break;
        default: return;
    }
	//printk("REG_SAR_OEN_GPIO ? %x u8Channel= 0x%x  -> 0x%x Mask :0x%x \n",REG_SAR_OEN_SAR_GPIO , u8Channel,u8CtrlData,u8Mask);
    HAL_SAR_WriteByte(REG_SAR_OEN_SAR_GPIO, u8CtrlData, u8Mask);
}


void MHal_SAR_SwPackEnable(BOOL bEnable)
{
	//HAL_SAR_WriteByte(REG_SAR_SW_PACK_EN, 1, _SAR_SW_PACK_EN);
}

void MHal_SAR_SetInterrupt(void)
{
	MHal_SAR_REG(REG_SAR_INT_MASK)=0xFC;
	MHal_SAR_REG(REG_SAR_INT_CLR)=0xFC;
	MHal_SAR_REG(REG_SAR_INT_FORCE)=0xFC;
}

