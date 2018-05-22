#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "GPD_adp.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "apiGPD.h"
#include "apiGPD_v2.h"

#include "MsDevice.h"


#ifdef MSOS_TYPE_LINUX_KERNEL

#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   copy_from_user
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     copy_to_user
#endif

#else
#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   memcpy
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     memcpy
#endif

#endif


// MApi_CMD_GPD_Init
UADP_SPT_NAMEnNXT_DEF(GPD_RET_VALUE,0);
UADP_SPT_NAMEnNXT_DEF(GPD_INIT,1);

// MApi_CMD_GPD_InputSource
//UADP_SPT_NAMEnNXT_DEF(GPD_FILE_NAME,0);               // filename is not used, no need to copy
UADP_SPT_NAMEnNXT_DEF(GPD_INPUT_SOURCE_PARAM,0);
UADP_SPT_NAMEnNXT_DEF(GPD_INPUT_SOURCE,1);

// MApi_CMD_GPD_OutputDecode
UADP_SPT_NAMEnNXT_DEF(GPD_OUTPUT_DECODE,0);

// MApi_CMD_GPD_OutputDecodeROI
UADP_SPT_NAMEnNXT_DEF(GPD_ROI_INFO,0);
UADP_SPT_NAMEnNXT_DEF(GPD_OUTPUT_DECODE_ROI,1);

// MApi_CMD_GPD_OutputDecodeMGIF
UADP_SPT_NAMEnNXT_DEF(GPD_OUTPUT_DECODE_MGIF,0);
// MApi_CMD_GPD_GetDuration
UADP_SPT_NAMEnNXT_DEF(GPD_GETDURATION,0);
// MApi_CMD_GPD_SetGIFMode
UADP_SPT_NAMEnNXT_DEF(GPD_SETGIFMODE,0);
// MApi_CMD_GPD_ScalingEnable
UADP_SPT_NAMEnNXT_DEF(GPD_SCALINGENABLE,0);
// MApi_CMD_GPD_ScalingDisable  ==> no i/o para

// MApi_CMD_GPD_SetControl
UADP_SPT_NAMEnNXT_DEF(GPD_SET_CONTROL,0);
// MApi_CMD_GPD_GetControl
UADP_SPT_NAMEnNXT_DEF(GPD_GET_PITCH,0);
UADP_SPT_NAMEnNXT_DEF(GPD_GET_CONTROL_PITCH,1);
// MApi_CMD_GPD_GetCRCResult
UADP_SPT_NAMEnNXT_DEF(GPD_DATA_POINTER,0);
UADP_SPT_NAMEnNXT_DEF(GPD_GET_CRC,1);

MS_U32 GPD_adp_Init(FUtopiaIOctl* pIoctl)
{

    // MApi_CMD_GPD_Init
    UADP_SPT_NAME0NXT(GPD_INIT,GPD_INIT_PARAM);


    // MApi_CMD_GPD_InputSource
    UADP_SPT_NAME0NXT(GPD_INPUT_SOURCE_PARAM,gpd_pic_info);
    UADP_SPT_NAME1NXT(GPD_INPUT_SOURCE,GPD_INPUTSOURCE_PARAM,param,GPD_INPUT_SOURCE_PARAM);

    // MApi_CMD_GPD_OutputDecode
    UADP_SPT_NAME0NXT(GPD_OUTPUT_DECODE,GPD_OUTPUTDECODE_PARAM);

    // MApi_CMD_GPD_OutputDecodeROI
    UADP_SPT_NAME0NXT(GPD_ROI_INFO, gpd_roi_info);
    UADP_SPT_NAME1NXT(GPD_OUTPUT_DECODE_ROI,GPD_OUTPUTDECODEROI_PARAM, roi, GPD_ROI_INFO);

    // MApi_CMD_GPD_OutputDecodeMGIF
    UADP_SPT_NAME0NXT(GPD_OUTPUT_DECODE_MGIF,GPD_OUTPUTDECODE_PARAM);

    // MApi_CMD_GPD_GetDuration
    UADP_SPT_NAME0NXT(GPD_GETDURATION,GPD_GETDURATION_PARAM);

    // MApi_CMD_GPD_SetGIFMode
    UADP_SPT_NAME0NXT(GPD_SETGIFMODE,GPD_SETGIFMODE_PARAM);

    // MApi_CMD_GPD_ScalingEnable
    UADP_SPT_NAME0NXT(GPD_SCALINGENABLE,GPD_SCALINGENABLE_PARAM);

    // MApi_CMD_GPD_ScalingDisable
    // no i/o para

    // MApi_CMD_GPD_SetControl
    UADP_SPT_NAME0NXT(GPD_SET_CONTROL,GPD_CONTROL_PARAM);

    // MApi_CMD_GPD_GetControl
    UADP_SPT_NAME0NXT(GPD_GET_PITCH,gpd_get_pitch_info);
    UADP_SPT_NAME1NXT(GPD_GET_CONTROL_PITCH,GPD_GET_CONTROL_PARAM, param, GPD_GET_PITCH);

    // MApi_CMD_GPD_GetCRCResult
    UADP_SPT_NAME0NXT(GPD_DATA_POINTER,U32);
    UADP_SPT_NAME1NXT(GPD_GET_CRC,GPD_GETCRCRESULT_PARAM, pu8Data, GPD_DATA_POINTER);


	*pIoctl= (FUtopiaIOctl)GPD_adp_Ioctl;

    return 0;
}


MS_U32 GPD_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
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
        case MApi_CMD_GPD_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_INIT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_InputSource:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_INPUT_SOURCE, spt_GPD_INPUT_SOURCE,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_OutputDecode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_OUTPUT_DECODE, spt_GPD_OUTPUT_DECODE,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_OutputDecodeROI:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_OUTPUT_DECODE_ROI, spt_GPD_OUTPUT_DECODE_ROI,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_OutputDecodeMGIF:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_OUTPUT_DECODE_MGIF, spt_GPD_OUTPUT_DECODE_MGIF,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_GetDuration:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_GETDURATION, spt_GPD_GETDURATION,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_SetGIFMode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_SETGIFMODE, spt_GPD_SETGIFMODE,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_ScalingEnable:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_OUTPUT_DECODE_ROI, spt_GPD_OUTPUT_DECODE_ROI,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_ScalingDisable:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_GPD_SetControl:
            {
                GPD_CONTROL_PARAM gpdCtrlParam;
                CPY_FROM_USER((void*)(&gpdCtrlParam), (GPD_CONTROL_PARAM*)pArgs, sizeof(GPD_CONTROL_PARAM));
                switch(gpdCtrlParam.cmd_id)
                {
                    case E_GPD_USER_CMD_SET_ACCESS_REGION:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_SET_CONTROL, spt_GPD_SET_CONTROL,buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GPD_USER_CMD_SET_CACHEABLE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_SET_CONTROL, spt_GPD_SET_CONTROL,buffer_arg,sizeof(buffer_arg));
                        break;
                }
            }
            break;
        case MApi_CMD_GPD_GetControl:
            {
                GPD_GET_CONTROL_PARAM gpdGetCtrlParam;
                CPY_FROM_USER((void*)(&gpdGetCtrlParam), (GPD_GET_CONTROL_PARAM*)pArgs, sizeof(GPD_GET_CONTROL_PARAM));
                switch(gpdGetCtrlParam.cmd_id)
                {
                    case E_GPD_GET_CMD_GET_PITCH:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_GET_CONTROL_PITCH, spt_GPD_GET_CONTROL_PITCH,buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;

                }
            }
            break;
        case MApi_CMD_GPD_GetCRCResult:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPD_GET_CRC, spt_GPD_GET_CRC,buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;
    }
    return u32Ret;
}




