#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvWDT.h"
#include "drvWDT_v2.h"
#include "WDT_adp.h"
#include "utopia_adp.h"


#include <linux/uaccess.h>

// no pointer member
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_Init);
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_Stop);
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_SetTimer);
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_SetTimer_ms);
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_SetTimer_us);
UADP_SPT_0NXT_DEF(WDT_PRIVATE_PARAM_SetIntTimer);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_Count);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_INT);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_Rst);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_SetMaxMatch);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_HitMaxMatch);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_GetMaxMatch);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_GetCounter);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_GetSecond);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_Delay);
UADP_SPT_0NXT_DEF(TIMER_PRIVATE_PARAM_DelayMs);


// one pointer member

// two pointer member


MS_U32 WDT_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct

//set table
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_Init);
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_Stop);
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_SetTimer);
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_SetTimer_ms);
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_SetTimer_us);
    UADP_SPT_0NXT(WDT_PRIVATE_PARAM_SetIntTimer);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_Count);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_INT);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_Rst);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_SetMaxMatch);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_HitMaxMatch);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_GetMaxMatch);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_GetCounter);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_GetSecond);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_Delay);
    UADP_SPT_0NXT(TIMER_PRIVATE_PARAM_DelayMs);

    *pIoctl= (FUtopiaIOctl)WDT_adp_Ioctl;
    return 0;
}

MS_U32 WDT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MDrv_CMD_WDT_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_Init, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_STOP:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_Stop, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_CLEAR:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_CLEAR_RST_FLAG:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_IS_RESET:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_IS_ENABLE:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_SETTIMER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_SetTimer, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SETTIMER_MS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_SetTimer_ms, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SETTIMER_US:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_SetTimer_us, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SETINTTIMER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_PRIVATE_PARAM_SetIntTimer, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_COUNT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_Count, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_INT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_INT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_RST:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_Rst, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_SETMAXMATCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_SetMaxMatch, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_HITMAXMATCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_HitMaxMatch, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GETMAXMATCH:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_GetMaxMatch, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GETCOUNTER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_GetCounter, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GETSECOND:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_GetSecond, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_DELAY:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_Delay, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_DELAYMS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_PRIVATE_PARAM_DelayMs, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;
    }
	return u32Ret;
}

