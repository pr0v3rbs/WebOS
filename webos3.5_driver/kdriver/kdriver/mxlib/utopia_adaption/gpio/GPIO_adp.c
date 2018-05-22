#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "MsOS.h"
#include "utopia.h"
#include "drvGPIO.h"
#include "GPIO_adp.h"
#include "utopia_adp.h"
#include "drvGPIO_v2.h"




#include <linux/uaccess.h>

#define GPIO_LOCAL_BUFF_SIZE    128

// no pointer member
UADP_SPT_0NXT_DEF(MS_GPIO_NUM);
UADP_SPT_0NXT_DEF(GPIO_PRIVATE_PARAM);
// one pointer member

// two pointer member


MS_U32 GPIO_adp_Init(FUtopiaIOctl* pIoctl)
{
    //member of struct
    UADP_SPT_0NXT(MS_GPIO_NUM);
    UADP_SPT_0NXT(GPIO_PRIVATE_PARAM);
    *pIoctl= (FUtopiaIOctl)GPIO_adp_Ioctl;
    return 0;
}

MS_U32 GPIO_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1]={0};

    GPIO_PRIVATE_PARAM param_tmp;

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_GPIO_INIT:
    	    u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_GPIO_Set_HIGH:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Set_LOW:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
    	    break;
        case MDrv_CMD_GPIO_Set_INPUT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
    	    break;
    	case MDrv_CMD_GPIO_Get_INOUT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_PARAM, spt_GPIO_PRIVATE_PARAM,buffer_arg,sizeof(buffer_arg));
    	    break;
    	case MDrv_CMD_GPIO_Get_LEVEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_PARAM, spt_GPIO_PRIVATE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case  MDrv_CMD_GPIO_BUS_START:
            {
               GPIO_Bus_Data BusData;
               copy_from_user((void *)&BusData, (void __user*)pArgs, sizeof(GPIO_Bus_Data));

               MS_BOOL bRet = TRUE;
               MS_BOOL *pbRet_User = BusData.bRet;
               BusData.bRet = &bRet;
               u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &BusData);
               if (u32Ret == UTOPIA_STATUS_SUCCESS)
               {
                   copy_to_user((void __user*)pbRet_User, (void *)&bRet, sizeof(MS_BOOL));
               }
               else
               {
                   //printf("[Error] %s(%d) cmd:%d ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
               }
            }
            break;
        case MDrv_CMD_GPIO_SEND_BUS_DATA:
            {
                 GPIO_Bus_Data BusData;

                 if(0 != copy_from_user((void *)&BusData, (void __user*)pArgs, sizeof(GPIO_Bus_Data)))
                 {
                     return -EACCES;
                 }

                 MS_BOOL bRet = FALSE;
                 MS_BOOL *pbRet_User = BusData.bRet;
                 MS_U8 *pBuff_User = BusData.pBuf;
                 MS_U8 pBuff[GPIO_LOCAL_BUFF_SIZE]={0,};
                 MS_U8 *pBigBuff = NULL;
                 MS_U8 *pTempBuff = NULL;

                 if( BusData.u16byte > GPIO_LOCAL_BUFF_SIZE )
                 {
                    pBigBuff = (MS_U8 *)kmalloc((size_t)BusData.u16byte,GFP_KERNEL);
                    if( pBigBuff == NULL )
                    {
                        printk("[GPIO][line:%d]kmalloc fail\n",__LINE__);
                        break;
                    }
                    pTempBuff = pBigBuff;
                 }
                 else
                 {
                    pTempBuff = pBuff;
                 }

                 if(pBuff_User != NULL)
                 {
                     copy_from_user((void *)pTempBuff, (void __user *)pBuff_User, (size_t)BusData.u16byte);
                 }
                 else
                 {
                     if( pBigBuff )
                         kfree((void *)pBigBuff);
                     return UTOPIA_STATUS_FAIL;
                 }

                 BusData.pBuf = pTempBuff;
                 BusData.bRet = &bRet;

                 u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &BusData);

                 if (u32Ret == UTOPIA_STATUS_SUCCESS)
                 {
                    copy_to_user((void __user*)pbRet_User, (void *)&bRet, sizeof(MS_BOOL));
                 }
                 else
                 {
                     //printf("[Error] %s(%d) cmd:%d ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
                 }

                 if( pBigBuff )
                     kfree((void *)pBigBuff);
            }
            break;
        case MDrv_CMD_GPIO_GET_BUS_DATA:
            {
                 GPIO_Bus_Data BusData;
                 if(0 != copy_from_user((void *)&BusData, (void __user*)pArgs, sizeof(GPIO_Bus_Data)))
                 {
                     //printf("%s(%d) hank here\n",__FUNCTION__,__LINE__);
                     return -EACCES;
                 }

                 MS_BOOL bRet = FALSE;
                 MS_BOOL *pbRet_User = BusData.bRet;
                 MS_U8 pBuff[GPIO_LOCAL_BUFF_SIZE]={0,};
                 MS_U8 *pBigBuff = NULL;
                 MS_U8 *pBuff_User = BusData.pBuf;

                 BusData.pBuf = pBuff;
                 BusData.bRet = &bRet;

                 if( BusData.u16byte > GPIO_LOCAL_BUFF_SIZE )
                 {
                    pBigBuff = (MS_U8 *)kmalloc((size_t)BusData.u16byte,GFP_KERNEL);

                    if( pBigBuff == NULL )
                    {
                        printk("[GPIO][line:%d]kmalloc fail\n",__LINE__);
                        break;
                    }

                    BusData.pBuf = pBigBuff;
                 }

                 u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &BusData);
                 if (u32Ret == UTOPIA_STATUS_SUCCESS)
                 {
                     copy_to_user((void __user *)pBuff_User, (void *)BusData.pBuf, (size_t)(sizeof(MS_U8)*(BusData.u16byte)));
                     copy_to_user((void __user *)pbRet_User, (void *)&bRet, sizeof(MS_BOOL));
                 }
                 else
                 {
                     //printf("[Error] %s(%d) cmd:%d ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
                 }

                 if( pBigBuff )
                    kfree((void *)pBigBuff);
            }
            break;
        case MDrv_CMD_GPIO_BUS_STOP:
            {
                  GPIO_Bus_Data BusData;
                  copy_from_user((void *)&BusData, (void __user*)pArgs, sizeof(GPIO_Bus_Data));

                  MS_BOOL bRet = TRUE;
                  MS_BOOL *pbRet_User = BusData.bRet;
                  BusData.bRet = &bRet;
                  u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &BusData);
                  if (u32Ret == UTOPIA_STATUS_SUCCESS)
                  {
                      copy_to_user((void __user*)pbRet_User, (void *)&bRet, sizeof(MS_BOOL));
                  }
                  else
                  {
                      //printf("[Error] %s(%d) cmd:%d ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
                  }
            }
            break;
        case MDrv_CMD_GPIO_Get_INT_Polarity:
        case MDrv_CMD_GPIO_Set_INT_Polarity:
        case MDrv_CMD_GPIO_GetMap_GPIOIntToNum:
            {
                GPIO_PRIVATE_INT_PARAM eGpioIntData;

                copy_from_user((void *)&eGpioIntData, (void __user*)pArgs, sizeof(GPIO_PRIVATE_INT_PARAM));
                u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &eGpioIntData);

                if (u32Ret == UTOPIA_STATUS_SUCCESS)
                {
                    copy_to_user((void __user*)pArgs, (void *)&eGpioIntData, sizeof(GPIO_PRIVATE_INT_PARAM));
                }
                else
                {
                    printk("[Error] %s(%d) cmd:%ld ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
                }
            }
            break;
        default:
            break;
    }
	return u32Ret;
}

