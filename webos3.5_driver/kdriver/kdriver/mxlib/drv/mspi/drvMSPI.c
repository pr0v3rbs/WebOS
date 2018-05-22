////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMSPI.c
/// @brief  Master SPI Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


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

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMSPI.h"
#include "MsOS.h"

// Internal Definition
#include "regMSPI.h"
#include "halMSPI.h"
#include "drvMMIO.h"
#if (MSPI_UTOPIA20)
#include "utopia.h"
#endif
#include "drvGPIO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#if (MSPI_UTOPIA20)
MS_BOOL bU20MSPIOpened = FALSE;
void* pInstantMSPI;
void* pAttributeMSPI;
#endif

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
MS_U8 gu8MSPIConfig;
MS_U8 gu8MSPICurConfig[MSPI_CMD_TYPE];
MS_BOOL gbInitFlag = FALSE;
MS_U8 _u8MSPIDbgLevel;
static MS_U32 _gu32CsPad = 0xFF;
static MS_U32 _gu32DevID = 0xFF;
static MS_U32 _gu32MaxClk = 0;
static MS_S32 _gs32MSPI_Mutex;
//MSPI mutex wait time
#define MSPI_MUTEX_WAIT_TIME    3000
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DEBUG_MSPI(debug_level, x)     do { if (_u8MSPIDbgLevel >= (debug_level)) (x); } while(0)

MSPI_ErrorNo _MDrv_MSPI_Init_Ext(MS_U8 u8HWNum);

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Description : Set Chip Select
/// @return E_MSPI_OK : 
/// @return >1 : failed to set Chip select
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_ChipSelectSetting(MS_U8 u8Cs)
{
    MS_U8 u8CSMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8CSMax = HAL_MSPI_ChipSelectMax();
    if(u8Cs > u8CSMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
	else
        HAL_MSPI_SetChipSelect(u8Cs);
	return errnum;
}

//------------------------------------------------------------------------------
/// Description : Set TrStart timing of DC config
/// @return E_MSPI_OK : 
/// @return >1 : failed to set TrStart timing
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_DC_TrStartSetting(MS_U8 TrStart)
{
    MS_U8 u8TrStartMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TrStartMax = HAL_MSPI_DCConfigMax(E_MSPI_TRSTART);
    if(TrStart > u8TrStartMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
	else
        HAL_MSPI_SetDcTiming(E_MSPI_TRSTART ,TrStart);
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Set TrEnd timing of DC config
/// @return E_MSPI_OK : 
/// @return >1 : failed to set TrEnd timing
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_DC_TrEndSetting(MS_U8 TrEnd)
{
    MS_U8 u8TrEndMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TrEndMax = HAL_MSPI_DCConfigMax(E_MSPI_TREND);
    if(TrEnd > u8TrEndMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TREND ,TrEnd);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set TB timing of DC config 
/// @return E_MSPI_OK :
/// @return >1 : failed to set TB timing
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_DC_TBSetting(MS_U8 TB)
{
    MS_U8 u8TBMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TBMax = HAL_MSPI_DCConfigMax(E_MSPI_TB);
    if(TB > u8TBMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TB ,TB);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set TRW timing of DC config
/// @return E_MSPI_OK : 
/// @return >1 : failed to set TRW timging
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_DC_TRWSetting(MS_U8 TRW)
{
    MS_U8 u8TRWMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TRWMax = HAL_MSPI_DCConfigMax(E_MSPI_TRW);
    if(TRW > u8TRWMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TRW ,TRW);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock polarity of MSPI
/// @return E_MSPI_OK : 
/// @return >1 : failed to set clock polarity
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_CLK_PolaritySetting(MS_U8 u8Pol)
{
    MS_U8 u8PolarityMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8PolarityMax = HAL_MSPI_CLKConfigMax(E_MSPI_POL);
    if(u8Pol > u8PolarityMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_POL ,u8Pol);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock phase of MSPI
/// @return E_MSPI_OK : 
/// @return >1 : failed to set clock phase
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_CLK_PhaseSetting(MS_U8 u8Pha)
{
    MS_U8 u8PhaseMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8PhaseMax = HAL_MSPI_CLKConfigMax(E_MSPI_PHA);
    if(u8Pha > u8PhaseMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_PHA ,u8Pha);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock rate of MSPI
/// @return E_MSPI_OK : 
/// @return >1 : failed to set clock rate
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_CLK_ClockSetting(MS_U8 u8Clock)
{
    MS_U8 u8ClockMax;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8ClockMax = HAL_MSPI_CLKConfigMax(E_MSPI_CLK);
    if(u8Clock > u8ClockMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_CLK ,u8Clock);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set transfer bit pre frame for read/write buffer
/// @return E_MSPI_OK : 
/// @return >1 : failed to check paramter 
//------------------------------------------------------------------------------ 
static MSPI_ErrorNo _MDrv_Frame_BitSetting(MS_BOOL bDirect, MS_U8 u8Index, MS_U8 u8BitPerFrame)
{
    MS_U8 u8MAxBitPerFrame;
	MSPI_ErrorNo errnum = E_MSPI_OK;

    u8MAxBitPerFrame = HAL_MSPI_FrameConfigMax();
    if(u8BitPerFrame > u8MAxBitPerFrame)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else	
        HAL_MSPI_SetPerFrameSize(bDirect,  u8Index, u8BitPerFrame);
    return errnum;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// Description : Set detailed level of MSPI driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL _MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8MSPIDbgLevel = u8DbgLevel;

    return TRUE;
}

//-------------------------------------------------------
// Description : MSPI Power state
//-------------------------------------------------------
MS_U32 _MDrv_MSPI_SetPowerState(EN_POWER_MODE enPowerState)
{
    static EN_POWER_MODE enPreBDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (enPowerState == E_POWER_SUSPEND)
    {
        enPreBDMAPowerState = enPowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        gbInitFlag = FALSE;

    }
    else if (enPowerState == E_POWER_RESUME)
    {
        if (enPreBDMAPowerState == E_POWER_SUSPEND)
        {
            _MDrv_MSPI_Init_Ext(0);

            enPreBDMAPowerState = enPowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,enPowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo _MDrv_MasterSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
    MS_U32 u32NONPMBank, u32NONPMBankSize;
    MS_U32 u32PMBank, u32PMBankSize;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }

    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE;

    if (!MDrv_MMIO_GetBASE( &u32PMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &u32NONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("IOMap failure to get DRV_MMIO_PM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
    }
    if(u8HWNum > 2)
        return E_MSPI_PARAM_OVERFLOW;
    HAL_MSPI_MMIOConfig(u32PMBank, u32NONPMBank, u8HWNum);

    if(tMSPIConfig->BIntEnable)
    {
        //register IRQ handler
    }
    memcpy(gu8MSPICurConfig, tMSPIConfig->U8BitMapofConfig, sizeof(gu8MSPICurConfig));

    errorno = _MDrv_ChipSelectSetting(tMSPIConfig->U8ChipSel);
    if(errorno != E_MSPI_OK)
        return errorno;
    HAL_MSPI_IntEnable(tMSPIConfig->BIntEnable);
    HAL_MSPI_Init();
    gbInitFlag = TRUE;
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : MSPI initial Ext
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo _MDrv_MSPI_Init_Ext(MS_U8 u8HWNum)
{
    MS_U32 u32NONPMBank, u32NONPMBankSize;
    MS_U32 u32PMBank, u32PMBankSize;

    MSPI_ErrorNo errorno = E_MSPI_OK;

    if(gbInitFlag == TRUE)
    {
        printf("MSPI Driver Already init\r\n");
        return E_MSPI_INIT_FLOW_ERROR;
    }
    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE; 
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }

    if (!MDrv_MMIO_GetBASE( &u32PMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &u32NONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_INFO, printf("IOMap failure to get DRV_MMIO_PM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
        return E_MSPI_MMIO_ERROR;
    }
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, printf("IOMap address PM %lx NONPM %lx\n",u32PMBank, u32NONPMBank));

    HAL_MSPI_MMIOConfig(u32PMBank, u32NONPMBank, u8HWNum);

    memset(gu8MSPICurConfig, 0, sizeof(gu8MSPICurConfig));
    // default use CS0 for slave device 
    errorno = _MDrv_ChipSelectSetting(0);
    //default use polling mode 
    HAL_MSPI_IntEnable(0);
    HAL_MSPI_Init();
    if(_gu32MaxClk)
        HAL_MSPI_CLOCK_Config(_gu32MaxClk);
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, printf(" MSPI Init complete\n"));
    gbInitFlag = TRUE;
    //default clock setting
    errorno = _MDrv_CLK_PolaritySetting(1);
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, printf("PolaritySetting complete\n"));
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    errorno = _MDrv_CLK_PhaseSetting(0);
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    errorno = _MDrv_CLK_ClockSetting(0);
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    return E_MSPI_OK;
ERROR_HANDLE:
    errorno |= E_MSPI_INIT_FLOW_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI CLKconfig error errno =%d\n",errorno));
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo _MDrv_MSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
    MS_U32 u32NONPMBank, u32NONPMBankSize;
    MS_U32 u32PMBank, u32PMBankSize;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }
    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE;

    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    if (!MDrv_MMIO_GetBASE( &u32PMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &u32NONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("IOMap failure to get DRV_MMIO_PM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
    }
    if(u8HWNum > 2)
        return E_MSPI_PARAM_OVERFLOW;
    HAL_MSPI_MMIOConfig(u32PMBank, u32NONPMBank, u8HWNum);

    if(tMSPIConfig->BIntEnable)
    {
        //register IRQ handler
    }
    memcpy(gu8MSPICurConfig, tMSPIConfig->U8BitMapofConfig, sizeof(gu8MSPICurConfig));

    errorno = _MDrv_ChipSelectSetting(tMSPIConfig->U8ChipSel);
    if(errorno != E_MSPI_OK)
        return errorno;
    HAL_MSPI_IntEnable(tMSPIConfig->BIntEnable);
    HAL_MSPI_Init();
    gbInitFlag = TRUE;
    return errorno;
}
//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b IN :pointer to receive data from MSPI read buffer 
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_Read(MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8 u8FlowCheck = 0;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;
    // check config error
    if((gu8MSPICurConfig[MSPI_READ_OPERATION] != gu8MSPIConfig))
    {
        u8FlowCheck = gu8MSPICurConfig[MSPI_READ_OPERATION] ^ gu8MSPIConfig;
        if(u8FlowCheck & MSPI_DC_CONFIG)
            errorno |= E_MSPI_DCCONFIG_ERROR; 
        if(u8FlowCheck & MSPI_CLK_CONFIG)
            errorno |= E_MSPI_CLKCONFIG_ERROR;
        if(u8FlowCheck & MSPI_FRAME_CONFIG)
            errorno |= E_MSPI_FRAMECONFIG_ERROR;
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("Read Operation MSPI config error %d\n",errorno));
        return errorno;
    }
    HAL_MSPI_Read(pData, u16Size);
    return E_MSPI_OK;
}

//------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b OUT :pointer to write  data to MSPI write buffer 
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_Write(MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8 u8FlowCheck = 0;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    MS_U16 u16Index = 0;

    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;
    // check config error
    if((gu8MSPICurConfig[MSPI_WRITE_OPERATION] != gu8MSPIConfig))
    {
        u8FlowCheck = gu8MSPICurConfig[MSPI_WRITE_OPERATION] ^ gu8MSPIConfig;
        if(u8FlowCheck & MSPI_DC_CONFIG)
            errorno |= E_MSPI_DCCONFIG_ERROR; 
        if(u8FlowCheck & MSPI_CLK_CONFIG)
            errorno |= E_MSPI_CLKCONFIG_ERROR;
        if(u8FlowCheck & MSPI_FRAME_CONFIG)
            errorno |= E_MSPI_FRAMECONFIG_ERROR;
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("Write Operation MSPI config error %d\n",errorno));
        return errorno;
    }
    // write operation
    HAL_MSPI_Wirte(pData, u16Size);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_DCConfig(MSPI_DCConfig *ptDCConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptDCConfig == NULL)
    {
        HAL_MSPI_Reset_DCConfig();
        return E_MSPI_OK;
    }
    errnum = _MDrv_DC_TrStartSetting(ptDCConfig->u8TrStart);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TrEndSetting(ptDCConfig->u8TrEnd);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TBSetting(ptDCConfig->u8TB);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TRWSetting(ptDCConfig->u8TRW);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    gu8MSPIConfig |= MSPI_DC_CONFIG;
    return E_MSPI_OK;

ERROR_HANDLE:
    errnum |= E_MSPI_DCCONFIG_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI DCconfig error errno =%d\n",errnum));
    return errnum;	
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_CLKConfig(MSPI_CLKConfig *ptCLKConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptCLKConfig == NULL)
    {
        HAL_MSPI_Reset_CLKConfig();
        return E_MSPI_OK;
    }

    errnum = _MDrv_CLK_PolaritySetting(ptCLKConfig->BClkPolarity);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_CLK_PhaseSetting(ptCLKConfig->BClkPhase);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_CLK_ClockSetting(ptCLKConfig->U8Clock);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    gu8MSPIConfig |= MSPI_CLK_CONFIG;
    return E_MSPI_OK;

ERROR_HANDLE:
    errnum |= E_MSPI_CLKCONFIG_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI CLKconfig error errno =%d\n",errnum));
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MSPI_FRAMEConfig(MSPI_FrameConfig *ptFrameConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_U8 u8Index = 0;
    MS_U8 u8BufSize;

    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptFrameConfig == NULL)
    {
        HAL_MSPI_Reset_FrameConfig();
        return E_MSPI_OK;
    }
    // read buffer bit config 
    u8BufSize = sizeof(ptFrameConfig->u8RBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_READ_INDEX, u8Index, ptFrameConfig->u8RBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
             DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI FRAMEconfig error errno =%d\n",errnum));
            return errnum;
        }
    }
    //write buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8WBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_WRITE_INDEX, u8Index, ptFrameConfig->u8WBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI FRAMEconfig error errno =%d\n",errnum));
            return errnum;
        }
    }

    gu8MSPIConfig |= MSPI_FRAME_CONFIG;
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Enable Slave device  
//------------------------------------------------------------------------------
void _MDrv_MSPI_SlaveEnable(MS_BOOL Enable)
{
    HAL_MSPI_SlaveEnable(Enable);
    if(_gu32CsPad != 0xFF)
    {
        if(Enable)
        {
            mdrv_gpio_set_low(_gu32CsPad);
        }
        else
        {
            mdrv_gpio_set_high(_gu32CsPad);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b IN :pointer to receive data from MSPI read buffer 
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_Read(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8 u8FlowCheck = 0;
    MSPI_ErrorNo errorno = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_Read_return;
    }
    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_Read_return;
    }

    // check config error
    if((gu8MSPICurConfig[MSPI_READ_OPERATION] != gu8MSPIConfig))
    {
        u8FlowCheck = gu8MSPICurConfig[MSPI_READ_OPERATION] ^ gu8MSPIConfig;
        if(u8FlowCheck & MSPI_DC_CONFIG)
            errorno |= E_MSPI_DCCONFIG_ERROR; 
        if(u8FlowCheck & MSPI_CLK_CONFIG)
            errorno |= E_MSPI_CLKCONFIG_ERROR;
        if(u8FlowCheck & MSPI_FRAME_CONFIG)
            errorno |= E_MSPI_FRAMECONFIG_ERROR;
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("Read Operation MSPI config error %d\n",errorno));
        goto _MDrv_MasterSPI_Read_return;
    }

    HAL_MSPI_Read(pData, u16Size);
_MDrv_MasterSPI_Read_return:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : write data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b OUT :pointer to write  data to MSPI write buffer 
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_Write(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8 u8FlowCheck = 0;
    MSPI_ErrorNo errorno = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_Write_return;
    }

    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_Write_return;
    }

    // check config error
    if((gu8MSPICurConfig[MSPI_WRITE_OPERATION] != gu8MSPIConfig))
    {
        u8FlowCheck = gu8MSPICurConfig[MSPI_WRITE_OPERATION] ^ gu8MSPIConfig;
        if(u8FlowCheck & MSPI_DC_CONFIG)
            errorno |= E_MSPI_DCCONFIG_ERROR; 
        if(u8FlowCheck & MSPI_CLK_CONFIG)
            errorno |= E_MSPI_CLKCONFIG_ERROR;
        if(u8FlowCheck & MSPI_FRAME_CONFIG)
            errorno |= E_MSPI_FRAMECONFIG_ERROR;
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("Write Operation MSPI config error %d\n",errorno));
        goto _MDrv_MasterSPI_Write_return;
    }

    // write operation
    HAL_MSPI_Wirte(pData, u16Size);
_MDrv_MasterSPI_Write_return:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param u32DevID      \b IN device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_DCConfig(MS_U32 u32DevID, MSPI_DCConfig *ptDCConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum =  E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }

    if(ptDCConfig == NULL)
    {
        HAL_MSPI_Reset_DCConfig(); 
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TrStartSetting(ptDCConfig->u8TrStart);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TrEndSetting(ptDCConfig->u8TrEnd);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TBSetting(ptDCConfig->u8TB);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TRWSetting(ptDCConfig->u8TRW);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto _MDrv_MasterSPI_DCConfig_return;
    }
    gu8MSPIConfig |= MSPI_DC_CONFIG;

_MDrv_MasterSPI_DCConfig_return:
    
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI DCconfig error errno =%d\n",errnum));
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param u32DevID       \b IN  device id of slave device 
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_CLKConfig(MS_U32 u32DevID, MSPI_CLKConfig *ptCLKConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_BOOL bRet = FALSE;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_CLKConfig_return;
    }
    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_CLKConfig_return;
    }

    if(ptCLKConfig == NULL)
    {
        HAL_MSPI_Reset_CLKConfig(); 
        goto _MDrv_MasterSPI_CLKConfig_return;
    }

    errnum = _MDrv_CLK_PolaritySetting(ptCLKConfig->BClkPolarity);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto _MDrv_MasterSPI_CLKConfig_return;
    }
    errnum = _MDrv_CLK_PhaseSetting(ptCLKConfig->BClkPhase);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto _MDrv_MasterSPI_CLKConfig_return;
    }
    bRet = HAL_MSPI_CLOCK_Config(ptCLKConfig->u32MAXClk);
    if(bRet)
        errnum = E_MSPI_OK;
    else
        errnum = E_MSPI_OPERATION_ERROR;
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto _MDrv_MasterSPI_CLKConfig_return;
    }
    gu8MSPIConfig |= MSPI_CLK_CONFIG;

_MDrv_MasterSPI_CLKConfig_return:
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI CLKconfig error errno =%d\n",errnum));
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum;

}


//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param u32DevID      \b IN   device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_FRAMEConfig(MS_U32 u32DevID, MSPI_FrameConfig *ptFrameConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_U8 u8Index = 0;
    MS_U8 u8BufSize;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {        
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_FRAMEConfigreturn;
    }
    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_FRAMEConfigreturn;
    }

    if(ptFrameConfig == NULL)
    {
        HAL_MSPI_Reset_FrameConfig(); 
        goto _MDrv_MasterSPI_FRAMEConfigreturn;
    }
    // read buffer bit config 
    u8BufSize = sizeof(ptFrameConfig->u8RBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_READ_INDEX, u8Index, ptFrameConfig->u8RBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI FRAMEconfig error errno =%d\n",errnum));
            goto _MDrv_MasterSPI_FRAMEConfigreturn;
        }
    }
    //write buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8WBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_WRITE_INDEX, u8Index, ptFrameConfig->u8WBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, printf("MSPI FRAMEconfig error errno =%d\n",errnum));
            goto _MDrv_MasterSPI_FRAMEConfigreturn;
        }
    }
_MDrv_MasterSPI_FRAMEConfigreturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    gu8MSPIConfig |= MSPI_FRAME_CONFIG;
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Enable Slave device  
//------------------------------------------------------------------------------
MSPI_ErrorNo _MDrv_MasterSPI_SlaveEnable(MS_U32 u32DevID, MS_BOOL Enable)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto _MDrv_MasterSPI_SlaveEnablereturn;
    }

    HAL_MSPI_SlaveEnable(Enable);
    if(Enable && _gu32CsPad != 0xFF)
    {
        mdrv_gpio_set_low(_gu32CsPad);
    }
    else if(_gu32CsPad != 0xFF)
    {
        mdrv_gpio_set_high(_gu32CsPad);
    }
_MDrv_MasterSPI_SlaveEnablereturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum; 

}

MS_BOOL _MDrv_MasterSPI_CsPadConfig(MS_U32 u32DevID, MS_U32 u32CsPad)
{
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    _gu32CsPad =  u32CsPad;
    _gu32DevID =  u32DevID;
    mdrv_gpio_set_high(_gu32CsPad);
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return TRUE;
}

MS_BOOL _MDrv_MasterSPI_MaxClkConfig(MS_U32 u32DevID, MS_U32 u32MaxClk)
{
    MS_BOOL bRet = FALSE;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    if(_gu32DevID != u32DevID)
    {
        printf("%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        goto _MDrv_MasterSPI_MaxClkConfigreturn;
    }
    _gu32MaxClk = u32MaxClk;
    bRet = HAL_MSPI_CLOCK_Config(u32MaxClk);

_MDrv_MasterSPI_MaxClkConfigreturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return bRet;
}

#if (MSPI_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_MSPI_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MS_U32 u32Ret = 0;
    if (FALSE == bU20MSPIOpened)
    {
//        u32Ret =UtopiaOpen(MODULE_MSPI|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute);
        u32Ret =UtopiaOpen(MODULE_MSPI, pInstance, u32ModuleVersion, pAttribute);
        if(u32Ret != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open MSPI fail %lx\n",u32Ret);
            return FALSE;
        }
        bU20MSPIOpened = TRUE;
    }
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// Description : Set detailed level of MSPI driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevel)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_BOOL bResult = 1;
    PMSPI_SETDBGLEVEL pMSPIParam_DbgLevel = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_DbgLevel = (PMSPI_SETDBGLEVEL)malloc(sizeof(MSPI_SETDBGLEVEL));
    pMSPIParam_DbgLevel->u8DbgLevel=u8DbgLevel;
    pMSPIParam_DbgLevel->bReturn = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_DbgLEvel,(MS_U32*)pMSPIParam_DbgLevel);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    bResult = pMSPIParam_DbgLevel->bReturn;
    free(pMSPIParam_DbgLevel);

    return bResult;

#else
    return MDrv_MSPI_SetDbgLevel(u8DbgLevel);
#endif
}

//------------------------------------------------------------------------------
/// Description : MSPI initial Ext
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo MDrv_MSPI_Init_Ext(MS_U8 u8HWNum)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_INIT_EXT pMSPIParam_InitExt = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_InitExt = (PMSPI_INIT_EXT)malloc(sizeof(MSPI_INIT_EXT));
    pMSPIParam_InitExt->u8HWNum=u8HWNum;
    pMSPIParam_InitExt->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_Init_Ext,(MS_U32*)pMSPIParam_InitExt);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_InitExt->u16Return;
    free(pMSPIParam_InitExt);

    return u16Result;    
#else
    return MDrv_MSPI_Init_Ext(u8HWNum);
#endif
}

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo MDrv_MSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_INIT pMSPIParam_Init = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Init = (PMSPI_INIT)malloc(sizeof(MSPI_INIT));
    pMSPIParam_Init->u8HWNum=u8HWNum;
    pMSPIParam_Init->ptMSPIConfig = tMSPIConfig;
    pMSPIParam_Init->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_Init,(MS_U32*)pMSPIParam_Init);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Init->u16Return;
    free(pMSPIParam_Init);

    return u16Result;
#else
    return _MDrv_MSPI_Init(tMSPIConfig, u8HWNum);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b IN :pointer to receive data from MSPI read buffer 
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Read(MS_U8 *pData, MS_U16 u16Size)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_READ pMSPIParam_ReadBytes = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_ReadBytes = (PMSPI_READ)malloc(sizeof(MSPI_READ));
    pMSPIParam_ReadBytes->pu8Data=pData;
    pMSPIParam_ReadBytes->u16Size=u16Size;
    pMSPIParam_ReadBytes->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_Read,(MS_U32*)pMSPIParam_ReadBytes);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_ReadBytes->u16Return;
    free(pMSPIParam_ReadBytes);

    return u16Result;
#else
    return _MDrv_MSPI_Read(pData,u16Size);
#endif
}

//------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b OUT :pointer to write  data to MSPI write buffer 
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Write(MS_U8 *pData, MS_U16 u16Size)
{
#if (MSPI_UTOPIA20)
     MS_U32 u32Ret;
     MS_U16 u16Result = 1;
     PMSPI_WRITE pMSPIParam_WriteBytes = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_WriteBytes = (PMSPI_WRITE)malloc(sizeof(MSPI_WRITE));
    pMSPIParam_WriteBytes->pu8Data=pData;
    pMSPIParam_WriteBytes->u16Size=u16Size;
    pMSPIParam_WriteBytes->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_Write,(MS_U32*)pMSPIParam_WriteBytes);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_WriteBytes->u16Return;
    free(pMSPIParam_WriteBytes);

    return u16Result;
#else
    return _MDrv_MSPI_Read(pData,u16Size);
#endif

}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_DCConfig(MSPI_DCConfig *ptDCConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_DCCONFIG pMSPIParam_Dcconfig = NULL;
	MSPI_DCConfig DCConfig;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Dcconfig = (PMSPI_DCCONFIG)malloc(sizeof(MSPI_DCCONFIG));
	pMSPIParam_Dcconfig->ptDCConfig = &DCConfig;
    pMSPIParam_Dcconfig->ptDCConfig->u8TB = ptDCConfig->u8TB;
    pMSPIParam_Dcconfig->ptDCConfig->u8TrEnd = ptDCConfig->u8TrEnd;
    pMSPIParam_Dcconfig->ptDCConfig->u8TrStart = ptDCConfig->u8TrStart;
    pMSPIParam_Dcconfig->ptDCConfig->u8TRW = ptDCConfig->u8TRW;
    pMSPIParam_Dcconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_DCConfig,(MS_U32*)pMSPIParam_Dcconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Dcconfig->u16Return;
    free(pMSPIParam_Dcconfig);

    return u16Result;
#else
    _MDrv_MSPI_DCConfig(ptDCConfig);
#endif
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_CLKConfig(MSPI_CLKConfig *ptCLKConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_CLKCONFIG pMSPIParam_Clkconfig = NULL;
	MSPI_CLKConfig  CLKConfig;
	
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Clkconfig = (PMSPI_CLKCONFIG)malloc(sizeof(MSPI_CLKCONFIG));	
	pMSPIParam_Clkconfig->ptCLKConfig = &CLKConfig;
    pMSPIParam_Clkconfig->ptCLKConfig->U8Clock = ptCLKConfig->U8Clock;
    pMSPIParam_Clkconfig->ptCLKConfig->u32MAXClk = ptCLKConfig->u32MAXClk;
    pMSPIParam_Clkconfig->ptCLKConfig->BClkPhase = ptCLKConfig->BClkPhase;
    pMSPIParam_Clkconfig->ptCLKConfig->BClkPolarity= ptCLKConfig->BClkPolarity;
    pMSPIParam_Clkconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_CLKConfig,(MS_U32*)pMSPIParam_Clkconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Clkconfig->u16Return;
    free(pMSPIParam_Clkconfig);


    return u16Result;
#else
    _MDrv_MSPI_CLKConfig(ptCLKConfig);
#endif
}


//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_FRAMEConfig(MSPI_FrameConfig *ptFrameConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    MS_U8 u8Index = 0;
    PMSPI_FRAMECONFIG pMSPIParam_Frameconfig = NULL;
	MSPI_FrameConfig FrameConfig;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Frameconfig = (PMSPI_FRAMECONFIG)malloc(sizeof(MSPI_FRAMECONFIG));
	pMSPIParam_Frameconfig->ptFrameConfig = &FrameConfig;
    for(u8Index = 0; u8Index < MSPI_WR_BIT; u8Index++)
    {
        pMSPIParam_Frameconfig->ptFrameConfig->u8WBitConfig[u8Index] = ptFrameConfig->u8WBitConfig[u8Index];
        pMSPIParam_Frameconfig->ptFrameConfig->u8RBitConfig[u8Index] = ptFrameConfig->u8RBitConfig[u8Index];
    }
    pMSPIParam_Frameconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_FRAMEConfig,(MS_U32*)pMSPIParam_Frameconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Frameconfig->u16Return;
    free(pMSPIParam_Frameconfig);

    return u16Result;
#else
    _MDrv_MSPI_FRAMEConfig(ptFrameConfig);
#endif
}

//------------------------------------------------------------------------------
/// Description : Enable Slave device  
//------------------------------------------------------------------------------
void MDrv_MSPI_SlaveEnable(MS_BOOL Enable)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    PMSPI_SLAVE_ENABLE pMSPIParam_SlaveEnable = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        printf("MSPI OPEN FAIL \r\n");
    pMSPIParam_SlaveEnable = (PMSPI_SLAVE_ENABLE)malloc(sizeof(MSPI_SLAVE_ENABLE));
    pMSPIParam_SlaveEnable->bEnable = Enable;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_SlaveEnable,(MS_U32*)pMSPIParam_SlaveEnable);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    free(pMSPIParam_SlaveEnable);
#else
    _MDrv_MSPI_SlaveEnable(Enable);
#endif
}

//------------------------------------------------------------------------------
/// Description : 
/// @return TRUE : chip support 
/// @return FALSE: 
//------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_HW_Support(void)
{
    return HAL_MSPI_HW_Support();
}

//-------------------------------------------------------
// Description : MSPI Power state
//-------------------------------------------------------
MS_U32 MDrv_MSPI_SetPowerState(EN_POWER_MODE enPowerState)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPISETPOWERSTATE pMSPIParam_PowerState = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_PowerState = (PMSPISETPOWERSTATE)malloc(sizeof(MSPISETPOWERSTATE));
    pMSPIParam_PowerState->enPowerState = enPowerState;
    pMSPIParam_PowerState->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_SetPowerState,(MS_U32*)pMSPIParam_PowerState);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_PowerState->u16Return;
    free(pMSPIParam_PowerState);
    return u16Result;
#else

    return _MDrv_MSPI_SetPowerState(enPowerState);
#endif
} 

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK : 
/// @return >1 : failed to initial 
//------------------------------------------------------------------------------ 
MSPI_ErrorNo MDrv_MasterSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMSPI_INIT pMSPIParam_Init = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Init = (PMSPI_INIT)malloc(sizeof(MSPI_INIT));
    pMSPIParam_Init->u8HWNum=u8HWNum;
    pMSPIParam_Init->ptMSPIConfig = tMSPIConfig;
    pMSPIParam_Init->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MSPI_Init,(MS_U32*)pMSPIParam_Init);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Init->u16Return;
    free(pMSPIParam_Init);

    return u16Result;
#else
    return _MDrv_MasterSPI_Init(tMSPIConfig, u8HWNum);
#endif

}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b IN :pointer to receive data from MSPI read buffer 
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Read(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMASTERSPI_READ pMSPIParam_ReadBytes = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_ReadBytes = (PMASTERSPI_READ)malloc(sizeof(MASTERSPI_READ));
    pMSPIParam_ReadBytes->pu8Data = pData;
    pMSPIParam_ReadBytes->u16Size = u16Size;
    pMSPIParam_ReadBytes->u32DevID = u32DevID;
    pMSPIParam_ReadBytes->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_Read,(MS_U32*)pMSPIParam_ReadBytes);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_ReadBytes->u16Return;
    free(pMSPIParam_ReadBytes);

    return u16Result;

#else
    return _MDrv_MasterSPI_Read(u32DevID, pData, u16Size);
#endif
}

//------------------------------------------------------------------------------
/// Description : write data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b OUT :pointer to write  data to MSPI write buffer 
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Write(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMASTERSPI_WRITE pMSPIParam_WriteBytes = NULL;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_WriteBytes = (PMASTERSPI_WRITE)malloc(sizeof(MASTERSPI_WRITE));
    pMSPIParam_WriteBytes->pu8Data = pData;
    pMSPIParam_WriteBytes->u16Size = u16Size;
    pMSPIParam_WriteBytes->u32DevID = u32DevID;
    pMSPIParam_WriteBytes->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_Write,(MS_U32*)pMSPIParam_WriteBytes);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_WriteBytes->u16Return;
    free(pMSPIParam_WriteBytes);

    return u16Result;
#else
    return _MDrv_MasterSPI_Write(u32DevID, pData, u16Size);
#endif
}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param u32DevID      \b IN device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_DCConfig(MS_U32 u32DevID, MSPI_DCConfig *ptDCConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMASTERSPI_DCCONFIG pMSPIParam_Dcconfig = NULL;
	MSPI_DCConfig DCConfig;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Dcconfig = (PMASTERSPI_DCCONFIG)malloc(sizeof(MASTERSPI_DCCONFIG));
	pMSPIParam_Dcconfig->ptDCConfig = &DCConfig;
    pMSPIParam_Dcconfig->ptDCConfig->u8TB = ptDCConfig->u8TB;
    pMSPIParam_Dcconfig->ptDCConfig->u8TrEnd = ptDCConfig->u8TrEnd;
    pMSPIParam_Dcconfig->ptDCConfig->u8TrStart = ptDCConfig->u8TrStart;
    pMSPIParam_Dcconfig->ptDCConfig->u8TRW = ptDCConfig->u8TRW;
    pMSPIParam_Dcconfig->u32DevID = u32DevID;
    pMSPIParam_Dcconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_DCConfig,(MS_U32*)pMSPIParam_Dcconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Dcconfig->u16Return;
    free(pMSPIParam_Dcconfig);

    return u16Result;
#else
    return _MDrv_MasterSPI_DCConfig(u32DevID, ptDCConfig);
#endif
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param u32DevID       \b IN  device id of slave device 
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_CLKConfig(MS_U32 u32DevID, MSPI_CLKConfig *ptCLKConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    PMASTERSPI_CLKCONFIG pMSPIParam_Clkconfig = NULL;
	MSPI_CLKConfig CLKConfig;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Clkconfig = (PMASTERSPI_CLKCONFIG)malloc(sizeof(MASTERSPI_CLKCONFIG));
	pMSPIParam_Clkconfig->ptCLKConfig = &CLKConfig;
    pMSPIParam_Clkconfig->ptCLKConfig->U8Clock = ptCLKConfig->U8Clock;
    pMSPIParam_Clkconfig->ptCLKConfig->u32MAXClk = ptCLKConfig->u32MAXClk;
    pMSPIParam_Clkconfig->ptCLKConfig->BClkPhase = ptCLKConfig->BClkPhase;
    pMSPIParam_Clkconfig->ptCLKConfig->BClkPolarity= ptCLKConfig->BClkPolarity;
    pMSPIParam_Clkconfig->u32DevID = u32DevID;
    pMSPIParam_Clkconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_CLKConfig,(MS_U32*)pMSPIParam_Clkconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Clkconfig->u16Return;
    free(pMSPIParam_Clkconfig);

    return u16Result;
#else
    _MDrv_MasterSPI_CLKConfig(u32DevID, ptCLKConfig);
#endif

}


//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param u32DevID      \b IN   device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_FRAMEConfig(MS_U32 u32DevID, MSPI_FrameConfig *ptFrameConfig)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_U16 u16Result = 1;
    MS_U8 u8Index = 0;
    PMASTERSPI_FRAMECONFIG pMSPIParam_Frameconfig = NULL;
	MSPI_FrameConfig FrameConfig;

    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        return 0;
    pMSPIParam_Frameconfig = (PMASTERSPI_FRAMECONFIG)malloc(sizeof(MASTERSPI_FRAMECONFIG));
	pMSPIParam_Frameconfig->ptFrameConfig = &FrameConfig;
    for(u8Index = 0; u8Index < MSPI_WR_BIT; u8Index++)
    {
        pMSPIParam_Frameconfig->ptFrameConfig->u8WBitConfig[u8Index] = ptFrameConfig->u8WBitConfig[u8Index];
        pMSPIParam_Frameconfig->ptFrameConfig->u8RBitConfig[u8Index] = ptFrameConfig->u8RBitConfig[u8Index];
    }
    pMSPIParam_Frameconfig->u16Return = 0;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_FRAMEConfig,(MS_U32*)pMSPIParam_Frameconfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    u16Result = pMSPIParam_Frameconfig->u16Return;
    free(pMSPIParam_Frameconfig);

    return u16Result;
#else
    _MDrv_MasterSPI_FRAMEConfig(u32DevID, ptFrameConfig);
#endif

}

//------------------------------------------------------------------------------
/// Description : Enable Slave device  
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_SlaveEnable(MS_U32 u32DevID, MS_BOOL Enable)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    PMASTERSPI_SLAVE_ENABLE pMSPIParam_SlaveEnable = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        printf("MSPI OPEN FAIL \r\n");
    pMSPIParam_SlaveEnable = (PMASTERSPI_SLAVE_ENABLE)malloc(sizeof(MASTERSPI_SLAVE_ENABLE));
    pMSPIParam_SlaveEnable->bEnable = Enable;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_SlaveEnable,(MS_U32*)pMSPIParam_SlaveEnable);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    free(pMSPIParam_SlaveEnable);
    return 0;
#else
    return _MDrv_MasterSPI_SlaveEnable(u32DevID, Enable);
#endif
}

MS_BOOL MDrv_MasterSPI_CsPadConfig(MS_U32 u32DevID, MS_U32 u32CsPad)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_BOOL bResult = FALSE;
    PMASTERSPI_CSPADCONFIG pMSPIParam_CsPadConfig = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        printf("MSPI OPEN FAIL \r\n");
    pMSPIParam_CsPadConfig = (PMASTERSPI_CSPADCONFIG)malloc(sizeof(MASTERSPI_CSPADCONFIG));
    pMSPIParam_CsPadConfig->u32CsPad = u32CsPad;
    pMSPIParam_CsPadConfig->u32DevID = u32DevID;
    pMSPIParam_CsPadConfig->bReturn = bResult;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_CSPadConfig,(MS_U32*)pMSPIParam_CsPadConfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    bResult = pMSPIParam_CsPadConfig->bReturn;
    free(pMSPIParam_CsPadConfig);
    return u32Ret;
#else
    _MDrv_MasterSPI_CsPadConfig(u32DevID, u32CsPad);
#endif

}

MS_BOOL MDrv_MasterSPI_MaxClkConfig(MS_U32 u32DevID, MS_U32 u32MaxClk)
{
#if (MSPI_UTOPIA20)
    MS_U32 u32Ret;
    MS_BOOL bResult = FALSE;
    PMASTERSPI_MAXCLKCONFIG pMSPIParam_MaxClkConfig = NULL;
    if (FALSE == _MDrv_MSPI_CheckUtopia20Open(&pInstantMSPI, 0, pAttributeMSPI))
        printf("MSPI OPEN FAIL \r\n");
    pMSPIParam_MaxClkConfig = (PMASTERSPI_MAXCLKCONFIG)malloc(sizeof(MASTERSPI_MAXCLKCONFIG));
    pMSPIParam_MaxClkConfig->u32MaxClk = u32MaxClk;
    pMSPIParam_MaxClkConfig->u32DevID = u32DevID;
    pMSPIParam_MaxClkConfig->bReturn = bResult;
    u32Ret = UtopiaIoctl(pInstantMSPI,MDrv_CMD_MasterSPI_MaxClkConfig,(MS_U32*)pMSPIParam_MaxClkConfig);
    if(u32Ret)
        printf("MSPI IOCTRL FAIL : %lx \r\n",u32Ret);
    bResult = pMSPIParam_MaxClkConfig->bReturn;
    free(pMSPIParam_MaxClkConfig);
    return u32Ret;
#else
    _MDrv_MasterSPI_MaxClkConfig(u32DevID, u32MaxClk);
#endif

}


