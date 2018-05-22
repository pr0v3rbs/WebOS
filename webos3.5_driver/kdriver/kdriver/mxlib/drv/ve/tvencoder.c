#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "utopia_driver_id.h"

#include "MsDevice.h"
#include "drvTVEncoder.h"

#include "MsOS.h"
#include "tvencoder_private.h"
#include "tvencoder.h"

enum{
    E_TVENCODER_POOL_ID_VE0,
} eTvencoderResourceID;
#define IOCTL_SUCCESS   0
#define MSG_TVE(x) //(x)
// ========== Global definition ==========
void* pUtopiaTVEncoder=NULL;

// this func will be call to init by utopia20 framework
void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType)
{
    TVENCODER_RESOURCE_PRIVATE * pResPri = NULL;
    void* psResource = NULL;
    // 1. deal with module
    //void* pUtopiaTVEncoder=NULL;
    UtopiaModuleCreate(MODULE_TVENCODER, 8, &pUtopiaTVEncoder);
    UtopiaModuleRegister(pUtopiaTVEncoder);
    // register func for module, after register here,then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaTVEncoder, (FUtopiaOpen)TVENCODEROpen, (FUtopiaClose)TVENCODERClose, (FUtopiaIOctl)TVENCODERIoctl);

    // 2. deal with resource
    //void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare VE for another
    UtopiaModuleAddResourceStart(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0);
    // resource can alloc private for internal use, ex, VE_RESOURCE_PROVATE
    UtopiaResourceCreate("tvencoder0",sizeof(TVENCODER_RESOURCE_PRIVATE),&psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaTVEncoder, psResource, E_TVENCODER_POOL_ID_VE0);
    // end function to add res
    UtopiaModuleAddResourceEnd(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0);

    if(UtopiaResourceObtain(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0, &psResource)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not register
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;
    }

    UtopiaResourceRelease(psResource);

}

MS_U32 TVENCODEROpen(void** ppInstance,MS_U32 u32ModuleVersion, void* pAttribute)
{
    TVENCODER_INSTANT_PRIVATE *pVEpir = NULL;
    printf("[VE info] VE open\n");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d",__FUNCTION__,__LINE__);
    // instance is allocated here, also can allocate private for internal use, ex
    UtopiaInstanceCreate(sizeof(TVENCODER_INSTANT_PRIVATE),ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance,(void**)&pVEpir);
    //pVEpir->fpTVESetInputSRC = (IOCTL_TVENCODER_SETINPUTSRC)MDrv_VE_SetInputSource;

    return UTOPIA_STATUS_SUCCESS;



}

MS_U32 TVENCODERIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //UTOPIA_INSTANT* pInstant = (UTOPIA_INSTANT*)*pInstantTmp;
    //UTOPIA_RESOURCE* pstRes = NULL;

    //void* pModule = NULL;
    //UtopiaInstanceGetModule(pInstance, &pModule);
    //void* pResource = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
    MSG_TVE(printf("---This is in kernel---\n");)
    MSG_TVE(printf("\033[1;34m----- TVE cmd %d----\033[m\n",u32Cmd);)
#endif
    void* pstRes = NULL;
    TVENCODER_RESOURCE_PRIVATE * pstResPri = NULL;
    MS_U32 u32Ret =UTOPIA_STATUS_FAIL;
    ///TVENCODER_RESOURCE_PRIVATE *pstResPri = NULL;
    ///TVENCODER_INSTANT_PRIVATE *pstInstPri = NULL;

    // Assert

    TVENCODER_INSTANT_PRIVATE* psTVENCODERInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psTVENCODERInstPri);

    if(UtopiaResourceObtain(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0, &pstRes)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }


    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    // to check init first
    if((FALSE == pstResPri->bInited_Drv)  &&
        (u32Cmd != E_MDRV_CMD_TVE_Init))
    {
        return UTOPIA_STATUS_FAIL;
    }
    //TVENCODER_SetInputSRC pSetinputSRCParam = NULL;

    switch(u32Cmd)
    {
        case E_MDRV_CMD_TVE_GetInfo:
        {
            PTVE_GETINFO pGetInfo = (PTVE_GETINFO)pArgs;
            pGetInfo->eRet = E_TVENCODER_OK;
            pGetInfo->pVE_DrvInfo= _MDrv_VE_GetInfo();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_GetStatues:
        {
            PTVE_GETSTATUS pGetStatus = (PTVE_GETSTATUS)pArgs;
            if(E_VE_OK == _MDrv_VE_GetStatus(pGetStatus->pStaus))
            {
                pGetStatus->eRet = E_TVENCODER_OK;
            }
            else
            {
                pGetStatus->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetDbgLevel:
        {
            PTVE_SETDBGLEVEL pDbgLevel = (PTVE_SETDBGLEVEL)pArgs;
            _MDrv_VE_SetDbgLevel(pDbgLevel->u16Level);
            pDbgLevel->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SwitchInputSRC:
        {
            PTVE_SWITCH_SRC_INFO pSwitchSrc_info = (PTVE_SWITCH_SRC_INFO)pArgs;
            _MDrv_VE_SwitchInputSource(pSwitchSrc_info->pSwithcSrc_info);
            pSwitchSrc_info->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetInputSRC:
        {
            PTVE_SETINPUTSRC pSetInputSrc = (PTVE_SETINPUTSRC)pArgs;
            if(MS_VE_SWITCH_SRC_SUCCESS == _MDrv_VE_SetInputSource(pSetInputSrc->pInputSrcInfo))
            {
                pSetInputSrc->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetInputSrc->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SwitchOutputDest:
        {
            PTVE_SWITCHOUTPUTDEST pSwitchOutputDest = (PTVE_SWITCHOUTPUTDEST)pArgs;
            _MDrv_VE_SwitchOuputDest(pSwitchOutputDest->pSwitchInfo);
            pSwitchOutputDest->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOutputCtrl:
        {
            PTVE_SETOUTPUTCTRL pSetOutputCtrl = (PTVE_SETOUTPUTCTRL)pArgs;
            _MDrv_VE_SetOutputCtrl(pSetOutputCtrl->pOutputCtrl);
            pSetOutputCtrl->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOutputVideoStd:
        {
            PTVE_SETOUTPUTVIDEOSTD pSetOutputVideoStd = (PTVE_SETOUTPUTVIDEOSTD)pArgs;
            if(TRUE == _MDrv_VE_SetOutputVideoStd(pSetOutputVideoStd->VideoSystem))
            {
                pSetOutputVideoStd->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetOutputVideoStd->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Init:
        {
            if(pstResPri->bInited_Drv == FALSE)
            {
                PTVE_INIT pInit = (PTVE_INIT)pArgs;
                if(pInit->u32MIUAddress == NULL)
                {
                    while(!pstResPri->bInited_Drv); // wait init in other process
                    pInit->eRet = E_TVENCODER_OK;
                    u32Ret = UTOPIA_STATUS_SUCCESS;

                }
                else
                {
                    _MDrv_VE_Init(pInit->u32MIUAddress);
                    pInit->eRet = E_TVENCODER_OK;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                    pstResPri->bInited_Drv = TRUE;
                }
            }
            break;
        }

        case E_MDRV_CMD_TVE_Exit:
        {
            PTVE_EXIT pExit = (PTVE_EXIT)pArgs;
            if(TRUE == _MDrv_VE_Exit())
            {
                pExit->eRet = E_TVENCODER_OK;
            }
            else
            {
                pExit->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetWSSData:
        {
            PTVE_SETWSSDATA pSetWSSdata = (PTVE_SETWSSDATA) pArgs;
            _MDrv_VE_SetWSSData(pSetWSSdata->bEn,pSetWSSdata->u16WSSData);
            pSetWSSdata->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_GetWSSData:
        {
            PTVE_GETWSSDATA pGetWSSData = (PTVE_GETWSSDATA) pArgs;
            pGetWSSData->u16WSSData = _MDrv_VE_GetWSSData();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetMode:
        {
            PTVE_SETMODE pSetMode = (PTVE_SETMODE)pArgs;
            _MDrv_VE_SetMode(pSetMode-> pVESetMode);
            pSetMode->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetBlackSceen:
        {
            PTVE_SETBLACKSCREEN pSetBlackScreen = (PTVE_SETBLACKSCREEN)pArgs;
            _MDrv_VE_SetBlackScreen(pSetBlackScreen->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_IsBlackScreenEnabled:
        {
            PTVE_ISBLACKSCREENENABLE pBlackScreenEnable = (PTVE_ISBLACKSCREENENABLE)pArgs;
            pBlackScreenEnable->bEnable = _MDrv_VE_IsBlackScreenEnabled();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetTtxBuffer:
        {
            PTVE_SETTTXBUFFER pSetTtxBuffer = (PTVE_SETTTXBUFFER)pArgs;
            _MDrv_VE_SetTtxBuffer(pSetTtxBuffer->u32StartAddr,pSetTtxBuffer->u32Size);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_VE_GetTtxReadDoneStatus:
        {
            PTVE_GETTTXREADDONE pGetTtxReadDoneStatus = (PTVE_GETTTXREADDONE)pArgs;
            pGetTtxReadDoneStatus->bDone = _MDrv_VE_GetTtxReadDoneStatus();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnableCcSw:
        {
            PTVE_ENABLECCSW pEnableCcSw = (PTVE_ENABLECCSW)pArgs;
            _MDrv_VE_EnableCcSw(pEnableCcSw->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetCcRange:
        {
            PTVE_SETCCRANGE pSetCcRange = (PTVE_SETCCRANGE)pArgs;
            _MDrv_VE_SetCcRange(pSetCcRange->u16odd_start,pSetCcRange->u16odd_end,
                               pSetCcRange->u16even_start,pSetCcRange->u16even_end);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SendCcData:
        {
            PTVE_SENDCCDATA pSendCcData = (PTVE_SENDCCDATA) pArgs;
            _MDrv_VE_SendCcData(pSendCcData->bIsOdd,pSendCcData->u16data);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_set_display_window:
        {
            PTVE_SETDISPLAYWIN pSetDisplayWin = (PTVE_SETDISPLAYWIN)pArgs;
            _MDrv_VE_set_display_window(pSetDisplayWin->stDispWin);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOSD:
        {
            PTVE_SETOSD pSetOsd = (PTVE_SETOSD)pArgs;
            _MDrv_VE_SetOSD(pSetOsd->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetRGBIn:
        {
            PTVE_SETRGBIN pSetRGBIn = (PTVE_SETRGBIN)pArgs;
            _MDrv_VE_SetRGBIn(pSetRGBIn->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_Output_Video_Std:
        {
            PTVE_GETOUTPUTVIDEOSTD pOutputVideoSTD = (PTVE_GETOUTPUTVIDEOSTD)pArgs;
            pOutputVideoSTD->stVideoSys = _MDrv_VE_Get_Output_Video_Std();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetCaptureMode:
        {
            PTVE_SETCAPMODE pSetCaptureMode = (PTVE_SETCAPMODE)pArgs;
            _MDrv_VE_SetCaptureMode(pSetCaptureMode->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_GetDstInfo:
        {
            PTVE_GETDSTINFO pGetDstInfo = (PTVE_GETDSTINFO)pArgs;
            if(TRUE == _MApi_VE_GetDstInfo(pGetDstInfo->pDstInfo,pGetDstInfo->u32SizeofDstInfo))
            {
                pGetDstInfo->eRet = E_TVENCODER_OK;
            }
            else
            {
                pGetDstInfo->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_TestPattern:
        {
            PTVE_SETTESTPATTERN pSetTestPattern = (PTVE_SETTESTPATTERN)pArgs;
            _MDrv_VE_Set_TestPattern(pSetTestPattern->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_DisableRegWrite:
        {
            PTVE_DISABLEREGWRITE pDiableRegWrite = (PTVE_DISABLEREGWRITE)pArgs;
            if(E_VE_OK == _MDrv_VE_DisableRegWrite(pDiableRegWrite->bFlag))
            {
                pDiableRegWrite->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDiableRegWrite->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_AdjustPositionBase:
        {
            PTVE_ADJPOSITIONBASE pAdjPositionBase = (PTVE_ADJPOSITIONBASE)pArgs;
            if(E_VE_OK == _MDrv_VE_AdjustPositionBase(pAdjPositionBase->s32WAddrAdjustment,
                                                     pAdjPositionBase->s32RAddrAdjustment))
            {
                pAdjPositionBase->eRet = E_TVENCODER_OK;
            }
            else
            {
                pAdjPositionBase->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_Customer_Scaling:
        {
            PTVE_SETCUSSCALING pSetCusScaling = (PTVE_SETCUSSCALING)pArgs;
            if(E_VE_OK == _MDrv_VE_Set_Customer_Scaling(pSetCusScaling->pstVECusScalingInfo))
            {
                pSetCusScaling->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetCusScaling->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_InitVECapture:
        {
            P_TVE_INITCAPTURE pInitCapture = (P_TVE_INITCAPTURE)pArgs;
            _MDrv_VE_InitVECapture(pInitCapture->pVECapture);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnaVECapture:
        {
            PTVE_ENACAPTURE pEnableCapture =(PTVE_ENACAPTURE)pArgs;
            if(TRUE == _MDrv_VE_EnaVECapture(pEnableCapture->pstVECapState))
            {
                pEnableCapture->eRet = E_TVENCODER_OK;
            }
            else
            {
                pEnableCapture->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Adjust_FrameStart:
        {
            PTVE_ADJFRAMESTART pAdjustFrameStart = (PTVE_ADJFRAMESTART)pArgs;
            if(E_VE_OK == _MDrv_VE_Adjust_FrameStart(pAdjustFrameStart->stSrcWin,
                pAdjustFrameStart->pixel_offset))
            {
                pAdjustFrameStart->eRet = E_TVENCODER_OK;
            }
            else
            {
                pAdjustFrameStart->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetPowerState:
        {
            PTVE_SETPOWERSTATE pSetPowerState = (PTVE_SETPOWERSTATE)pArgs;

            if(TRUE == _MDrv_TVE_SetPowerState(pSetPowerState->ePowerState))
            {
                pSetPowerState->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetPowerState->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_CusTable:
        {
            PTVE_SETCUSTABLE pSetCusTable = (PTVE_SETCUSTABLE)pArgs;

            if(TRUE == _MDrv_VE_SetCusTable(pSetCusTable->VideoSystem,pSetCusTable->pTbl))
            {
                pSetCusTable->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetCusTable->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnableCusTable:
        {
            PTVE_ENABLECUSTABLE pSetEnableTable = (PTVE_ENABLECUSTABLE)pArgs;
            _MDrv_VE_EnableCusTable(pSetEnableTable->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_FrameColor:
        {
            PTVE_SETFRAMECOLOR pSetFrameColor = (PTVE_SETFRAMECOLOR)pArgs;
            pSetFrameColor->eRet = _MDrv_VE_SetFrameColor(pSetFrameColor->u32aRGB);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_OSDLayer:
        {
            PTVE_SETOSDLAYER pSetOSDLayer = (PTVE_SETOSDLAYER)pArgs;
            pSetOSDLayer->eRet = _MDrv_VE_Set_OSDLayer(pSetOSDLayer->eVideoOSDLayer);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_OSDLayer:
        {
            PTVE_GETOSDLAYER pGetOSDLayer = (PTVE_GETOSDLAYER)pArgs;
            pGetOSDLayer->eRet = _MDrv_VE_Get_OSDLayer();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_VideoAlpha:
        {
            PTVE_SETVIDEOALPHA pSetVideoAlpha = (PTVE_SETVIDEOALPHA)pArgs;
            pSetVideoAlpha->eRet = _MDrv_VE_Set_VideoAlpha(pSetVideoAlpha->u8Val);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_VideoAlpha:
        {
            PTVE_GETVIDEOALPHA pGetVideoAlpha = (PTVE_GETVIDEOALPHA)pArgs;
            pGetVideoAlpha->eRet = _MDrv_VE_Get_VideoAlpha(pGetVideoAlpha->pu8Val);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_DCS_SetType:
        {
            PTVE_DCSSETTYPE pDCSSetType= (PTVE_DCSSETTYPE)pArgs;

            if(TRUE == _MDrv_VE_DCS_SetType(pDCSSetType->bEnable,
                                            pDCSSetType->eType))
            {
                pDCSSetType->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDCSSetType->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_MDRV_CMD_TVE_DCS_SetActivationKey:
        {
            PTVE_DCSSETACTIVATIONKEY pDCSSetActivationKey= (PTVE_DCSSETACTIVATIONKEY)pArgs;

            if(TRUE == _MDrv_VE_DCS_SetActivationKey(pDCSSetActivationKey->pu8ActivationKeyTbl,
                                            pDCSSetActivationKey->u8ActivationKeyTblSize))
            {
                pDCSSetActivationKey->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDCSSetActivationKey->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

    }
    UtopiaResourceRelease(pstRes);
    return u32Ret; // FIXME: error code
}

MS_U32 TVENCODERClose(void* pInstance)
{
    //UTOPIA_INSTANT* pInstant = *pInstantTmp;
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    UtopiaInstanceDelete(pInstance);
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));
    //utopia_free(pInstant);
    return UTOPIA_STATUS_SUCCESS;
}
