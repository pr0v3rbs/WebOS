

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#else
#include <string.h>
#include <stdio.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "gpd.h"
#include "apiGPD.h"
#include "MsOS.h"

#include "utopia.h"
#include "apiGPD_priv.h"
#include "apiGPD_v2.h"



enum
{
    GPD_POOL_ID_GPD0=0,
    GPD_POOL_ID_GPD_MAX
} eGPDPoolID;

#ifdef ANDROID
#ifndef LOGD
#define printf(format, args...)          ALOGI(format, ##args);
#else
#define printf(format, args...)          LOGI(format, ##args);
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#undef printf
#define printf(format, args...)          printk(format, ##args);
#endif
#endif

#if ENABLE_GPD_MESSAGE
#define DEBUG_GPD(y)   y
#else
#define DEBUG_GPD(y)   // y
#endif

// this func will be call to init by utopia20 framework
void GPDRegisterToUtopia(FUtopiaOpen ModuleType)
{
    DEBUG_GPD(printf("%s(): start\n", __FUNCTION__));
    // 1. deal with module
    void* pModuleGPD_V2;
    UtopiaModuleCreate(MODULE_GPD, 0, &pModuleGPD_V2);
    UtopiaModuleRegister(pModuleGPD_V2);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleGPD_V2,
                                    (FUtopiaOpen)GPDOpen,
                                    (FUtopiaClose)GPDClose,
                                    (FUtopiaIOctl)GPDIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pModuleGPD_V2, GPD_POOL_ID_GPD0);
    UtopiaResourceCreate("GPD", sizeof(GPD_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleGPD_V2, psResource, GPD_POOL_ID_GPD0);
    UtopiaModuleAddResourceEnd(pModuleGPD_V2, GPD_POOL_ID_GPD0);
    DEBUG_GPD(printf("%s(): end\n", __FUNCTION__));

}

MS_U32 GPDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    MS_U8 i = 0;
    DEBUG_GPD(printf("%s(): start...\n", __FUNCTION__));


    //Create instance
    UtopiaInstanceCreate(sizeof(GPD_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psGPD_InstPri);

    //Recode decoder index
    psGPD_InstPri->Index_of_decoder = i;

    psGPD_InstPri->fpGPDInit = (IOCTL_GPD_INIT)_MApi_GPD_Init;
    psGPD_InstPri->fpGPDInputSource = (IOCTL_GPD_INPUT_SOURCE)_MApi_GPD_InputSource;
    psGPD_InstPri->fpGPDOutputDecode = (IOCTL_GPD_OUTPUT_DECODE)_MApi_GPD_OutputDecode;
    psGPD_InstPri->fpGPDOutputDecodeROI = (IOCTL_GPD_OUTPUT_DECODE_ROI)_MApi_GPD_OutputDecodeROI;
    psGPD_InstPri->fpGPDOutputDecodeMGIF = (IOCTL_GPD_OUTPUT_DECODE_MGIF)_MApi_GPD_OutputDecodeMGIF;
    psGPD_InstPri->fpGPDGetDuration = (IOCTL_GPD_GET_DURATION)_MApi_GPD_GetDuration;
    psGPD_InstPri->fpGPDSetGIFMode = (IOCTL_GPD_SET_GIF_MODE)_MApi_GPD_SetGIFMode;
    psGPD_InstPri->fpGPDScalingEnable = (IOCTL_GPD_SCALING_ENABLE)_MApi_GPD_ScalingEnable;
    psGPD_InstPri->fpGPDScalingDisable = (IOCTL_GPD_SCALING_DISABLE)_MApi_GPD_ScalingDisable;
    psGPD_InstPri->fpGPDSetControl = (IOCTL_GPD_SET_CONTROL)_MApi_GPD_SetControl;
    psGPD_InstPri->fpGPDGetControl = (IOCTL_GPD_GET_CONTROL)_MApi_GPD_GetControl;
    psGPD_InstPri->fpGPDGetCRCResult = (IOCTL_GPD_GET_CRC_RESULT)_MApi_GPD_GetCRCResult;

    void* pModuleGPD_V2;
    UtopiaModuleGetPtr(MODULE_GPD,&pModuleGPD_V2);

    // Check there is available decoder to be use
    if(UtopiaResourceObtain(pModuleGPD_V2, GPD_POOL_ID_GPD0, &psGPD_InstPri->pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceRelease(psGPD_InstPri->pResource);
    DEBUG_GPD(printf("%s(): end\n", __FUNCTION__));

    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 GPDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psGPD_InstPri);
    E_GPD_V2_IOCTL_CMD IOcmd = (E_GPD_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_SUCCESS;

    PGPD_INIT_PARAM pInit = NULL;
    PGPD_INPUTSOURCE_PARAM pInputSource = NULL;
    PGPD_OUTPUTDECODE_PARAM pOutputDecode = NULL;
    PGPD_OUTPUTDECODEROI_PARAM pOutputDecodeROI = NULL;
    PGPD_GETDURATION_PARAM pGetDuration = NULL;
    PGPD_SETGIFMODE_PARAM pSetGIFMode = NULL;
    PGPD_SCALINGENABLE_PARAM pScalingEnable = NULL;
    PGPD_CONTROL_PARAM pControl = NULL;
    PGPD_GET_CONTROL_PARAM pGetControl = NULL;
    PGPD_GETCRCRESULT_PARAM pGetCRCResult = NULL;

    switch(IOcmd)
    {
        case MApi_CMD_GPD_Init:
            pInit = (PGPD_INIT_PARAM)pArgs;
            pInit->retVal = psGPD_InstPri->fpGPDInit(pInit->SrcBuf);
            break;
        case MApi_CMD_GPD_InputSource:
            pInputSource = (PGPD_INPUTSOURCE_PARAM)pArgs;
            pInputSource->retVal = psGPD_InstPri->fpGPDInputSource(pInputSource->param, pInputSource->InputBuf, pInputSource->Size);
            break;
        case MApi_CMD_GPD_OutputDecode:
            pOutputDecode = (PGPD_OUTPUTDECODE_PARAM)pArgs;
            pOutputDecode->retVal = psGPD_InstPri->fpGPDOutputDecode(pOutputDecode->DecodeBuf, pOutputDecode->ocolor, pOutputDecode->Size);
            break;
        case MApi_CMD_GPD_OutputDecodeROI:
            pOutputDecodeROI = (PGPD_OUTPUTDECODEROI_PARAM)pArgs;
            pOutputDecodeROI->retVal = psGPD_InstPri->fpGPDOutputDecodeROI(pOutputDecodeROI->DecodeBuf, pOutputDecodeROI->ocolor, pOutputDecodeROI->Size, pOutputDecodeROI->roi);
            break;
        case MApi_CMD_GPD_OutputDecodeMGIF:
            pOutputDecode= (PGPD_OUTPUTDECODE_PARAM)pArgs;
            pOutputDecode->retVal = psGPD_InstPri->fpGPDOutputDecodeMGIF(pOutputDecode->DecodeBuf, pOutputDecode->ocolor, pOutputDecode->Size);
            break;
        case MApi_CMD_GPD_GetDuration:
            pGetDuration = (PGPD_GETDURATION_PARAM)pArgs;
            pGetDuration->retVal = psGPD_InstPri->fpGPDGetDuration();
            break;
        case MApi_CMD_GPD_SetGIFMode:
            pSetGIFMode = (PGPD_SETGIFMODE_PARAM)pArgs;
            psGPD_InstPri->fpGPDSetGIFMode(pSetGIFMode->mode);
            break;
        case MApi_CMD_GPD_ScalingEnable:
            pScalingEnable = (PGPD_SCALINGENABLE_PARAM)pArgs;
            psGPD_InstPri->fpGPDScalingEnable(pScalingEnable->mode);
            break;
        case MApi_CMD_GPD_ScalingDisable:
            psGPD_InstPri->fpGPDScalingDisable();
            break;
        case MApi_CMD_GPD_SetControl:
            pControl = (PGPD_CONTROL_PARAM)pArgs;
            pControl->retVal = psGPD_InstPri->fpGPDSetControl(pControl->cmd_id, pControl->param);
            break;
        case MApi_CMD_GPD_GetControl:
            pGetControl = (PGPD_GET_CONTROL_PARAM)pArgs;
            pGetControl->retVal = psGPD_InstPri->fpGPDGetControl(pGetControl->cmd_id, pGetControl->param);
            break;
        case MApi_CMD_GPD_GetCRCResult:
            pGetCRCResult = (PGPD_GETCRCRESULT_PARAM)pArgs;
            pGetCRCResult->retVal = psGPD_InstPri->fpGPDGetCRCResult(pGetCRCResult->pu8Data, pGetCRCResult->u32Size);
            break;

        default:
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            printf("Error Command = %ld\n",u32Cmd);
            break;

    }
    return ret;
}
MS_U32 GPDClose(void* pInstance)
{
    GPD_RESOURCE_PRIVATE* pGPD_ResourcePrivate = NULL;
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    DEBUG_GPD(printf("%s(): start\n", __FUNCTION__));

    // Get instance private
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGPD_InstPri);


    UtopiaResourceGetPrivate(psGPD_InstPri->pResource,(void*)(&pGPD_ResourcePrivate));

    // Set using decoder index to 0
    pGPD_ResourcePrivate->u8DecoderInUse[psGPD_InstPri->Index_of_decoder] = 0;

    UtopiaResourceRelease(psGPD_InstPri->pResource);

    UtopiaInstanceDelete(pInstance);
    DEBUG_GPD(printf("%s(): end\n", __FUNCTION__));

    return UTOPIA_STATUS_SUCCESS;
}


