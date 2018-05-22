#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvMIU.h"
#include "drvMIU_v2.h"
#include "MIU_adp.h"
#include "utopia_adp.h"

#include <linux/uaccess.h>


// no pointer member
//UADP_SPT_0NXT_DEF(MS_U32);
extern void* spt_MS_U32;
UADP_SPT_0NXT_DEF(MIU_RET);
UADP_SPT_0NXT_DEF(MIU_MIU_MASK);
UADP_SPT_0NXT_DEF(MIU_MIUREQ_MASK);
UADP_SPT_0NXT_DEF(MIU_SET_SSC);
UADP_SPT_0NXT_DEF(MIU_SET_SSCVALUE);
UADP_SPT_0NXT_DEF(MIU_PROYECT_ID);
UADP_SPT_0NXT_DEF(MIU_SELMIU);
UADP_SPT_0NXT_DEF(MIU_PortectInfo);
UADP_SPT_0NXT_DEF(MIU_GroupPriority);
UADP_SPT_0NXT_DEF(MIU_DRAM_SIZE);
UADP_SPT_0NXT_DEF(MIU_SETGROUPPRIORITY);
UADP_SPT_0NXT_DEF(MIU_SETHIGHPRIORITY);

// one pointer member
//UADP_SPT_1NXT_DEF(MIU_GET_LOADING);
UADP_SPT_1NXT_DEF(MIU_PROTECT);
UADP_SPT_1NXT_DEF(MIU_GETPROTECTINFO);
UADP_SPT_1NXT_DEF(MIU_PROTECT_ALIGN);

MS_U32 MIU_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct 
//set table
    UADP_SPT_0NXT(MIU_RET);
    UADP_SPT_0NXT(MIU_MIU_MASK);
    UADP_SPT_0NXT(MIU_MIUREQ_MASK);
    UADP_SPT_0NXT(MIU_SET_SSC);
    UADP_SPT_0NXT(MIU_SET_SSCVALUE);
    UADP_SPT_0NXT(MIU_PROYECT_ID);
    UADP_SPT_0NXT(MIU_SELMIU);
    UADP_SPT_0NXT(MIU_PortectInfo);
    UADP_SPT_0NXT(MIU_GroupPriority);
    UADP_SPT_0NXT(MIU_DRAM_SIZE);
    UADP_SPT_0NXT(MIU_PROTECT_ALIGN);
    UADP_SPT_0NXT(MIU_SETGROUPPRIORITY);
    UADP_SPT_0NXT(MIU_SETHIGHPRIORITY);

    //UADP_SPT_1NXT(MIU_GET_LOADING, u32Loading, MS_U32);
    UADP_SPT_1NXT(MIU_PROTECT, pu8ProtectId, MIU_PROYECT_ID);
    UADP_SPT_1NXT(MIU_GETPROTECTINFO, pInfo, MIU_PortectInfo);
    UADP_SPT_1NXT(MIU_PROTECT_ALIGN, u32PageShift, MS_U32);
     
    *pIoctl= (FUtopiaIOctl)MIU_adp_Ioctl;
    return 0;	
}

MS_U32 MIU_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_MIU_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_MIU_RET, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_OPM_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_W:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_SC_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVOP_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_W:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_AUDIO_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req:        
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIUREQ_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_UnMask_Req:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIUREQ_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_GetLoading:
            {
                 MIU_GET_LOADING MiuLoading;
                 MS_U32  u32tempLoading = 0;
                 MS_U32  *pBuff_User = 0;
                 
                 if(0 != copy_from_user(&MiuLoading, (void __user*)pArgs, sizeof(MIU_GET_LOADING)))
                 {
                     //printf("%s(%d) hank here\n",__FUNCTION__,__LINE__);
                     return -EACCES;
                 }
                 
                 pBuff_User = MiuLoading.u32Loading;

                 MiuLoading.u32Loading = (MS_U32 *) &u32tempLoading;
    
                 u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, &MiuLoading);

                 if (u32Ret == UTOPIA_STATUS_SUCCESS)
                 {
                    copy_to_user(pBuff_User, &u32tempLoading, sizeof(MS_U32));
                 }
                 else
                 {
                     //printf("[Error] %s(%d) cmd:%d ===> ioctl Error\n",__FUNCTION__,__LINE__,u32Cmd);
                 }        
            
            //u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_GET_LOADING, spt_MIU_GET_LOADING, buffer_arg, sizeof(buffer_arg));
            }
            break;    
        case MDrv_CMD_MIU_SetSsc:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SET_SSC, spt_MIU_SET_SSC, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetSscValue:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SET_SSCVALUE, spt_MIU_SET_SSCVALUE, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Protect:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT, spt_MIU_PROTECT, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SelMIU:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SELMIU, spt_MIU_SELMIU, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_GetProtectInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_GETPROTECTINFO, spt_MIU_GETPROTECTINFO, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetGroupPriority:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SETGROUPPRIORITY, spt_MIU_SETGROUPPRIORITY, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetHighPriority:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SETHIGHPRIORITY, spt_MIU_SETHIGHPRIORITY, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_PrintProtectInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Dram_Size:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_DRAM_SIZE, spt_MIU_DRAM_SIZE, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_ProtectAlign:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT_ALIGN, spt_MIU_PROTECT_ALIGN, buffer_arg, sizeof(buffer_arg));
            break;
        default:
            break;
    }
	return u32Ret;
}

