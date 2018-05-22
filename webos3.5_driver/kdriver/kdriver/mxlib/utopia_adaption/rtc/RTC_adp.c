#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvRTC.h"
#include "drvRTC_v2.h"
#include "RTC_adp.h"
#include "utopia_adp.h"


#include <linux/uaccess.h>

// no pointer member
UADP_SPT_0NXT_DEF(RTC_INIT_PARAM);
UADP_SPT_0NXT_DEF(RTC_GETCOUNT_PARAM);
UADP_SPT_0NXT_DEF(RTC_DEINIT_PARAM);
// one pointer member

// two pointer member


MS_U32 RTC_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct

//set table
    UADP_SPT_0NXT(RTC_INIT_PARAM);
    UADP_SPT_0NXT(RTC_GETCOUNT_PARAM);
    UADP_SPT_0NXT(RTC_DEINIT_PARAM);

    *pIoctl= (FUtopiaIOctl)RTC_adp_Ioctl;
    return 0;
}

MS_U32 RTC_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
    	case MDrv_CMD_RTC_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_RTC_INIT_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_RTC_Get_Count:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_RTC_GETCOUNT_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_RTC_DeInit:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_RTC_DEINIT_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
    	    break;
        default:
            break;
    }
	return u32Ret;
}

