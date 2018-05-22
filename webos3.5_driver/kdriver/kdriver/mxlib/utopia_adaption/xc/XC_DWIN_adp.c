#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "XC_DWIN_adp.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "apiXC_DWIN_v2.h"
#include "apiXC_DWIN.h"

//MDrv_CMD_DIP_INIT,
//MDrv_CMD_DIP_GET_RESOURCE,
//MDrv_CMD_DIP_RELEASE_RESOURCE,
//MDrv_CMD_DIP_QUERY_RESOURCE,
UADP_SPT_NAMEnNXT_DEF(DIP_RESOURCE,0);
//MDrv_CMD_DIP_SET_INPUTSOURCE,
//MDrv_CMD_DIP_SET_WINDOW,
UADP_SPT_NAMEnNXT_DEF(DIP_SET_WINDOW,0);
//MDrv_CMD_DIP_SET_WINPROPERTY,
UADP_SPT_NAMEnNXT_DEF(DIP_SET_WIN_PROPERTY,0);
//MDrv_CMD_DIP_SET_PROPERTY,
UADP_SPT_NAMEnNXT_DEF(DIP_SET_PROPERTY,0);
//MDrv_CMD_DIP_GET_BUFINFO,
UADP_SPT_NAMEnNXT_DEF(DIP_BUF_INFO,0);
//MDrv_CMD_DIP_INTERRUPT,
UADP_SPT_NAMEnNXT_DEF(DIP_INTER_STUS,0);
//MDrv_CMD_DIP_TRIGGER,
UADP_SPT_NAMEnNXT_DEF(DIP_TRIGGER_MODE,0);
//MDrv_CMD_DIP_SET_DIPR_PROPERTY,
UADP_SPT_NAMEnNXT_DEF(DIP_SET_DIPR_PROPERTY,0);


#define SPT(Name) spt_##Name


MS_U32 DIP_adp_Init(FUtopiaIOctl* pIoctl)
{
    
    //MDrv_CMD_DIP_INIT,
    //MDrv_CMD_DIP_GET_RESOURCE,
    //MDrv_CMD_DIP_RELEASE_RESOURCE,
    //MDrv_CMD_DIP_QUERY_RESOURCE,
    UADP_SPT_NAME0NXT(DIP_RESOURCE,DIP_RESOURCE);
	//MDrv_CMD_DIP_SET_INPUTSOURCE,
    //MDrv_CMD_DIP_SET_WINDOW,
    UADP_SPT_NAME0NXT(DIP_SET_WINDOW,DIP_SET_WINDOW);
    //MDrv_CMD_DIP_SET_WINPROPERTY,
    UADP_SPT_NAME0NXT(DIP_SET_WIN_PROPERTY,DIP_SET_WIN_PROPERTY);
    //MDrv_CMD_DIP_SET_PROPERTY,
    UADP_SPT_NAME0NXT(DIP_SET_PROPERTY,DIP_SET_PROPERTY);
    //MDrv_CMD_DIP_GET_BUFINFO,
    UADP_SPT_NAME0NXT(DIP_BUF_INFO,DIP_BUF_INFO);
    //MDrv_CMD_DIP_INTERRUPT,
    UADP_SPT_NAME0NXT(DIP_INTER_STUS,DIP_INTER_STUS);
    //MDrv_CMD_DIP_TRIGGER,
    UADP_SPT_NAME0NXT(DIP_TRIGGER_MODE,DIP_TRIGGER_MODE);
    //MDrv_CMD_DIP_SET_DIPR_PROPERTY,
    UADP_SPT_NAME0NXT(DIP_SET_DIPR_PROPERTY,DIP_SET_DIPR_PROPERTY);
    printk("\33[0;36m   %s:%d    \33[m \n",__FUNCTION__,__LINE__);
	*pIoctl= (FUtopiaIOctl)DIP_adp_Ioctl;
    return 0;
}

MS_U32 DIP_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

   // printk("\33[0;31m [Start][%s][%d]u32Cmd=%d \33[m;\n",__FUNCTION__,__LINE__,u32Cmd);
    switch(u32Cmd)
    {
        case MDrv_CMD_DIP_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_RESOURCE),SPT(DIP_RESOURCE), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_GET_RESOURCE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_RESOURCE),SPT(DIP_RESOURCE), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_RELEASE_RESOURCE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_RESOURCE),SPT(DIP_RESOURCE), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_QUERY_RESOURCE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_RESOURCE),SPT(DIP_RESOURCE), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_SET_INPUTSOURCE:

            break;
        case MDrv_CMD_DIP_SET_WINDOW:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_SET_WINDOW),SPT(DIP_SET_WINDOW), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_SET_WINPROPERTY:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_SET_WIN_PROPERTY),SPT(DIP_SET_WIN_PROPERTY), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_SET_PROPERTY:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_SET_PROPERTY),SPT(DIP_SET_PROPERTY), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_GET_BUFINFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_BUF_INFO),SPT(DIP_BUF_INFO), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_INTERRUPT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_INTER_STUS),SPT(DIP_INTER_STUS), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_TRIGGER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_TRIGGER_MODE),SPT(DIP_TRIGGER_MODE), buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DIP_SET_DIPR_PROPERTY:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(DIP_SET_DIPR_PROPERTY),SPT(DIP_SET_DIPR_PROPERTY), buffer_arg,sizeof(buffer_arg));
            break;
		default:
			break;

    }
   // printk("\33[0;34m [End][%s][%d]u32Cmd=%d \33[m;\n",__FUNCTION__,__LINE__,u32Cmd);


	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




