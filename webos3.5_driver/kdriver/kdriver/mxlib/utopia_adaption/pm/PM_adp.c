#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvPM.h"
#include "drvPM_v2.h"
#include "PM_adp.h"
#include "utopia_adp.h"


#include <linux/uaccess.h>

// no pointer member
UADP_SPT_0NXT_DEF(PM_RtcParam);

UADP_SPT_0NXT_DEF(PM_PRIVATE_PARAM_SetSPIOffsetForMCU);
UADP_SPT_0NXT_DEF(PM_PRIVATE_PARAM_SetDRAMOffsetForMCU);
UADP_SPT_0NXT_DEF(PM_PRIVATE_PARAM_Check_Version);
UADP_SPT_0NXT_DEF(PM_PRIVATE_PARAM_RTCSystemTime);



// one pointer member
UADP_SPT_1NXT_DEF(PM_PRIVATE_PARAM_RtcInit);
UADP_SPT_1NXT_DEF(PM_PRIVATE_PARAM_RtcSetCounter);
UADP_SPT_1NXT_DEF(PM_PRIVATE_PARAM_RtcGetCounter);
UADP_SPT_1NXT_DEF(PM_PRIVATE_PARAM_RtcSetMatchCounter);
UADP_SPT_1NXT_DEF(PM_PRIVATE_PARAM_RtcGetMatchCounter);


// two pointer member


MS_U32 PM_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct

//set table
    UADP_SPT_0NXT(PM_PRIVATE_PARAM_SetSPIOffsetForMCU);
    UADP_SPT_0NXT(PM_PRIVATE_PARAM_SetDRAMOffsetForMCU);
    UADP_SPT_0NXT(PM_PRIVATE_PARAM_Check_Version);
    UADP_SPT_0NXT(PM_PRIVATE_PARAM_RTCSystemTime);

    UADP_SPT_1NXT(PM_PRIVATE_PARAM_RtcInit, pPmRtcParam, PM_RtcParam);
    UADP_SPT_1NXT(PM_PRIVATE_PARAM_RtcSetCounter, pPmRtcParam, PM_RtcParam);
    UADP_SPT_1NXT(PM_PRIVATE_PARAM_RtcGetCounter, pPmRtcParam, PM_RtcParam);
    UADP_SPT_1NXT(PM_PRIVATE_PARAM_RtcSetMatchCounter, pPmRtcParam, PM_RtcParam);
    UADP_SPT_1NXT(PM_PRIVATE_PARAM_RtcGetMatchCounter, pPmRtcParam, PM_RtcParam);

    *pIoctl= (FUtopiaIOctl)PM_adp_Ioctl;
    return 0;
}

MS_U32 PM_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MDrv_CMD_PM_RtcInit:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RtcInit, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcSetCounter:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RtcSetCounter, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcGetCounter:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RtcGetCounter, spt_PM_PRIVATE_PARAM_RtcGetCounter,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcSetMatchCounter:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RtcSetMatchCounter, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcGetMatchCounter:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RtcGetMatchCounter, spt_PM_PRIVATE_PARAM_RtcGetMatchCounter,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcSetSystemTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RTCSystemTime, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcGetSystemTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_PM_PRIVATE_PARAM_RTCSystemTime,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_RtcSetMatchTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_RTCSystemTime, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_SetSPIOffsetForMCU:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_SetSPIOffsetForMCU, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_SetSRAMOffsetForMCU:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_SetDRAMOffsetForMCU:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_SetDRAMOffsetForMCU, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_GetWakeupSource:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_GetWakeupKey:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_Disable51:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_Check_Version:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PM_PRIVATE_PARAM_Check_Version, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PM_GetSRAMSize:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_GetIRPowerOnKey:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_PM_LoadWakeUp51:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        default:
            break;
    }
	return u32Ret;
}

