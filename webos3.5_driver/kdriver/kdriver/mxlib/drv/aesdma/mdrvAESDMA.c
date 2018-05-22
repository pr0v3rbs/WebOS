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
#include "drvAESDMA.h"
#include "drvAESDMA_private.h"
#include "MsOS.h"

enum
{
    AESDMA_POOL_ID_AES = 0,
    AESDMA_POOL_ID_SHA,
    AESDMA_POOL_ID_RSA,
} eAesdmaPoolID;


//#ifdef MS_DEBUG_MSG
//#undef MS_DEBUG_MSG
//#define MS_DEBUG_MSG(x) x
//#endif



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
    pAesdmaPri->fpAESDMASetSecureKey = MDrv_AESDMA_SetSecureKey;
    pAesdmaPri->fpAESDMASetPS = _MDrv_AESDMA_SetPS;
    pAesdmaPri->fpAESDMAPSRelease = _MDrv_AESDMA_PSRelease;
    pAesdmaPri->fpAESDMAGetPSMatchedByteCNT = _MDrv_AESDMA_GetPSMatchedByteCNT;
    pAesdmaPri->fpAESDMAGetPSMatchedPTN = _MDrv_AESDMA_GetPSMatchedPTN;
    pAesdmaPri->fpAESDMANotify = _MDrv_AESDMA_Notify;
    pAesdmaPri->fpAESDMARand = NULL;
    pAesdmaPri->fpAESDMAGetRandNum = _MDrv_AESDMA_Get_Rand_Num;
    pAesdmaPri->fpRSACalculate = _MDrv_RSA_Calculate;
    pAesdmaPri->fpRSAIsFinished = _MDrv_RSA_IsFinished;
    pAesdmaPri->fpRSAOutput = _MDrv_RSA_Output;
    pAesdmaPri->fpSHACalculate = _MDrv_SHA_Calculate;
    pAesdmaPri->fpSHACalculateManual = MDrv_SHA_CalculateManual;
    pAesdmaPri->fpAESDMAParserMaskscrmb = _MDrv_AESDMA_Parser_MaskScrmb;
    pAesdmaPri->fpAESDMAParserSetScrmbPatten = _MDrv_AESDMA_Parser_SetScrmbPattern;
    pAesdmaPri->fpAESDMAParserBypassPid = _MDrv_AESDMA_Parser_BypassPid;
    pAesdmaPri->fpAESDMAParserSetPid = _MDrv_AESDMA_Parser_SetPid;
    pAesdmaPri->fpAESDMAParserEncrypt = _MDrv_AESDMA_Parser_Encrypt;
    pAesdmaPri->fpAESDMAParserDecrypt = _MDrv_AESDMA_Parser_Decrypt;
    pAesdmaPri->fpAESDMAParserQueryPidCount = _MDrv_AESDMA_Parser_QueryPidCount;
    pAesdmaPri->fpAESDMAParserSetAddedScrmbPattern = _MDrv_AESDMA_Parser_SetAddedScrmbPattern;
    pAesdmaPri->fpAESDMASetKeySel = _MDrv_AESDMA_SetKeySel;
    pAesdmaPri->fpAESDMASetOddIv = _MDrv_AESDMA_SetOddIV;
    pAesdmaPri->fpAESDMAIsSecretKeyInNormalBlank = _MDrv_AESDMA_IsSecretKeyInNormalBank;
    pAesdmaPri->fpHDCPHDCPProcessCipher = MDrv_HDCP_ProcessCipher;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AESDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    PAESDMA_INIT pInitParam = NULL;
    PAESDMA_FILE_INOUT pFileInOutParam = NULL;
    PAESDMA_SEL_ENG pSelEngParam = NULL;
    PAESDMA_SET_PS pSetPSParam = NULL;
    PAESDMA_NOTIFY pNotifyParam = NULL;
    PRSA_CALCULATE pRSACalParam = NULL;
    PRSA_OUTPUT pRSAOutParam = NULL;
    PSHA_CALCULATE pSHACalParam = NULL;
    PSHA_CALCULATE_MANUAL pSHACalManualParam = NULL;
    PAESDMA_PARSER_PID_CONFIGURATIONS pParserPidConfigurations = NULL;

    MS_U32 u32Ret = 0;
    MS_BOOL bEnable = 0;
	AESDMA_INSTANT_PRIVATE* psAesInstPri = NULL;
    AESDMA_RESOURCE_SHARED* psAesResourceShared = NULL;
    RSA_RESOURCE_SHARED* psRSAResourceShared = NULL;
    HDCP_CIPHER* psHDCPData = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void*)&psAesInstPri);
	void* pResource = NULL;
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;
    MS_U32 u32IVarrayAddr = 0;
    MS_U32 u32KeyarryAddr = 0;
    MS_U32 u32timeout_count = 0;
    MS_U32 u32timeout_value = 0;
    MS_U8  *pu8RandParam = NULL;

    switch(u32Cmd)
    {
        case MDrv_CMD_AESDMA_Init:
            {
                pInitParam = (PAESDMA_INIT)pArgs;
                u32Ret = (psAesInstPri->fpAESDMAInit)(pInitParam->u32miu0addr,pInitParam->u32miu1addr,pInitParam->u32miunum);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetIV:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetIV\n"));
                memcpy((void *)psAesInstPri->InitVector,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_Reset:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_Reset\n"));
                u32Ret = (psAesInstPri->fpAESDMAReset)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
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
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetKey\n"));
                memcpy((void *)psAesInstPri->CipherKey,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SelEng:
            {
                pSelEngParam = (PAESDMA_SEL_ENG)pArgs;
                psAesInstPri->eMode = pSelEngParam->eMode;
                psAesInstPri->bDescrypt = pSelEngParam->bDescrypt;
                psAesInstPri->bSetEngine = TRUE;
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
                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set mode and select encrypt or decrypt
                if (psAesInstPri->bSetEngine == TRUE)
                {
                    (psAesInstPri->fpAESDMASelEng)(psAesInstPri->eMode,psAesInstPri->bDescrypt);
                }

                //AESDMA set IV
                if((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_TDES_CTR)||

                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    u32IVarrayAddr = (MS_U32)&psAesInstPri->InitVector[0];
                    (psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);
                }

                //CBC segmental decryption case
                if((((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)|| (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))&& psAesInstPri->bDescrypt == TRUE)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR))
                {
                    MS_U32 FileinVirtAddr, i;

                    FileinVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileinAddr);

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
                    u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherKey[0];
                    (psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
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

            	if ((psAesInstPri->fpAESDMAGetStatus)(&u32Ret) == UTOPIA_STATUS_SUCCESS)
            	{
            		MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_GetStatus, Ret: 0x%x\n", (int)u32Ret));
            		*(MS_U32 *)pArgs = u32Ret;
            		return UTOPIA_STATUS_SUCCESS;
            	}
            	else
            	{
            		MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_GetStatus. Fail\n"));
            		return UTOPIA_STATUS_FAIL;
            	}


            }
        case MDrv_CMD_AESDMA_IsFinished:
            {
                if(pArgs)
                {
                    u32timeout_value = *(MS_U32 *)pArgs;
                }
                else
                {
                    printf("[AESDMA INFO] Please pass timeout value to driver\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

                if ((u32timeout_value < 1) | (u32timeout_value > AES_MAX_TIMEOUT_VALUE))
                {
                    printf("[AESDMA INFO] Invalid timeout value\n");
                    return UTOPIA_STATUS_ERR_INV;
                }


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

                while ((psAesInstPri->fpAESDMAIsFinished)() != DRVAESDMA_OK)
				{
					MsOS_DelayTaskUs(10);
					u32timeout_count++;

					//timeout
					if(u32timeout_count>u32timeout_value)
					{
						(psAesInstPri->fpAESDMAReset)();
                        UtopiaResourceRelease(pResource);
                        printf("[AESDMA ERROR] AESDMA timeout happened\n");
						return UTOPIA_STATUS_FAIL;
					}

				}

                //CBC segmental encryption case
                if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                    && psAesInstPri->bDescrypt == FALSE)
                {
                    //printf("[AESDMA]FileOutStartAddr PA = %x\n",psAesInstPri->u32FileOutSAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 FileOutStartVirtAddr, i;

                    FileOutStartVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileOutSAddr);

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
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetClk bEnable = %x\n", bEnable));
                u32Ret = (psAesInstPri->fpAESDMASetClk)(bEnable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetSecureKey:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetSecureKey\n"));
                psAesInstPri->bSecretKey = 1;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetPS:
            {
                pSetPSParam = (PAESDMA_SET_PS)pArgs;
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetPS, %x, %x, %x, %x\n", (int)pSetPSParam->u32PTN, (int)pSetPSParam->u32Mask, (int)pSetPSParam->bPSin_Enable, (int)pSetPSParam->bPSout_Enable));
                u32Ret = (psAesInstPri->fpAESDMASetPS)(pSetPSParam->u32PTN,pSetPSParam->u32Mask,pSetPSParam->bPSin_Enable,pSetPSParam->bPSout_Enable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_PSRelease:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_PSRelease \n"));
                u32Ret = (psAesInstPri->fpAESDMAPSRelease)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedByteCNT:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_GetPSMatchedByteCNT \n"));
                (psAesInstPri->fpAESDMAGetPSMatchedByteCNT)(&u32Ret);
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedPTN:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_GetPSMatchedPTN \n"));
                u32Ret = (psAesInstPri->fpAESDMAGetPSMatchedPTN)();
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_Notify:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_Notify \n"));
                pNotifyParam = (PAESDMA_NOTIFY)pArgs;
                u32Ret = (psAesInstPri->fpAESDMANotify)(pNotifyParam->eEvents,pNotifyParam->pfCallback);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_Rand:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_Rand \n"));
                pu8RandParam = (MS_U8 *)pArgs;
                u32Ret = (psAesInstPri->fpAESDMAGetRandNum)(pu8RandParam);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_Calculate:
            {
                pRSACalParam = (PRSA_CALCULATE)pArgs;
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_RSA_Calculate mode: 0x%x\n", (int)pRSACalParam->eMode));

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

                u32Ret = (psAesInstPri->fpRSACalculate)(pRSACalParam->pstSign, pRSACalParam->pstKey, pRSACalParam->eMode);

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_IsFinished:
            {
                if(pArgs)
                {
                    u32timeout_value = *(MS_U32 *)pArgs;
                }
                else
                {
                    printf("[RSA INFO] Please pass timeout value to driver\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

                if (u32timeout_value > RSA_MAX_TIMEOUT_VALUE)
                {
                    printf("[RSA INFO] Invalid timeout value\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

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
				
                while ((psAesInstPri->fpRSAIsFinished)() != DRVAESDMA_OK)
				{
					MsOS_DelayTaskUs(10);
					u32timeout_count++;

					//timeout
					if(u32timeout_count>u32timeout_value)
					{
                        UtopiaResourceRelease(pResource);
                        printf("[RSA ERROR] RSA timeout happened\n");
						return UTOPIA_STATUS_FAIL;
					}

				}

                MS_DEBUG_MSG(printf("[RSA INFO]MDrv_RSA_IsFinished\n"));
                return UTOPIA_STATUS_SUCCESS;

            }
        case MDrv_CMD_RSA_Output:
            {
            	MS_DEBUG_MSG(printf("[RSA INFO]MDrv_CMD_RSA_Output\n"));
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
                MS_DEBUG_MSG(printf("[RSA INFO] Mode: 0x%x\n", pRSAOutParam->eMode));
                u32Ret = (psAesInstPri->fpRSAOutput)(pRSAOutParam->eMode,pRSAOutParam->pstRSAOut);

                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_Calculate:
            {
            	MS_DEBUG_MSG(printf("[RSA INFO]MDrv_CMD_SHA_Calculate\n"));
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
                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);
                
                //AESDMA trigger start for using AESDMA replace BDMA case
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_CalculateManual:
            {
                pSHACalManualParam = (PSHA_CALCULATE_MANUAL)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_SHA, &pResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculateManual)(pSHACalManualParam->stCfg,pSHACalManualParam->eStage,pSHACalManualParam->u32DoneBytes,pSHACalManualParam->pu8SetIV);
                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }

        case MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN:
            {
			    if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAesInstPri->eAddedScrmbPattern = *(DrvAESDMA_ScrmbPattern*)pArgs;
                MS_DEBUG_MSG(printf("[RSA INFO]MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN eAddedScrmbPattern = %x\n", psAesInstPri->eAddedScrmbPattern));
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

//                *(MS_U8 *)pArgs = psAesInstPri->fpAESDMAParserQueryPidCount();
                psAesInstPri->fpAESDMAParserQueryPidCount(pArgs);
                MS_DEBUG_MSG(printf("[RSA INFO]MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT pArgs = %x\n", *(MS_U8 *)pArgs));
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_MASKSCRMB:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;			
					
                bEnable = *(MS_BOOL *)pArgs;
                psAesInstPri->bMaskScrmb = bEnable;
                MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_MASKSCRMB bMaskScrmb = %x\n",(int)bEnable));
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAesInstPri->eScrmbPattern = *(DrvAESDMA_ScrmbPattern*)pArgs;
                MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN eScrmbPattern = %x\n",(int)psAesInstPri->eScrmbPattern));

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_BYPASSPID:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                bEnable = *(MS_BOOL *)pArgs;

                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_BYPASSPID, bEnable = %x\n", (int)bEnable));

                psAesInstPri->bBypassPid = bEnable;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_SETPID:
            {
            	MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_SETPID: %s\n", pArgs == NULL ? "NULL" : "Hit"));
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;
					
                pParserPidConfigurations = (PAESDMA_PARSER_PID_CONFIGURATIONS)pArgs;

                if (pParserPidConfigurations->u8PidIndex == 0)
                {
                    psAesInstPri->u8PidIndex = pParserPidConfigurations->u8PidIndex;
                    psAesInstPri->u16Pid = pParserPidConfigurations->u16Pid;
                }
                else if (pParserPidConfigurations->u8PidIndex == 1)
                {
                    psAesInstPri->u8PidIndex1 = pParserPidConfigurations->u8PidIndex;
                    psAesInstPri->u16Pid1 = pParserPidConfigurations->u16Pid;
                }

                MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_SETPID u8PidIndex = %x, u16Pid1 = %x\n",(int)pParserPidConfigurations->u8PidIndex, (int)pParserPidConfigurations->u16Pid));

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_ENCRYPT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAesInstPri->eParserMode = *(DrvAESDMA_ParserMode*)pArgs;
                psAesInstPri->bParserDecrypt = FALSE;
                MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_ENCRYPT eParserMode = %x\n",(int)psAesInstPri->eParserMode));
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_DECRYPT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;			
					
                psAesInstPri->eParserMode = *(DrvAESDMA_ParserMode*)pArgs;
                psAesInstPri->bParserDecrypt = TRUE;

                MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_DECRYPT eParserMode = %x\n",(int)psAesInstPri->eParserMode));
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_Start:
            {
            	MS_DEBUG_MSG(printf("\n[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_Start\n"));
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
                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set mode and select encrypt or decrypt
                if (psAesInstPri->bSetEngine == TRUE)
                {
                    (psAesInstPri->fpAESDMASelEng)(psAesInstPri->eMode,psAesInstPri->bDescrypt);
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bEnableTwoKey==1)
                {
                    u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherOddKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_ODD_KEY);

                    u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_EVEN_KEY);
                }
                else
                {
                    if(psAesInstPri->bSecretKey==1)
                    {
                        (psAesInstPri->fpAESDMASetSecureKey)();
                    }
                    else
                    {
                        u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherKey[0];
                        (psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                    }
                }

                //AESDMA set IV
                if((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    u32IVarrayAddr = (MS_U32)&psAesInstPri->InitVector[0];
                    (psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);

                    if(psAesInstPri->bEnableTwoKey==1)
                    {
                        u32IVarrayAddr = (MS_U32)&psAesInstPri->InitVectorOdd[0];
                        (psAesInstPri->fpAESDMASetOddIv)((MS_U32*)u32IVarrayAddr);
                    }
                }

                // HW Parser configurations
                if (psAesInstPri->bParserDecrypt == TRUE)
                {
                    (psAesInstPri->fpAESDMAParserDecrypt)(psAesInstPri->eParserMode);
                }
                else
                {
                    (psAesInstPri->fpAESDMAParserEncrypt)(psAesInstPri->eParserMode);
                    (psAesInstPri->fpAESDMAParserSetAddedScrmbPattern)(psAesInstPri->eAddedScrmbPattern);
                }

                (psAesInstPri->fpAESDMAParserSetScrmbPatten)(psAesInstPri->eScrmbPattern);
                (psAesInstPri->fpAESDMAParserMaskscrmb)(psAesInstPri->bMaskScrmb);

                // Set PID
                if (psAesInstPri->bBypassPid == TRUE)
                {
                	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_Start, ByPass Pid\n"));
                    (psAesInstPri->fpAESDMAParserBypassPid)(psAesInstPri->bBypassPid);
                }
                else
                {
                	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_Start, Set Pid 0x%x - 0x%x, 0x%x - 0x%x \n", psAesInstPri->u8PidIndex, psAesInstPri->u16Pid, psAesInstPri->u8PidIndex1, psAesInstPri->u16Pid1));
                    (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex, psAesInstPri->u16Pid);
                    (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex1, psAesInstPri->u16Pid1);
                }

                // AESDMA trigger start
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_Start\n"));
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }

        case MDrv_CMD_AESDMA_SetOddKey:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_SetOddKey\n"));
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                memcpy((void *)psAesInstPri->CipherOddKey,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_ENABLE_Two_Key:
            {
            	MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_ENABLE_Two_Key\n"));
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                bEnable = *(MS_BOOL *)pArgs;
                psAesInstPri->bEnableTwoKey = bEnable;

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_SetOddIV:
            {
                memcpy((void *)psAesInstPri->InitVectorOdd,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                if ( psAesInstPri->fpAESDMAIsSecretKeyInNormalBlank(pArgs) == DRVAESDMA_OK )
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }

        case MDrv_CMD_HDCP_MDrv_PROCESSCIPHER:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psHDCPData = (HDCP_CIPHER *)pArgs;

                if ( psAesInstPri->fpHDCPHDCPProcessCipher(psHDCPData->u8Idx, (void*)psHDCPData->pu8privData, (void*)psHDCPData->pu8Ks, (void*)psHDCPData->pu8LC128) == DRVAESDMA_OK )
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
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

