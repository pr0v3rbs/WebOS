#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvAESDMA.h"
#include "drvAESDMA_v2.h"
#include "AESDMA_adp.h"


#include "utopia_adp.h"
//#include "MsOS.h"


//Top
UADP_STRUCT_POINTER_TABLE spt_AESDMA_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_FILE_INOUT[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_SEL_ENG[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_SET_PS[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_NOTIFY[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_RAND[10];
UADP_STRUCT_POINTER_TABLE spt_SHA_CALCULATE[10];
UADP_STRUCT_POINTER_TABLE spt_SHA_CALCULATE_MANUAL[10];

UADP_STRUCT_POINTER_TABLE spt_AESDMA_SetKeyList[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_SetIvList[10];

//Bottom
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_RSASig[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_RSAKey[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_RSAOut[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_HASHCFG[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_SHADataInfo[10];
UADP_STRUCT_POINTER_TABLE spt_DrvAESDMA_SecureInfo[10];

UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserSetScrmbPattern[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserPyPassPid[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserSetPid[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserEncrypt[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserDecrypt[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_ParserSetAddedScrmbPattern[10];
UADP_STRUCT_POINTER_TABLE spt_AESDMA_GetStatus[10];
//UADP_STRUCT_POINTER_TABLE spt_AESDMA_MDrvProcessCipher[10];




MS_U32 AESDMA_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    printk("AESDMA_adp_Init\n");

    UADP_SPT_BGN(&spt_AESDMA_INIT[0],sizeof(AESDMA_INIT));
    UADP_SPT_FIN(&spt_AESDMA_INIT[1]);
	

	
    UADP_SPT_BGN(&spt_AESDMA_FILE_INOUT[0], sizeof(AESDMA_FILE_INOUT));
    UADP_SPT_FIN(&spt_AESDMA_FILE_INOUT[1]);


    UADP_SPT_BGN(&spt_AESDMA_SEL_ENG[0], sizeof(AESDMA_SEL_ENG));
    UADP_SPT_FIN(&spt_AESDMA_SEL_ENG[1]);


	UADP_SPT_BGN(&spt_AESDMA_SET_PS[0],sizeof(AESDMA_SET_PS));
    UADP_SPT_FIN(&spt_AESDMA_SET_PS[1]);


    UADP_SPT_BGN(&spt_AESDMA_NOTIFY[0],sizeof(AESDMA_NOTIFY));
    UADP_SPT_FIN(&spt_AESDMA_NOTIFY[1]);

			
    UADP_SPT_BGN(&spt_AESDMA_RAND[0],sizeof(AESDMA_RAND));
    UADP_SPT_FIN(&spt_AESDMA_RAND[1]);


	UADP_SPT_BGN(&spt_SHA_CALCULATE[0], sizeof(SHA_CALCULATE));
    UADP_SPT_FIN(&spt_SHA_CALCULATE[1]);

    UADP_SPT_BGN(&spt_SHA_CALCULATE_MANUAL[0], sizeof(SHA_CALCULATE_MANUAL));
    UADP_SPT_FIN(&spt_SHA_CALCULATE_MANUAL[1]);


	//bottom

    UADP_SPT_BGN(&spt_DrvAESDMA_RSAKey[0],sizeof(DrvAESDMA_RSAKey));
    UADP_SPT_FIN(&spt_DrvAESDMA_RSAKey[1]);

    UADP_SPT_BGN(&spt_DrvAESDMA_RSASig[0],sizeof(DrvAESDMA_RSASig));
    UADP_SPT_FIN(&spt_DrvAESDMA_RSASig[1]);

    UADP_SPT_BGN(&spt_DrvAESDMA_RSAOut[0],sizeof(DrvAESDMA_RSAOut));
    UADP_SPT_FIN(&spt_DrvAESDMA_RSAOut[1]);

    UADP_SPT_BGN(&spt_DrvAESDMA_SecureInfo[0],sizeof(DrvAESDMA_SecureInfo));
    UADP_SPT_FIN(&spt_DrvAESDMA_SecureInfo[1]);

    UADP_SPT_BGN(&spt_DrvAESDMA_SHADataInfo[0], sizeof(DrvAESDMA_SHADataInfo));
    UADP_SPT_FIN(&spt_DrvAESDMA_SHADataInfo[1]);

    UADP_SPT_BGN(&spt_DrvAESDMA_HASHCFG[0], sizeof(DrvAESDMA_HASHCFG));
    UADP_SPT_FIN(&spt_DrvAESDMA_HASHCFG[3]);
    
    UADP_SPT_BGN(&spt_AESDMA_SetKeyList[0],sizeof(MS_U32)*4);
    UADP_SPT_FIN(&spt_AESDMA_SetKeyList[1]);

    UADP_SPT_BGN(&spt_AESDMA_SetIvList[0],sizeof(MS_U32)*4);
    UADP_SPT_FIN(&spt_AESDMA_SetIvList[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserSetScrmbPattern[0], sizeof(AESDMA_PARSER_CONFIGURATIONS));
    UADP_SPT_FIN(&spt_AESDMA_ParserSetScrmbPattern[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserPyPassPid[0], sizeof(AESDMA_PARSER_PID_CONFIGURATIONS));
    UADP_SPT_FIN(&spt_AESDMA_ParserPyPassPid[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserSetPid[0], sizeof(AESDMA_PARSER_PID_CONFIGURATIONS));
    UADP_SPT_FIN(&spt_AESDMA_ParserSetPid[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserEncrypt[0], sizeof(DrvAESDMA_ParserMode));
    UADP_SPT_FIN(&spt_AESDMA_ParserEncrypt[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserDecrypt[0], sizeof(DrvAESDMA_ParserMode));
    UADP_SPT_FIN(&spt_AESDMA_ParserDecrypt[1]);

    UADP_SPT_BGN(&spt_AESDMA_ParserSetAddedScrmbPattern[0], sizeof(AESDMA_PARSER_CONFIGURATIONS));
    UADP_SPT_FIN(&spt_AESDMA_ParserSetAddedScrmbPattern[1]);

    UADP_SPT_BGN(&spt_AESDMA_GetStatus[0], sizeof(MS_U32));
    UADP_SPT_FIN(&spt_AESDMA_GetStatus[1]);

//    UADP_SPT_BGN(&spt_AESDMA_MDrvProcessCipher[0], sizeof(HDCP_CIPHER));
//    UADP_SPT_FIN(&spt_AESDMA_MDrvProcessCipher[1]);


	*pIoctl= (FUtopiaIOctl)AESDMA_adp_Ioctl;
	return 0;

}

/*
MS_U32 AESDMA_DDI_Open(void* pInstant,const void* const pAttribute)
{
    return UtopiaOpen(MODULE_AESDMA,pInstant,0,pAttribute);
}
*/
MS_U32 AESDMA_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_AESDMA_Init :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_INIT,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_SetIV:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SetIvList,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_Reset:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetFileInOut:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_FILE_INOUT, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_SetKey:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SetKeyList,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_SelEng:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SEL_ENG,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_Start:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_GetStatus,spt_AESDMA_GetStatus,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_IsFinished:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetClk:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetSecureKey:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetPS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SET_PS,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PSRelease:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_GetPSMatchedByteCNT:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_GetPSMatchedPTN:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_Notify:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_NOTIFY,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_Rand:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
		    break;
        case MDrv_CMD_RSA_Calculate:
            {
                void* pDatatmp_RSASig;
                void* pDatatmp_RSAKey;
                RSA_CALCULATE tmp_RSA;

                copy_from_user((void *)&tmp_RSA, (void __user *)pArgs, sizeof (RSA_CALCULATE));

                pDatatmp_RSASig = kmalloc(sizeof (DrvAESDMA_RSASig),GFP_KERNEL);

                if( pDatatmp_RSASig )
                    copy_from_user(pDatatmp_RSASig, (void __user *)tmp_RSA.pstSign, sizeof (DrvAESDMA_RSASig));
                else{
                    printk("[AESDMA][line:%d]kmalloc fail\n",__LINE__);
                    break;
                }

                pDatatmp_RSAKey = kmalloc(sizeof (DrvAESDMA_RSAKey),GFP_KERNEL);
                if( pDatatmp_RSAKey )
                    copy_from_user(pDatatmp_RSAKey, (void __user *)tmp_RSA.pstKey, sizeof (DrvAESDMA_RSAKey));
                else{
                    printk("[AESDMA][line:%d]kmalloc fail\n",__LINE__);
                    kfree(pDatatmp_RSASig);
                    break;
                }

                tmp_RSA.pstSign = pDatatmp_RSASig;
                tmp_RSA.pstKey  = pDatatmp_RSAKey;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp_RSA);
                kfree(pDatatmp_RSASig);
                kfree(pDatatmp_RSAKey);
            }
            break;
        case MDrv_CMD_RSA_IsFinished:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_RSA_Output:
            {
                void* pDatatmp_RSAOut;
                void* pDatatmp_RSAUserOut;
                RSA_OUTPUT tmp_RSA;

                copy_from_user((void *)&tmp_RSA, (void __user *)pArgs, sizeof (RSA_OUTPUT));

                pDatatmp_RSAOut = kmalloc(sizeof (DrvAESDMA_RSAOut),GFP_KERNEL);

                if( pDatatmp_RSAOut )
                    copy_from_user(pDatatmp_RSAOut, (void __user *)tmp_RSA.pstRSAOut, sizeof (RSA_OUTPUT));
                else{
                    printk("[AESDMA][line:%d]kmalloc fail\n",__LINE__);
                    break;
                }

                pDatatmp_RSAUserOut = tmp_RSA.pstRSAOut;
                tmp_RSA.pstRSAOut = pDatatmp_RSAOut;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp_RSA);

                copy_to_user((void __user *)pDatatmp_RSAUserOut, (void *)tmp_RSA.pstRSAOut, sizeof(DrvAESDMA_RSAOut));
                kfree(pDatatmp_RSAOut);
            }
		    break;
        case MDrv_CMD_SHA_Calculate:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SHA_CALCULATE,spt_SHA_CALCULATE,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_Memcpy:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SHA_CalculateManual:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SHA_CALCULATE_MANUAL,spt_SHA_CALCULATE_MANUAL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_MASKSCRMB:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserSetScrmbPattern,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_BYPASSPID:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserPyPassPid,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_SETPID:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserSetPid,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_ENCRYPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserEncrypt,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_DECRYPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserDecrypt,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_Start:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_ParserSetAddedScrmbPattern,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_ENABLE_Two_Key:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetOddKey:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SetKeyList,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_AESDMA_SetOddIV:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_SetIvList,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_AESDMA_SetKey_Ex:
            break;
        case MDrv_CMD_AESDMA_SetIV_Ex:
            break;
        case MDrv_CMD_HDCP_MDrv_PROCESSCIPHER:
//            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AESDMA_MDrvProcessCipher,NULL,buffer_arg,sizeof(buffer_arg));
            break;

		//case MDrv_CMD_Event_Poll:
		//	UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
		default:
			break;

    }


    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




