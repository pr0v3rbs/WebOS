#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsTypes.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvWDT.h"
#include "drvWDT_v2.h"
#include "drvWDT_private.h"
#include "MsOS.h"

enum
{
    WDT_POOL_ID = 0,
} eWdtPoolID;

void WDTRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    void* psResource = NULL;
    
    UtopiaModuleCreate(MODULE_WDT, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)WDTOpen, (FUtopiaClose)WDTClose, (FUtopiaIOctl)WDTIoctl);

    // 2. deal with resource
    UtopiaModuleAddResourceStart(pUtopiaModule, WDT_POOL_ID);
    UtopiaResourceCreate("WDT", sizeof(WDT_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, WDT_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, WDT_POOL_ID);
}

MS_U32 WDTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    WDT_INSTANT_PRIVATE *pWdtPri = NULL;
    void *pWdtPriVoid = NULL;
    MS_DEBUG_MSG(printf("\n[WDT INFO]WDT open \n"));

    UtopiaInstanceCreate(sizeof(WDT_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pWdtPriVoid);

    pWdtPri = (WDT_INSTANT_PRIVATE *)pWdtPriVoid;
    
    pWdtPri->fpWDTInit = _MDrv_WDT_Init;
    pWdtPri->fpWDTStop = _MDrv_WDT_Stop;
    pWdtPri->fpWDTClear = _MDrv_WDT_Clear;
    pWdtPri->fpWDTClearRstFlag = _MDrv_WDT_ClearRstFlag;
    pWdtPri->fpWDTIsReset = _MDrv_WDT_IsReset;
    
    pWdtPri->fpWDTIsEnable = _MDrv_WDT_IsEnable;
    pWdtPri->fpWDTSetTimer = _MDrv_WDT_SetTimer;
    pWdtPri->fpWDTSetTimer_ms = _MDrv_WDT_SetTimer_ms;
    pWdtPri->fpWDTSetTimer_us = _MDrv_WDT_SetTimer_us;
    pWdtPri->fpWDTSetIntTimer = _MDrv_WDT_SetIntTimer;
    
    pWdtPri->fpTIMERCount = _MDrv_TIMER_Count;
    pWdtPri->fpTIMERInt = _MDrv_TIMER_INT;
    pWdtPri->fpTIMERRst = _MDrv_TIMER_Rst;
    pWdtPri->fpTIMERSetMaxMatch = _MDrv_TIMER_SetMaxMatch;
    pWdtPri->fpTIMERHitMaxMatch = _MDrv_TIMER_HitMaxMatch;
    
    pWdtPri->fpTIMERGetMaxMatch = _MDrv_TIMER_GetMaxMatch;
    pWdtPri->fpTIMERGetCounter = _MDrv_TIMER_GetCounter;
    pWdtPri->fpTIMERGetSecond = _MDrv_TIMER_GetSecond;
    pWdtPri->fpTIMERGetMs = _MDrv_TIMER_GetMs;
    pWdtPri->fpTIMERDelay = _MDrv_TIMER_Delay;
    
    pWdtPri->fpTIMERDelayMs = _MDrv_TIMER_DelayMs;

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 WDTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	
    void* pModule = NULL;
    void* pResource = NULL;
    WDT_PRIVATE_PARAM *param = NULL;
    MS_U32 u32Ret = 0;
    WDT_INSTANT_PRIVATE* psWDTInstPri = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    if (pArgs != NULL)
        param= (WDT_PRIVATE_PARAM *) pArgs;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psWDTInstPri);	
	
    switch(u32Cmd)
    {
        case MDrv_CMD_WDT_INIT:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTInit(param->privateInit.eLevel);
            UtopiaResourceRelease(pResource);  
            break;
            
        case MDrv_CMD_WDT_STOP:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTStop(param->privateStop.eLevel);
            UtopiaResourceRelease(pResource);          
            break;
            
        case MDrv_CMD_WDT_CLEAR:
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTClear();
            UtopiaResourceRelease(pResource);           
            break;
            
        case MDrv_CMD_WDT_CLEAR_RST_FLAG:
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTClearRstFlag();
            UtopiaResourceRelease(pResource);     
            break;
                
        case MDrv_CMD_WDT_IS_RESET:
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTIsReset();
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_WDT_IS_ENABLE:
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTIsEnable();
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_WDT_SETTIMER:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTSetTimer(param->privateSetTimer.eLevel, param->privateSetTimer.sec);
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_WDT_SETTIMER_MS:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTSetTimer_ms(param->privateSetTimer_ms.eLevel, param->privateSetTimer_ms.msec);
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_WDT_SETTIMER_US:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTSetTimer_us(param->privateSetTimer_us.eLevel, param->privateSetTimer_us.usec);
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_WDT_SETINTTIMER:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpWDTSetIntTimer(param->privateSetIntTimer.eLevel, param->privateSetIntTimer.sec);
            UtopiaResourceRelease(pResource);            
            break;
            
        case MDrv_CMD_TIMER_COUNT:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERCount(param->privateCount.eTimer, param->privateCount.bEnable);
            UtopiaResourceRelease(pResource);            
            break;        
            
        case MDrv_CMD_TIMER_INT:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERInt(param->privateINT.eTimer,param->privateINT.bEnable);
            UtopiaResourceRelease(pResource);           
            break;
            
        case MDrv_CMD_TIMER_RST:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERRst(param->privateRst.eTimer);
            UtopiaResourceRelease(pResource);                
            break;
            
        case MDrv_CMD_TIMER_SETMAXMATCH:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERSetMaxMatch(param->privateSetMaxMatch.eTimer,param->privateSetMaxMatch.u32MaxTimer);
            UtopiaResourceRelease(pResource);                        
            break;
            
        case MDrv_CMD_TIMER_HITMAXMATCH:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpTIMERHitMaxMatch(param->privateHitMaxMatch.eTimer);
            UtopiaResourceRelease(pResource);          
            break;
            
        case MDrv_CMD_TIMER_GETMAXMATCH:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpTIMERGetMaxMatch(param->privateGetMaxMatch.eTimer);
            UtopiaResourceRelease(pResource);          
            break;
            
        case MDrv_CMD_TIMER_GETCOUNTER:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpTIMERGetCounter(param->privateGetCounter.eTimer);
            UtopiaResourceRelease(pResource);           
            break;
            
        case MDrv_CMD_TIMER_GETSECOND:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpTIMERGetSecond(param->privateGetSecond.eTimer);
            UtopiaResourceRelease(pResource);         
            break;
            
        case MDrv_CMD_TIMER_GETMS:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            u32Ret=psWDTInstPri->fpTIMERGetMs(param->privateGetMs.eTimer);
            UtopiaResourceRelease(pResource);            
            break;
            
        case MDrv_CMD_TIMER_DELAY:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERDelay(param->privateDelay.eTimer,param->privateDelay.u32Second);
            UtopiaResourceRelease(pResource);        
            break;
            
        case MDrv_CMD_TIMER_DELAYMS:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, WDT_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }      
            psWDTInstPri->fpTIMERDelayMs(param->privateDelayMs.eTimer,param->privateDelayMs.u32MS);
            UtopiaResourceRelease(pResource);           
            break;
                                                                                                                                                                                                
        default:
            break;
    };

	return UTOPIA_STATUS_SUCCESS;
}
 

MS_U32 WDTClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}
 
