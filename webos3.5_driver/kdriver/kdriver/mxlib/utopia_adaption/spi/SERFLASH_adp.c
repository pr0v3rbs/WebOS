#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>




#include "MsTypes.h"
#include "utopia.h"
#include "drvSERFLASH.h"
#include "drvSERFLASH_v2.h"
#include "SERFLASH_adp.h"


#include "utopia_adp.h"
//#include "MsOS.h"

//UADP_SPT_0NXT_DEF(MS_U8);
//UADP_SPT_0NXT_DEF(MS_U32);
extern void* spt_MS_U32;
extern void* spt_MS_U8;


UADP_SPT_0NXT_DEF(SERFLASH_DrvStatus);
UADP_SPT_0NXT_DEF(SERFLASH_ADDRERASE);
UADP_SPT_0NXT_DEF(SERFLASH_BLKERASE);
UADP_SPT_0NXT_DEF(SERFLASH_SECERASE);
UADP_SPT_0NXT_DEF(SERFLASH_DMA);
UADP_SPT_0NXT_DEF(SERFLASH_WP);
UADP_SPT_0NXT_DEF(SERFLASH_CHIPSELECT);
UADP_SPT_0NXT_DEF(SERFLASH_WPDISABLERANGESET);
UADP_SPT_0NXT_DEF(SERFLASH_WPAREA);
UADP_SPT_0NXT_DEF(SERFLASH_WRITESTATUSREG);
UADP_SPT_0NXT_DEF(SERFLASH_COPYHND);
UADP_SPT_0NXT_DEF(SERFLASH_SETGPIO);
UADP_SPT_0NXT_DEF(SERFLASH_CLKDIV);
UADP_SPT_0NXT_DEF(SERFLASH_SETMODE);
UADP_SPT_0NXT_DEF(SERFLASH_SETCKG);
UADP_SPT_0NXT_DEF(SERFLASH_SET2XREAD);
UADP_SPT_0NXT_DEF(SERFLASH_SETDBGLEVEL);
UADP_SPT_0NXT_DEF(SERFLASH_SETWPINFO);


UADP_SPT_1NXT_DEF(SERFLASH_READID);
UADP_SPT_1NXT_DEF(SERFLASH_READ);
UADP_SPT_1NXT_DEF(SERFLASH_ADDRTOBLK);
UADP_SPT_1NXT_DEF(SERFLASH_BLKTOADDR);
UADP_SPT_1NXT_DEF(SERFLASH_WRITE);
UADP_SPT_1NXT_DEF(SERFLASH_READSTATUSREG);
UADP_SPT_1NXT_DEF(SERFLASH_GETSTATUS);
UADP_SPT_1NXT_DEF(SERFLASH_DETECTSIZE);

MS_U32 SERFLASH_adp_Init(FUtopiaIOctl* pIoctl)
{
    //member of struct 
    UADP_SPT_1NXT(SERFLASH_READID, pu8FlashID, MS_U8);
    UADP_SPT_1NXT(SERFLASH_READ, user_buffer, MS_U8);
    UADP_SPT_1NXT(SERFLASH_ADDRTOBLK, pu32BlockIndex, MS_U32);
    UADP_SPT_1NXT(SERFLASH_BLKTOADDR, pu32FlashAddr, MS_U32);
    UADP_SPT_1NXT(SERFLASH_WRITE, user_buffer, MS_U8);
    UADP_SPT_1NXT(SERFLASH_READSTATUSREG, pu8StatusReg, MS_U8);
    UADP_SPT_1NXT(SERFLASH_GETSTATUS, pDrvStatus, SERFLASH_DrvStatus);
    UADP_SPT_1NXT(SERFLASH_DETECTSIZE, u32FlashSize, MS_U32);

    UADP_SPT_0NXT(SERFLASH_ADDRERASE);
    UADP_SPT_0NXT(SERFLASH_BLKERASE);
    UADP_SPT_0NXT(SERFLASH_SECERASE);
    UADP_SPT_0NXT(SERFLASH_DMA);
    UADP_SPT_0NXT(SERFLASH_WP);
    UADP_SPT_0NXT(SERFLASH_CHIPSELECT);
    UADP_SPT_0NXT(SERFLASH_WPDISABLERANGESET);
    UADP_SPT_0NXT(SERFLASH_WPAREA);
    UADP_SPT_0NXT(SERFLASH_WRITESTATUSREG);
    UADP_SPT_0NXT(SERFLASH_COPYHND);
    UADP_SPT_0NXT(SERFLASH_SETGPIO);
    UADP_SPT_0NXT(SERFLASH_CLKDIV);
    UADP_SPT_0NXT(SERFLASH_SETMODE);
    UADP_SPT_0NXT(SERFLASH_SETCKG);
    UADP_SPT_0NXT(SERFLASH_SET2XREAD);
    UADP_SPT_0NXT(SERFLASH_SETDBGLEVEL);
    UADP_SPT_0NXT(SERFLASH_SETWPINFO);

    *pIoctl= (FUtopiaIOctl)SERFLASH_adp_Ioctl;
    return 0;
}


MS_U32 SERFLASH_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1024];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_SERFLASH_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_GETSTATUS, spt_SERFLASH_GETSTATUS, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SetDbgLevel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SETDBGLEVEL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SetWPInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SETWPINFO, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_DetectType:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_DetectSize:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_DETECTSIZE, spt_SERFLASH_DETECTSIZE, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_Set2XRead:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SET2XREAD, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SetCKG:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SETCKG, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_ClkDiv:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_CLKDIV, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SetMode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SETMODE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_ReadUID:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_ChipSelect:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_CHIPSELECT, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_Init:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_ReadID:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_READID, spt_SERFLASH_READID, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_Read:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_READ, spt_SERFLASH_READ, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_EraseChip:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_AddressToBlock:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_SERFLASH_ADDRTOBLK, spt_SERFLASH_ADDRTOBLK,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_BlockToAddress:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_SERFLASH_BLKTOADDR, spt_SERFLASH_BLKTOADDR,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_AddressErase:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_ADDRERASE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_BlockErase:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_BLKERASE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SectorErase:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SECERASE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_CheckWriteDone:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_Write:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_WRITE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_DMA:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_DMA, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_WriteProtect:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_WP, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_WriteProtect_Enable_All_Range:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_WriteProtect_Disable_All_Range:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_SERFLASH_WriteProtect_Disable_Range_Set:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_WPDISABLERANGESET, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_WriteProtect_Area:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_WPAREA, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_ReadStatusRegister:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_READSTATUSREG, spt_SERFLASH_READSTATUSREG, buffer_arg,sizeof(buffer_arg));
            break;          
        case MDrv_CMD_SERFLASH_ReadStatusRegister2:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_READSTATUSREG, spt_SERFLASH_READSTATUSREG, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_WriteStatusRegister:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_WRITESTATUSREG, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_CopyHnd:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_COPYHND, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SERFLASH_SetGPIO:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SERFLASH_SETGPIO, NULL, buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;
    };

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




