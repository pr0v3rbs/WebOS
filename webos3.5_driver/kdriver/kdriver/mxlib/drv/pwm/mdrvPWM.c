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
#include "drvPWM.h"
#include "drvPWM_v2.h"
#include "drvPWM_private.h"
#include "MsOS.h"



enum
{
    PWM_POOL_ID_PWM0=0
} ePWMPoolID;

void PWMRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_PWM, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)PWMOpen, (FUtopiaClose)PWMClose, (FUtopiaIOctl)PWMIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, PWM_POOL_ID_PWM0);
    UtopiaResourceCreate("PWM", sizeof(PWM_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, PWM_POOL_ID_PWM0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, PWM_POOL_ID_PWM0);

   // FLASHRegisterToUtopia only call once, so driver init code can put here,v
}

MS_U32 PWMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    PWM_INSTANT_PRIVATE *pPwmPri = NULL;
    printf("\n[PWM INFO] pwm open");    

    UtopiaInstanceCreate(sizeof(PWM_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pPwmPri);

    pPwmPri->fpPWMOen = _MDrv_PWM_Oen;
    pPwmPri->fpPWMPeriod = _MDrv_PWM_Period;
    pPwmPri->fpPWMDutyCycle = _MDrv_PWM_DutyCycle;
    pPwmPri->fpPWMUnitDiv = _MDrv_PWM_UnitDiv;
    pPwmPri->fpPWMDiv = _MDrv_PWM_Div;
    pPwmPri->fpPWMPolarity = _MDrv_PWM_Polarity;
    pPwmPri->fpPWMVdben = _MDrv_PWM_Vdben;
    pPwmPri->fpPWMResetEn = _MDrv_PWM_ResetEn;
    pPwmPri->fpPWMDben = _MDrv_PWM_Dben;
    pPwmPri->fpPWMShift = _MDrv_PWM_Shift;
    pPwmPri->fpPWMNvsync = _MDrv_PWM_Nvsync;

    MDrv_PWM_Init(E_PWM_DBGLV_ALL);
    return 0;
}


MS_U32 PWMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    MS_U32 u32Ret = 0;
    PWM_PRIVATE_PARAM *param = NULL;
    if (pArgs != NULL)
        param= (PWM_PRIVATE_PARAM *) pArgs;

    PWM_INSTANT_PRIVATE* psPWMInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPWMInstPri);	

    if(param == NULL)
        return UTOPIA_STATUS_ERR_INV; 
    
    switch(u32Cmd)
    {
        case MDrv_CMD_PWM_Oen:
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psPWMInstPri->fpPWMOen(param->privateOen.u8IndexPWM,
                                          param->privateOen.bOenPWM); 
            UtopiaResourceRelease(pResource);
            break; 
        case MDrv_CMD_PWM_Period:
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psPWMInstPri->fpPWMPeriod(param->privatePeriod.u8IndexPWM,
                                             param->privatePeriod.u32PeriodPWM); 
            UtopiaResourceRelease(pResource);        
            break;      
        case MDrv_CMD_PWM_DutyCycle:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psPWMInstPri->fpPWMDutyCycle(param->privateDutyCycle.u8IndexPWM,
                                                param->privateDutyCycle.u32DutyPWM); 
            UtopiaResourceRelease(pResource);            
            break;
        case MDrv_CMD_PWM_UnitDiv:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
			
            u32Ret=psPWMInstPri->fpPWMUnitDiv(param->privateUnitDiv.u16UnitDivPWM); 
            UtopiaResourceRelease(pResource);           
            break;                        
        case MDrv_CMD_PWM_Div:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMDiv(param->privateDiv.u8IndexPWM,
                                   param->privateDiv.u16DivPWM); 
            UtopiaResourceRelease(pResource);            
            break;            
        case MDrv_CMD_PWM_Polarity:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMPolarity(param->privatePolarity.u8IndexPWM,
                                        param->privatePolarity.bPolPWM); 
            UtopiaResourceRelease(pResource);           
            break;
        case MDrv_CMD_PWM_Vdben:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMVdben(param->privateVdben.u8IndexPWM,
                                     param->privateVdben.bVdbenPWM); 
            UtopiaResourceRelease(pResource);              
            break;
        case MDrv_CMD_PWM_ResetEn:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMResetEn(param->privateResetEn.u8IndexPWM,
                                       param->privateResetEn.bRstPWM); 
            UtopiaResourceRelease(pResource);            
            break;
        case MDrv_CMD_PWM_Dben:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMDben(param->privateDben.u8IndexPWM,
                                    param->privateDben.bDbenPWM); 
            UtopiaResourceRelease(pResource);          
            break;
        case MDrv_CMD_PWM_Shift:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            u32Ret=psPWMInstPri->fpPWMShift(param->privateShift.u8IndexPWM,
                                            param->privateShift.u32DutyPWM); 
            UtopiaResourceRelease(pResource);          
            break;
        case MDrv_CMD_PWM_Nvsync:
             if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            
            psPWMInstPri->fpPWMNvsync(param->privateNvsync.u8IndexPWM,
                                      param->privateNvsync.bNvsPWM); 
            UtopiaResourceRelease(pResource); 
            break;
        default:
            break;
    }

    return 0; // FIXME: error code

}

MS_U32 PWMClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}
 