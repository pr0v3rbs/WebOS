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
#include "drvPM.h"
#include "drvPM_v2.h"
#include "drvPM_private.h"
#include "MsOS.h"

enum
{
    PM_POOL_ID = 0,
} ePmPoolID;

void PMRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    void* psResource = NULL;
    
    UtopiaModuleCreate(MODULE_PM, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)PMOpen, (FUtopiaClose)PMClose, (FUtopiaIOctl)PMIoctl);

    // 2. deal with resource
    UtopiaModuleAddResourceStart(pUtopiaModule, PM_POOL_ID);
    UtopiaResourceCreate("PM", sizeof(PM_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, PM_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, PM_POOL_ID);
}

MS_U32 PMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    PM_INSTANT_PRIVATE *pPmPri = NULL;
    void *pPmPriVoid = NULL;
    MS_DEBUG_MSG(printf("\n[PM INFO]PM open \n"));

    UtopiaInstanceCreate(sizeof(PM_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pPmPriVoid);

    pPmPri = (PM_INSTANT_PRIVATE *)pPmPriVoid;
    pPmPri->fpPMRtcInit = _MDrv_PM_RtcInit;
    pPmPri->fpPMRtcSetCounter = _MDrv_PM_RtcSetCounter;
    pPmPri->fpPMRtcGetCounter = _MDrv_PM_RtcGetCounter;
    pPmPri->fpPMRtcSetMatchCounter = _MDrv_PM_RtcSetMatchCounter;
    pPmPri->fpPMRtcGetMatchCounter = _MDrv_PM_RtcGetMatchCounter;
    
    pPmPri->fpPMSetSPIOffsetForMCU = _MDrv_PM_SetSPIOffsetForMCU;
    pPmPri->fpPMSetSRAMOffsetForMCU = _MDrv_PM_SetSRAMOffsetForMCU;
    pPmPri->fpPMSetDRAMOffsetForMCU = _MDrv_PM_SetDRAMOffsetForMCU;
    pPmPri->fpPMGetWakeupSource = _MDrv_PM_GetWakeupSource;
    
    pPmPri->fpPMGetWakeupKey = _MDrv_PM_GetWakeupKey;
    pPmPri->fpPMDisable51 = _MDrv_PM_Disable51;
    pPmPri->fpPMCheckVersion = _MDrv_PM_Check_Version;
    pPmPri->fpPMGetSRAMSize= _MDrv_PM_GetSRAMSize;
    pPmPri->fpPMGetIRPowerOnKey = _MDrv_PM_GetIRPowerOnKey;
    pPmPri->fpPMRtcSetSystemTime = MDrv_PM_RTCSetSystemTime_U2K;
    pPmPri->fpPMRtcGetSystemTime = MDrv_PM_RTCGetSystemTime_U2K;
    pPmPri->fpPMRtcSetMatchTime = MDrv_PM_RTCSetMatchTime_U2K;
    pPmPri->fpPMLoadWakeUp51 = _MDrv_PM_LoadWakeUp51;

    MDrv_PM_SetIOMapBase();

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 PMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    void* pResource = NULL;
    PM_PRIVATE_PARAM *param = NULL;
    MS_U32 u32Ret = 0;
    PM_INSTANT_PRIVATE* psPMInstPri = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if (pArgs != NULL)
        param= (PM_PRIVATE_PARAM *) pArgs;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPMInstPri);
	
    switch(u32Cmd)
    {
    	case MDrv_CMD_PM_RtcInit:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
            
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMRtcInit(param->privateRtcInit.pPmRtcParam); 
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_PM_RtcSetCounter:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMRtcSetCounter(param->privateRtcSetCounter.pPmRtcParam);
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_PM_RtcGetCounter:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMRtcGetCounter(param->privateRtcGetCounter.pPmRtcParam);
            UtopiaResourceRelease(pResource);
            break;            
        case MDrv_CMD_PM_RtcSetMatchCounter:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMRtcSetMatchCounter(param->privateRtcSetMatchCounter.pPmRtcParam);
            UtopiaResourceRelease(pResource);
            break;               
        case MDrv_CMD_PM_RtcGetMatchCounter:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMRtcGetMatchCounter(param->privateRtcGetMatchCounter.pPmRtcParam);
            UtopiaResourceRelease(pResource);
            break; 
        case MDrv_CMD_PM_RtcSetSystemTime:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                    
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psPMInstPri->fpPMRtcSetSystemTime(param->privateRtcSystemTime.u32PmSysTime);
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_PM_RtcGetSystemTime:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
            
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            param->privateRtcSystemTime.u32PmSysTime = psPMInstPri->fpPMRtcGetSystemTime();
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_PM_RtcSetMatchTime:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psPMInstPri->fpPMRtcSetMatchTime(param->privateRtcSystemTime.u32PmSysTime);
            UtopiaResourceRelease(pResource);
            break;  

        case MDrv_CMD_PM_SetSPIOffsetForMCU:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psPMInstPri->fpPMSetSPIOffsetForMCU(param->privateSetSPIOffsetForMCU.BANK);
            UtopiaResourceRelease(pResource);
            break;             
        case MDrv_CMD_PM_SetSRAMOffsetForMCU:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psPMInstPri->fpPMSetSRAMOffsetForMCU();
            UtopiaResourceRelease(pResource);
            break;    
        case MDrv_CMD_PM_SetDRAMOffsetForMCU:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psPMInstPri->fpPMSetDRAMOffsetForMCU(param->privateSetDRAMOffsetForMCU.u32Offset);
            UtopiaResourceRelease(pResource);
            break;             
        case MDrv_CMD_PM_GetWakeupSource:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMGetWakeupSource();
            UtopiaResourceRelease(pResource);
            break;                
        case MDrv_CMD_PM_GetWakeupKey:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMGetWakeupKey();
            UtopiaResourceRelease(pResource);
            break;              
        case MDrv_CMD_PM_Disable51:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMDisable51();
            UtopiaResourceRelease(pResource);
            break;               
        case MDrv_CMD_PM_Check_Version:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV; 
                        
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMCheckVersion(param->privateCheck_Version.bCheck);
            UtopiaResourceRelease(pResource);
            break;             
        case MDrv_CMD_PM_GetSRAMSize:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMGetSRAMSize();
            UtopiaResourceRelease(pResource);
            break;    
        case MDrv_CMD_PM_GetIRPowerOnKey:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret=psPMInstPri->fpPMGetIRPowerOnKey();
            UtopiaResourceRelease(pResource);
            break; 
        case MDrv_CMD_PM_LoadWakeUp51:
            if(UtopiaResourceObtain(pModule, PM_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            //printf("[David] MDrv_CMD_PM_LoadWakeUp51 \n");
            psPMInstPri->fpPMLoadWakeUp51();
            UtopiaResourceRelease(pResource);
            break;    
        default:
            break;
    };

	return u32Ret;
}

MS_U32 PMClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}

