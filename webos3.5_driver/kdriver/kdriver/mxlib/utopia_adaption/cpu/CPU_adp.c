#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>




#include "MsTypes.h"
#include "utopia.h"
#include "drvCPU.h"
#include "drvCPU_v2.h"
#include "CPU_adp.h"


#include "utopia_adp.h"
//#include "MsOS.h"

UADP_SPT_0NXT_DEF(COPRO_DrvStatus);

UADP_SPT_0NXT_DEF(CPU_ENABLE_PARAM);
UADP_SPT_0NXT_DEF(CPU_SETHOST_PARAM);
UADP_SPT_0NXT_DEF(CPU_INITEND_PARAM);
UADP_SPT_0NXT_DEF(CPU_SETDBGLEVEL_PARAM);
UADP_SPT_0NXT_DEF(CPU_GETINFO_PARAM);
UADP_SPT_1NXT_DEF(CPU_GETSTATUS_PARAM);

// for cmd MDrv_COPRO_GetInfo

MS_U32 CPU_adp_Init(FUtopiaIOctl* pIoctl)
{
    //member of struct 
    UADP_SPT_1NXT(CPU_GETSTATUS_PARAM, pStatus, COPRO_DrvStatus);

    UADP_SPT_0NXT(CPU_ENABLE_PARAM);
    UADP_SPT_0NXT(CPU_SETHOST_PARAM);
    UADP_SPT_0NXT(CPU_INITEND_PARAM);
    UADP_SPT_0NXT(CPU_SETDBGLEVEL_PARAM);
    UADP_SPT_0NXT(CPU_GETINFO_PARAM);
    UADP_SPT_0NXT(COPRO_DrvStatus);

    *pIoctl= (FUtopiaIOctl)CPU_adp_Ioctl;
    return 0;
}

MS_U32 CPU_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_CPU_InitFront:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_CPU_InitEnd:
           u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_INITEND_PARAM, NULL, buffer_arg,sizeof(buffer_arg));
           break;
        case MDrv_CMD_CPU_Disable:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_CPU_Enable:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_ENABLE_PARAM, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_CPU_SetHost:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_SETHOST_PARAM, NULL, buffer_arg,sizeof(buffer_arg));
            break;
       case MDrv_CMD_CPU_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_GETINFO_PARAM, spt_CPU_GETINFO_PARAM, buffer_arg,sizeof(buffer_arg));
            break;	
        case MDrv_CMD_CPU_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_GETSTATUS_PARAM, spt_CPU_GETSTATUS_PARAM, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_CPU_SetDbgLevel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_CPU_SETDBGLEVEL_PARAM, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_CPU_GetBase:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_CPU_QueryClock:
        {
            MS_U32 u32CpuClk = 0;
            
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,&u32CpuClk);
            copy_to_user((void *)pArgs, (void *)&u32CpuClk, sizeof(MS_U32));
            
        }
            break;
        default:
            break;
    };

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

