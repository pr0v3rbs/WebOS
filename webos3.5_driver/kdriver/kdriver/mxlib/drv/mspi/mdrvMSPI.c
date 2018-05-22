#include "MsTypes.h"
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#else
#include <linux/slab.h>
#endif

#include "utopia_dapi.h"
#include "utopia.h"
#include "drvMSPI.h"
#include "drvMSPI_private.h"
#include "MsOS.h"

enum
{
    MSPI_POOL_ID=0
} eMspiPoolID;

void MSPIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
#ifdef PRINT
    printf("\n[MSPI INFO] master spi register to utopia \n");
#endif
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_MSPI, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)MSPIOpen, (FUtopiaClose)MSPIClose, (FUtopiaIOctl)MSPIIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, MSPI_POOL_ID);
    UtopiaResourceCreate("MSPI", sizeof(MSPI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, MSPI_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, MSPI_POOL_ID);

}

MS_U32 MSPIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
#ifdef PRINT
    printf("\n[MSPI INFO] master spi open \n");
#endif
    MSPI_INSTANT_PRIVATE *pMSPIPri = NULL;
    UtopiaInstanceCreate(sizeof(MSPI_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pMSPIPri);
#if (MSPI_UTOPIA20)
    pMSPIPri->fpMSPI_Init_Ext = _MDrv_MSPI_Init_Ext;
    pMSPIPri->fpMSPI_Init = _MDrv_MSPI_Init;
    pMSPIPri->fpMSPI_Read = _MDrv_MSPI_Read;
    pMSPIPri->fpMSPI_Write = _MDrv_MSPI_Write;
    pMSPIPri->fpMSPI_DCConfig = _MDrv_MSPI_DCConfig;
    pMSPIPri->fpMSPI_CLKConfig = _MDrv_MSPI_CLKConfig;
    pMSPIPri->fpMSPI_FRAMEConfig = _MDrv_MSPI_FRAMEConfig;
    pMSPIPri->fpMSPI_SlaveEnable = _MDrv_MSPI_SlaveEnable;
    pMSPIPri->fpMSPI_SetPowerState = _MDrv_MSPI_SetPowerState; 
    pMSPIPri->fpMSPI_SetDbgLevel = _MDrv_MSPI_SetDbgLevel;
    pMSPIPri->fpMasterSPI_Init = _MDrv_MasterSPI_Init;
    pMSPIPri->fpMasterSPI_Read = _MDrv_MasterSPI_Read;
    pMSPIPri->fpMasterSPI_Write = _MDrv_MasterSPI_Write;
    pMSPIPri->fpMasterSPI_DCConfig = _MDrv_MasterSPI_DCConfig;
    pMSPIPri->fpMasterSPI_CLKConfig = _MDrv_MasterSPI_CLKConfig;
    pMSPIPri->fpMasterSPI_FrameConfig = _MDrv_MasterSPI_FRAMEConfig;
    pMSPIPri->fpMasterSPI_SlaveEnable = _MDrv_MasterSPI_SlaveEnable;
    pMSPIPri->fpMasterSPI_CsPadConfig = _MDrv_MasterSPI_CsPadConfig;
    pMSPIPri->fpMasterSPI_MaxClkConfig = _MDrv_MasterSPI_MaxClkConfig;
#else
    pMSPIPri->fpMSPI_Init_Ext = MDrv_MSPI_Init_Ext;
    pMSPIPri->fpMSPI_Init = MDrv_MSPI_Init;
    pMSPIPri->fpMSPI_Read = MDrv_MSPI_Read;
    pMSPIPri->fpMSPI_Write = MDrv_MSPI_Write;
    pMSPIPri->fpMSPI_DCConfig = MDrv_MSPI_DCConfig;
    pMSPIPri->fpMSPI_CLKConfig = MDrv_MSPI_CLKConfig;
    pMSPIPri->fpMSPI_FRAMEConfig = MDrv_MSPI_FRAMEConfig;
    pMSPIPri->fpMSPI_SlaveEnable = MDrv_MSPI_SlaveEnable;
    pMSPIPri->fpMSPI_SetPowerState = MDrv_MSPI_SetPowerState; 
    pMSPIPri->fpMSPI_SetDbgLevel = MDrv_MSPI_SetDbgLevel;
    pMSPIPri->fpMasterSPI_Init = MDrv_MasterSPI_Init;
    pMSPIPri->fpMasterSPI_Read = MDrv_MasterSPI_Read;
    pMSPIPri->fpMasterSPI_Write = MDrv_MasterSPI_Write;
    pMSPIPri->fpMasterSPI_DCConfig = MDrv_MasterSPI_DCConfig;
    pMSPIPri->fpMasterSPI_CLKConfig = MDrv_MasterSPI_CLKConfig;
    pMSPIPri->fpMasterSPI_FrameConfig = MDrv_MasterSPI_FRAMEConfig;
    pMSPIPri->fpMasterSPI_SlaveEnable = MDrv_MasterSPI_SlaveEnable;
    pMSPIPri->fpMasterSPI_CsPadConfig = MDrv_MasterSPI_CsPadConfig;
    pMSPIPri->fpMasterSPI_MaxClkConfig = MDrv_MasterSPI_MaxClkConfig;
#endif

    return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 MSPIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    PMSPI_INIT_EXT pInitExtParam = NULL;
    PMSPI_INIT pInitParam = NULL;
    PMSPI_READ pReadParam = NULL;
    PMSPI_WRITE pWriteParam = NULL;
    PMSPI_DCCONFIG pDcconfigParam = NULL;
    PMSPI_CLKCONFIG pCLKConfigParam = NULL;
    PMSPI_FRAMECONFIG pFrameConfigParam = NULL;
    PMSPI_SLAVE_ENABLE pSlaveEnableParam = NULL;
    PMSPISETPOWERSTATE pSetPowerStateParam = NULL; 
    PMSPI_SETDBGLEVEL pSetDbgLevelParam = NULL;
    PMASTERSPI_READ pMSPIReadParam = NULL;
    PMASTERSPI_WRITE pMSPIWriteParam = NULL;
    PMASTERSPI_DCCONFIG pMSPIDcconfigParam = NULL;
    PMASTERSPI_CLKCONFIG pMSPICLKConfigParam = NULL;
    PMASTERSPI_FRAMECONFIG pMSPIFrameConfigParam = NULL;
    PMASTERSPI_SLAVE_ENABLE pMSPISlaveEnableParam = NULL;
    PMASTERSPI_CSPADCONFIG pMSPISetCsPadConfigParam = NULL;
    PMASTERSPI_MAXCLKCONFIG pMSPIMaxClkConfigParam = NULL;

    MS_U32 u32Ret;
    MSPI_INSTANT_PRIVATE* psMSPIInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psMSPIInstPri);

    switch(u32Cmd)
    {
        case MDrv_CMD_MSPI_Init_Ext:
            pInitExtParam = (PMSPI_INIT_EXT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Init_Ext(pInitExtParam->u8HWNum);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_Init:
            pInitParam = (PMSPI_INIT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                    return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Init(pInitParam->ptMSPIConfig, pInitParam->u8HWNum);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_Read:
            pReadParam = (PMSPI_READ)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                    return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Read(pReadParam->pu8Data, pReadParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_Write:
            pWriteParam = (PMSPI_WRITE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
              
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Write(pWriteParam->pu8Data, pWriteParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_DCConfig:
            pDcconfigParam = (PMSPI_DCCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            psMSPIInstPri->fpMSPI_DCConfig(pDcconfigParam->ptDCConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_CLKConfig:
            pCLKConfigParam = (PMSPI_CLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_CLKConfig(pCLKConfigParam->ptCLKConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_FRAMEConfig:
            pFrameConfigParam = (PMSPI_FRAMECONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_FRAMEConfig(pFrameConfigParam->ptFrameConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_SlaveEnable:
            pSlaveEnableParam = (PMSPI_SLAVE_ENABLE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            psMSPIInstPri->fpMSPI_SlaveEnable(pSlaveEnableParam->bEnable);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_SetPowerState:
            pSetPowerStateParam = (PMSPISETPOWERSTATE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
			u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_SetPowerState(pSetPowerStateParam->enPowerState);
            UtopiaResourceRelease(pResource);
            return 0; 
        case MDrv_CMD_MSPI_DbgLEvel:
            pSetDbgLevelParam = (PMSPI_SETDBGLEVEL)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }			  
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_SetDbgLevel(pSetDbgLevelParam->u8DbgLevel);
            UtopiaResourceRelease(pResource);
            return 0;

        //===================================================================
        //              MSPI  NEW API of support slave device id 
        //===================================================================
        case MDrv_CMD_MasterSPI_Init:
            pInitParam = (PMSPI_INIT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Init(pInitParam->ptMSPIConfig, pInitParam->u8HWNum);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_Read:
            pMSPIReadParam = (PMASTERSPI_READ)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Read(pMSPIReadParam->u32DevID, pMSPIReadParam->pu8Data, pMSPIReadParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_Write:
            pMSPIWriteParam = (PMASTERSPI_WRITE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Write(pMSPIWriteParam->u32DevID, pMSPIWriteParam->pu8Data, pMSPIWriteParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_DCConfig:
            pMSPIDcconfigParam = (PMASTERSPI_DCCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_DCConfig(pMSPIDcconfigParam->u32DevID, pMSPIDcconfigParam->ptDCConfig);
            UtopiaResourceRelease(pResource);
            return 0;

        case MDrv_CMD_MasterSPI_CLKConfig:
            pMSPICLKConfigParam = (PMASTERSPI_CLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_CLKConfig(pMSPICLKConfigParam->u32DevID, pMSPICLKConfigParam->ptCLKConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_FRAMEConfig:
            pMSPIFrameConfigParam = (PMASTERSPI_FRAMECONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_FrameConfig(pMSPIFrameConfigParam->u32DevID, pMSPIFrameConfigParam->ptFrameConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_SlaveEnable:
            pMSPISlaveEnableParam = (PMASTERSPI_SLAVE_ENABLE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_SlaveEnable(pMSPISlaveEnableParam->u32DevID, pMSPISlaveEnableParam->bEnable);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_CSPadConfig:
            pMSPISetCsPadConfigParam = (PMASTERSPI_CSPADCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_CsPadConfig(pMSPISetCsPadConfigParam->u32DevID, pMSPISetCsPadConfigParam->u32CsPad);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_MaxClkConfig:
			pMSPIMaxClkConfigParam = (PMASTERSPI_MAXCLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_MaxClkConfig(pMSPIMaxClkConfigParam->u32DevID, pMSPIMaxClkConfigParam->u32MaxClk);
            UtopiaResourceRelease(pResource);
            return 0;
        default:
            break;
    };

	return 0; // FIXME: error code
}

MS_U32 MSPIClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}



