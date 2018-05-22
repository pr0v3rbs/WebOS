//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRVGPIO_C__
#define __DRVGPIO_C__

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsTypes.h"
#include "drvGPIO.h"
#include "halGPIO.h"
#include "drvMMIO.h"
#if (GPIO_UTOPIA20)
#include "utopia.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GPIO_DBG_FUNC()             if (_geDbfLevel >= E_GPIO_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(printf("\t====   %s   ====\n", __FUNCTION__);)}
#define GPIO_DBG_INFO(x, args...)   if (_geDbfLevel >= E_GPIO_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(printf(x, ##args);)}
#define GPIO_DBG_ERR(x, args...)    if (_geDbfLevel >= E_GPIO_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(printf(x, ##args);)}

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static const struct gpio_operations *gpio_op;
extern const struct gpio_operations __gpio;

static MS_BOOL _gbInitGPIO = FALSE;
static GPIO_Info _gsInfo;
static GPIO_DbgLv _geDbfLevel = E_GPIO_DBGLV_NONE;
static MSIF_Version _drv_gpio_version = {
    .DDI = { GPIO_DRV_VERSION },
};

#if (GPIO_UTOPIA20)
MS_BOOL bU20GPIOOpened = FALSE;
void* pInstantGpio;
void* pAttributeGpio;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

GPIO_Mode_Str gpio_Info_Str[END_GPIO_NUM+1];

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
static MS_BOOL _GPIO_Is_Init(void)
{
    GPIO_DBG_FUNC();
    if (!_gbInitGPIO)
        GPIO_DBG_ERR("Call MDrv_GPIO_Init first!\n");
    return _gbInitGPIO;
}

MS_BOOL MDrv_GPIO_SetIOMapBase(void)
{
    MS_U32 u32MMIOBaseAdr;
    MS_U32 u32NonPMBankSize;

    GPIO_DBG_FUNC();

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_GPIO))
    {
        GPIO_DBG_ERR("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_GPIO_SetIOMapBase(u32MMIOBaseAdr);

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_PM))
    {
        GPIO_DBG_ERR("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_GPIO_SetIOMapBase_PM(u32MMIOBaseAdr);

    return TRUE;
}


//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
GPIO_Result MDrv_GPIO_GetLibVer(const MSIF_Version **ppVersion)
{
    GPIO_DBG_FUNC();

    if(!ppVersion)
        return E_GPIO_FAIL;

    *ppVersion = &_drv_gpio_version;
    return E_GPIO_OK;
}

GPIO_Result MDrv_GPIO_SetDbgLevel(GPIO_DbgLv eLevel)
{
    GPIO_DBG_INFO("%s level: %u\n", __FUNCTION__, eLevel);

    _geDbfLevel = eLevel;
    return E_GPIO_OK;
}

const GPIO_Info* MDrv_GPIO_GetInfo(void)
{
    GPIO_DBG_FUNC();

    if(!_GPIO_Is_Init())
        return (GPIO_Info *)0;
    _gsInfo.u32GPIONum = END_GPIO_NUM;
    _gsInfo.u32IOMap = 0;
    _gsInfo.u32IOMap_PM = 0;

    return (&_gsInfo);
}

void MDrv_GPIO_GetStatus(GPIO_Status *pStatus)
{
    GPIO_DBG_FUNC();

    if(!_GPIO_Is_Init())
        return;

    pStatus->bInit = _gbInitGPIO;
    pStatus->u8DbgLevel = _geDbfLevel;
}

void _mdrv_gpio_init(void)
{
    int i=0;
    GPIO_DBG_FUNC();

    _gbInitGPIO = TRUE;
    gpio_op = &__gpio;
    MDrv_GPIO_SetIOMapBase();
    HAL_GPIO_Interrupt_Init();

    for(i=0;i<=END_GPIO_NUM;i++)
    {
        gpio_Info_Str[i]=E_GPIO_STR_NONE;
    }
}


void _mdrv_gpio_set_high(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_high(gpio);
    }
    gpio_Info_Str[gpio]=E_GPIO_STR_SetHigh;
}

void _mdrv_gpio_set_low(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_low(gpio);
    }
    gpio_Info_Str[gpio]=E_GPIO_STR_SetLow;
}

void _mdrv_gpio_set_input(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_input(gpio);
    }
    gpio_Info_Str[gpio]=E_GPIO_STR_SetInput;
}
#if 0
void mdrv_gpio_set_output(MS_GPIO_NUM gpio)
{
    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        gpio_op->set_output(gpio);
    }
}
#endif
int _mdrv_gpio_get_inout(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_inout(gpio);
    }
    else
    {
        return 0;
    }
}

int _mdrv_gpio_get_level(MS_GPIO_NUM gpio)
{
    GPIO_DBG_FUNC();

    MS_ASSERT(gpio_op);
    if (gpio_op)
    {
        return gpio_op->get_level(gpio);
    }
    else
    {
        return 0;
    }
}

GPIO_Result mdrv_gpio_attach_interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Attach_Interrupt(gpio_num,gpio_edge_type,pCallback);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_detach_interrupt(MS_GPIO_NUM gpio_num)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Datach_Interrupt(gpio_num);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_enable_interrupt(MS_GPIO_NUM gpio)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Enable_Interrupt(gpio);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

GPIO_Result mdrv_gpio_disable_interrupt(MS_GPIO_NUM gpio)
{
    GPIO_Result ret_val;

    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        ret_val = HAL_GPIO_Disable_Interrupt(gpio);
    }
    else
    {
        ret_val = E_GPIO_FAIL;
    }

    return ret_val;
}

void mdrv_gpio_disable_interrupt_all(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Disable_Interrupt_all();
    }
}

void mdrv_gpio_enable_interrupt_all(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Enable_Interrupt_all();
    }
}

void mdrv_gpio_interrupt_action(void)
{
    GPIO_DBG_FUNC();
    if(_GPIO_Is_Init())
    {
        HAL_GPIO_Interrupt_Action();
    }
}

MS_U16 MDrv_GPIO_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U16 u16Return = FALSE;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _gbInitGPIO = FALSE;
        _prev_u16PowerState = u16PowerState;
        return 2;               // suspend OK
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            mdrv_gpio_init();
            _prev_u16PowerState = u16PowerState;
                        u16Return = 1;      // resume OK
        }
        else
        {
                         printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
                         u16Return = 3;     // suspend failed
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
    }

    return u16Return;
}

#if (GPIO_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_GPIO_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20GPIOOpened)
    {
        if(UtopiaOpen(MODULE_GPIO|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        //if(UtopiaOpen(MODULE_GPIO, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open GPIO fail\n");
            return FALSE;
        }
        bU20GPIOOpened = TRUE;
    }
    return TRUE;
}
#endif

void mdrv_gpio_init(void)
{
#if (GPIO_UTOPIA20)
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_INIT,(MS_U32*)NULL);
#else
     _mdrv_gpio_init();
#endif
}


void mdrv_gpio_set_high(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
    PGPIO_PRIVATE_PARAM pGpioParam_SetHigh = NULL;
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return;
    pGpioParam_SetHigh = (PGPIO_PRIVATE_PARAM)malloc(sizeof(GPIO_PRIVATE_PARAM));
    pGpioParam_SetHigh->eGpio=gpio;
    pGpioParam_SetHigh->level=NULL;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Set_HIGH,(void*)pGpioParam_SetHigh);
    free(pGpioParam_SetHigh);
#else
     _mdrv_gpio_set_high(gpio);
#endif
}

void mdrv_gpio_set_low(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
    PGPIO_PRIVATE_PARAM pGpioParam_SetLow = NULL;
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return;
    pGpioParam_SetLow = (PGPIO_PRIVATE_PARAM)malloc(sizeof(GPIO_PRIVATE_PARAM));
    pGpioParam_SetLow->eGpio=gpio;
    pGpioParam_SetLow->level=NULL;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Set_LOW,(MS_U32*)pGpioParam_SetLow);
    free(pGpioParam_SetLow);
#else
     _mdrv_gpio_set_low(gpio);
#endif
}
int mdrv_gpio_get_inout(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
    int value;
    PGPIO_PRIVATE_PARAM pGpioParam_GetInout = NULL;
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return 0;
    pGpioParam_GetInout = (PGPIO_PRIVATE_PARAM)malloc(sizeof(GPIO_PRIVATE_PARAM));
    pGpioParam_GetInout->eGpio=gpio;
    pGpioParam_GetInout->level=NULL;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Get_INOUT,(MS_U32*)pGpioParam_GetInout);
    value = pGpioParam_GetInout->level;
    free(pGpioParam_GetInout);
    return (int)value;
#else
     return _mdrv_gpio_get_inout(gpio);
#endif
}

void mdrv_gpio_set_input(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
    PGPIO_PRIVATE_PARAM pGpioParam_SetInput = NULL;
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return;
    pGpioParam_SetInput = (PGPIO_PRIVATE_PARAM)malloc(sizeof(GPIO_PRIVATE_PARAM));
    pGpioParam_SetInput->eGpio=gpio;
    pGpioParam_SetInput->level=NULL;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Set_INPUT,(void*)pGpioParam_SetInput);
    free(pGpioParam_SetInput);
#else
    _mdrv_gpio_set_input(gpio);
#endif
}

int mdrv_gpio_get_level(MS_GPIO_NUM gpio)
{
#if (GPIO_UTOPIA20)
    int value=0;
    PGPIO_PRIVATE_PARAM pGpioParam_GetLevel = NULL;
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return 0;
    pGpioParam_GetLevel = (PGPIO_PRIVATE_PARAM)malloc(sizeof(GPIO_PRIVATE_PARAM));
    pGpioParam_GetLevel->eGpio=gpio;
    pGpioParam_GetLevel->level=NULL;
    UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Get_LEVEL,(void*)pGpioParam_GetLevel);
    value = pGpioParam_GetLevel->level;
    free(pGpioParam_GetLevel);
    return (int)value;
#else
     return _mdrv_gpio_get_level(gpio);
#endif
}

//================================================================
static void gpio_delay(MS_U32 u32LoopDelay)
{
    while(u32LoopDelay--)
    {
    #ifdef __mips__
        __asm__ __volatile__ ("nop");
    #endif

    #ifdef __AEONR2__
        __asm__ __volatile__ ("l.nop");
    #endif

    #ifdef __arm__
        __asm__ __volatile__ ("mov r0, r0");
    #endif
    }
}

#define GPIO_CHECK_PIN_DUMMY 3200
#define GPIO_ACKNOWLEDGE 0
#define GPIO_LOW 0
#define GPIO_HIGH 1

static int gpio_scl_get_level(MS_U32 u32padSCL)
{
    return _mdrv_gpio_get_level(u32padSCL) ? GPIO_HIGH : GPIO_LOW;
}

static int gpio_sda_get_level(MS_U32 u32padSDA)
{
    return _mdrv_gpio_get_level(u32padSDA) ? GPIO_HIGH : GPIO_LOW;
}

static void gpio_scl_check_high(MS_U32 u32padSCL)
{
    MS_U16 u16Dummy = GPIO_CHECK_PIN_DUMMY;

    _mdrv_gpio_set_input(u32padSCL);

    while (u16Dummy--)
    {
        if(_mdrv_gpio_get_level(u32padSCL) == GPIO_HIGH)
            break;
    }
}

static void gpio_scl_check_low(MS_U32 u32padSCL)
{
    MS_U16 u16Dummy = GPIO_CHECK_PIN_DUMMY;

    _mdrv_gpio_set_low(u32padSCL);

    while (u16Dummy--)
    {
        if(_mdrv_gpio_get_level(u32padSCL) == GPIO_LOW)
            break;
    }
}

static void gpio_sda_check_high(MS_U32 u32padSDA)
{
    MS_U16 u16Dummy = GPIO_CHECK_PIN_DUMMY;

    _mdrv_gpio_set_input(u32padSDA);

    while (u16Dummy--)
    {
        if(_mdrv_gpio_get_level(u32padSDA) == GPIO_HIGH)
            break;
    }
}

MS_BOOL _mdrv_gpio_bus_start(GPIO_Bus_Data *pBusData)   // Start
{
    MS_BOOL bStatus = TRUE;    // success status

    MS_U32 u32padSDA;
    MS_U32 u32LoopDelay;

    u32padSDA=pBusData->padSDA;
    u32LoopDelay=pBusData->u32LoopDelay;

    gpio_sda_check_high(u32padSDA);
    gpio_delay(u32LoopDelay);

    gpio_scl_check_high(pBusData->padSCL);
    gpio_delay(pBusData->u32LoopDelay);

    // check pin error
    _mdrv_gpio_set_input(pBusData->padSCL);
    _mdrv_gpio_set_input(pBusData->padSDA);

    if ((gpio_scl_get_level(pBusData->padSCL) == GPIO_LOW) || (gpio_sda_get_level(pBusData->padSDA) == GPIO_HIGH))
    {
        _mdrv_gpio_set_high(pBusData->padSCL);
        _mdrv_gpio_set_high(pBusData->padSDA);
        bStatus = FALSE;
    }
    else // success
    {
        _mdrv_gpio_set_low(pBusData->padSDA);
        gpio_delay(pBusData->u32LoopDelay);
        _mdrv_gpio_set_low(pBusData->padSCL);
    }

    return bStatus;     //vain
}

static MS_BOOL SendByte(MS_U32 padSCL,MS_U32 padSDA,MS_U32 u32LoopDelay,MS_BOOL ack,MS_U8 u8dat,MS_U16 u16Retry)   // Be used int IIC_SendByte
{
    MS_U8 u8Mask = 0x80;
    int bAck; // acknowledge bit
    MS_U8 i;


    for(i=0;i<u16Retry;i++)
    {
        u8Mask = 0x80;

        while ( u8Mask )
        {
            if (u8dat & u8Mask)
            {
                _mdrv_gpio_set_input(padSDA);
            }
            else
            {
                _mdrv_gpio_set_low(padSDA);
            }

            gpio_delay(u32LoopDelay);
            gpio_scl_check_high(padSCL);
            gpio_delay(u32LoopDelay);
            gpio_scl_check_low(padSCL);

            u8Mask >>= 1; // next
        }
        bAck=0;
        if(ack)
        {
            // recieve acknowledge
            _mdrv_gpio_set_input(padSDA);
            gpio_delay(u32LoopDelay);
            gpio_scl_check_high(padSCL);

            gpio_delay(u32LoopDelay);
            bAck = _mdrv_gpio_get_level(padSDA); // recieve acknowlege
            _mdrv_gpio_set_low(padSCL);

            gpio_delay(u32LoopDelay);

            //for I2c waveform sharp
            if (bAck)
                _mdrv_gpio_set_high(padSDA);
            else
                _mdrv_gpio_set_low(padSDA);
        }


        _mdrv_gpio_set_input(padSDA);

        gpio_delay(u32LoopDelay);

        if(bAck==0)
            return TRUE;
    }
    return FALSE;
}

MS_BOOL _mdrv_gpio_send_bus_data(GPIO_Bus_Data *pBusData)   //Write Byte
{
    MS_U16 i;
    MS_BOOL bRet=TRUE;


    MS_U16 u16size=pBusData->u16byte;
    //MS_U8 Buf[u16size];
    //memcpy(Buf,(pBusData->pBuf),u16size);

    for(i=0;i<pBusData->u16byte;i++)
    {
        bRet=SendByte(pBusData->padSCL,pBusData->padSDA,pBusData->u32LoopDelay,pBusData->ack,pBusData->pBuf[i],pBusData->u16Retry);
        if(bRet==FALSE)
            break;
    }
    return bRet;
}

static MS_U8 GetByte(MS_U32 padSCL,MS_U32 padSDA,MS_U32 u32LoopDelay,MS_BOOL ack,MS_U16 u16Ack)
{
    MS_U8 u8Mask = 0x80;
    MS_U8 u8Receive = 0;

    _mdrv_gpio_set_input(padSDA);
     u8Mask = 0x80;
    while(u8Mask)
    {
        gpio_delay(u32LoopDelay);
        gpio_scl_check_high(padSCL);
        gpio_delay(u32LoopDelay);
        if (_mdrv_gpio_get_level(padSDA) == GPIO_HIGH)
        {
            u8Receive |= u8Mask;
        }
        u8Mask>>=1;
        _mdrv_gpio_set_low(padSCL);
        gpio_scl_check_low(padSCL);
    }

    if(ack)
    {
        if (u16Ack)
        {
            _mdrv_gpio_set_low(padSDA);    // acknowledge
        }
        else
        {
            gpio_sda_check_high(padSDA);  // non-acknowledge
        }
    }

    gpio_delay(u32LoopDelay);
    gpio_scl_check_high(padSCL);
    gpio_delay(u32LoopDelay);
    _mdrv_gpio_set_low(padSCL);

    gpio_delay(u32LoopDelay);

    return u8Receive;
}


MS_BOOL _mdrv_gpio_get_bus_data(GPIO_Bus_Data *pBusData)   //Read Byte
{
    MS_U16 i;
    MS_U8 u8Receive = 0;
    MS_U16 u16Ack;

    for(i=0;i<pBusData->u16byte;i++)
    {
        if(i>=(pBusData->u16byte-1))
            u16Ack=0;  //last byte
        else
            u16Ack=1;

        u8Receive=GetByte(pBusData->padSCL,pBusData->padSDA,pBusData->u32LoopDelay,pBusData->ack,u16Ack);
        pBusData->pBuf[i]=u8Receive;
    }
    return TRUE;
}

void _mdrv_gpio_bus_stop(GPIO_Bus_Data *pBusData)
{
    _mdrv_gpio_set_low(pBusData->padSCL);
    gpio_delay(pBusData->u32LoopDelay);
    _mdrv_gpio_set_low(pBusData->padSDA);

    gpio_delay(pBusData->u32LoopDelay);
    _mdrv_gpio_set_input(pBusData->padSCL);
    gpio_delay(pBusData->u32LoopDelay);
    _mdrv_gpio_set_input(pBusData->padSDA);
    gpio_delay(pBusData->u32LoopDelay);
}

//==============================================================
MS_BOOL mdrv_gpio_bus_start(GPIO_Bus_Data *pBusData)   // Start
{
#if (GPIO_UTOPIA20)
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return  0;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_BUS_START,(MS_U32*)pBusData))
    {
                printf("Ioctl mdrv_gpio_bus_start fail\n");
        return FALSE;
    }

    return *(pBusData->bRet);
#else
    return _mdrv_gpio_bus_start(pBusData);
#endif
}

MS_BOOL mdrv_gpio_send_bus_data(GPIO_Bus_Data *pBusData)   //Write Byte
{
#if (GPIO_UTOPIA20)

    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return 0;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_SEND_BUS_DATA,(MS_U32*)pBusData))
    {
                printf("Ioctl mdrv_gpio_send_bus_data fail\n");
        return FALSE;
    }

    return *(pBusData->bRet);
#else
    return _mdrv_gpio_send_bus_data(pBusData);
#endif
}

MS_BOOL mdrv_gpio_get_bus_data(GPIO_Bus_Data *pBusData)   //Read Byte
{
#if (GPIO_UTOPIA20)

    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return 0;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_GET_BUS_DATA,(MS_U32*)pBusData))
    {
                printf("Ioctl mdrv_gpio_get_bus_data fail\n");
        return FALSE;
    }

    return (MS_BOOL)TRUE;
#else
    return _mdrv_gpio_get_bus_data(pBusData);
#endif

}

void mdrv_gpio_bus_stop(GPIO_Bus_Data* pBusData)   // Stop
{
#if (GPIO_UTOPIA20)
    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return;

    UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_BUS_STOP, (MS_U32*)pBusData);
#else
    _mdrv_gpio_bus_stop(pBusData);
#endif
}

#endif      //__DRVGPIO_C__

static void MDrv_GPIO_RESUME_Init_STR(GPIO_Mode_Str *gGpioModeStr)
{
    int i=0;

    for(i=1;i<=END_GPIO_NUM;i++)
    {
        switch(gGpioModeStr[i])
        {
            case E_GPIO_STR_SetHigh:
                _mdrv_gpio_set_high(i);
                break;
            case E_GPIO_STR_SetLow:
                _mdrv_gpio_set_low(i);
                break;
            case E_GPIO_STR_SetInput:
                _mdrv_gpio_set_input(i);
                break;
            case E_GPIO_STR_NONE:
            default:
                break;
        }
    }
}

void MDrv_GPIO_Copy_STR(GPIO_Mode_Str* gGpioModeStr)
{
    memcpy(gGpioModeStr,gpio_Info_Str,sizeof(gpio_Info_Str));   //  ???
}

MS_U16 MDrv_GPIO_SetPowerState_U2K(GPIO_Mode_Str *gGpioModeStr, EN_POWER_MODE u32PowerState, void* pModule)
{
    MS_U16 u16Return = FALSE;
#if defined(MSOS_TYPE_LINUX_KERNEL)

    switch(u32PowerState)
    {
        case E_POWER_RESUME:
            {
                MDrv_GPIO_RESUME_Init_STR(gGpioModeStr);
                u16Return = TRUE;
            }
        break;

        case E_POWER_SUSPEND:
            {
                u16Return = TRUE;
            }
        break;

        default:
            break;
    }

#endif
    return u16Return;
}

int suspend_gpio(void)
{
#define GPI_PM_OPT1 43
        //Low:       Internal Micom
        //HIGH:     External Micom
        if(_mdrv_gpio_get_level(GPI_PM_OPT1))
        {
//LMTASKWBS-3420 & LMTASKWBS-3436 : suspend/resume fail with DC quick on/off
#define RETENTION_DISABLE   3
#define INSTANT_BOOT        5
#define WOL_WAKE_UP_SOC     6
#define M_RFMODULE_RESET    20

            gpio_Info_Str[RETENTION_DISABLE] = E_GPIO_STR_SetHigh;
            gpio_Info_Str[INSTANT_BOOT] = E_GPIO_STR_SetInput;
            gpio_Info_Str[WOL_WAKE_UP_SOC] = E_GPIO_STR_SetHigh;
            gpio_Info_Str[M_RFMODULE_RESET] = E_GPIO_STR_NONE; //skip as resume

            _mdrv_gpio_set_high(WOL_WAKE_UP_SOC); //set high
            _mdrv_gpio_set_low(M_RFMODULE_RESET); //set low
        }

    return 0;
}

int resume_gpio(void)
{
    MDrv_GPIO_RESUME_Init_STR(gpio_Info_Str);
    return 0;
}

#if (GPIO_UTOPIA20)
GPIO_Result MDrv_GPIO_Get_INT_Polarity_U2K(InterruptNum eGPIO_IRQnum, GPIO_Edge *ptr_gpio_edge_type)
{
    *ptr_gpio_edge_type = E_GPIO_NONE;

    if (_GPIO_Is_Init() == FALSE)
    {
        return E_GPIO_FAIL;
    }

    return HAL_GPIO_Get_INT_Polarity(eGPIO_IRQnum, ptr_gpio_edge_type);
}

GPIO_Result MDrv_GPIO_Set_INT_Polarity_U2K(InterruptNum eGPIO_IRQnum, GPIO_Edge gpio_edge_type)
{
    if (_GPIO_Is_Init() == FALSE)
    {
        return E_GPIO_FAIL;
    }

    return HAL_GPIO_Set_INT_Polarity(eGPIO_IRQnum, gpio_edge_type);
}

GPIO_Result MDrv_GPIO_GetMap_GPIOIntToNum_U2K(InterruptNum eGPIO_IRQnum, MS_GPIO_NUM *pGpioNum)
{
    *pGpioNum = 0;

    if (_GPIO_Is_Init() == FALSE)
    {
        return E_GPIO_FAIL;
    }

    return HAL_GPIO_GetMap_GPIOIntToNum(eGPIO_IRQnum, pGpioNum);
}
#endif


GPIO_Result MDrv_GPIO_Get_INT_Polarity(MS_U32 eGPIO_IRQnum, GPIO_Edge *ptr_gpio_edge_type)
{
#if (GPIO_UTOPIA20)

    GPIO_PRIVATE_INT_PARAM eGpioIntData;

    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return E_GPIO_FAIL;

    eGpioIntData.eGPIONum = 0;
    eGpioIntData.eGPIO_IRQnum = (InterruptNum)eGPIO_IRQnum;
    eGpioIntData.gpio_edge_type = E_GPIO_NONE;
    eGpioIntData.result = E_GPIO_FAIL;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Get_INT_Polarity,(MS_U32*)(&eGpioIntData)))
    {
        printf("Ioctl mdrv_gpio_get_bus_data fail\n");
        return E_GPIO_FAIL;
    }

    *ptr_gpio_edge_type = eGpioIntData.gpio_edge_type;

    return eGpioIntData.result;

#else
    return HAL_GPIO_Get_INT_Polarity((InterruptNum)eGPIO_IRQnum, ptr_gpio_edge_type);
#endif
}

GPIO_Result MDrv_GPIO_Set_INT_Polarity(MS_U32 eGPIO_IRQnum, GPIO_Edge gpio_edge_type)
{
#if (GPIO_UTOPIA20)

    GPIO_PRIVATE_INT_PARAM eGpioIntData;

    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return E_GPIO_FAIL;

    eGpioIntData.eGPIONum = 0;
    eGpioIntData.eGPIO_IRQnum = (InterruptNum)eGPIO_IRQnum;
    eGpioIntData.gpio_edge_type = gpio_edge_type;
    eGpioIntData.result = E_GPIO_FAIL;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio,MDrv_CMD_GPIO_Set_INT_Polarity,(MS_U32*)(&eGpioIntData)))
    {
        printf("Ioctl mdrv_gpio_get_bus_data fail\n");
        return E_GPIO_FAIL;
    }

    return eGpioIntData.result;

#else
    return HAL_GPIO_Set_INT_Polarity((InterruptNum) eGPIO_IRQnum, gpio_edge_type);
#endif
}

GPIO_Result MDrv_GPIO_GetMap_GPIOIntToNum(MS_U32 eGPIO_IRQnum, MS_GPIO_NUM *pGpioNum)
{
#if (GPIO_UTOPIA20)

    GPIO_PRIVATE_INT_PARAM eGpioIntData;

    if (FALSE == _MDrv_GPIO_CheckUtopia20Open(&pInstantGpio, 0, pAttributeGpio))
        return E_GPIO_FAIL;

    eGpioIntData.eGPIONum = 0;
    eGpioIntData.eGPIO_IRQnum = (InterruptNum) eGPIO_IRQnum;
    eGpioIntData.gpio_edge_type = E_GPIO_NONE;
    eGpioIntData.result = E_GPIO_FAIL;

    if (UTOPIA_STATUS_FAIL == UtopiaIoctl(pInstantGpio, MDrv_CMD_GPIO_GetMap_GPIOIntToNum,(MS_U32*)(&eGpioIntData)))
    {
        printf("Ioctl mdrv_gpio_get_bus_data fail\n");
        return E_GPIO_FAIL;
    }

    *pGpioNum = eGpioIntData.eGPIO_IRQnum;

    return eGpioIntData.result;

#else
    return HAL_GPIO_GetMap_GPIOIntToNum((InterruptNum) eGPIO_IRQnum, pGpioNum);
#endif
}

static GPIO_Result MDrv_GPIO_CheckInterrupt(InterruptNum eGPIO_IRQnum)
{
    GPIO_Result result;
    MS_GPIO_NUM eGpioNum = 0;

    result =  MDrv_GPIO_GetMap_GPIOIntToNum(eGPIO_IRQnum, &eGpioNum);

    if (result != E_GPIO_OK && eGpioNum!= 0)
    {
        printf("[Error] %s, %d, fail , error interrupt number:%d\n", __FUNCTION__, __LINE__, eGPIO_IRQnum);
        return E_GPIO_FAIL;
    }

    return E_GPIO_OK;
}

GPIO_Result MDrv_GPIO_EnableInterrupt(InterruptNum eGPIO_IRQnum)
{
    MsOS_EnableInterrupt(eGPIO_IRQnum);
    return E_GPIO_OK;
}

GPIO_Result MDrv_GPIO_DisableInterrupt(InterruptNum eGPIO_IRQnum)
{
    MsOS_DisableInterrupt(eGPIO_IRQnum);
    return E_GPIO_OK;
}


GPIO_Result MDrv_GPIO_AttachInterrupt(InterruptNum eGPIO_IRQnum, InterruptCb pFunc, GPIO_Edge gpio_edge)
{
    if ( MDrv_GPIO_CheckInterrupt(eGPIO_IRQnum)  != E_GPIO_OK)
    {
        printf("[Error] %s, %d, fail , error interrupt number:%d\n", __FUNCTION__, __LINE__, eGPIO_IRQnum);
        return E_GPIO_FAIL;
    }

    MDrv_GPIO_Set_INT_Polarity(eGPIO_IRQnum, gpio_edge);
    MsOS_AttachInterrupt(eGPIO_IRQnum, pFunc);

    return E_GPIO_OK;
}


