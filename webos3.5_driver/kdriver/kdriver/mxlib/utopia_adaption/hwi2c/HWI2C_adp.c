#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/uaccess.h>
#include "MsTypes.h"
#include "utopia.h"
#include "drvHWI2C.h"
#include "drvHWI2C_v2.h"
#include "HWI2C_adp.h"
#include "utopia_adp.h"

// no pointer member
//UADP_SPT_0NXT_DEF(MS_U8);
//UADP_SPT_0NXT_DEF(HWI2C_Status);
extern void* spt_MS_U8;

UADP_SPT_0NXT_DEF(HWI2C_UnitCfg);


UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SelectPort);
UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetClk);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetReadMode);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_WriteByte);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SelectPort1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetClkP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetReadModeP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_WriteByteP1);
//UADP_SPT_0NXT_DEF(HWI2C_PRIVATE_PARAM_SetDbgLevel);
// one pointer member
UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_Init);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetPortIndex);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_ReadByte);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_ReadByteP1);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetLibVer);
//UADP_SPT_1NXT_DEF(HWI2C_PRIVATE_PARAM_GetStatus);
// two pointer member
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_WriteBytes);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_ReadBytes);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_WriteBytesP1);
//UADP_SPT_2NXT_DEF(HWI2C_PRIVATE_PARAM_ReadBytesP1);

MS_U32 HWI2C_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
    UADP_SPT_0NXT(HWI2C_UnitCfg);
//set table
    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SelectPort);
    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetClk);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetReadMode);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_WriteByte);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SelectPort1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetClkP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetReadModeP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_WriteByteP1);
//    UADP_SPT_0NXT(HWI2C_PRIVATE_PARAM_SetDbgLevel);

    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_Init, psCfg, HWI2C_UnitCfg);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetPortIndex, pu8Port, MS_U8);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_ReadByte, pData, MS_U8);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_ReadByteP1, pData, MS_U8);
    //UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetLibVer, ppVersion, MSIF_Version);
//    UADP_SPT_1NXT(HWI2C_PRIVATE_PARAM_GetStatus, pStatus, HWI2C_Status);

    //UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_WriteBytes, pRegAddr, MS_U8, pData, MS_U8);
    //UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_ReadBytes, pRegAddr, MS_U8, pData, MS_U8);
  //  UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_WriteBytesP1, pRegAddr, MS_U8, pData, MS_U8);
  //  UADP_SPT_2NXT(HWI2C_PRIVATE_PARAM_ReadBytesP1, pRegAddr, MS_U8, pData, MS_U8);
//set table


    *pIoctl= (FUtopiaIOctl)HWI2C_adp_Ioctl;
    return 0;
}

MS_U32 HWI2C_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1]={0};

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_HWI2C_Init :
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_Init, spt_HWI2C_PRIVATE_PARAM_Init,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_HWI2C_Start:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_HWI2C_Stop:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_HWI2C_SelectPort:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_SelectPort, spt_HWI2C_PRIVATE_PARAM_SelectPort,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_HWI2C_SetClk:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_HWI2C_PRIVATE_PARAM_SetClk, spt_HWI2C_PRIVATE_PARAM_SetClk,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_HWI2C_WriteBytes:
            {
                HWI2C_PRIVATE_PARAM_WriteBytes paramW;
                MS_U8 u8Addr[64];
                MS_U8 u8Data[512];
                MS_U32 DataSize = 0;
                MS_U8 *tmpBuf = NULL;
                MS_BOOL *bReturn_bak = NULL;
                MS_BOOL tmp_Return = 0;

                copy_from_user(&paramW,pArgs,sizeof(HWI2C_PRIVATE_PARAM_WriteBytes));

                if (paramW.uAddrCnt > 0)
                    copy_from_user(u8Addr, paramW.pRegAddr,paramW.uAddrCnt);

                DataSize = paramW.uSize;
                if (DataSize > 0)
                {
                    //check user data size is enough or not
                    if(DataSize > 512)
                    {
                        tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                        if(tmpBuf == NULL)
                        {
                            return 1;  // 1: fail....
                        }

                        copy_from_user(tmpBuf, paramW.pData, DataSize);
                    }
                    else
                        copy_from_user(u8Data, paramW.pData, DataSize);

                }

                //backup bReturn user address
                bReturn_bak = paramW.bReturn;
                paramW.bReturn = &tmp_Return;

                if(DataSize > 512)
                    paramW.pData = tmpBuf;
                else
                    paramW.pData = u8Data;

                paramW.pRegAddr = u8Addr;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramW);

                if(DataSize > 512)
                    kfree(tmpBuf);

                copy_to_user((void *)bReturn_bak , (void *) paramW.bReturn, sizeof(MS_BOOL));

            }
            break;
        case MDrv_CMD_HWI2C_WriteBytes_CUST:
            {
                HWI2C_PRIVATE_PARAM_WriteBytes_CUST paramW;

                MS_U8 u8Addr[64];
                MS_U8 u8Data[512];
                MS_U32 DataSize = 0;
                MS_U8 *tmpBuf = NULL;
                MS_BOOL *bReturn_bak = NULL;
                MS_BOOL tmp_Return = 0;

                copy_from_user(&paramW,pArgs,sizeof(HWI2C_PRIVATE_PARAM_WriteBytes_CUST));

                if (paramW.uAddrCnt > 0)
                    copy_from_user(u8Addr, paramW.pRegAddr,paramW.uAddrCnt);

                DataSize = paramW.uSize;
                if (DataSize > 0)
                {
                    //check user data size is enough or not
                    if(DataSize > 512)
                    {
                        tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                        if(tmpBuf == NULL)
                        {
                            return 1;  // 1: fail....
                        }

                        copy_from_user(tmpBuf, paramW.pData, DataSize);
                    }
                    else
                        copy_from_user(u8Data, paramW.pData, DataSize);

                }

                //backup bReturn user address
                bReturn_bak = paramW.bReturn;
                paramW.bReturn = &tmp_Return;

                if(DataSize > 512)
                    paramW.pData = tmpBuf;
                else
                    paramW.pData = u8Data;

                paramW.pRegAddr = u8Addr;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramW);

                if(DataSize > 512)
                    kfree(tmpBuf);

                copy_to_user((void *)bReturn_bak , (void *) paramW.bReturn, sizeof(MS_BOOL));
            }
            break;
        case MDrv_CMD_HWI2C_ReadBytes:
            {
                HWI2C_PRIVATE_PARAM_ReadBytes paramR;
                MS_U8 u8Addr[64];
                MS_U8 u8Data[512];
                MS_U32 DataSize = 0;
                MS_U8 *tmpBuf = NULL;
                MS_U8 *RxBuf_bak = NULL;
                MS_BOOL *bReturn_bak = NULL;
                MS_BOOL tmp_Return = 0;

                copy_from_user(&paramR,pArgs,sizeof(HWI2C_PRIVATE_PARAM_ReadBytes));

                if (paramR.uAddrCnt > 0)
                    copy_from_user(u8Addr, paramR.pRegAddr,paramR.uAddrCnt);

                DataSize = paramR.uSize;
                //check user data size
                if (DataSize > 512)
                {
                    tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                    if(tmpBuf == NULL)
                    {
                        return 1;  // 1: fail....
                    }
                }

                //Backup user data buffer
                RxBuf_bak = paramR.pData;
                bReturn_bak = paramR.bReturn;
                paramR.bReturn = &tmp_Return;

                if(DataSize > 512)
                    paramR.pData = tmpBuf;
                else
                    paramR.pData = u8Data;

                paramR.pRegAddr = u8Addr;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramR);

                if (DataSize > 0)
                {
                    if (DataSize > 512)
                    {
                        copy_to_user(RxBuf_bak, tmpBuf, DataSize);
                        kfree(tmpBuf);
                    }
                    else
                        copy_to_user(RxBuf_bak, u8Data, DataSize);
                }

                copy_to_user((void *)bReturn_bak , (void *)paramR.bReturn, sizeof(MS_BOOL));
            }
            break;

        case MDrv_CMD_HWI2C_ReadBytes_CUST:
            {
                HWI2C_PRIVATE_PARAM_ReadBytes_CUST paramR;
                MS_U8 u8Addr[64];
                MS_U8 u8Data[512];
                MS_U32 DataSize = 0;
                MS_U8 *tmpBuf = NULL;
                MS_U8 *RxBuf_bak = NULL;
                MS_BOOL *bReturn_bak = NULL;
                MS_BOOL tmp_Return = 0;

                copy_from_user(&paramR,pArgs,sizeof(HWI2C_PRIVATE_PARAM_ReadBytes_CUST));

                if (paramR.uAddrCnt > 0)
                    copy_from_user(u8Addr, paramR.pRegAddr,paramR.uAddrCnt);

                DataSize = paramR.uSize;
                //check user data size
                if (DataSize > 512)
                {
                    tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                    if(tmpBuf == NULL)
                    {
                        return 1;  // 1: fail....
                    }
                }

                //Backup user data buffer
                RxBuf_bak = paramR.pData;
                bReturn_bak = paramR.bReturn;
                paramR.bReturn = &tmp_Return;

                if(DataSize > 512)
                    paramR.pData = tmpBuf;
                else
                    paramR.pData = u8Data;

                paramR.pRegAddr = u8Addr;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramR);

                if (DataSize > 0)
                {
                    if (DataSize > 512)
                    {
                        copy_to_user(RxBuf_bak, tmpBuf, DataSize);
                        kfree(tmpBuf);
                    }
                    else
                        copy_to_user(RxBuf_bak, u8Data, DataSize);
                }

                copy_to_user((void *)bReturn_bak , (void *)paramR.bReturn, sizeof(MS_BOOL));
            }
            break;
        default:
            break;
    }
	return u32Ret;
}

