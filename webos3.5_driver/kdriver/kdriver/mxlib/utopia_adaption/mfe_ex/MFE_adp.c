#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "apiMFE_priv.h"
#include "apiMFE_v2.h"

#include "utopia_adp.h"
#include "MFE_adp.h"

#define MFE_OBUF_1080P_SIZE (2 * 1024 * 1024)

//Function parameter
UADP_STRUCT_POINTER_TABLE spt_MFE_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_ENCODE[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_GETVOL[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_COMPRESSONEPICTURE[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_GETOUTBUFFER[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_GETOUTPUTINFO[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_SETVUI[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_POWERSUSPEND[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_CLEAR_POWERSUSPEND[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_DEINIT[10];

//not verify
UADP_STRUCT_POINTER_TABLE spt_MFE_SET_COLORFORMAT[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_SET_BITRATE_FRAMERATE[10];


//Function parameter's pointer
//spt_MFE_INIT
UADP_STRUCT_POINTER_TABLE spt_MFE_PVR_Info[10];
UADP_STRUCT_POINTER_TABLE spt_MFE_BITSTREAM_BUFFER[10];
//spt_MFE_GETVOL
UADP_STRUCT_POINTER_TABLE spt_MFE_header_info[10];
//spt_MFE_GETOUTBUFFER
UADP_STRUCT_POINTER_TABLE spt_MFE_OUTBUF[10];
MS_U32 MFE_adp_Init(FUtopiaIOctl* pIoctl)
{
    //set table
    printk("MFE_adp_Init\n");

    UADP_SPT_BGN( &spt_MFE_INIT[0], sizeof(MFE_INIT_PARAM));
    UADP_SPT_NXT( &spt_MFE_INIT[1], MFE_INIT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_INIT[2]);

    UADP_SPT_BGN( &spt_MFE_ENCODE[0], sizeof(MFE_INIT_PARAM));
    UADP_SPT_NXT( &spt_MFE_ENCODE[1], MFE_INIT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_ENCODE[2]);

    UADP_SPT_BGN( &spt_MFE_DEINIT[0], sizeof(MFE_INIT_PARAM));
    UADP_SPT_NXT( &spt_MFE_DEINIT[1], MFE_INIT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_DEINIT[2]);

    UADP_SPT_BGN( &spt_MFE_GETVOL[0], sizeof(MFE_GETVOL_PARAM));
    UADP_SPT_NXT( &spt_MFE_GETVOL[1], MFE_GETVOL_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_GETVOL[2]);

    UADP_SPT_BGN( &spt_MFE_SET_COLORFORMAT[0], sizeof(MFE_INIT_PARAM));
    UADP_SPT_NXT( &spt_MFE_SET_COLORFORMAT[1], MFE_INIT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_SET_COLORFORMAT[2]);

    UADP_SPT_BGN( &spt_MFE_SET_BITRATE_FRAMERATE[0], sizeof(MFE_INIT_PARAM));
    UADP_SPT_NXT( &spt_MFE_SET_BITRATE_FRAMERATE[1], MFE_INIT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_SET_BITRATE_FRAMERATE[2]);

    UADP_SPT_BGN( &spt_MFE_COMPRESSONEPICTURE[0], sizeof(MFE_COMPRESSONEPICTURE_PARAM));
    UADP_SPT_NXT( &spt_MFE_COMPRESSONEPICTURE[1], MFE_COMPRESSONEPICTURE_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_COMPRESSONEPICTURE[2]);

    UADP_SPT_BGN( &spt_MFE_GETOUTBUFFER[0], sizeof(MFE_GET_OBUF_PARAM));
    UADP_SPT_NXT( &spt_MFE_GETOUTBUFFER[1], MFE_GET_OBUF_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_NXT( &spt_MFE_GETOUTBUFFER[2], MFE_GET_OBUF_PARAM, outbuf, spt_MFE_OUTBUF);
    UADP_SPT_FIN( &spt_MFE_GETOUTBUFFER[3]);

    UADP_SPT_BGN( &spt_MFE_GETOUTPUTINFO[0], sizeof(MFE_GETOUTPUT_PARAM));
    UADP_SPT_NXT( &spt_MFE_GETOUTPUTINFO[1], MFE_GETOUTPUT_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_GETOUTPUTINFO[2]);

    UADP_SPT_BGN( &spt_MFE_SETVUI[0], sizeof(MFE_SETVUI_PARAM));
    UADP_SPT_NXT( &spt_MFE_SETVUI[1], MFE_SETVUI_PARAM, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_SETVUI[2]);

    UADP_SPT_BGN( &spt_MFE_PVR_Info[0], sizeof(PVR_Info));
    UADP_SPT_FIN( &spt_MFE_PVR_Info[1]);

    UADP_SPT_BGN( &spt_MFE_POWERSUSPEND[0], sizeof(MFE_GETCOFG_POWERSUSPEND));
    UADP_SPT_NXT( &spt_MFE_POWERSUSPEND[1], MFE_GETCOFG_POWERSUSPEND, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_POWERSUSPEND[2]);

    UADP_SPT_BGN( &spt_MFE_CLEAR_POWERSUSPEND[0], sizeof(MFE_GETCOFG_POWERSUSPEND));
    UADP_SPT_NXT( &spt_MFE_CLEAR_POWERSUSPEND[1], MFE_GETCOFG_POWERSUSPEND, mfe_Info, spt_MFE_PVR_Info);
    UADP_SPT_FIN( &spt_MFE_CLEAR_POWERSUSPEND[2]);

    UADP_SPT_BGN(&spt_MFE_header_info[0], sizeof(SPSPPS_INFO_t));
    UADP_SPT_FIN(&spt_MFE_header_info[1]);

    UADP_SPT_BGN(&spt_MFE_OUTBUF[0], sizeof(MEMMAP_t));
    UADP_SPT_FIN(&spt_MFE_OUTBUF[1]);

    *pIoctl= (FUtopiaIOctl)MFE_adp_Ioctl;
    printk("MFE_adp_Init done\n");
    return 0;

}

MS_U32 MFE_adp_Ioctl(void *pInstanceTmp, MS_U32 u32Cmd, void *const pArgs)
{
    MS_U32 u32Ret = 0xFF;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }
	
    //printk("MFE_adp_Ioctl ctrl = 0x%x\n", (unsigned int)u32Cmd);
    switch(u32Cmd)
    {
        case MApi_CMD_MFE_Init:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_INIT, spt_MFE_INIT, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_Encode:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_ENCODE, spt_MFE_ENCODE, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_GetVOL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_GETVOL, spt_MFE_GETVOL, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_CompressOnePicture:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_COMPRESSONEPICTURE, spt_MFE_COMPRESSONEPICTURE, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_GetOutBuffer:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_GETOUTBUFFER, spt_MFE_GETOUTBUFFER, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_DeInit:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_DEINIT, spt_MFE_DEINIT, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_SetColorFormat:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_SET_COLORFORMAT, spt_MFE_SET_COLORFORMAT, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_SetBitrateFramerate:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_SET_BITRATE_FRAMERATE, spt_MFE_SET_BITRATE_FRAMERATE, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_GetOutputInfo:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_GETOUTPUTINFO, spt_MFE_GETOUTPUTINFO, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_SetVUI:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_SETVUI, spt_MFE_SETVUI, buffer_arg, sizeof(buffer_arg));
            break;
        case MApi_CMD_MFE_GETCONF_POWERSUSPEND:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_POWERSUSPEND, spt_MFE_POWERSUSPEND, buffer_arg, sizeof(buffer_arg));
        break;
        case MApi_CMD_MFE_SETCONF_CLEARPOWERSUSPEND:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MFE_POWERSUSPEND, spt_MFE_POWERSUSPEND, buffer_arg, sizeof(buffer_arg));
        break;

        default:
            break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

