#include "MsTypes.h"
#if defined(MSOS_TYPE_LINUX_KERNEL)
    #include <linux/slab.h>
    #include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "drvVBI_v2.h"
#include "drvVBI_private.h"
#include "utopia_dapi.h"
#include "drvVBI.h"
#include "drvDMX_TTX.h"
#include "MsOS.h"
#include "utopia.h"

#define VBI_DEBUG 0

#if (VBI_DEBUG == 1)
  #define VBI_DEBUG_MSG(x)       x
#else
  #define VBI_DEBUG_MSG(x)
#endif


void VBIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;

	VBI_DEBUG_MSG(printf("\n[VBI INFO] vbi register start\n"));
    
    // 1. deal with module
    UtopiaModuleCreate(MODULE_VBI, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)VBIOpen, (FUtopiaClose)VBIClose, (FUtopiaIOctl)VBIIoctl);

    // 2. deal with resource (2 resources for DTV & ATV)
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, VBI_POOL_ID_VBI0);
    UtopiaResourceCreate("VBI0", sizeof(VBI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, VBI_POOL_ID_VBI0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, VBI_POOL_ID_VBI0);

    psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, VBI_POOL_ID_VBI1);
    UtopiaResourceCreate("VBI1", sizeof(VBI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, VBI_POOL_ID_VBI1);
    UtopiaModuleAddResourceEnd(pUtopiaModule, VBI_POOL_ID_VBI1);
    
}

MS_U32 VBIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	VBI_DEBUG_MSG(printf("\n[VBI INFO] vbi open start\n"));
    
    VBI_INSTANT_PRIVATE *pVbiPri = NULL;
    void *pVbiPriVoid = NULL;
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));   

    UtopiaInstanceCreate(sizeof(VBI_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pVbiPriVoid);

    pVbiPri = (VBI_INSTANT_PRIVATE*)pVbiPriVoid;
    // For ATV
    pVbiPri->fpVBIGetLibVer                       = (IOCTL_VBI_GETLIBVER                       )MDrv_VBI_GetLibVer;
    pVbiPri->fpVBIGetInfo                         = (IOCTL_VBI_GETINFO                         )MDrv_VBI_GetInfo;
    pVbiPri->fpVBIGetStatus                       = (IOCTL_VBI_GETSTATUS                       )_MDrv_VBI_GetStatus;
    pVbiPri->fpVBISetDbgLevel                     = (IOCTL_VBI_SET_DBGLEVEL                    )MDrv_VBI_SetDbgLevel;
    pVbiPri->fpVBIInit                            = (IOCTL_VBI_INIT                            )_MDrv_VBI_Init;    
    pVbiPri->fpVBIExit                            = (IOCTL_VBI_EXIT                            )_MDrv_VBI_Exit;
    pVbiPri->fpVBIRegisterCB                      = (IOCTL_VBI_REGISTERCB                      )_MDrv_VBI_RegisterCB;
    pVbiPri->fpVBIRingBufferReset                 = (IOCTL_VBI_RINGBUFFER_RESET                )_MDrv_VBI_RingBuffer_Reset;
    pVbiPri->fpVBIInitializeTTXSlicer             = (IOCTL_VBI_INITIALIZETTXSLICER             )_MDrv_VBI_InitializeTTXSlicer;
    pVbiPri->fpVBIEnableTTXSlicer                 = (IOCTL_VBI_ENABLETTXSLICER                 )_MDrv_VBI_EnableTTXSlicer;
    pVbiPri->fpVBIIsVPSReady                      = (IOCTL_VBI_ISVPS_READY                     )_MDrv_VBI_IsVPS_Ready;
    pVbiPri->fpVBIIsTTXReady                      = (IOCTL_VBI_ISTTX_READY                     )_MDrv_VBI_IsTTX_Ready;
    pVbiPri->fpVBIIsWSSReady                      = (IOCTL_VBI_ISWSS_READY                     )_MDrv_VBI_IsWSS_Ready;
    pVbiPri->fpVBIGetWSSData                      = (IOCTL_VBI_GETWSS_DATA                     )_MDrv_VBI_GetWSS_Data;
    pVbiPri->fpVBIGetVPSData                      = (IOCTL_VBI_GETVPS_DATA                     )_MDrv_VBI_GetVPS_Data;
    pVbiPri->fpVBISetVideoStandard                = (IOCTL_VBI_SETVIDEOSTANDARD                )_MDrv_VBI_SetVideoStandard;
    pVbiPri->fpVBITTXPacketBufferIsEmpty          = (IOCTL_VBI_TTX_PACKETBUFFERISEMPTY         )_MDrv_VBI_TTX_PacketBufferIsEmpty;
    pVbiPri->fpVBITTXCheckCircuitReady            = (IOCTL_VBI_TTX_CHECKCIRCUITREADY           )_MDrv_VBI_TTX_CheckCircuitReady;
    pVbiPri->fpVBITTXGetPacketCount               = (IOCTL_VBI_TTX_GETPACKETCOUNT              )_MDrv_VBI_TTX_GetPacketCount;
    pVbiPri->fpVBITTXGetPackets                   = (IOCTL_VBI_TTX_GETPACKETS                  )_MDrv_VBI_TTX_GetPackets;
    pVbiPri->fpVBITTXGetPacket                    = (IOCTL_VBI_TTX_GETPACKET                   )_MDrv_VBI_TTX_GetPacket;
    pVbiPri->fpVBITTXPacktetBufferIsOverflow      = (IOCTL_VBI_TTX_PACKETBUFFERISOVERFLOW      )_MDrv_VBI_TTX_PacketBufferIsOverflow;
    pVbiPri->fpVBITTXPacketBufferGetNoOfOverflows = (IOCTL_VBI_TTX_PACKETBUFFERGETNOOFOVERFLOWS)_MDrv_VBI_TTX_PacketBufferGetNoOfOverflows;
    pVbiPri->fpVBITTXEnableLine                   = (IOCTL_VBI_TTX_ENABLELINE                  )_MDrv_VBI_TTX_EnableLine;
    pVbiPri->fpVBISyncMemory                      = (IOCTL_VBI_SYNCMEMORY                      )_MDrv_VBI_SyncMemory;
    pVbiPri->fpVBICCInitSlicer                    = (IOCTL_VBI_CC_INITSLICER                   )_MDrv_VBI_CC_InitSlicer;
    pVbiPri->fpVBICCInitYPbYr                     = (IOCTL_VBI_CC_INITYPBYR                    )_MDrv_VBI_CC_InitYPbYr;
    pVbiPri->fpVBICCSetDataRate                   = (IOCTL_VBI_CC_SETDATARATE                  )_MDrv_VBI_CC_SetDataRate;
    pVbiPri->fpVBICCGetInfo                       = (IOCTL_VBI_CC_GETINFO                      )_MDrv_VBI_CC_GetInfo;
    pVbiPri->fpVBICCSetFrameCnt                   = (IOCTL_VBI_CC_SETFRAMECNT                  )_MDrv_VBI_CC_SetFrameCnt;
    pVbiPri->fpVBICCEnableSlicer                  = (IOCTL_VBI_CC_ENABLESLICER                 )_MDrv_VBI_CC_EnableSlicer;
    pVbiPri->fpVBICCEnableLine                    = (IOCTL_VBI_CC_ENABLELINE                   )_MDrv_VBI_CC_EnableLine;
    pVbiPri->fpVBIWSSSetVpsByteNum                = (IOCTL_VBI_WSS_SETVPSBYTENUM               )_MDrv_VBI_WSS_SetVpsByteNum;
    pVbiPri->fpVBIGetCompleteVPSData              = (IOCTL_VBI_GETCOMPLETEVPS_DATA             )_MDrv_VBI_GetCompleteVPS_Data;

    // For DTV
    pVbiPri->fpDMXTTXRingBufferReset              = (IOCTL_DMX_TTX_RINGBUFFER_RESET            )_MDrv_DMX_TTX_RingBuffer_Reset;
    pVbiPri->fpDMXTTXInit                         = (IOCTL_DMX_TTX_INIT                        )_MDrv_DMX_TTX_Init;
    pVbiPri->fpDMXTTXExit                         = (IOCTL_DMX_TTX_EXIT                        )_MDrv_DMX_TTX_Exit;
    pVbiPri->fpDMXTTXSetFilterID                  = (IOCTL_DMX_TTX_SETFILTERID                 )_MDrv_DMX_TTX_SetFilterID;
    pVbiPri->fpDMXTTXSetCB                        = (IOCTL_DMX_TTX_SETCB                       )_MDrv_DMX_TTX_SetCB;
    pVbiPri->fpDMXTTXPacketBufferIsEmpty          = (IOCTL_DMX_TTX_PACKETBUFFERISEMPTY         )_MDrv_DMX_TTX_PacketBufferIsEmpty;
    pVbiPri->fpDMXTTXGetPackets                   = (IOCTL_DMX_TTX_GETPACKETS                  )_MDrv_DMX_TTX_GetPackets;
    pVbiPri->fpDMXTTXGetPacket                    = (IOCTL_DMX_TTX_GETPACKET                   )_MDrv_DMX_TTX_GetPacket;
    pVbiPri->fpDMXTTXPacketBufferIsOverflow       = (IOCTL_DMX_TTX_PACKETBUFFERISOVERFLOW      )_MDrv_DMX_TTX_PacketBufferIsOverflow;
    pVbiPri->fpDMXTTXPacketBufferGetNoOfOverflows = (IOCTL_DMX_TTX_PACKETBUFFERGETNOOFOVERFLOWS)_MDrv_DMX_TTX_PacketBufferGetNoOfOverflows;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 VBIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{    
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    VBI_DEBUG_MSG(printf("\n[VBI INFO] vbi ioctl cmd = %ld\n",u32Cmd));
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    VBI_INSTANT_PRIVATE*  psVbiInstPri = NULL;
    void* pModule       = NULL;
	void* pResource_atv = NULL;
    void* pResource_dtv = NULL;
    
    UtopiaInstanceGetModule(pInstance, &pModule);
	UtopiaInstanceGetPrivate(pInstance, (void**)&psVbiInstPri); // Get instance
	
    PVBI_GETLIBVER              pGetLibVer            = NULL;
    PVBI_GETINFO_PARAM          pGetInfoParam         = NULL;
    PVBI_GETSTATUS              pGetStatus            = NULL;
    PVBI_SETDBGLEVEL            pSetDbgLevel          = NULL;
    PVBI_INIT_TYPE              pInitType             = NULL;
    PVBI_REGISTER_CB            pRegisterCB           = NULL;
    PVBI_INITIALIZER_TTX_SLICER pInitializerTTXSlicer = NULL;
    PVBI_ENABLE_TTX_SLICER      pEnableTTXSlicer      = NULL;
    PVBI_GET_VPS_DATA           pGetVPSData           = NULL;
    PVBI_SET_VIDEO_STANDARD     pSetVideoStandard     = NULL;
    PVBI_TTX_GET_PACKETS        pGetPackets           = NULL;
    PVBI_TTX_GET_PACKET         pGetPacket            = NULL;
    PVBI_TTX_ENABLE_LINE        pEnableLine           = NULL;
    PVBI_SYNC_MEMORY            pSyncMemory           = NULL;
    PVBI_CC_INIT_SLICER         pCCInitSlicer         = NULL;
    PVBI_CC_INIT_YPBYR          pCCInitYPbYr          = NULL;
    PVBI_CC_SETDATARATE         pCCSetDataRate        = NULL;
    PVBI_CC_GETINFO             pGetInfo              = NULL;
    PVBI_CC_SET_FRAMECNT        pCCSetFrameCnt        = NULL;
    PVBI_CC_ENABLE_SLICER       pCCEnableSlicer       = NULL;
    PVBI_CC_ENABLE_LINE         pCCEnableLine         = NULL;
    PVBI_VBI_WSS_VPSBYTENUM     pWSSSetVpsByteCnt     = NULL;
    PVBI_GET_COMPLETE_VPS_DATA  pGetCompleteVPSData   = NULL;
    PVBI_SET_VIDEO_STANDARD     pSetCCVideoStandard   = NULL;
    
    PDMX_TTX_SET_FILTERID       pDMXSetFilterID       = NULL;
    PDMX_TTX_SET_CB             pDMXSetCB             = NULL;  
    
    PVBI_CMD                    pVbiCmd               = NULL;    
    PDMX_TTX_CMD                pDmxTtxCmd            = NULL;

    PVBI_GET_DATA pGetData        = NULL;
    PVBI_BOOL     pCheckTureFalse = NULL;

    VBI_DrvInfo *pVBIInfo;
    
    if(u32Cmd <= MDrv_CMD_VBI_WSS_SetVpsByteNum ||  u32Cmd == MDrv_CMD_VBI_Exit)
    { 
        if(UtopiaResourceObtain(pModule, VBI_POOL_ID_VBI0, &pResource_atv) == UTOPIA_STATUS_FAIL)
        {
            return UTOPIA_STATUS_ERR_RESOURCE;
        }
    }
    else
    {
        if(UtopiaResourceObtain(pModule, VBI_POOL_ID_VBI1, &pResource_dtv) == UTOPIA_STATUS_FAIL)
        {
            return UTOPIA_STATUS_ERR_RESOURCE;
        }
    }
    
    switch(u32Cmd)
    {
        //=============================================
        // ATV - TTX, CC
        //=============================================
        case MDrv_CMD_VBI_GetLibVer:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetLibVer\n");
            pGetLibVer = (PVBI_GETLIBVER)pArgs;
            if(psVbiInstPri->fpVBIGetLibVer(pGetLibVer->ppVersion) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetInfo:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetInfo\n");
            pGetInfoParam = (PVBI_GETINFO_PARAM)pArgs;
            pVBIInfo = psVbiInstPri->fpVBIGetInfo();
            memcpy (pGetInfoParam,pVBIInfo,sizeof(VBI_DrvInfo));
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetStatus:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetStatus\n");
            pGetStatus = (PVBI_GETSTATUS)pArgs;
            if(psVbiInstPri->fpVBIGetStatus(pGetStatus->pDrvStatus) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SetDbgLevel:
            //printf("VBIIoctl - MDrv_CMD_VBI_SetDbgLevel\n");
            pSetDbgLevel = (PVBI_SETDBGLEVEL)pArgs;
            if(psVbiInstPri->fpVBISetDbgLevel(pSetDbgLevel->u16DbgSwitch) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_Init:
            //printf("VBIIoctl - MDrv_CMD_VBI_Init\n");
            pInitType = (PVBI_INIT_TYPE)pArgs;
            if(psVbiInstPri->fpVBIInit(pInitType->cmd, pInitType->type) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_RegisterCB:
            //printf("VBIIoctl - MDrv_CMD_VBI_RegisterCB\n");
            pRegisterCB = (PVBI_REGISTER_CB)pArgs;
            psVbiInstPri->fpVBIRegisterCB(pRegisterCB->pFN,
                                          pRegisterCB->bufferAddr,
                                          pRegisterCB->length);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_RingBuffer_Reset:
            //printf("VBIIoctl - MDrv_CMD_VBI_RingBuffer_Reset\n");
            psVbiInstPri->fpVBIRingBufferReset();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_InitializeTTXSlicer:
            //printf("VBIIoctl - MDrv_CMD_VBI_InitializeTTXSlicer\n");
            pInitializerTTXSlicer = (PVBI_INITIALIZER_TTX_SLICER)pArgs;
            psVbiInstPri->fpVBIInitializeTTXSlicer(pInitializerTTXSlicer->bufferAddr,
                                                   pInitializerTTXSlicer->packetCount);       
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_EnableTTXSlicer:
            //printf("VBIIoctl - MDrv_CMD_VBI_EnableTTXSlicer\n");
            pEnableTTXSlicer = (PVBI_ENABLE_TTX_SLICER)pArgs;
            psVbiInstPri->fpVBIEnableTTXSlicer(pEnableTTXSlicer->bEnable);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsVPS_Ready:
            //printf("VBIIoctl - MDrv_CMD_VBI_IsVPS_Ready\n"); 
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsVPSReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsTTX_Ready:
            //printf("VBIIoctl - MDrv_CMD_VBI_IsTTX_Ready\n"); 
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsTTXReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsWSS_Ready:
            //printf("VBIIoctl - MDrv_CMD_VBI_IsWSS_Ready\n"); 
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsWSSReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetWSS_Data:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetWSS_Data\n");              
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->data = psVbiInstPri->fpVBIGetWSSData();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetVPS_Data:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetVPS_Data\n");            
            pGetVPSData = (PVBI_GET_VPS_DATA)pArgs;
            psVbiInstPri->fpVBIGetVPSData(pGetVPSData->lowerWord, pGetVPSData->higherWord);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetCompleteVPS_Data:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetVPS_Data\n");            
            pGetCompleteVPSData = (PVBI_GET_COMPLETE_VPS_DATA)pArgs;
            psVbiInstPri->fpVBIGetCompleteVPSData(pGetCompleteVPSData->pBuffer, pGetCompleteVPSData->length);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SetVideoStandard:
            //printf("VBIIoctl - MDrv_CMD_VBI_SetVideoStandard\n");
            pSetVideoStandard = (PVBI_SET_VIDEO_STANDARD)pArgs;
            psVbiInstPri->fpVBISetVideoStandard(pSetVideoStandard->eStandard);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsEmpty:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferIsEmpty\n");            
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck= psVbiInstPri->fpVBITTXPacketBufferIsEmpty();            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_CheckCircuitReady:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_CheckCircuitReady\n");            
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBITTXCheckCircuitReady();               
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPacketCount:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_GetPacketCount\n");            
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->data = psVbiInstPri->fpVBITTXGetPacketCount();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPackets:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_GetPackets\n");  
            pGetPackets = (PVBI_TTX_GET_PACKETS)pArgs;            
            if(psVbiInstPri->fpVBITTXGetPackets(pGetPackets->dataAddr, pGetPackets->length) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPacket:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_GetPacket\n");  
            pGetPacket = (PVBI_TTX_GET_PACKET)pArgs;
            if(psVbiInstPri->fpVBITTXGetPacket(pGetPacket->packetAddress) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsOverflow:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferIsOverflow\n"); 
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBITTXPacktetBufferIsOverflow();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows\n");   
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->data = psVbiInstPri->fpVBITTXPacketBufferGetNoOfOverflows();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_EnableLine:
            //printf("VBIIoctl - MDrv_CMD_VBI_TTX_EnableLine\n");
            pEnableLine = (PVBI_TTX_ENABLE_LINE)pArgs;
            psVbiInstPri->fpVBITTXEnableLine(pEnableLine->StartLine,
                                             pEnableLine->EndLine);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SyncMemory:
            //printf("VBIIoctl - MDrv_CMD_VBI_SyncMemory\n");  
            pSyncMemory = (PVBI_SYNC_MEMORY)pArgs;            
            if(psVbiInstPri->fpVBISyncMemory(pSyncMemory->u32Start, pSyncMemory->u32Size) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_InitSlicer:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_InitSlicer\n");
            pCCInitSlicer = (PVBI_CC_INIT_SLICER)pArgs;
            psVbiInstPri->fpVBICCInitSlicer(pCCInitSlicer->u32RiuAddr,
                                            pCCInitSlicer->bufferAddr,
                                            pCCInitSlicer->packetCount);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_InitYPbYr:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_InitYPbYr\n");
            pCCInitYPbYr = (PVBI_CC_INIT_YPBYR)pArgs;
            psVbiInstPri->fpVBICCInitYPbYr(pCCInitYPbYr->cvbs_no);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_SetDataRate:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_SetDataRate\n");
            pCCSetDataRate = (PVBI_CC_SETDATARATE)pArgs;
            if(psVbiInstPri->fpVBICCSetDataRate(pCCSetDataRate->ptable) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_GetInfo:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_GetInfo\n");  
            pGetInfo = (PVBI_CC_GETINFO)pArgs;
            pGetInfo->info = psVbiInstPri->fpVBICCGetInfo(pGetInfo->selector);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_SetFrameCnt:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_SetFrameCnt\n");
            pCCSetFrameCnt = (PVBI_CC_SET_FRAMECNT)pArgs;
            psVbiInstPri->fpVBICCSetFrameCnt(pCCSetFrameCnt->cnt);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_EnableSlicer:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_EnableSlicer\n");
            pCCEnableSlicer = (PVBI_CC_ENABLE_SLICER)pArgs;
            psVbiInstPri->fpVBICCEnableSlicer(pCCEnableSlicer->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_EnableLine:
            //printf("VBIIoctl - MDrv_CMD_VBI_CC_EnableLine\n");
            pCCEnableLine = (PVBI_CC_ENABLE_LINE)pArgs;
            psVbiInstPri->fpVBICCEnableLine(pCCEnableLine->StartLine,
                                            pCCEnableLine->EndLine,
                                            pCCEnableLine->mode);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_WSS_SetVpsByteNum:
            //printf("VBIIoctl - MDrv_CMD_VBI_WSS_SetVpsByteNum\n");
            pWSSSetVpsByteCnt = (PVBI_VBI_WSS_VPSBYTENUM)pArgs;
            psVbiInstPri->fpVBIWSSSetVpsByteNum(pWSSSetVpsByteCnt->cnt);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;   
        case MDrv_CMD_VBI_CC_SetVideoStandard:
            pSetCCVideoStandard = (PVBI_SET_VIDEO_STANDARD)pArgs;
            psVbiInstPri->fpVBICCSetVideoStandard(pSetCCVideoStandard->eStandard);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
        //=============================================
        // DTV - TTX
        //=============================================
        case MDrv_CMD_DMX_TTX_RingBuffer_Reset:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_RingBuffer_Reset\n");
            psVbiInstPri->fpDMXTTXRingBufferReset();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;        
        case MDrv_CMD_DMX_TTX_Init:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_Init\n");
            pInitializerTTXSlicer = (PVBI_INITIALIZER_TTX_SLICER)pArgs;
            if(psVbiInstPri->fpDMXTTXInit(pInitializerTTXSlicer->cmd, pInitializerTTXSlicer->bufferAddr, pInitializerTTXSlicer->packetCount) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;        
        case MDrv_CMD_DMX_TTX_SetFilterID:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_SetFilterID\n");
            pDMXSetFilterID = (PDMX_TTX_SET_FILTERID)pArgs;
            psVbiInstPri->fpDMXTTXSetFilterID(pDMXSetFilterID->fid);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;        
        case MDrv_CMD_DMX_TTX_SetCB:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_SetCB\n");
            pDMXSetCB = (PDMX_TTX_SET_CB)pArgs;
            psVbiInstPri->fpDMXTTXSetCB(pDMXSetCB->fn);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;        
        case MDrv_CMD_DMX_TTX_PacketBufferIsEmpty:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferIsEmpty\n"); 
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXPacketBufferIsEmpty();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_GetPackets:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_GetPackets\n");  
            pGetPackets = (PVBI_TTX_GET_PACKETS)pArgs;
            if(psVbiInstPri->fpDMXTTXGetPackets(pGetPackets->dataAddr, pGetPackets->length) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_GetPacket:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_GetPacket\n");  
            pGetPacket = (PVBI_TTX_GET_PACKET)pArgs;
            if(psVbiInstPri->fpDMXTTXGetPacket(pGetPacket->packetAddress) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferIsOverflow:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferIsOverflow\n");  
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXPacketBufferIsOverflow();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows:
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows\n");  
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->data = psVbiInstPri->fpDMXTTXPacketBufferGetNoOfOverflows();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_Exit:       
            //printf("VBIIoctl - MDrv_CMD_VBI_Exit\n");  
            pVbiCmd = (PVBI_CMD)pArgs;            
            if(psVbiInstPri->fpVBIExit(pVbiCmd->cmd) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_Exit:            
            //printf("VBIIoctl - MDrv_CMD_DMX_TTX_Exit\n");  
            pDmxTtxCmd = (PDMX_TTX_CMD)pArgs;            
            if(psVbiInstPri->fpDMXTTXExit(pDmxTtxCmd->cmd) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        default:
            break;
    };

    if(u32Cmd <= MDrv_CMD_VBI_WSS_SetVpsByteNum ||  u32Cmd == MDrv_CMD_VBI_Exit )
    {
        UtopiaResourceRelease(pResource_atv);
    }
    else
    {
        UtopiaResourceRelease(pResource_dtv);
    }
     
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));
	return u32Ret; // FIXME: error code
}

MS_U32 VBIClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);
	return UTOPIA_STATUS_SUCCESS;
}

