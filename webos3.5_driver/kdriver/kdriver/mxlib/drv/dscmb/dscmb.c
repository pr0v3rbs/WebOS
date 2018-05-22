#include "MsTypes.h"
#include "utopia.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "drvDSCMB_v2.h"
#include "dscmb_private.h"

#include "utopia_dapi.h"
#include "drvDSCMB.h"
#include "MsOS.h"
// MACRO definition
#define DBG_EN  0

#define DSCMB_ASSERT_RETURN(x, y) if(!(x)) {printf("Assert Fail : %s, %d\n", __FUNCTION__, __LINE__); return y;}

#if DBG_EN
    #define DSCMB_PRINT(x) printf(x);

#else
    #define DSCMB_PRINT(x)

#endif
//
// ========== Global definition ==========
void* pModuleDscmb = NULL;

#if DBG_EN
static const char *aCmdStr[E_MDRV_CMD_DSCMB_MAX] = { "E_MDRV_CMD_DSCMB_Init\n",
                                                     "E_MDRV_CMD_DSCMB_Exit\n",
                                                     "E_MDRV_CMD_DSCMB_SetPowerState\n",
                                                     "E_MDRV_CMD_DSCMB_FltAlloc\n",
                                                     "E_MDRV_CMD_DSCMB_FltFree\n",
                                                     "E_MDRV_CMD_DSCMB_FltConnectFltId\n",
                                                     "E_MDRV_CMD_DSCMB_FltDisconnectFltId\n",
                                                     "E_MDRV_CMD_DSCMB_FltDisconnectPid\n",
                                                     "E_MDRV_CMD_DSCMB_FltDscmb\n",
                                                     "E_MDRV_CMD_DSCMB_FltTypeSet\n",
                                                     "E_MDRV_CMD_DSCMB_FltKeySet\n",
                                                     "E_MDRV_CMD_DSCMB_FltKeyReset\n",
                                                     "E_MDRV_CMD_DSCMB_FltIVSet\n",
                                                     "E_MDRV_CMD_DSCMB_HDCP2_SetRiv\n",
                                                     "E_MDRV_CMD_DSCMB_Multi2_SetRound\n",
                                                     "E_MDRV_CMD_DSCMB_Multi2_SetSysKey\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_PVR_RecCtrl\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_SetDefaultCAVid\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngSetAlgo\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngSetKey\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngResetKey\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngSetIV\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngSetSwitch\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_EngSetFSCB\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_KLadder_AtomicExec\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_KLadder_ETSI\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_QueryCap\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_SetDBGLevel\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_GetLibVer\n",
                                                     "E_MDRV_CMD_DSCMB_PidFlt_GetConnectStatus\n"};

#endif


// variable export to driver layer
MS_U32* _u32FreeSlot;
stDscmbTspMap (*DscmbTspMap)[MAX_NUM];

void DSCMBRegisterToUtopia(void)
{
    DSCMB_RESOURCE_PRIVATE * pResPri = NULL;

    // 1. deal with module
    UtopiaModuleCreate(MODULE_DSCMB, 8, &pModuleDscmb);
    UtopiaModuleRegister(pModuleDscmb);
    UtopiaModuleSetupFunctionPtr(pModuleDscmb, (FUtopiaOpen)DSCMBOpen, (FUtopiaClose)DSCMBClose, (FUtopiaIOctl)DSCMBIoctl);

    // 2. deal with resource
    void* psResource = NULL;

    UtopiaResourceCreate("DSCMB", sizeof(DSCMB_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleDscmb, psResource, E_DSCMB_RESOURCE);

    UtopiaModuleAddResourceEnd(pModuleDscmb, E_DSCMB_RESOURCE);

    if(UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &psResource) != 0)
    {
        printf("Dscmb - UtopiaResourceObtain fail\n");
        return;
    }

    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not regiser yet
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;
    }

    UtopiaResourceRelease(psResource);

/*
	UtopiaStartResourceAdd(pUtopiaModule, E_DSCMB_POOL_ID_ENG0);
    psResource = (UTOPIA_RESOURCE*)UtopiaResourceCreate("DSCMB_ENG0", sizeof(DSCMB_RESOURCE_PRIVATE));
    UtopiaResourceRegister(pUtopiaModule, psResource, E_DSCMB_POOL_ID_ENG0);

    psResource = NULL;
	UtopiaStartResourceAdd(pUtopiaModule, E_DSCMB_POOL_ID_ENG1);
    psResource = (UTOPIA_RESOURCE*)UtopiaResourceCreate("DSCMB_ENG1", sizeof(DSCMB_RESOURCE_PRIVATE));
    UtopiaResourceRegister(pUtopiaModule, psResource, E_DSCMB_POOL_ID_ENG1);

    psResource = NULL;
	UtopiaStartResourceAdd(pUtopiaModule, E_DSCMB_POOL_ID_ENG2);
    psResource = (UTOPIA_RESOURCE*)UtopiaResourceCreate("DSCMB_ENG2", sizeof(DSCMB_RESOURCE_PRIVATE));
    UtopiaResourceRegister(pUtopiaModule, psResource, E_DSCMB_POOL_ID_ENG2);
*/
}

MS_U32 DSCMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
	UtopiaInstanceCreate(sizeof(DSCMB_INSTANT_PRIVATE), ppInstance);

    DSCMB_INSTANT_PRIVATE* psDscmbInstPri = NULL;
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psDscmbInstPri);
    memset(&psDscmbInstPri->pInitVector, 0x0, DSCMB_IV_LENGTH);
    memset(&psDscmbInstPri->pCipherKey, 0x0, DSCMB_KEY_LENGTH);

    //MDrv_DSCMB_Init();
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DSCMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pu32Args)
{
	void* pstRes = NULL;
	MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
	DSCMB_RESOURCE_PRIVATE * pstResPri = NULL;
	DSCMB_INSTANT_PRIVATE *pstInstPri = NULL;
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));

    DSCMB_ASSERT_RETURN((pInstance != NULL), UTOPIA_STATUS_PARAMETER_ERROR);

    DSCMB_ASSERT_RETURN((u32Cmd < E_MDRV_CMD_DSCMB_MAX), UTOPIA_STATUS_NOT_SUPPORTED);

    DSCMB_PRINT(aCmdStr[u32Cmd]);

    //DSCMB_INSTANT_PRIVATE* psDscmbInstPri = (DSCMB_INSTANT_PRIVATE*)UtopiaInstantGetPrivate(pInstant);

    //UtopiaResourceObtainToInstant(pInstant, E_DSCMB_RESOURCE);
    if(UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &pstRes) != 0)
    {
        printf("Dscmb - UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    if((FALSE == pstResPri->bInited_Drv)  &&
        (u32Cmd != E_MDRV_CMD_DSCMB_Init) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_QueryCap))
    {
        UtopiaResourceRelease(pstRes);
        return UTOPIA_STATUS_FAIL;
    }

    _u32FreeSlot = pstResPri->u32FreeSlot;
    DscmbTspMap = pstResPri->DscmbTspMap;

    switch(u32Cmd)
    {
        case E_MDRV_CMD_DSCMB_Init:
            if(pstResPri->bInited_Drv == FALSE)
            {
                if(TRUE == _MDrv_DSCMB_Init())
                {
                    pstResPri->bInited_Drv = TRUE;
                }
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case E_MDRV_CMD_DSCMB_Exit:
            if(pstResPri->bInited_Drv == TRUE)
            {
                if(TRUE == _MDrv_DSCMB_Exit())
                {
                    pstResPri->bInited_Drv = FALSE;
                }
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
            
        case E_MDRV_CMD_DSCMB_QueryCap:
        {
            DSCMB_QUERYCAP * psQuery = (DSCMB_QUERYCAP *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetCap(
                        psQuery->u32EngId,
                        psQuery->eQueryType,
                        psQuery->pInput,
                        psQuery->pOutput))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            break;
        }
        
        case E_MDRV_CMD_DSCMB_OptConfig:
        {
            DSCMB_OPTCONFIG * psOpt = (DSCMB_OPTCONFIG *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_OptConfig(
                        psOpt->u32EngId,
                        psOpt->eOpt,
                        psOpt->pInput,
                        psOpt->pOutput,
                        psOpt->bSet))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            
            break;
        }
                
        case E_MDRV_CMD_DSCMB_FltAlloc:
            {
                //MS_U32 u32Flt = 0;
                DSCMB_FLTALLOC* pFltAlloc = (DSCMB_FLTALLOC *)pu32Args;
                //u32EngId = (MS_U32)psDscmbInstPri->ePoolId;

                pFltAlloc->u32DscmbId = _MDrv_DSCMB2_FltAlloc_Ex(pFltAlloc->u32EngId,
                                                                 pFltAlloc->eFltType);
                if(pFltAlloc->u32DscmbId != DRV_DSCMB_FLT_NULL)
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltConnectFltId:
            {
                DSCMB_CONNECTFLT * pConnectFlt = (DSCMB_CONNECTFLT *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltConnectFltId(pConnectFlt->u32EngId,
                                                        pConnectFlt->u32DscmbId,
                                                        pConnectFlt->u32DmxFltId))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;
        case E_MDRV_CMD_DSCMB_FltDisconnectFltId:
            {
                DSCMB_DISCONNECTFLT * pDisconnectFlt = (DSCMB_DISCONNECTFLT *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltDisconnectFltId(pDisconnectFlt->u32EngId,
                                                           pDisconnectFlt->u32DscmbId,
                                                           pDisconnectFlt->u32DmxFltId))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltConnectPid:
            {
                DSCMB_CONNECTPID * pstConnectPid = (DSCMB_CONNECTPID *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltConnectPid(
                            pstConnectPid->u32EngId,
                            pstConnectPid->u32DscmbId,
                            pstConnectPid->u32Pid))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltDisconnectPid:
            {
                DSCMB_DISCONNECTPID * pstDisconnectPid = (DSCMB_DISCONNECTPID *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltDisconnectPid(pstDisconnectPid->u32EngId,
                                                         pstDisconnectPid->u32DscmbId,
                                                         pstDisconnectPid->u32Pid))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltDscmb:
            {
                DSCMB_FLTDSCMB * pstFltDscmb = (DSCMB_FLTDSCMB *)pu32Args;

                if(TRUE == _MDrv_DSCMB2_FltDscmb(pstFltDscmb->u32EngId,
                                                 pstFltDscmb->u32DscmbId,
                                                 pstFltDscmb->bDscmb))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltFree:
            {
                //u32EngId = (MS_U32)psDscmbInstPri->ePoolId;
                DSCMB_FLTFREE * pFltFree = (DSCMB_FLTFREE *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltFree(pFltFree->u32EngId,
                                                 pFltFree->u32DscmbId))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltIVSet:
            {
                UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);
                DSCMB_FLTIVSET * pstFltIVSet = (DSCMB_FLTIVSET *)pu32Args;

                if(TRUE == _MDrv_DSCMB2_FltIVSet(pstFltIVSet->u32EngId,
                                                 pstFltIVSet->u32DscmbId,
                                                 pstFltIVSet->eKeyType,
                                                 pstFltIVSet->pu8IV))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                ///@@ FIXME : IV belong to each DSCMB id, not to one engine
                memcpy(pstInstPri->pInitVector, pstFltIVSet->pu8IV, DSCMB_IV_LENGTH);
                pstInstPri->eKeyType = pstFltIVSet->eKeyType;
            }
            break;

        case E_MDRV_CMD_DSCMB_FltKeyReset:
            {
                DSCMB_FLTKEYRESET * pstFltKeyReset = (DSCMB_FLTKEYRESET *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltKeyReset(pstFltKeyReset->u32EngId,
                                                    pstFltKeyReset->u32DscmbId,
                                                    pstFltKeyReset->eKeyType))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltKeySet:
            {
                UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);
                DSCMB_FLTKEYSET * pstFltKeySet = (DSCMB_FLTKEYSET *)pu32Args;

                if(TRUE == _MDrv_DSCMB2_FltKeySet(pstFltKeySet->u32EngId,
                                                  pstFltKeySet->u32DscmbId,
                                                  pstFltKeySet->eKeyType,
                                                  pstFltKeySet->pu8Key))
                {
                    ///@@ FIXME : Key belong to each DSCMB id, not to one engine
                    memcpy(pstInstPri->pCipherKey, pstFltKeySet->pu8Key, DSCMB_KEY_LENGTH);
                    pstInstPri->eKeyType = pstFltKeySet->eKeyType;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_FltTypeSet:
            {
                UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);
                DSCMB_TYPESET *pstTypeSet = (DSCMB_TYPESET *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_FltTypeSet(pstTypeSet->u32EngId,
                                                   pstTypeSet->u32DscmbId,
                                                   pstTypeSet->eType))
                {
                    pstInstPri->eType = pstTypeSet->eType;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv:
            {
                DSCMB_HDCP2_SETRIV * pstSetRiv = (DSCMB_HDCP2_SETRIV *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_HDCP2_SetRIV(pstSetRiv->u32EngId,
                                                     pstSetRiv->pu8Riv))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_Multi2_SetRound:
            {
                DSCMB_MULTI2_SETROUND * pstSetRnd = (DSCMB_MULTI2_SETROUND *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_Multi2_SetRound(pstSetRnd->u32EngId,
                                                        pstSetRnd->u32Round))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_Multi2_SetSysKey:
            {
                DSCMB_MULTI2_SETSYSKEY * psSetSysKey = (DSCMB_MULTI2_SETSYSKEY *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_Multi2_SetSystemKey(psSetSysKey->u32EngId,
                                                            (MS_U8*)psSetSysKey->pu8SysKey))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus:
            {
                DSCMB_DSCMBSTATUS * pDscmbStatus = (DSCMB_DSCMBSTATUS *)pu32Args;
                if(TRUE == _MDrv_DSCMB2_PidFlt_ScmbStatus(pDscmbStatus->u32EngId,
                                                          pDscmbStatus->u32DmxFltId,
                                                          pDscmbStatus->pScmbLevel))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        default:
            printf("Unsupport dscmb command 0x%x\n", (int)u32Cmd);
            break;
    };

    UtopiaResourceRelease(pstRes);
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));
	return u32Ret;
}

MS_U32 DSCMBClose(void* pInstance)
{
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));
	UtopiaInstanceDelete(pInstance);
	return UTOPIA_STATUS_SUCCESS;
}



