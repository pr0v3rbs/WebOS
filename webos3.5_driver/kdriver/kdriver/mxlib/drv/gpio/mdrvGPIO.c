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
#include "drvGPIO.h"
#include "drvGPIO_private.h"
#include "MsOS.h"
#include "halGPIO.h"
#include "drvGPIO_v2.h"


enum
{
    GPIO_POOL_ID = 0,
} eGpioPoolID;


typedef struct _GPIO_STR
{
	MS_U32 u32PowerState;
	GPIO_Mode_Str gGpioModeStr[END_GPIO_NUM+1];

}GPIO_STR;


void GPIORegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    void* psResource = NULL;

    UtopiaModuleCreate(MODULE_GPIO, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)GPIOOpen, (FUtopiaClose)GPIOClose, (FUtopiaIOctl)GPIOIoctl);

    #if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)GPIOStr);
    UtopiaModuleSetSTRPrivate(pUtopiaModule, sizeof(GPIO_STR));
    #endif

    // 2. deal with resource
    UtopiaModuleAddResourceStart(pUtopiaModule, GPIO_POOL_ID);
    UtopiaResourceCreate("GPIO", sizeof(GPIO_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, GPIO_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, GPIO_POOL_ID);
}

MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    GPIO_INSTANT_PRIVATE *pGpioPri = NULL;
    void *pGpioPriVoid = NULL;
    MS_DEBUG_MSG(printf("\n[GPIO INFO]GPIO open \n"));

    UtopiaInstanceCreate(sizeof(GPIO_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pGpioPriVoid);

    pGpioPri = (GPIO_INSTANT_PRIVATE *)pGpioPriVoid;
    pGpioPri->fpGPIOInit = _mdrv_gpio_init;
    pGpioPri->fpGPIOSetHigh = _mdrv_gpio_set_high;
    pGpioPri->fpGPIOSetLow = _mdrv_gpio_set_low;
    pGpioPri->fpGPIOSetInput = _mdrv_gpio_set_input;
    pGpioPri->fpGPIOGetInout = _mdrv_gpio_get_inout;
    pGpioPri->fpGPIOGetLevel = _mdrv_gpio_get_level;
    pGpioPri->fpGPIOBusStart= _mdrv_gpio_bus_start;
    pGpioPri->fpGPIOSendBusData = _mdrv_gpio_send_bus_data;
    pGpioPri->fpGPIOGetBusData = _mdrv_gpio_get_bus_data;
    pGpioPri->fpGPIOBusStop= _mdrv_gpio_bus_stop;
    pGpioPri->fpGPIOGetIntPolarity = MDrv_GPIO_Get_INT_Polarity_U2K;
    pGpioPri->fpGPIOSetIntPolarity = MDrv_GPIO_Set_INT_Polarity_U2K;
    pGpioPri->fpGPIOGetMapGpioIntToNum = MDrv_GPIO_GetMap_GPIOIntToNum_U2K; 

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GPIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    void* pResource = NULL;
    GPIO_PRIVATE_PARAM *param = NULL;
    GPIO_INSTANT_PRIVATE* psGPIOInstPri = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if (pArgs != NULL)
        param= (GPIO_PRIVATE_PARAM *) pArgs;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGPIOInstPri);

    switch(u32Cmd)
    {
    	case MDrv_CMD_GPIO_INIT:
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psGPIOInstPri->fpGPIOInit();
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_GPIO_Set_HIGH:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psGPIOInstPri->fpGPIOSetHigh(param->eGpio);
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_GPIO_Set_LOW:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psGPIOInstPri->fpGPIOSetLow(param->eGpio);
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_GPIO_Set_INPUT:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psGPIOInstPri->fpGPIOSetInput(param->eGpio);
          //  printf("MDrv_CMD_GPIO_Set_INPUT = %d\n",param->level);
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_GPIO_Get_INOUT:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;
            
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            param->level = psGPIOInstPri->fpGPIOGetInout(param->eGpio);
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_GPIO_Get_LEVEL:

            if(param == NULL)
                return UTOPIA_STATUS_ERR_INV;            
            
            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            param->level = psGPIOInstPri->fpGPIOGetLevel(param->eGpio);
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_GPIO_BUS_START:
			{
				GPIO_Bus_Data *BusData;

				BusData = NULL;
				if (pArgs != NULL)
				{
					BusData=(GPIO_Bus_Data *) pArgs;
				}
				else
				{
					return UTOPIA_STATUS_ERR_INV;
				}

				if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
				{
					printf("UtopiaResourceObtainToInstant fail\n");
					return UTOPIA_STATUS_ERR_RESOURCE;
				}

				*(BusData->bRet)=(psGPIOInstPri->fpGPIOBusStart)(BusData);

				UtopiaResourceRelease(pResource);
				break;
        	}
		case MDrv_CMD_GPIO_SEND_BUS_DATA:
			{
			GPIO_Bus_Data *BusData;
			BusData = NULL;
			if (pArgs != NULL)
			{
				BusData=(GPIO_Bus_Data *) pArgs;
			}
			else
			{
				return UTOPIA_STATUS_ERR_INV;
			}

            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

			*(BusData->bRet) =(psGPIOInstPri->fpGPIOSendBusData)(BusData);

            UtopiaResourceRelease(pResource);
			break;
			}
		case MDrv_CMD_GPIO_GET_BUS_DATA:
			{
			GPIO_Bus_Data *BusData;
			BusData = NULL;

			if (pArgs != NULL)
			{
				BusData=(GPIO_Bus_Data *) pArgs;
			}
			else
			{
				return UTOPIA_STATUS_ERR_INV;
			}

            if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

			*(BusData->bRet)=(psGPIOInstPri->fpGPIOGetBusData)(BusData);

			UtopiaResourceRelease(pResource);
			break;
			}
		case MDrv_CMD_GPIO_BUS_STOP:
		    {
			GPIO_Bus_Data *BusData;
	        BusData = NULL;

			if (pArgs != NULL)
			{
				BusData=(GPIO_Bus_Data *) pArgs;
			}
			else
			{
				return UTOPIA_STATUS_ERR_INV;
			}

			if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
			{
				printf("UtopiaResourceObtainToInstant fail\n");
				return UTOPIA_STATUS_ERR_RESOURCE;
			}
			(psGPIOInstPri->fpGPIOBusStop)(BusData);

			UtopiaResourceRelease(pResource);
			break;
			}
        case MDrv_CMD_GPIO_Get_INT_Polarity:
            {
                GPIO_PRIVATE_INT_PARAM *pGpioIntData = (GPIO_PRIVATE_INT_PARAM *) param;

                if(param == NULL)
                    return UTOPIA_STATUS_ERR_INV;

                if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
                    return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pGpioIntData->result = (psGPIOInstPri->fpGPIOGetIntPolarity)(pGpioIntData->eGPIO_IRQnum, &(pGpioIntData->gpio_edge_type));

                UtopiaResourceRelease(pResource);
            }
            break;
			
        case MDrv_CMD_GPIO_Set_INT_Polarity:
            {
                GPIO_PRIVATE_INT_PARAM *pGpioIntData = (GPIO_PRIVATE_INT_PARAM *) param;

                if(param == NULL)
                    return UTOPIA_STATUS_ERR_INV;                

                if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
                    return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pGpioIntData->result = (psGPIOInstPri->fpGPIOSetIntPolarity)(pGpioIntData->eGPIO_IRQnum, pGpioIntData->gpio_edge_type);
			
                UtopiaResourceRelease(pResource);
            }
			break;
			
        case MDrv_CMD_GPIO_GetMap_GPIOIntToNum:
            {
                GPIO_PRIVATE_INT_PARAM *pGpioIntData = (GPIO_PRIVATE_INT_PARAM *) param;

                if(param == NULL)
                    return UTOPIA_STATUS_ERR_INV;

                if(UtopiaResourceObtain(pModule, GPIO_POOL_ID, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
                    return UTOPIA_STATUS_ERR_RESOURCE;
			}

                pGpioIntData->result = (psGPIOInstPri->fpGPIOGetMapGpioIntToNum)(pGpioIntData->eGPIO_IRQnum, &(pGpioIntData->eGPIONum));

                UtopiaResourceRelease(pResource);
            }
            break;		
        default:
	   return UTOPIA_STATUS_NOT_SUPPORTED;
            break;
    };

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GPIOClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_U32 GPIOStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_SUCCESS;
    GPIO_STR* pSTRPrivate;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pSTRPrivate);

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        printf("[%s][%d] Suspend Start!!!!!\n",__FUNCTION__,__LINE__);
        MDrv_GPIO_Copy_STR((pSTRPrivate->gGpioModeStr));
        pSTRPrivate->u32PowerState = u32PowerState;
        printf("[%s][%d] Suspend End!!!!!\n",__FUNCTION__,__LINE__);
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        printf("[%s][%d] Resume Start!!!!!\n",__FUNCTION__,__LINE__);
        MDrv_GPIO_SetPowerState_U2K((pSTRPrivate->gGpioModeStr), u32PowerState, pModule);
        pSTRPrivate->u32PowerState=u32PowerState;
        printf("[%s][%d] Resume End!!!!!\n",__FUNCTION__,__LINE__);
    }
    else
    {
        printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}
#endif

