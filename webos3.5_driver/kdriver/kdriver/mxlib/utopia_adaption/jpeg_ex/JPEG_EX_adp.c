#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "JPEG_EX_adp.h"
#include "utopia.h"
#include "utopia_adp.h"

#include "apiJPEG.h"
#include "apiJPEG_v2.h"

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






UADP_SPT_NAMEnNXT_DEF(JPEG_BOOL,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_U8,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_U16,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_U32,0);



UADP_SPT_NAMEnNXT_DEF(JPEG_BOOL_PARAM,0);


UADP_SPT_NAMEnNXT_DEF(JPEG_InitParam,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_INIT_PARAM,1);


UADP_SPT_NAMEnNXT_DEF(JPEG_GETAPP0INFO_PARAM,3);

UADP_SPT_NAMEnNXT_DEF(JPEG_RESULT_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_GETERRORCODE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_JPEGEVENT_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_SETMBUFFERVALID_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_UPDATEREADINFO_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_PROCESSEOF_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_SETERRORCODE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_RESOLUTION_PARAM,0);


UADP_SPT_NAMEnNXT_DEF(JPEG_MSIF_Version,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_Info,3);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETINFO_PARAM,1);


UADP_SPT_NAMEnNXT_DEF(JPEG_Status,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETSTATUS_PARAM,1);

typedef struct
{
    MSIF_Version* pMSIF_Version;
}PMSIF_Version;
UADP_SPT_NAMEnNXT_DEF(JPEG_PMSIF_Version,1);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETLIBVER_PARAM,1);


UADP_SPT_NAMEnNXT_DEF(JPEG_BuffLoadType,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETBUFFLOADTYPE_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_WAITDONE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_EXIF_DateTime,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFDATETIME_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_EXIF_Orientation,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFORIENTATION_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFSTRING_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFU16_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFU32_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_RATIONAL,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETEXIFRATIONAL_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_GETFREEMEMORY_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_SUPPORTCMYK_PARAM,0);


typedef struct
{
    JPEG_MPO_INDEX_INFO* pMPOIndex;
}PJPEG_MPO_INDEX_INFO;

UADP_SPT_NAMEnNXT_DEF(JPEG_MPO_INDEX_INFO,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_PJPEG_MPO_INDEX_INFO,1);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETMPOINDEX_PARAM,1);

typedef struct
{
    JPEG_MPO_INDEX_INFO* pMPOIndex;
}PJPEG_MPO_ATTRIBUTE_INFO;

UADP_SPT_NAMEnNXT_DEF(JPEG_MPO_INDEX_INFO,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_PJPEG_MPO_ATTRIBUTE_INFO,1);
UADP_SPT_NAMEnNXT_DEF(JPEG_GETMPOATTR_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(JPEG_SETMPOBUFFER_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_VERIFICATION_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_GET_CTRL_PARAM,1);

typedef struct
{
    char info[12];
}JPEG_INFO_12BYTE;
UADP_SPT_NAMEnNXT_DEF(JPEG_EXIF_MANUFACTURER,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GET_EXIF_MANUFACTURER,1);

typedef struct
{
    char info[8];
}JPEG_INFO_8BYTE;
UADP_SPT_NAMEnNXT_DEF(JPEG_EXIF_SHUTTER_SPEED_VALUE,0);
UADP_SPT_NAMEnNXT_DEF(JPEG_GET_EXIF_SHUTTER_SPEED_VALUE,1);


UADP_SPT_NAMEnNXT_DEF(JPEG_CB_CTRL_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(JPEG_ENABLE_ISR,0);






MS_U32 JPEG_EX_adp_Init(FUtopiaIOctl* pIoctl)
{
    UADP_SPT_NAME0NXT(JPEG_BOOL,MS_BOOL);
    UADP_SPT_NAME0NXT(JPEG_U8,MS_U8);
    UADP_SPT_NAME0NXT(JPEG_U16,MS_U16);
    UADP_SPT_NAME0NXT(JPEG_U32,MS_U32);

    UADP_SPT_NAME0NXT(JPEG_BOOL_PARAM,JPEG_BOOL_PARAM);

    UADP_SPT_NAME0NXT(JPEG_InitParam,JPEG_InitParam);
    UADP_SPT_NAME1NXT(JPEG_INIT_PARAM,JPEG_INIT_PARAM,pInitParam,JPEG_InitParam);

    UADP_SPT_NAME3NXT(JPEG_GETAPP0INFO_PARAM,JPEG_GETAPP0INFO_PARAM,unit,JPEG_U8,x,JPEG_U16,y,JPEG_U16);

    UADP_SPT_NAME0NXT(JPEG_RESULT_PARAM,JPEG_RESULT_PARAM);

    UADP_SPT_NAME0NXT(JPEG_GETERRORCODE_PARAM,JPEG_GETERRORCODE_PARAM);

    UADP_SPT_NAME0NXT(JPEG_JPEGEVENT_PARAM,JPEG_JPEGEVENT_PARAM);

    UADP_SPT_NAME0NXT(JPEG_SETMBUFFERVALID_PARAM,JPEG_SETMBUFFERVALID_PARAM);

    UADP_SPT_NAME0NXT(JPEG_UPDATEREADINFO_PARAM,JPEG_UPDATEREADINFO_PARAM);

    UADP_SPT_NAME0NXT(JPEG_PROCESSEOF_PARAM,JPEG_PROCESSEOF_PARAM);

    UADP_SPT_NAME0NXT(JPEG_SETERRORCODE_PARAM,JPEG_SETERRORCODE_PARAM);

    UADP_SPT_NAME0NXT(JPEG_RESOLUTION_PARAM,JPEG_RESOLUTION_PARAM);

    UADP_SPT_NAME0NXT(JPEG_MSIF_Version,MSIF_Version);
    UADP_SPT_NAME3NXT(JPEG_Info,JPEG_Info,pu8DrvVer,JPEG_MSIF_Version,pu8HalVer,JPEG_U8,pu8FwVer,JPEG_U8);
    UADP_SPT_NAME1NXT(JPEG_GETINFO_PARAM,JPEG_GETINFO_PARAM,pJPEG_Info,JPEG_Info);

    UADP_SPT_NAME0NXT(JPEG_Status,JPEG_Status);
    UADP_SPT_NAME1NXT(JPEG_GETSTATUS_PARAM,JPEG_GETSTATUS_PARAM,pJPEG_Status,JPEG_Status);

    UADP_SPT_NAME1NXT(JPEG_PMSIF_Version,PMSIF_Version,pMSIF_Version,JPEG_MSIF_Version);
    UADP_SPT_NAME1NXT(JPEG_GETLIBVER_PARAM,JPEG_GETLIBVER_PARAM,ppVersion,JPEG_PMSIF_Version);

    UADP_SPT_NAME0NXT(JPEG_BuffLoadType,JPEG_BuffLoadType);
    UADP_SPT_NAME1NXT(JPEG_GETBUFFLOADTYPE_PARAM,JPEG_GETBUFFLOADTYPE_PARAM,pBuffLoadType,JPEG_BuffLoadType);

    UADP_SPT_NAME0NXT(JPEG_WAITDONE_PARAM,JPEG_WAITDONE_PARAM);

    UADP_SPT_NAME0NXT(JPEG_EXIF_DateTime,JPEG_EXIF_DateTime);
    UADP_SPT_NAME1NXT(JPEG_GETEXIFDATETIME_PARAM,JPEG_GETEXIFDATETIME_PARAM,DateTime,JPEG_EXIF_DateTime);

    UADP_SPT_NAME0NXT(JPEG_EXIF_Orientation,JPEG_EXIF_Orientation);
    UADP_SPT_NAME1NXT(JPEG_GETEXIFORIENTATION_PARAM,JPEG_GETEXIFORIENTATION_PARAM,eOrientation,JPEG_EXIF_Orientation);

    UADP_SPT_NAME1NXT(JPEG_GETEXIFSTRING_PARAM,JPEG_GETEXIFSTRING_PARAM,pu8String,JPEG_U8);

    UADP_SPT_NAME1NXT(JPEG_GETEXIFU16_PARAM,JPEG_GETEXIFU16_PARAM,pu16Value,JPEG_U16);

    UADP_SPT_NAME1NXT(JPEG_GETEXIFU32_PARAM,JPEG_GETEXIFU32_PARAM,pu32Value,JPEG_U32);

    UADP_SPT_NAME0NXT(JPEG_RATIONAL,JPEG_RATIONAL);
    UADP_SPT_NAME1NXT(JPEG_GETEXIFRATIONAL_PARAM,JPEG_GETEXIFRATIONAL_PARAM,pRational,JPEG_RATIONAL);

    UADP_SPT_NAME0NXT(JPEG_GETFREEMEMORY_PARAM,JPEG_GETFREEMEMORY_PARAM);

    UADP_SPT_NAME0NXT(JPEG_SUPPORTCMYK_PARAM,JPEG_SUPPORTCMYK_PARAM);

    UADP_SPT_NAME0NXT(JPEG_MPO_INDEX_INFO,JPEG_MPO_INDEX_INFO);
    UADP_SPT_NAME1NXT(JPEG_PJPEG_MPO_INDEX_INFO,PJPEG_MPO_INDEX_INFO,pMPOIndex,JPEG_MPO_INDEX_INFO);
    UADP_SPT_NAME1NXT(JPEG_GETMPOINDEX_PARAM,JPEG_GETMPOINDEX_PARAM,ppMPOIndex,JPEG_PJPEG_MPO_INDEX_INFO);

    UADP_SPT_NAME0NXT(JPEG_MPO_INDEX_INFO,JPEG_MPO_INDEX_INFO);
    UADP_SPT_NAME1NXT(JPEG_PJPEG_MPO_ATTRIBUTE_INFO,PJPEG_MPO_ATTRIBUTE_INFO,pMPOIndex,JPEG_MPO_INDEX_INFO);
    UADP_SPT_NAME1NXT(JPEG_GETMPOATTR_PARAM,JPEG_GETMPOATTR_PARAM,ppMPOAttr,JPEG_PJPEG_MPO_ATTRIBUTE_INFO);

    UADP_SPT_NAME0NXT(JPEG_SETMPOBUFFER_PARAM,JPEG_SETMPOBUFFER_PARAM);

    UADP_SPT_NAME0NXT(JPEG_VERIFICATION_PARAM,JPEG_VERIFICATION_PARAM);

    UADP_SPT_NAME1NXT(JPEG_GET_CTRL_PARAM,JPEG_GET_CTRL_PARAM,param,JPEG_U32);

    UADP_SPT_NAME0NXT(JPEG_EXIF_MANUFACTURER,JPEG_INFO_12BYTE);
    UADP_SPT_NAME1NXT(JPEG_GET_EXIF_MANUFACTURER,JPEG_GET_CTRL_PARAM,param,JPEG_EXIF_MANUFACTURER);

    UADP_SPT_NAME0NXT(JPEG_EXIF_SHUTTER_SPEED_VALUE,JPEG_INFO_12BYTE);
    UADP_SPT_NAME1NXT(JPEG_GET_EXIF_SHUTTER_SPEED_VALUE,JPEG_GET_CTRL_PARAM,param,JPEG_EXIF_SHUTTER_SPEED_VALUE);

    UADP_SPT_NAME0NXT(JPEG_CB_CTRL_PARAM,JPEG_CB_EVENT_PARAM);

    UADP_SPT_NAME0NXT(JPEG_ENABLE_ISR,JPEG_ENABLEISR_PARAM);



	*pIoctl= (FUtopiaIOctl)JPEG_EX_adp_Ioctl;

    return 0;
}


MS_U32 JPEG_EX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case MApi_CMD_JPEG_EnableOJPD:
        case MApi_CMD_JPEG_IsProgressive:
        case MApi_CMD_JPEG_ThumbnailFound:
        case MApi_CMD_JPEG_SetMHEG5:
        case MApi_CMD_JPEG_IsMPOFormat:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_BOOL_PARAM, spt_JPEG_BOOL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_Init_UsingOJPD:
        case MApi_CMD_JPEG_Init:// TODO: there is callback inside
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_INIT_PARAM, spt_JPEG_INIT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_get_APP0_info:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETAPP0INFO_PARAM, spt_JPEG_GETAPP0INFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_DecodeHdr:
        case MApi_CMD_JPEG_Decode:
        case MApi_CMD_JPEG_HdlVidxChk:
        case MApi_CMD_JPEG_DisableISR:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_RESULT_PARAM, spt_JPEG_RESULT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMaxDecodeResolution:
        case MApi_CMD_JPEG_SetProMaxDecodeResolution:
        case MApi_CMD_JPEG_SetMPOMaxDecodeResolution:
        case MApi_CMD_JPEG_SetMPOProMaxDecodeResolution:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_RESOLUTION_PARAM, spt_JPEG_RESOLUTION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_Exit:
        case MApi_CMD_JPEG_Rst:
        case MApi_CMD_JPEG_PowerOn:
        case MApi_CMD_JPEG_PowerOff:
        case MApi_CMD_JPEG_DumpMPO:
        case MApi_CMD_JPEG_Debug:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd, NULL);
            break;
        case MApi_CMD_JPEG_GetErrorCode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETERRORCODE_PARAM, spt_JPEG_GETERRORCODE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetJPDEventFlag:
        case MApi_CMD_JPEG_SetJPDEventFlag:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_JPEGEVENT_PARAM, spt_JPEG_JPEGEVENT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetCurVidx:
        case MApi_CMD_JPEG_GetWidth:
        case MApi_CMD_JPEG_GetHeight:
        case MApi_CMD_JPEG_GetOriginalWidth:
        case MApi_CMD_JPEG_GetOriginalHeight:
        case MApi_CMD_JPEG_GetNonAlignmentWidth:
        case MApi_CMD_JPEG_GetNonAlignmentHeight:
        case MApi_CMD_JPEG_GetAlignedPitch:
        case MApi_CMD_JPEG_GetAlignedPitch_H:
        case MApi_CMD_JPEG_GetAlignedWidth:
        case MApi_CMD_JPEG_GetAlignedHeight:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_U16, spt_JPEG_U16,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetScaleDownFactor:
        case MApi_CMD_JPEG_SetDbgLevel:
        case MApi_CMD_JPEG_GetDbgLevel:
        case MApi_CMD_JPEG_SetNJPDInstance:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_U8, spt_JPEG_U8,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMRBufferValid:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_SETMBUFFERVALID_PARAM, spt_JPEG_SETMBUFFERVALID_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_UpdateReadInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_UPDATEREADINFO_PARAM, spt_JPEG_UPDATEREADINFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_ProcessEOF:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_PROCESSEOF_PARAM, spt_JPEG_PROCESSEOF_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetErrCode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_SETERRORCODE_PARAM, spt_JPEG_SETERRORCODE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETINFO_PARAM, spt_JPEG_GETINFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETSTATUS_PARAM, spt_JPEG_GETSTATUS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetLibVer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETLIBVER_PARAM, spt_JPEG_GETLIBVER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetBuffLoadType:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETBUFFLOADTYPE_PARAM, spt_JPEG_GETBUFFLOADTYPE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_EnableISR:// TODO: callback
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_ENABLE_ISR, spt_JPEG_ENABLE_ISR,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_WaitDone:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_WAITDONE_PARAM, spt_JPEG_WAITDONE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFDateTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFDATETIME_PARAM, spt_JPEG_GETEXIFDATETIME_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFOrientation:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFORIENTATION_PARAM, spt_JPEG_GETEXIFORIENTATION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFManufacturer:
        case MApi_CMD_JPEG_GetEXIFModel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFSTRING_PARAM, spt_JPEG_GETEXIFSTRING_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFFlash:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFU16_PARAM, spt_JPEG_GETEXIFU16_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFISOSpeedRatings:
        case MApi_CMD_JPEG_GetEXIFImageWidth:
        case MApi_CMD_JPEG_GetEXIFImageHeight:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFU32_PARAM, spt_JPEG_GETEXIFU32_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFShutterSpeedValue:
        case MApi_CMD_JPEG_GetEXIFApertureValue:
        case MApi_CMD_JPEG_GetEXIFExposureBiasValue:
        case MApi_CMD_JPEG_GetEXIFFocalLength:
        case MApi_CMD_JPEG_GetEXIFExposureTime:
        case MApi_CMD_JPEG_GetEXIFFNumber:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETEXIFRATIONAL_PARAM, spt_JPEG_GETEXIFRATIONAL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetFreeMemory:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETFREEMEMORY_PARAM, spt_JPEG_GETFREEMEMORY_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetDataOffset:
        case MApi_CMD_JPEG_GetSOFOffset:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_U32, spt_JPEG_U32,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SupportCMYK:
        case MApi_CMD_JPEG_SupportRGB:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_SUPPORTCMYK_PARAM, spt_JPEG_SUPPORTCMYK_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetMPOIndex:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETMPOINDEX_PARAM, spt_JPEG_GETMPOINDEX_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetMPOAttr:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GETMPOATTR_PARAM, spt_JPEG_GETMPOATTR_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMPOBuffer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_SETMPOBUFFER_PARAM, spt_JPEG_SETMPOBUFFER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetVerificationMode:
        case MApi_CMD_JPEG_GetVerificationMode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_VERIFICATION_PARAM, spt_JPEG_VERIFICATION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetControl:
            {
            JPEG_GET_CTRL_PARAM jpegGetCtrlParam;
            CPY_FROM_USER((void*)(&jpegGetCtrlParam), (JPEG_GET_CTRL_PARAM*)pArgs, sizeof(JPEG_GET_CTRL_PARAM));
            switch(jpegGetCtrlParam.eGetID)
            {
                case E_JPEG_GET_EXIF_MANUFACTURER://96
                case E_JPEG_GET_EXIF_MODEL:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GET_EXIF_MANUFACTURER, spt_JPEG_GET_EXIF_MANUFACTURER,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_JPEG_GET_EXIF_FLASH://32
                case E_JPEG_GET_EXIF_ISO_SPEED_RATINGS:
                case E_JPEG_GET_EXIF_IMAGE_WIDTH:
                case E_JPEG_GET_EXIF_EXPOSURE_PROGRAM:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GET_CTRL_PARAM, spt_JPEG_GET_CTRL_PARAM,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE://64
                case E_JPEG_GET_EXIF_APERTURE_VALUE:
                case E_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE:
                case E_JPEG_GET_EXIF_FOCAL_LENGTH:
                case E_JPEG_GET_EXIF_IMAGE_HEIGHT:
                case E_JPEG_GET_EXIF_EXPOSURE_TIME:
                case E_JPEG_GET_EXIF_F_NUMBER:
                case E_JPEG_GET_Y_SAMPLING_FACTOR_HORIZONTAL:
                case E_JPEG_GET_Y_SAMPLING_FACTOR_VERTICAL:
                case E_JPEG_GET_COMPONENTS_NUM:
                case E_JPEG_GET_IS_PROGRESSIVE:
                case E_JPEG_GET_COLOR_FORMAT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE, spt_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE,buffer_arg,sizeof(buffer_arg));
                    break;
            }
            }
            break;
        case MApi_CMD_JPEG_GetCbEvent:
        case MApi_CMD_JPEG_SetCbEvent:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_JPEG_CB_CTRL_PARAM, spt_JPEG_CB_CTRL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;
    }

    return u32Ret;
}




