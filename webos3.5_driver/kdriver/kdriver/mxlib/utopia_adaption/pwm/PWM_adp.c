#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvPWM.h"
#include "drvPWM_v2.h"
#include "PWM_adp.h"
#include "utopia_adp.h"


#include <linux/uaccess.h>

// no pointer member
UADP_SPT_0NXT_DEF(PWM_PARAM_Oen);
UADP_SPT_0NXT_DEF(PWM_PARAM_Period);
UADP_SPT_0NXT_DEF(PWM_PARAM_DutyCycle);
UADP_SPT_0NXT_DEF(PWM_PARAM_UnitDiv);
UADP_SPT_0NXT_DEF(PWM_PARAM_Div);
UADP_SPT_0NXT_DEF(PWM_PARAM_Polarity);
UADP_SPT_0NXT_DEF(PWM_PARAM_Vdben);
UADP_SPT_0NXT_DEF(PWM_PARAM_ResetEn);
UADP_SPT_0NXT_DEF(PWM_PARAM_Dben);
UADP_SPT_0NXT_DEF(PWM_PARAM_Shift);
UADP_SPT_0NXT_DEF(PWM_PARAM_Nvsync);


// one pointer member

// two pointer member


MS_U32 PWM_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct

//set table
    UADP_SPT_0NXT(PWM_PARAM_Oen);
    UADP_SPT_0NXT(PWM_PARAM_Period);
    UADP_SPT_0NXT(PWM_PARAM_DutyCycle);
    UADP_SPT_0NXT(PWM_PARAM_UnitDiv);
    UADP_SPT_0NXT(PWM_PARAM_Div);
    UADP_SPT_0NXT(PWM_PARAM_Polarity);
    UADP_SPT_0NXT(PWM_PARAM_Vdben);
    UADP_SPT_0NXT(PWM_PARAM_ResetEn);
    UADP_SPT_0NXT(PWM_PARAM_Dben);
    UADP_SPT_0NXT(PWM_PARAM_Shift);
    UADP_SPT_0NXT(PWM_PARAM_Nvsync);

    *pIoctl= (FUtopiaIOctl)PWM_adp_Ioctl;
    return 0;
}

MS_U32 PWM_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MDrv_CMD_PWM_Oen:
	    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Oen, spt_PWM_PARAM_Oen,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Period:
	    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Period, spt_PWM_PARAM_Period,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_DutyCycle:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_DutyCycle, spt_PWM_PARAM_DutyCycle,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_UnitDiv:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_UnitDiv, spt_PWM_PARAM_UnitDiv,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Div:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Div, spt_PWM_PARAM_Div,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Polarity:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Polarity, spt_PWM_PARAM_Polarity,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Vdben:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Vdben, spt_PWM_PARAM_Vdben,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_ResetEn:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_ResetEn, spt_PWM_PARAM_ResetEn,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Dben:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Dben, spt_PWM_PARAM_Dben,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Shift:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Shift, spt_PWM_PARAM_Shift,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_PWM_Nvsync:
	        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PWM_PARAM_Nvsync, spt_PWM_PARAM_Nvsync,buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;
    }
	return u32Ret;
}

