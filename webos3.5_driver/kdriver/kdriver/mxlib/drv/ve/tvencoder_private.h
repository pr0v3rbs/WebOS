#ifndef _TVE_PRIVATE_
#define _TVE_PRIVATE_

#include "drvTVEncoder.h"

#define MAGIC_NUM 0x12345678

typedef struct _TVENCODER_RESOURCE_PRIVATE
{
    MS_BOOL bInited_Drv;
    MS_U32  u32Magic;
}TVENCODER_RESOURCE_PRIVATE;

const VE_DrvInfo * _MDrv_VE_GetInfo(void);
VE_Result _MDrv_VE_GetStatus(VE_DrvStatus* pDrvStatus);
void _MDrv_VE_SetDbgLevel(MS_U16 level);
void _MDrv_VE_SwitchInputSource(PMS_Switch_VE_Src_Info pSwitchInfo);
MS_SWITCH_VE_SRC_STATUS _MDrv_VE_SetInputSource(PMS_VE_InputSrc_Info pInputSrcInfo);
void _MDrv_VE_SwitchOuputDest(PMS_Switch_VE_Dest_Info pSwitchInfo);
void _MDrv_VE_SetOutputCtrl(PMS_VE_Output_Ctrl pOutputCtrl);
MS_BOOL _MDrv_VE_SetOutputVideoStd(MS_VE_VIDEOSYS VideoSystem);
void _MDrv_VE_Init(MS_PHYADDR u32MIUAddress);
MS_BOOL _MDrv_VE_Exit(void);
void _MDrv_VE_SetWSSData(MS_BOOL bEn, MS_U16 u16WSSData);
MS_U16 _MDrv_VE_GetWSSData(void);
void _MDrv_VE_SetMode(MS_VE_Set_Mode_Type *pVESetMode);
void _MDrv_VE_SetBlackScreen(MS_BOOL bEn);
MS_BOOL _MDrv_VE_IsBlackScreenEnabled(void);
void _MDrv_VE_SetTtxBuffer(MS_U32 u32StartAddr, MS_U32 u32Size);
MS_BOOL _MDrv_VE_GetTtxReadDoneStatus(void);
void _MDrv_VE_EnableCcSw(MS_BOOL bEnable);
void _MDrv_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end);
void _MDrv_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data);
void _MDrv_VE_set_display_window(MS_VE_WINDOW_TYPE stDispWin);
void _MDrv_VE_SetOSD(MS_BOOL bEnable);
void _MDrv_VE_SetRGBIn(MS_BOOL bEnable);
MS_VE_VIDEOSYS _MDrv_VE_Get_Output_Video_Std(void);
void _MDrv_VE_SetCaptureMode(MS_BOOL bEnable);
MS_BOOL _MApi_VE_GetDstInfo(MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
void _MDrv_VE_Set_TestPattern(MS_BOOL bEn);
VE_Result _MDrv_VE_DisableRegWrite(MS_BOOL bFlag);
VE_Result _MDrv_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment);
VE_Result  _MDrv_VE_Set_Customer_Scaling(MS_VE_CusScalingInfo *pstVECusScalingInfo);
void _MDrv_VE_InitVECapture(PMS_VE_Output_CAPTURE pVECapture);
MS_BOOL _MDrv_VE_EnaVECapture(PMS_VE_VECAPTURESTATE pstVECapState);
VE_Result _MDrv_VE_Adjust_FrameStart(MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset);
MS_U32 _MDrv_TVE_SetPowerState(EN_POWER_MODE u16PowerState);

MS_BOOL _MDrv_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl);
void _MDrv_VE_EnableCusTable(MS_BOOL bEnable);
VE_Result _MDrv_VE_SetFrameColor(MS_U32 u32aRGB);
VE_Result _MDrv_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer);
MS_VE_OSD_LAYER_SEL _MDrv_VE_Get_OSDLayer(void);
VE_Result _MDrv_VE_Set_VideoAlpha(MS_U8 u8Val);
VE_Result _MDrv_VE_Get_VideoAlpha(MS_U8 *pu8Val);

VE_Result _MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType);
VE_Result _MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);


MS_U32 TVENCODEROpen(void** ppInstance,MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 TVENCODERClose(void* pInstance);
MS_U32 TVENCODERIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#endif //_TVE_PRIVATE_
