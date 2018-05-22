#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/uaccess.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvMSPI.h"
#include "drvMSPI_v2.h"
#include "MSPI_adp.h"


#include "utopia_adp.h"
//#include "MsOS.h"

//UADP_SPT_0NXT_DEF(MS_U8);
//UADP_SPT_0NXT_DEF(MS_U32);
extern void* spt_MS_U32;
extern void* spt_MS_U8;

UADP_SPT_0NXT_DEF(MSPI_INIT_EXT);
UADP_SPT_0NXT_DEF(MSPI_SLAVE_ENABLE);
UADP_SPT_0NXT_DEF(MSPISETPOWERSTATE);
UADP_SPT_0NXT_DEF(MASTERSPI_SLAVE_ENABLE);
UADP_SPT_0NXT_DEF(MASTERSPI_CSPADCONFIG);
UADP_SPT_0NXT_DEF(MASTERSPI_MAXCLKCONFIG);
UADP_SPT_0NXT_DEF(MSPI_SETDBGLEVEL);
UADP_SPT_0NXT_DEF(MSPI_config);
UADP_SPT_0NXT_DEF(MSPI_DCConfig);
UADP_SPT_0NXT_DEF(MSPI_FrameConfig);
UADP_SPT_0NXT_DEF(MSPI_CLKConfig);

UADP_SPT_1NXT_DEF(MSPI_INIT);
UADP_SPT_1NXT_DEF(MSPI_DCCONFIG);
UADP_SPT_1NXT_DEF(MSPI_CLKCONFIG);
UADP_SPT_1NXT_DEF(MSPI_FRAMECONFIG);
UADP_SPT_1NXT_DEF(MASTERSPI_DCCONFIG);
UADP_SPT_1NXT_DEF(MASTERSPI_CLKCONFIG);
UADP_SPT_1NXT_DEF(MASTERSPI_FRAMECONFIG);



MS_U32 MSPI_adp_Init(FUtopiaIOctl* pIoctl)
{
    //member of struct
    UADP_SPT_0NXT(MSPI_INIT_EXT);
    UADP_SPT_0NXT(MSPI_SLAVE_ENABLE);
    UADP_SPT_0NXT(MSPISETPOWERSTATE);
    UADP_SPT_0NXT(MASTERSPI_SLAVE_ENABLE);
    UADP_SPT_0NXT(MASTERSPI_CSPADCONFIG);
    UADP_SPT_0NXT(MASTERSPI_MAXCLKCONFIG);
    UADP_SPT_0NXT(MSPI_SETDBGLEVEL);
    UADP_SPT_0NXT(MSPI_config);
    UADP_SPT_0NXT(MSPI_DCConfig);
    UADP_SPT_0NXT(MSPI_FrameConfig);
    UADP_SPT_0NXT(MSPI_CLKConfig);
    UADP_SPT_1NXT(MSPI_INIT, ptMSPIConfig, MSPI_config);
    UADP_SPT_1NXT(MSPI_DCCONFIG, ptDCConfig, MSPI_DCConfig);
    UADP_SPT_1NXT(MSPI_CLKCONFIG, ptCLKConfig, MSPI_CLKConfig);
    UADP_SPT_1NXT(MSPI_FRAMECONFIG, ptFrameConfig, MSPI_FrameConfig);
    UADP_SPT_1NXT(MASTERSPI_DCCONFIG, ptDCConfig, MSPI_DCConfig);
    UADP_SPT_1NXT(MASTERSPI_CLKCONFIG, ptCLKConfig, MSPI_CLKConfig);
    UADP_SPT_1NXT(MASTERSPI_FRAMECONFIG, ptFrameConfig, MSPI_FrameConfig);

    *pIoctl= (FUtopiaIOctl)MSPI_adp_Ioctl;
    return 0;
}

MS_U32 MSPI_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MDrv_CMD_MSPI_Init_Ext:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_INIT_EXT, spt_MSPI_INIT_EXT, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_INIT, spt_MSPI_INIT, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_Read:
            {
                MSPI_READ stParamRead;
                MS_U8 *tmpBuf = NULL;
                MS_U8 *RxBuf_bak = NULL;
                MS_U32 DataSize = 0;
                MS_BOOL tmp_Return = 0;
                copy_from_user(&stParamRead,pArgs,sizeof(MSPI_READ));
                DataSize = stParamRead.u16Size;
                tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                //allocate kernel space buffer
                if(tmpBuf == NULL)
                {
                    return 1;  // 1: fail....
                }
                //backup user data address
                RxBuf_bak = stParamRead.pu8Data;
                stParamRead.pu8Data = tmpBuf;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&stParamRead);

                copy_to_user(RxBuf_bak, tmpBuf, DataSize);
                stParamRead.pu8Data = RxBuf_bak;
                copy_to_user(&stParamRead,pArgs,sizeof(MSPI_READ));
                kfree(tmpBuf);
            }
            break;
        case MDrv_CMD_MSPI_Write:
            {
                MSPI_WRITE stParamWrite;
                MS_U8 *tmpBuf = NULL;
                MS_U32 DataSize = 0;
                copy_from_user(&stParamWrite,pArgs,sizeof(MSPI_WRITE));
                DataSize = stParamWrite.u16Size;
                tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                //allocate kernel space buffer
                if(tmpBuf == NULL)
                {
                    return 1;  // 1: fail....
                }
                copy_from_user(tmpBuf, stParamWrite.pu8Data, DataSize);
                stParamWrite.pu8Data = tmpBuf;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&stParamWrite);
                copy_to_user(&stParamWrite,pArgs,sizeof(MSPI_WRITE));
                kfree(tmpBuf);
            }
            break;
        case MDrv_CMD_MSPI_DCConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_DCCONFIG, spt_MSPI_DCCONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_CLKConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_CLKCONFIG, spt_MSPI_CLKCONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_FRAMEConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_FRAMECONFIG, spt_MSPI_FRAMECONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_SlaveEnable:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_SLAVE_ENABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_SetPowerState:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPISETPOWERSTATE, spt_MSPISETPOWERSTATE, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MSPI_DbgLEvel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_SETDBGLEVEL, spt_MSPI_SETDBGLEVEL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_Read:
            {
                MASTERSPI_READ stParamRead;
                MS_U8 *tmpBuf = NULL;
                MS_U8 *RxBuf_bak = NULL;
                MS_U32 DataSize = 0;
                copy_from_user(&stParamRead,pArgs,sizeof(MASTERSPI_READ));
                DataSize = stParamRead.u16Size;
                tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                //allocate kernel space buffer
                if(tmpBuf == NULL)
                {
                    return 1;  // 1: fail....
                }
                //backup user data address
                RxBuf_bak = stParamRead.pu8Data;
                stParamRead.pu8Data = tmpBuf;

                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&stParamRead);

                copy_to_user(RxBuf_bak, tmpBuf, DataSize);
                stParamRead.pu8Data = RxBuf_bak;
                copy_to_user(&stParamRead,pArgs,sizeof(MASTERSPI_READ));
                kfree(tmpBuf);
            }
            break;
        case MDrv_CMD_MasterSPI_Write:
            {
                MASTERSPI_WRITE stParamWrite;
                MS_U8 *tmpBuf = NULL;
                MS_U32 DataSize = 0;
                copy_from_user(&stParamWrite,pArgs,sizeof(MASTERSPI_WRITE));
                DataSize = stParamWrite.u16Size;
                tmpBuf = kmalloc(DataSize, GFP_KERNEL);
                //allocate kernel space buffer
                if(tmpBuf == NULL)
                {
                    return 1;  // 1: fail....
                }
                copy_from_user(tmpBuf, stParamWrite.pu8Data, DataSize);
                stParamWrite.pu8Data = tmpBuf;
                u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&stParamWrite);
                copy_to_user(&stParamWrite,pArgs,sizeof(MASTERSPI_WRITE));
                kfree(tmpBuf);
            }
            break;

        case MDrv_CMD_MasterSPI_DCConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MASTERSPI_DCCONFIG, spt_MASTERSPI_DCCONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_CLKConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_CLKCONFIG, spt_MSPI_CLKCONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_FRAMEConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MSPI_FRAMECONFIG, spt_MSPI_FRAMECONFIG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_SlaveEnable:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_MASTERSPI_SLAVE_ENABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_CSPadConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_MASTERSPI_CSPADCONFIG, spt_MASTERSPI_CSPADCONFIG,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_MasterSPI_MaxClkConfig:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_MASTERSPI_MAXCLKCONFIG, spt_MASTERSPI_MAXCLKCONFIG,buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;
    };

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

