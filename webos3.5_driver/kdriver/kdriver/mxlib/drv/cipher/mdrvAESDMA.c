#include "MsTypes.h"
#include <stdio.h>
#include <string.h>
#include "utopia_dapi.h"
#include "utopia.h"
//#include "drvAESDMA.h"
#include "drvCIPHER.h"
#include "drvAESDMA_private.h"
#include "MsOS.h"

enum
{
    AESDMA_POOL_ID_AES = 0,
    AESDMA_POOL_ID_SHA,
    AESDMA_POOL_ID_RSA,
} eAesdmaPoolID;

MS_U32 _AESDMA_MSB2LSB(MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_AESDMA, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)AESDMAOpen, (FUtopiaClose)AESDMAClose, (FUtopiaIOctl)AESDMAIoctl);

    // 2. deal with resource
    void* psResource = NULL;
	UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_AES);
    UtopiaResourceCreate("AES", sizeof(AESDMA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_AES);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_AES);

    UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_SHA);
    psResource = NULL;
    UtopiaResourceCreate("SHA", sizeof(SHA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_SHA);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_SHA);

    UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_RSA);
    psResource = NULL;
    UtopiaResourceCreate("RSA", sizeof(RSA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_RSA);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_RSA);

}

MS_U32 AESDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	MS_DEBUG_MSG(printf("\n[AESDMA INFO]AESDMA open \n"));

    AESDMA_INSTANT_PRIVATE *pAesdmaPri = NULL;
    void *pAesdmaPriVoid = NULL;
    UtopiaInstanceCreate(sizeof(AESDMA_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pAesdmaPriVoid);

    pAesdmaPri = (AESDMA_INSTANT_PRIVATE *)pAesdmaPriVoid;

    pAesdmaPri->fpAESDMAInit = _MDrv_AESDMA_Init;
    pAesdmaPri->fpAESDMASetIV = _MDrv_AESDMA_SetIV;
    pAesdmaPri->fpAESDMAReset = _MDrv_AESDMA_Reset;
    pAesdmaPri->fpAESDMASetFileInOut = _MDrv_AESDMA_SetFileInOut;
    pAesdmaPri->fpAESDMASetKey = _MDrv_AESDMA_SetKey;
    pAesdmaPri->fpAESDMASelEng = _MDrv_AESDMA_SelEng;
    pAesdmaPri->fpAESDMAStart = _MDrv_AESDMA_Start;
    pAesdmaPri->fpAESDMAGetStatus = _MDrv_AESDMA_GetStatus;
    pAesdmaPri->fpAESDMAIsFinished = _MDrv_AESDMA_IsFinished;
    pAesdmaPri->fpAESDMASetClk = _MDrv_AESDMA_Set_Clk;
    pAesdmaPri->fpAESDMASetSecureKey = _MDrv_AESDMA_SetSecureKey;
    pAesdmaPri->fpAESDMASetPS = _MDrv_AESDMA_SetPS;
    pAesdmaPri->fpAESDMAPSRelease = _MDrv_AESDMA_PSRelease;
    pAesdmaPri->fpAESDMAGetPSMatchedByteCNT = _MDrv_AESDMA_GetPSMatchedByteCNT;
    pAesdmaPri->fpAESDMAGetPSMatchedPTN = _MDrv_AESDMA_GetPSMatchedPTN;
    pAesdmaPri->fpAESDMANotify = _MDrv_AESDMA_Notify;
    pAesdmaPri->fpAESDMARand = _MDrv_AESDMA_Rand;
    pAesdmaPri->fpRSACalculate = _MDrv_RSA_Calculate;
    pAesdmaPri->fpRSAIsFinished = _MDrv_RSA_IsFinished;
    pAesdmaPri->fpRSAOutput = _MDrv_RSA_Output;
    pAesdmaPri->fpSHACalculate = _MDrv_SHA_Calculate;
    pAesdmaPri->fpSHACalculateManual = _MDrv_SHA_CalculateManual;
    pAesdmaPri->fpAESDMASetKey_Ex = _MDrv_AESDMA_SetKey_Ex;
    pAesdmaPri->fpAESDMASetIV_Ex = _MDrv_AESDMA_SetIV_Ex;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AESDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    PAESDMA_INIT pInitParam = NULL;
    PAESDMA_FILE_INOUT pFileInOutParam = NULL;
    PAESDMA_SEL_ENG pSelEngParam = NULL;
    PAESDMA_SET_IV pSetIV = NULL;
    PAESDMA_SET_KEY pSetKey = NULL;
    PAESDMA_SET_KEY_EX pSetKeyEx = NULL;
    PAESDMA_SET_IV_EX pSetIVEx = NULL;
    PAESDMA_SET_PS pSetPSParam = NULL;
    PAESDMA_NOTIFY pNotifyParam = NULL;
    PAESDMA_RAND pRandParam = NULL;
    PRSA_CALCULATE pRSACalParam = NULL;
    PRSA_OUTPUT pRSAOutParam = NULL;
    PSHA_CALCULATE pSHACalParam = NULL;
    PSHA_CALCULATE_MANUAL pSHACalManuParam = NULL;
    MS_U32 u32Ret = 0;
    MS_BOOL bEnable = 0;
	AESDMA_INSTANT_PRIVATE* psAesInstPri = NULL;
    AESDMA_RESOURCE_SHARED* psAesResourceShared = NULL;
    RSA_RESOURCE_SHARED* psRSAResourceShared = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psAesInstPri);
	void* pResource = NULL;
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;
    MS_U32 u32IVarrayAddr[4] = {0};
    MS_U32 u32KeyarryAddr[4] = {0};

    switch(u32Cmd)
    {
        case MDrv_CMD_AESDMA_Init:
            {
                pInitParam = (PAESDMA_INIT)pArgs;
                u32Ret = (psAesInstPri->fpAESDMAInit)(pInitParam->u32miu0addr,pInitParam->u32miu1addr,pInitParam->u32miunum);
                if (u32Ret == DRVAESDMA_OK)
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
        case MDrv_CMD_AESDMA_SetIV:
            {
                pSetIV = (PAESDMA_SET_IV)pArgs;

                u32IVarrayAddr[3] = _AESDMA_MSB2LSB((MS_U8 *)pSetIV->pInitVector);
                u32IVarrayAddr[2] = _AESDMA_MSB2LSB((MS_U8 *)pSetIV->pInitVector + 4);
                u32IVarrayAddr[1] = _AESDMA_MSB2LSB((MS_U8 *)pSetIV->pInitVector + 8);
                u32IVarrayAddr[0] = _AESDMA_MSB2LSB((MS_U8 *)pSetIV->pInitVector + 12);

                memcpy((void *)psAesInstPri->InitVector,(MS_U8 *)u32IVarrayAddr,16);
                psAesInstPri->u32IVLen = 16;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_Reset:
            {
                u32Ret = (psAesInstPri->fpAESDMAReset)();
                if (u32Ret == DRVAESDMA_OK)
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
        case MDrv_CMD_AESDMA_SetFileInOut:
            {
                pFileInOutParam = (PAESDMA_FILE_INOUT)pArgs;
                psAesInstPri->u32FileinAddr = pFileInOutParam->u32FileinAddr;
                psAesInstPri->u32FileInNum = pFileInOutParam->u32FileInNum;
                psAesInstPri->u32FileOutSAddr = pFileInOutParam->u32FileOutSAddr;
                psAesInstPri->u32FileOutEAddr = pFileInOutParam->u32FileOutEAddr;
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileinAddr = %lx\n",psAesInstPri->u32FileinAddr));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileInNum = %lx\n",psAesInstPri->u32FileInNum));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileOutSAddr = %lx\n",psAesInstPri->u32FileOutSAddr));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileOutEAddr = %lx\n",psAesInstPri->u32FileOutEAddr));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetKey:
            {
                pSetKey = (PAESDMA_SET_KEY)pArgs;
                if(pSetKey->pCipherKey == NULL)
                {
                    psAesInstPri->ptrKey = 0;
                }
                else
                {
                    u32KeyarryAddr[3] = _AESDMA_MSB2LSB((MS_U8 *)pSetKey->pCipherKey);
                    u32KeyarryAddr[2] = _AESDMA_MSB2LSB((MS_U8 *)pSetKey->pCipherKey + 4);
                    u32KeyarryAddr[1] = _AESDMA_MSB2LSB((MS_U8 *)pSetKey->pCipherKey + 8);
                    u32KeyarryAddr[0] = _AESDMA_MSB2LSB((MS_U8 *)pSetKey->pCipherKey + 12);
                    memcpy((void *)psAesInstPri->CipherKey,(MS_U8 *)u32KeyarryAddr,16);
                    psAesInstPri->ptrKey = (MS_U32)psAesInstPri->CipherKey;
                }

                psAesInstPri->u32KeyLen = 16;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SelEng:
            {
                pSelEngParam = (PAESDMA_SEL_ENG)pArgs;
                psAesInstPri->eMode = pSelEngParam->eMode;
                psAesInstPri->bDescrypt = pSelEngParam->bDescrypt;
                MS_DEBUG_MSG(printf("[AESDMA INFO]mode = %x\n",psAesInstPri->eMode));
                MS_DEBUG_MSG(printf("[AESDMA INFO]Descrypt = %x\n",psAesInstPri->bDescrypt));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_Start:
            {
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_AES, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        printf("[AESDMA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(AESDMA_RESOURCE_SHARED));
                }

                psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;

                psAesResourceShared->pAESResource = pResource;
                MS_DEBUG_MSG(printf("\n[AESDMA INFO]pResource = %x\n",(int)psAesResourceShared->pAESResource));

                //AESDMA reset
//                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set mode and select encrypt or decrypt
                (psAesInstPri->fpAESDMASelEng)(psAesInstPri->eMode,psAesInstPri->bDescrypt);

                //AESDMA set IV
                if((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    //u32IVarrayAddr = (MS_U32)&psAesInstPri->InitVector[0];
                    //(psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);
                    (psAesInstPri->fpAESDMASetIV_Ex)(&psAesInstPri->InitVector[0], psAesInstPri->u32IVLen);
                }

                //CBC segmental decryption case
                //if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                //    && psAesInstPri->bDescrypt == TRUE)
                if((((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))&& psAesInstPri->bDescrypt == TRUE)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR))
                {
                    //printf("[AESDMA]FileInAddr PA = %x\n",psAesInstPri->u32FileinAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 FileinVirtAddr, i;

                    FileinVirtAddr = MsOS_MPool_PA2KSEG1(psAesInstPri->u32FileinAddr);

                    if(psAesInstPri->eMode != E_DRVAESDMA_CIPHER_CTR)
                    {
                        for(i = 0; i < 16; i++)
                        {
                            psAesInstPri->InitVector[i] = *(MS_U8 *)(FileinVirtAddr + psAesInstPri->u32FileInNum-1-i);
                            MS_DEBUG_MSG(printf("[AESDMA INFO]Init Vector[%d] = 0x%x\n",(int)i, psAesInstPri->InitVector[i]));
                        }
                    }
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bSecretKey==1)
                {
                    (psAesInstPri->fpAESDMASetSecureKey)();
                }
                else
                {
                    //u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherKey[0];
                    //(psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                    (psAesInstPri->fpAESDMASetKey_Ex)((MS_U8*)psAesInstPri->ptrKey, psAesInstPri->u32KeyLen);
                }

                //AESDMA trigger start
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_Start\n"));
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_GetStatus:
            {
                u32Ret = (psAesInstPri->fpAESDMAGetStatus)();
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_IsFinished:
            {
                if (FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    printf("[AESDMA INFO]query Resouce shared memory fail\n");
                    return UTOPIA_STATUS_ERR_NOMEM;
                }
                else
                {
                    psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;
                    pResource = psAesResourceShared->pAESResource;
                    MS_DEBUG_MSG(printf("\n[AESDMA INFO]pResource = %x\n",(int)psAesResourceShared->pAESResource));
                }

                while ((psAesInstPri->fpAESDMAIsFinished)() != DRVAESDMA_OK);

                //CBC segmental encryption case
                if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                    && psAesInstPri->bDescrypt == FALSE)
                {
                    //printf("[AESDMA]FileOutStartAddr PA = %x\n",psAesInstPri->u32FileOutSAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 FileOutStartVirtAddr, i;

                    FileOutStartVirtAddr = MsOS_MPool_PA2KSEG1(psAesInstPri->u32FileOutSAddr);

                    for(i = 0; i < 16; i++)
                    {
                        psAesInstPri->InitVector[i] = *(MS_U8 *)(FileOutStartVirtAddr+psAesInstPri->u32FileInNum-1-i);
                        MS_DEBUG_MSG(printf("[AESDMA INFO]Init Vector[%d] = 0x%x\n",(int)i, psAesInstPri->InitVector[i]));
                    }
                }

                //clear secret key setting
                psAesInstPri->bSecretKey = 0;

                UtopiaResourceRelease(pResource);
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_AESDMA_IsFinished\n"));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetClk:
            {
                bEnable = *(MS_BOOL *)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetClk)(bEnable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetSecureKey:
            {
                psAesInstPri->bSecretKey = 1;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetPS:
            {
                pSetPSParam = (PAESDMA_SET_PS)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetPS)(pSetPSParam->u32PTN,pSetPSParam->u32Mask,pSetPSParam->bPSin_Enable,pSetPSParam->bPSout_Enable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_PSRelease:
            {
                u32Ret = (psAesInstPri->fpAESDMAPSRelease)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedByteCNT:
            {
                u32Ret = (psAesInstPri->fpAESDMAGetPSMatchedByteCNT)();
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedPTN:
            {
                u32Ret = (psAesInstPri->fpAESDMAGetPSMatchedPTN)();
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_Notify:
            {
                pNotifyParam = (PAESDMA_NOTIFY)pArgs;
                u32Ret = (psAesInstPri->fpAESDMANotify)(pNotifyParam->eEvents,pNotifyParam->pfCallback);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_Rand:
            {
                pRandParam = (PAESDMA_RAND)pArgs;
                u32Ret = (psAesInstPri->fpAESDMARand)(pRandParam->u32PABuf,pRandParam->u32Size);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_Calculate:
            {
                pRSACalParam = (PRSA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_RSA, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        printf("[RSA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(RSA_RESOURCE_SHARED));
                }

                psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;

                psRSAResourceShared->pRSAResource = pResource;
                MS_DEBUG_MSG(printf("\n[RSA INFO]pResource = %x\n",(int)psRSAResourceShared->pRSAResource));

                u32Ret = (psAesInstPri->fpRSACalculate)(pRSACalParam->pstSign,pRSACalParam->pstKey,pRSACalParam->eMode);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_IsFinished:
            {
                u32Ret = (psAesInstPri->fpRSAIsFinished)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_Output:
            {
                if (FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    printf("[RSA INFO]query Resouce shared memory fail\n");
                    return UTOPIA_STATUS_ERR_NOMEM;
                }
                else
                {
                    psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
                    pResource = psRSAResourceShared->pRSAResource;
                    MS_DEBUG_MSG(printf("\n[RSA INFO]pResource = %x\n",(int)psRSAResourceShared->pRSAResource));
                }

                pRSAOutParam = (PRSA_OUTPUT)pArgs;
                while((psAesInstPri->fpRSAIsFinished)()==1);
                u32Ret = (psAesInstPri->fpRSAOutput)(pRSAOutParam->eMode,pRSAOutParam->pstRSAOut);

                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_Calculate:
            {
                pSHACalParam = (PSHA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_SHA, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculate)(pSHACalParam->eMode,pSHACalParam->u32PAInBuf,pSHACalParam->u32Size,pSHACalParam->u32PAOutBuf);
                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_Memcpy:
            {
                //AESDMA trigger start for using AESDMA replace BDMA case
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_CalculateManual:
            {
                pSHACalManuParam = (PSHA_CALCULATE_MANUAL)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_SHA, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculateManual)(pSHACalManuParam->stCfg,pSHACalManuParam->eStage,pSHACalManuParam->u32DoneBytes,pSHACalManuParam->pu8SetIV);
                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetKey_Ex:
            {
                pSetKeyEx = (PAESDMA_SET_KEY_EX)pArgs;
                if(pSetKeyEx->pu8Key == NULL)
                {
                    psAesInstPri->ptrKey = 0;
                }
                else
                {
                    memcpy((void *)psAesInstPri->CipherKey, pSetKeyEx->pu8Key, pSetKeyEx->u32Len);
                    psAesInstPri->ptrKey = (MS_U32)psAesInstPri->CipherKey;
                }

                psAesInstPri->u32KeyLen = pSetKeyEx->u32Len;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetIV_Ex:
            {
                pSetIVEx = (PAESDMA_SET_IV_EX)pArgs;
                memcpy((void *)psAesInstPri->InitVector, pSetIVEx->pu8IV, pSetIVEx->u32Len);
                psAesInstPri->u32IVLen = pSetIVEx->u32Len;
                return UTOPIA_STATUS_SUCCESS;
            }
        default:
            break;
    };

	return 0;
}

MS_U32 AESDMAClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}

