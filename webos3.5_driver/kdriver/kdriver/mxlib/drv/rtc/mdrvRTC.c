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
#include "drvRTC.h"
#include "drvRTC_v2.h"
#include "drvRTC_private.h"
#include "MsOS.h"

enum
{
    RTC_POOL_ID = 0,
} eRtcPoolID;

void RTCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    void* psResource = NULL;
    
    UtopiaModuleCreate(MODULE_RTC, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)RTCOpen, (FUtopiaClose)RTCClose, (FUtopiaIOctl)RTCIoctl);

    // 2. deal with resource
    UtopiaModuleAddResourceStart(pUtopiaModule, RTC_POOL_ID);
    UtopiaResourceCreate("RTC", sizeof(RTC_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, RTC_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, RTC_POOL_ID);
}

MS_U32 RTCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    RTC_INSTANT_PRIVATE *pRtcPri = NULL;
    void *pRtcPriVoid = NULL;
    MS_DEBUG_MSG(printf("\n[RTC INFO]RTC open \n"));

    UtopiaInstanceCreate(sizeof(RTC_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pRtcPriVoid);

    pRtcPri = (RTC_INSTANT_PRIVATE *)pRtcPriVoid;
    
    pRtcPri->fpRTCInit = _MDrv_RTC_Init;
    pRtcPri->fpRTCGetCount = _MDrv_RTC_GetCount;
    pRtcPri->fpRTCDeInit = _MDrv_RTC_DeInit;

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 RTCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    MS_U32 u32Ret = 0;
    //MS_U32 u32SearchAddr;
    //MS_U32 u32CRC32;

    RTC_PRIVATE_PARAM *param = NULL;
    if (pArgs != NULL)
        param= (RTC_PRIVATE_PARAM *) pArgs;

    RTC_INSTANT_PRIVATE* psRTCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psRTCInstPri);	

    if(param == NULL)
        return UTOPIA_STATUS_ERR_INV; 
	
    switch(u32Cmd)
    {
    	case MDrv_CMD_RTC_INIT:
            if(UtopiaResourceObtain(pModule, RTC_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psRTCInstPri->fpRTCInit(param->privateInit.eRtc,
                                           param->privateInit.u32Freq,
                                           param->privateInit.u32Xtal,
                                           param->privateInit.u32Sec,
                                           param->privateInit.pIntCb); 
            UtopiaResourceRelease(pResource);
            break;
    	
        case MDrv_CMD_RTC_Get_Count:
            if(UtopiaResourceObtain(pModule, RTC_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psRTCInstPri->fpRTCGetCount(param->privateGetCount.eRtc);
            UtopiaResourceRelease(pResource);
            break;
  
        case MDrv_CMD_RTC_DeInit:
            if(UtopiaResourceObtain(pModule, RTC_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psRTCInstPri->fpRTCDeInit(param->privateDeInit.eRtc);
            UtopiaResourceRelease(pResource);
            break;

        default:
            break;
    };

	return u32Ret;
}

MS_U32 RTCClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}
 
