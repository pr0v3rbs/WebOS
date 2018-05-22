

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#include <stdio.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "MsOS.h"

#include "utopia.h"

#include "apiMFE_priv.h"
#include "apiMFE_v2.h"

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "madp_ms_dprintf.h"
#include "mdrv_mfe_io.h"
#include "drv_mfe_st.h"
#include "madp_mfe.h"
#include "apiVOIPMFE.h"




enum
{
    MFE_POOL_ID_MFE0=0,
    MFE_POOL_ID_MFE_MAX
} eMfePoolID;



// this func will be call to init by utopia20 framework
void MFERegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pModuleMFE_V2 = NULL;
    //printf("MFERegisterToUtopia\n");
    // 1. deal with module
    UtopiaModuleCreate(MODULE_MFE, 4, &pModuleMFE_V2);
    UtopiaModuleRegister(pModuleMFE_V2);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleMFE_V2,
                                    (FUtopiaOpen)MFEOpen,
                                    (FUtopiaClose)MFEClose,
                                    (FUtopiaIOctl)MFEIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pModuleMFE_V2, MFE_POOL_ID_MFE0);
    UtopiaResourceCreate("MFE", sizeof(MFE_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleMFE_V2, psResource, MFE_POOL_ID_MFE0);
    UtopiaModuleAddResourceEnd(pModuleMFE_V2, MFE_POOL_ID_MFE0);

    //init Instance number
    MFE_RESOURCE_PRIVATE* pMFE_ResourcePrivate;
    UtopiaResourceGetPrivate(psResource,(void*)(&pMFE_ResourcePrivate));
    pMFE_ResourcePrivate->sAliveInstance = 0;
}

MS_U32 MFEOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MFE_INSTANCE_PRIVATE* psMFE_InstPri = NULL;
    printf("MFEOpen\n");
    //Create instance
    UtopiaInstanceCreate(sizeof(MFE_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psMFE_InstPri);

    psMFE_InstPri->fpMFEInit = (IOCTL_MFE_INIT)_MApi_MFE_Init;
    psMFE_InstPri->fpMFEEncode = (IOCTL_MFE_ENCODE)_MApi_MFE_Encode;
    psMFE_InstPri->fpMFEGetVOL = (IOCTL_MFE_GETVOL)_MApi_MFE_GetVOL;
    psMFE_InstPri->fpMFECompressOnePicture = (IOCTL_MFE_COMPRESSONEPICTURE)_MApi_MFE_CompressOnePicture;
    psMFE_InstPri->fpMFEDeInit = (IOCTL_MFE_DEINIT)_MApi_MFE_DeInit;
    psMFE_InstPri->fpMFEPowerOff = (IOCTL_MFE_POWEROFF)_MApi_MFE_PowerOff;

    psMFE_InstPri->fpMFEGetOBuf = (IOCTL_MFE_GETOBUF)_MApi_MFE_GetOBuf;

    psMFE_InstPri->fpMFESetBitrateFramerate = (IOCTL_MFE_SETBITRATE_FRAMERATE)_MApi_MFE_SetBitrateFramerate;
    psMFE_InstPri->fpMFESetColorFormat = (IOCTL_MFE_SETCOLORFORMAT)_MApi_MFE_SetColorFormat;
    psMFE_InstPri->fpMFESetISR = (IOCTL_MFE_SETISR) _MApi_MFE_ISREnable;
    psMFE_InstPri->fpMFEResetSPSPPS = (IOCTL_MFE_RESETSPSPPS)_MApi_MFE_ResetSPSPPS;
    psMFE_InstPri->fpMFESetFrameType = (IOCTL_MFE_SETFRAMETYPE)_MApi_MFE_SetFrameType;
    psMFE_InstPri->fpMFEGetOutputInfo = (IOCTL_MFE_GETOUTPUT_INFO)_MApi_MFE_GetOutputInfo;
    psMFE_InstPri->fpMFESetVUI = (IOCTL_MFE_SETVUI)_MApi_MFE_SetVUI;
    psMFE_InstPri->fpMFEGetPowerSuspend = (IOCTL_MFE_GETPOWERSUPSPEND)_MApi_MFE_GetConf_PowerSuspend;
    psMFE_InstPri->fpMFEClearPowerSuspend = (IOCTL_MFE_CLEARPOWERSUPSPEND)_MApi_MFE_SetConf_Clear_PowerSuspend;
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 MFEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    MFE_INSTANCE_PRIVATE* psMFE_InstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psMFE_InstPri);
    E_MFE_V2_IOCTL_CMD IOcmd = (E_MFE_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_SUCCESS;
//    printf("MFEIoctl\n");
    void* pResource = NULL;
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, MFE_POOL_ID_MFE0, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    switch(IOcmd)
    {
        case MApi_CMD_MFE_Init:
            {
                MFE_RESOURCE_PRIVATE* pMFE_ResourcePrivate;
                UtopiaResourceGetPrivate(pResource,(void*)(&pMFE_ResourcePrivate));
                pMFE_ResourcePrivate->sAliveInstance += 1;
                printf("MFEOpen sAliveInstance = %d\n",(int)pMFE_ResourcePrivate->sAliveInstance);
                printf("MApi_CMD_MFE_Init\n");
                PMFE_INIT_PARAM pInit = NULL;
                pInit = (PMFE_INIT_PARAM)pArgs;
                printf("MApi_CMD_MFE_Init, pInit=%p\n",pInit);
                pInit->retVal = psMFE_InstPri->fpMFEInit(pInit->mfe_Info);
                if(pInit->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_Encode:
            {
                PMFE_INIT_PARAM pInit = NULL;
                pInit = (PMFE_INIT_PARAM)pArgs;
                pInit->retVal = psMFE_InstPri->fpMFEEncode(pInit->mfe_Info);
                if(pInit->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_GetVOL:
            {
                PMFE_GETVOL_PARAM pGetVOL = NULL;
                pGetVOL = (PMFE_GETVOL_PARAM)pArgs;
                pGetVOL->retVal = psMFE_InstPri->fpMFEGetVOL(pGetVOL->mfe_Info,pGetVOL->header_info);
                if(pGetVOL->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_CompressOnePicture:
            {
                PMFE_COMPRESSONEPICTURE_PARAM pCompressOnePicture = NULL;
                pCompressOnePicture = (PMFE_COMPRESSONEPICTURE_PARAM)pArgs;
                pCompressOnePicture->retVal =
                    psMFE_InstPri->fpMFECompressOnePicture(pCompressOnePicture->mfe_Info,pCompressOnePicture->YUVPlane,pCompressOnePicture->bForceIframe);
                if(pCompressOnePicture->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_DeInit:
            {
                PMFE_INIT_PARAM pInit = NULL;
                pInit = (PMFE_INIT_PARAM)pArgs;
                MFE_RESOURCE_PRIVATE* pMFE_ResourcePrivate;
                UtopiaResourceGetPrivate(pResource,(void*)(&pMFE_ResourcePrivate));
                pMFE_ResourcePrivate->sAliveInstance -= 1;
                //printf("[%s][%d]MFEDeINIT sAliveInstance = %d\n",(int)pMFE_ResourcePrivate->sAliveInstance,__FUNCTION__,__LINE__);
                pInit->retVal = psMFE_InstPri->fpMFEDeInit(pInit->mfe_Info);
                if(pInit->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_PowerOff:
            {
                PMFE_INIT_PARAM pInit = NULL;
                pInit = (PMFE_INIT_PARAM)pArgs;
                pInit->retVal = psMFE_InstPri->fpMFEPowerOff(pInit->mfe_Info);
                if(pInit->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_GetOutBuffer:
            {
                PMFE_GET_OBUF_PARAM pGetOBuf = NULL;
                pGetOBuf = (PMFE_GET_OBUF_PARAM)pArgs;
                pGetOBuf->retVal = psMFE_InstPri->fpMFEGetOBuf(pGetOBuf->mfe_Info,pGetOBuf->outbuf);
                if(pGetOBuf->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SetBitrateFramerate:
            {
                PMFE_INIT_PARAM pSet = NULL;
                pSet = (PMFE_INIT_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFESetBitrateFramerate(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SetColorFormat:
            {
                PMFE_INIT_PARAM pSet = NULL;
                pSet = (PMFE_INIT_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFESetColorFormat(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SetISR:
            {
                PMFE_INIT_PARAM pSet = NULL;
                pSet = (PMFE_INIT_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFESetISR(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_ResetSPSPPS:
            {
                PMFE_INIT_PARAM pSet = NULL;
                pSet = (PMFE_INIT_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFEResetSPSPPS(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SetFrameType:
            {
                PMFE_SET_PARAM pSet = NULL;
                pSet = (PMFE_SET_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFESetFrameType(pSet->mfe_Info,pSet->frametype);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_GetOutputInfo:
            {
                PMFE_GETOUTPUT_PARAM pSet = NULL;
                pSet = (PMFE_GETOUTPUT_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFEGetOutputInfo(pSet->mfe_Info, &pSet->frametype, &pSet->size);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SetVUI:
            {
                PMFE_SETVUI_PARAM pSet = NULL;
                pSet = (PMFE_SETVUI_PARAM)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFESetVUI(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_GETCONF_POWERSUSPEND:
            {
                PMFE_GETCOFG_POWERSUSPEND pSet = NULL;
                pSet = (PMFE_GETCOFG_POWERSUSPEND)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFEGetPowerSuspend(pSet->mfe_Info, &(pSet->u32MFEPWRSuspend));
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }
        case MApi_CMD_MFE_SETCONF_CLEARPOWERSUSPEND:
            {
                PMFE_GETCOFG_POWERSUSPEND pSet = NULL;
                pSet = (PMFE_GETCOFG_POWERSUSPEND)pArgs;
                pSet->retVal = psMFE_InstPri->fpMFEClearPowerSuspend(pSet->mfe_Info);
                if(pSet->retVal == FALSE)
                    ret = UTOPIA_STATUS_FAIL;
                break;
            }

        default:
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            printf("Error Command = %ld\n",u32Cmd);
            break;

    }

    UtopiaResourceRelease(pResource);
    return ret;
}
MS_U32 MFEClose(void* pInstance)
{
    void* pResource = NULL;
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, MFE_POOL_ID_MFE0, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    MFE_RESOURCE_PRIVATE* pMFE_ResourcePrivate;
    UtopiaResourceGetPrivate(pResource,(void*)(&pMFE_ResourcePrivate));
    //pMFE_ResourcePrivate->sAliveInstance -= 1;
    //printf("pMFE_ResourcePrivate->sAliveInstance = %d,%s,%d\n",pMFE_ResourcePrivate->sAliveInstance,__FUNCTION__,__LINE__);

    if(pMFE_ResourcePrivate->sAliveInstance < 0) {
        printf("[ERROR]MFEClose sAliveInstance = %d, %s,%d\n",
            (int)pMFE_ResourcePrivate->sAliveInstance,__FUNCTION__,__LINE__);
        UtopiaResourceRelease(pResource);
        return UTOPIA_STATUS_ERR_RESOURCE;
    } else if(pMFE_ResourcePrivate->sAliveInstance == 0) {
        MFE_INSTANCE_PRIVATE* psMFE_InstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void*)&psMFE_InstPri);
        psMFE_InstPri->fpMFEPowerOff(NULL);
        printf("power off MFE HW\n");
    }
    printf("MFEClose sAliveInstance = %d\n",(int)pMFE_ResourcePrivate->sAliveInstance);
    UtopiaResourceRelease(pResource);

    UtopiaInstanceDelete(pInstance);
    return UTOPIA_STATUS_SUCCESS;
}


