#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvBDMA.h"
#include "drvBDMA_v2.h"
#include "BDMA_adp.h"
#include "utopia_adp.h"

// no pointer member
//UADP_SPT_0NXT_DEF(MS_U32);
extern void* spt_MS_U32;
UADP_SPT_0NXT_DEF(BDMA_CRC32_PARAM);
UADP_SPT_0NXT_DEF(BDMA_PATTERN_FILL_PARAM);
UADP_SPT_0NXT_DEF(BDMA_MEMCOPY_PARAM);
UADP_SPT_0NXT_DEF(BDMA_FLASHCOPY2DRAM_PARAM);
UADP_SPT_0NXT_DEF(BDMA_Status);
UADP_SPT_0NXT_DEF(BDMA_MOBF_PS);

// one pointer member
UADP_SPT_1NXT_DEF(BDMA_GETSTATUS_PARAM);
UADP_SPT_1NXT_DEF(BDMA_SEARCH_PARAM);
#ifdef MOBF_ENABLE
UADP_SPT_1NXT_DEF(BDMA_MOBFSEARCH_PARAM);
#endif
MS_U32 BDMA_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
//set table
    UADP_SPT_0NXT(BDMA_CRC32_PARAM);
    UADP_SPT_0NXT(BDMA_PATTERN_FILL_PARAM);
    UADP_SPT_0NXT(BDMA_MEMCOPY_PARAM);
    UADP_SPT_0NXT(BDMA_FLASHCOPY2DRAM_PARAM);

    UADP_SPT_1NXT(BDMA_GETSTATUS_PARAM, pStatus, BDMA_Status);
    UADP_SPT_1NXT(BDMA_SEARCH_PARAM, pSearchAddr, MS_U32);
	#ifdef MOBF_ENABLE
    UADP_SPT_1NXT(BDMA_MOBFSEARCH_PARAM, pMobfPsCfg, BDMA_MOBF_PS);
	#endif

//set table


    *pIoctl= (FUtopiaIOctl)BDMA_adp_Ioctl;
    return 0;
}

MS_U32 BDMA_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MDrv_CMD_BDMA_MemCopy:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_MEMCOPY_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_Search:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_BDMA_SEARCH_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_CRC32:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_CRC32_PARAM, spt_BDMA_CRC32_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_Pattern_Fill:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_PATTERN_FILL_PARAM, spt_BDMA_PATTERN_FILL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_FlashCopy2Dram:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_FLASHCOPY2DRAM_PARAM, NULL, buffer_arg,sizeof(buffer_arg));
            break;
		#ifdef MOBF_ENABLE
        case MDrv_CMD_BDMA_MOBFSearch:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_MOBFSEARCH_PARAM, spt_BDMA_MOBFSEARCH_PARAM, buffer_arg,sizeof(buffer_arg));
            break;
		#endif
        case MDrv_CMD_BDMA_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_BDMA_GETSTATUS_PARAM, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_GetInfo:
            break;
        case MDrv_CMD_BDMA_GetLibVer:
            break;
        case MDrv_CMD_BDMA_GetMinSize:
            break;
        case MDrv_CMD_BDMA_SetDbgLevel:
            break;
        case MDrv_CMD_BDMA_SetSPIOffsetForMCU:
        case MDrv_CMD_BDMA_WaitFlashDone:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        default:
            break;
    }
	return u32Ret;
}

