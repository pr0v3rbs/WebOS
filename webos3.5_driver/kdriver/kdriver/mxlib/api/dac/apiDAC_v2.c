#define _DAC_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDAC.h"
#include "apiDAC_private.h"
#include "apiDAC_v2.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
// for semaphore POOL


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// this func will be call to init by utopia20 framework
void DACRegisterToUtopia(void)
{
    MS_U32 u32ResourceStatusCheck[E_DAC_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_DAC, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DACOpen, (FUtopiaClose)DACClose, (FUtopiaIOctl)DACIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, E_DAC_POOL_ID_INTERNAL_VARIABLE);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    u32ResourceStatusCheck[E_DAC_POOL_ID_INTERNAL_VARIABLE] = UtopiaResourceCreate("dac", sizeof(DAC_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, E_DAC_POOL_ID_INTERNAL_VARIABLE);

	UtopiaModuleAddResourceEnd(pUtopiaModule, E_DAC_POOL_ID_INTERNAL_VARIABLE);

    //4. init resource private members here   (aka, global variable)
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pDACResourcePrivate));

    //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
    if (u32ResourceStatusCheck[E_DAC_POOL_ID_INTERNAL_VARIABLE] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pDACResourcePrivate->bResourceRegistered = TRUE;
        memset(&(pDACResourcePrivate->stapiDAC),0,sizeof(pDACResourcePrivate->stapiDAC));
    }
}

MS_U32 DACOpen(void** ppInstance, const void* const pAttribute)
{
    DAC_INSTANT_PRIVATE *pDACPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(DAC_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pDACPri);

    pDACPri->fpDACGetLibVer             = MApi_DAC_GetLibVer_U2;
    pDACPri->fpDACGetInfo               = MApi_DAC_GetInfo_U2;
    pDACPri->fpDACGetStatus             = MApi_DAC_GetStatus_U2;
    pDACPri->fpDACSetDbgLevel           = MApi_DAC_SetDbgLevel_U2;
    pDACPri->fpDACInit                  = MApi_DAC_Init_U2;
    pDACPri->fpDACEnable                = MApi_DAC_Enable_U2;
    pDACPri->fpDACSetClkInv             = MApi_DAC_SetClkInv_U2;
    pDACPri->fpDACSetYPbPrOutputTiming  = MApi_DAC_SetYPbPrOutputTiming_U2;
    pDACPri->fpDACSetOutputSource       = MApi_DAC_SetOutputSource_U2;
    pDACPri->fpDACSetOutputLevel        = MApi_DAC_SetOutputLevel_U2;
    pDACPri->fpDACSetOutputSwapSel      = MApi_DAC_SetOutputSwapSel_U2;
    pDACPri->fpDACOnOffSD               = MApi_DAC_OnOffSD_U2;
    pDACPri->fpDACOnOffHD               = MApi_DAC_OnOffHD_U2;
    pDACPri->fpDACClkSel                = MApi_DAC_ClkSel_U2;
    pDACPri->fpDACDumpTable             = MApi_DAC_DumpTable_U2;
    pDACPri->fpDACExit                  = MApi_DAC_Exit_U2;
    pDACPri->fpDACSetIHalfOutput        = MApi_DAC_SetIHalfOutput_U2;
    pDACPri->fpDACSetQuartOutput        = MApi_DAC_SetQuartOutput_U2;
    pDACPri->fpDACSetDacState           = MApi_DAC_SetDacState_U2;
    pDACPri->fpDACHotPlugDetect         = MApi_DAC_HotPlugDetect_U2;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DACIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    if(pInstance == NULL)
    {
        printf("[%s] pInstance is NULL\n",__FUNCTION__);
        return UTOPIA_STATUS_FAIL;
    }
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    DAC_INSTANT_PRIVATE* psDACInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psDACInstPri);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    switch(u32Cmd)
    {
        case E_DAC_GET_LIBVER:
        {
            pstDAC_GET_LIBVER ptr = (pstDAC_GET_LIBVER)pArgs;
            ptr->eReturnValue = psDACInstPri->fpDACGetLibVer(ptr->ppVersion);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_INFO:
        {
            pstDAC_GET_INFO ptr = (pstDAC_GET_INFO)pArgs;
            ptr->stReturnValue = psDACInstPri->fpDACGetInfo();
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_STATUS:
        {
            pstDAC_GET_STATUS ptr = (pstDAC_GET_STATUS)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACGetStatus(ptr->pDacStatus);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_DBG_LEVEL:
        {
            pstDAC_SET_DBG_LEVEL ptr = (pstDAC_SET_DBG_LEVEL)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACSetDbgLevel(ptr->u16DbgSwitch);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_INIT:
        {
            pstDAC_INIT ptr = (pstDAC_INIT)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACInit();
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ENABLE:
        {
            pstDAC_ENABLE ptr = (pstDAC_ENABLE)pArgs;
            psDACInstPri->fpDACEnable(ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_CLKINV:
        {
            pstDAC_SET_CLKINV ptr = (pstDAC_SET_CLKINV)pArgs;
            psDACInstPri->fpDACSetClkInv(ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;

        }
        case E_DAC_SET_YPBPR_OUTPUTTIMING:
        {
            pstDAC_SET_YPBPR_OUTPUTTIMING ptr = (pstDAC_SET_YPBPR_OUTPUTTIMING)pArgs;
            psDACInstPri->fpDACSetYPbPrOutputTiming(ptr->eTiming);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_SOURCE:
        {
            pstDAC_SET_OUTPUT_SOURCE ptr = (pstDAC_SET_OUTPUT_SOURCE)pArgs;
            psDACInstPri->fpDACSetOutputSource(ptr->enOutputType,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_LEVEL:
        {
            pstXC_SET_OUTPUT_LEVEL ptr = (pstXC_SET_OUTPUT_LEVEL)pArgs;
            psDACInstPri->fpDACSetOutputLevel(ptr->enLevel,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_SWAPSEL:
        {
            pstDAC_SET_OUTPUT_SWAPSEL ptr = (pstDAC_SET_OUTPUT_SWAPSEL)pArgs;
            psDACInstPri->fpDACSetOutputSwapSel(ptr->enSwap,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ONOFF_SD:
        {
            pstDAC_ONOFF_SD ptr = (pstDAC_ONOFF_SD)pArgs;
            psDACInstPri->fpDACOnOffSD(ptr->enBit);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ONOFF_HD:
        {
            pstDAC_ONOFF_HD ptr = (pstDAC_ONOFF_HD)pArgs;
            psDACInstPri->fpDACOnOffHD(ptr->enBit);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_CLKSEL:
        {
            pstDAC_CLKSEL ptr = (pstDAC_CLKSEL)pArgs;
            psDACInstPri->fpDACClkSel(ptr->eTiming,ptr->ebits);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_DUMP_TABLE:
        {
            pstDAC_DUMP_TABLE ptr = (pstDAC_DUMP_TABLE)pArgs;
            psDACInstPri->fpDACDumpTable(ptr->pDACTable,ptr->u8DACtype);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_EXIT:
        {
            psDACInstPri->fpDACExit();
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_IHALF_OUTPUT:
        {
            pstDAC_SET_IHALF_OUTPUT ptr = (pstDAC_SET_IHALF_OUTPUT)pArgs;
            psDACInstPri->fpDACSetIHalfOutput(ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_QUART_OUTPUT:
        {
            pstDAC_SET_QUART_OUTPUT ptr = (pstDAC_SET_QUART_OUTPUT)pArgs;
            psDACInstPri->fpDACSetQuartOutput(ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_DAC_STATE:
        {
            pstDAC_SET_DAC_STATE ptr = (pstDAC_SET_DAC_STATE)pArgs;
            psDACInstPri->fpDACSetDacState(ptr->bEnabled,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_HOTPLUG_DETECT:
        {
            pstDAC_HOTPLUG_DETECT ptr = (pstDAC_HOTPLUG_DETECT)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACHotPlugDetect(ptr->SelDAC,ptr->DetectType,ptr->State);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        default:
            printf("[%s] out of  DAC cmd:%lx\n",__FUNCTION__,u32Cmd);
            break;
    };

    //printf("(%s) Done\n\n",__FUNCTION__);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DACClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


