#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mhal_xc_chip_config.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drv_sc_ip.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_if_v2.h"

void HDMI_IF_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpHDMI_Func_Caps = 	MDrv_HDMI_Func_Caps_U2;
    psXCInstPri->fpHDMI_init = 	MDrv_HDMI_init_U2;
    psXCInstPri->fpHDMI_Exit = 	MDrv_HDMI_Exit_U2;
    psXCInstPri->fpHDMI_pkt_reset = 	MDrv_HDMI_pkt_reset_U2;
    psXCInstPri->fpHDMI_pullhpd = 	MDrv_HDMI_pullhpd_U2;
    psXCInstPri->fpHDMI_GC_Info = 	MDrv_HDMI_GC_Info_U2;
    psXCInstPri->fpHDMI_Packet_Received = 	MDrv_HDMI_Packet_Received_U2;
    psXCInstPri->fpHDMI_Get_ColorFormat = 	MDrv_HDMI_Get_ColorFormat_U2;
    psXCInstPri->fpHDMI_Get_ColorRange = 	MDrv_HDMI_Get_ColorRange_U2;
    psXCInstPri->fpHDMI_Set_EQ = 	MDrv_HDMI_Set_EQ_U2;
    psXCInstPri->fpHDMI_Set_EQ_To_Port = MDrv_HDMI_Set_EQ_To_Port_U2;
    psXCInstPri->fpHDMI_Audio_MUTE_Enable = 	MDrv_HDMI_Audio_MUTE_Enable_U2;
    psXCInstPri->fpHDMI_Audio_Status_Clear = 	MDrv_HDMI_Audio_Status_Clear_U2;
    psXCInstPri->fpHDMI_Get_AspectRatio = 	MDrv_HDMI_Get_AspectRatio_U2;
    psXCInstPri->fpHDMI_Get_ActiveFormat_AspectRatio = 	MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2;
    psXCInstPri->fpHDMI_Get_AVIInfoFrameVer = 	MDrv_HDMI_Get_AVIInfoFrameVer_U2;
    psXCInstPri->fpHDMI_err_status_update = 	MDrv_HDMI_err_status_update_U2;
    psXCInstPri->fpHDMI_Get_TMDS_freq     =     MDrv_HDMI_GetTMDSFreq_U2;
    psXCInstPri->fpHDMI_Get_PollingStatus = 	MDrv_HDMI_Get_PollingStatus_U2;
    psXCInstPri->fpHDMI_IsHDMI_Mode = 	MDrv_HDMI_IsHDMI_Mode_U2;
    psXCInstPri->fpHDMI_Get_MID_info = 	MDrv_HDMI_Get_MID_info_U2;
    psXCInstPri->fpHDMI_get_packet_value = 	MDrv_HDMI_get_packet_value_U2;
    psXCInstPri->fpDVI_ChannelPhaseStatus = 	MDrv_DVI_ChannelPhaseStatus_U2;
    psXCInstPri->fpDVI_SoftwareReset = 	MDrv_DVI_SoftwareReset_U2;
    psXCInstPri->fpdvi_reset = 	mdrv_dvi_reset_U2;
    psXCInstPri->fpHDMI_Get_AVIInfoActiveInfoPresent = MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2;
    psXCInstPri->fpDVI_ClkPullLow = 	MDrv_DVI_ClkPullLow_U2;
    psXCInstPri->fpDVI_SwitchSrc = 	MDrv_DVI_SwitchSrc_U2;
    psXCInstPri->fpHDMI_SetForClockLessThan70Mhz = 	MDrv_HDMI_SetForClockLessThan70Mhz_U2;
    psXCInstPri->fpHDMI_dvi_adjust = 	MDrv_HDMI_dvi_adjust_U2;
    psXCInstPri->fpHDMI_SetUpdatePhaseLineCount = 	MDrv_HDMI_SetUpdatePhaseLineCount_U2;
    psXCInstPri->fpHDCP_Enable = 	MDrv_HDCP_Enable_U2;
    psXCInstPri->fpHDMI_SetHdcpEnable = MDrv_HDMI_SetHdcpEnable_U2;
    psXCInstPri->fpHDCP_ClearStatus = 	MDrv_HDCP_ClearStatus_U2;
    psXCInstPri->fpHDCP_initproductionkey = 	MDrv_HDCP_initproductionkey_U2;
    psXCInstPri->fpHDCP_GetStatus = 	MDrv_HDCP_GetStatus_U2;
    psXCInstPri->fpHDCP_Vsync_end_en = 	MDrv_HDCP_Vsync_end_en_U2;
    psXCInstPri->fpHDMI_audio_output = 	MDrv_HDMI_audio_output_U2;
    psXCInstPri->fpHDMI_audio_cp_hdr_info = 	MDrv_HDMI_audio_cp_hdr_info_U2;
    psXCInstPri->fpHDMI_audio_channel_status = 	MDrv_HDMI_audio_channel_status_U2;
    psXCInstPri->fpHDMI_GetLibVer = 	MDrv_HDMI_GetLibVer_U2;
    psXCInstPri->fpHDMI_GetInfo = 	MDrv_HDMI_GetInfo_U2;
    psXCInstPri->fpHDMI_GetStatus = 	MDrv_HDMI_GetStatus_U2;
    psXCInstPri->fpHDMI_READ_DDCRAM =   MDrv_HDMI_READ_DDCRAM_U2;
    psXCInstPri->fpHDMI_PROG_DDCRAM = MDrv_HDMI_PROG_DDCRAM_U2;
    psXCInstPri->fpHDMI_Get_Ext_Colorimetry = MDrv_HDMI_Get_ExtColorimetry_U2;
    psXCInstPri->fpHDMI_Get_Content_Type = 	MDrv_HDMI_Get_Content_Type_U2;
    psXCInstPri->fpHDMI_Get_Pixel_Repetition = 	MDrv_HDMI_Get_Pixel_Repetition_U2;
    psXCInstPri->fpHDMI_Check4K2K        = MDrv_HDMI_Check4K2K_U2;
    psXCInstPri->fpHDMI_3D_4Kx2K_Process = 	MDrv_HDMI_3D_4Kx2K_Process_U2;
    psXCInstPri->fpHDMI_SetHPD_HL = MDrv_HDMI_SetHPD_HL_U2;
    psXCInstPri->fpHDMI_AVG_ScaleringDown = MDrv_HDMI_AVG_ScaleringDown_U2;
    psXCInstPri->fpHDMI_Check_Additional_Format = 	MDrv_HDMI_Check_Additional_Format_U2;
    psXCInstPri->fpHDMI_Get_3D_Structure = 	MDrv_HDMI_Get_3D_Structure_U2;
    psXCInstPri->fpHDMI_Get_3D_Ext_Data = 	MDrv_HDMI_Get_3D_Ext_Data_U2;
    psXCInstPri->fpHDMI_Get_3D_Meta_Field = 	MDrv_HDMI_Get_3D_Meta_Field_U2;
    psXCInstPri->fpHDMI_Get_VIC_Code = 	MDrv_HDMI_Get_VIC_Code_U2;
    psXCInstPri->fpHDMI_Get_4Kx2K_VIC_Code = 	MDrv_HDMI_Get_4Kx2K_VIC_Code_U2;
    psXCInstPri->fpHDMI_ARC_PINControl = 	MDrv_HDMI_ARC_PINControl_U2;
	psXCInstPri->fpDVI_Software_Reset = 	MDrv_DVI_Software_Reset_U2;
    psXCInstPri->fpHDMI_Set_PowerState =    MDrv_HDMI_SetPowerState_U2;
    psXCInstPri->fpHDMI_dvi_hf_adjust =     MDrv_DVI_HF_adjust_U2;
    psXCInstPri->fpHDMI_checkhdmi20_setting =     MDrv_HDMI_CheckHDMI20_Setting_U2;
    psXCInstPri->fpHDCP_GetHde = 	MDrv_HDMI_GetHde_U2;
    psXCInstPri->fpHDCP_GetVde = 	MDrv_HDMI_GetVde_U2;
    psXCInstPri->fpHDCP_GetHtt = 	MDrv_HDMI_GetHtt_U2;
    psXCInstPri->fpHDCP_GetVtt = 	MDrv_HDMI_GetVtt_U2;
    psXCInstPri->fpHDMI_StablePolling = MDrv_HDMI_StablePolling_U2;
    psXCInstPri->fpHDMI_GetSourceVersion = MDrv_HDMI_GetSourceVersion_U2;
    psXCInstPri->fpHDMI_Set5VDetectGPIOSelect = MDrv_HDMI_Set5VDetectGPIOSelect_U2;
    psXCInstPri->fpHDMI_GetDEStableStatus=     MDrv_HDMI_GetDEStableStatus_U2;
    psXCInstPri->fpHDMI_DDCChannelEnable=     MDrv_HDMI_DDCChannelEnable_U2;
	/*************************** HDCP22 ***************************/
	psXCInstPri->fpHDCP22_InitCBFunc = 		MDrv_HDCP22_InitCBFunc_U2;
	psXCInstPri->fpHDCP22_PortInit = 		MDrv_HDCP22_PortInit_U2;
	psXCInstPri->fpHDCP22_PollingReadDone =	MDrv_HDCP22_PollingReadDone_U2;
	psXCInstPri->fpHDCP22_EnableCipher = 	MDrv_HDCP22_EnableCipher_U2;
	psXCInstPri->fpHDCP22_SendMsg = 		MDrv_HDCP22_SendMsg_U2;
	psXCInstPri->fpHDCP22_Handler = 		MDrv_HDCP22_Handler_U2;
	/*************************** HDCP22 ***************************/
    return;
}

MS_U32 _HDMI_IF_Ioctl(void * pInstance, MS_U32 u32Cmd, void * pArgs)
{
    //void* pModule = NULL;
    //void* pResource = NULL;
    XC_INSTANCE_PRIVATE* psXCInstPri = NULL;

    //UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psXCInstPri)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
    }

    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    switch(u32Cmd)
    {
        case E_XC_HDMI_IF_CMD_FUNC_CAPS:
            {

                pstHDMI_FUNC_CAPS pXCArgs = (pstHDMI_FUNC_CAPS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDMI_Func_Caps(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_INIT:
            {
                psXCInstPri->fpHDMI_init(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_EXIT:
            {

                psXCInstPri->fpHDMI_Exit(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PKT_RESET:
            {

                pstHDMI_PKT_RESET pXCArgs = (pstHDMI_PKT_RESET) pArgs;
                psXCInstPri->fpHDMI_pkt_reset(pInstance, pXCArgs->breset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PULLHPD:
            {

                pstHDMI_PULLHPD pXCArgs = (pstHDMI_PULLHPD) pArgs;
                psXCInstPri->fpHDMI_pullhpd(pInstance, pXCArgs->bHighLow,
                                                       pXCArgs->enInputPortType,
                                                       pXCArgs->bInverse);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GC_INFO:
            {

                pstHDMI_GC_INFO pXCArgs = (pstHDMI_GC_INFO) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDMI_GC_Info(pInstance, pXCArgs->gcontrol);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PACKET_RECEIVED:
            {

                pstHDMI_PACKET_RECEIVED pXCArgs = (pstHDMI_PACKET_RECEIVED) pArgs;
                pXCArgs->stReturnValue = psXCInstPri->fpHDMI_Packet_Received(pInstance, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_COLORFORMAT:
            {

                pstHDMI_GET_COLORFORMAT pXCArgs = (pstHDMI_GET_COLORFORMAT) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ColorFormat(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_COLORRANGE:
            {

                pstHDMI_GET_COLORRANGE pXCArgs = (pstHDMI_GET_COLORRANGE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ColorRange(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_SET_EQ:
            {

                pstHDMI_SET_EQ pXCArgs = (pstHDMI_SET_EQ) pArgs;
                psXCInstPri->fpHDMI_Set_EQ(pInstance, pXCArgs->enEq, pXCArgs->u8EQValue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT:
            {
                pstHDMI_SET_EQ_TO_PORT pXCArgs = (pstHDMI_SET_EQ_TO_PORT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Set_EQ_To_Port(pInstance, pXCArgs->enEq, pXCArgs->u8EQValue, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE:
            {

                pstHDMI_AUDIO_MUTE_ENABLE pXCArgs = (pstHDMI_AUDIO_MUTE_ENABLE) pArgs;
                psXCInstPri->fpHDMI_Audio_MUTE_Enable(pInstance, pXCArgs->u16MuteEvent,
                                                                 pXCArgs->u16MuteMask);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR:
            {

                psXCInstPri->fpHDMI_Audio_Status_Clear(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_ASPECTRATIO:
            {

                pstHDMI_GET_ASPECTRATIO pXCArgs = (pstHDMI_GET_ASPECTRATIO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_AspectRatio(pInstance, pXCArgs->bEn_PAR
                                                                                     , pXCArgs->bEn_AFAR);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO:
            {

                pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO pXCArgs = (pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ActiveFormat_AspectRatio(pInstance, pXCArgs->bEn_AFAR);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER:
            {

                pstHDMI_GET_AVIINFOFRAMEVER pXCArgs = (pstHDMI_GET_AVIINFOFRAMEVER) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_AVIInfoFrameVer(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE:
            {

                pstHDMI_ERR_STATUS_UPDATE pXCArgs = (pstHDMI_ERR_STATUS_UPDATE) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_err_status_update(pInstance,
                                                                               pXCArgs->u8value,
                                                                               pXCArgs->bread);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_TMDS_FREQ:
            {

                pstHDMI_GET_TMDS_FREQ pXCArgs = (pstHDMI_GET_TMDS_FREQ) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_TMDS_freq(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS:
            {

                pstHDMI_GET_POLLINGSTATUS pXCArgs = (pstHDMI_GET_POLLINGSTATUS) pArgs;
                psXCInstPri->fpHDMI_Get_PollingStatus(pInstance, pXCArgs->pStatus);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ISHDMI_MODE:
            {

                pstHDMI_ISHDMI_MODE pXCArgs = (pstHDMI_ISHDMI_MODE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_IsHDMI_Mode(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_MID_INFO:
            {

                pstHDMI_GET_MID_INFO pXCArgs = (pstHDMI_GET_MID_INFO) pArgs;
                psXCInstPri->fpHDMI_Get_MID_info(pInstance, pXCArgs->pu8ManufactorData);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_PACKET_VALUE:
            {

                pstHDMI_GET_PACKET_VALUE pXCArgs = (pstHDMI_GET_PACKET_VALUE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_get_packet_value(pInstance,
                                                                             pXCArgs->u8state,
                                                                             pXCArgs->u8byte_idx,
                                                                             pXCArgs->pValue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_CHANNELPHASESTATUS:
            {

                pstDVI_CHANNELPHASESTATUS pXCArgs = (pstDVI_CHANNELPHASESTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpDVI_ChannelPhaseStatus(pInstance, pXCArgs->u8Channel);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_SOFTWARERESET:
            {

                pstDVI_SOFTWARERESET pXCArgs = (pstDVI_SOFTWARERESET) pArgs;
                psXCInstPri->fpDVI_SoftwareReset(pInstance, pXCArgs->u16Reset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_RESET:
            {

                psXCInstPri->fpdvi_reset(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT:
            {
                pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT pXCArgs = (pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Get_AVIInfoActiveInfoPresent(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_CLKPULLLOW:
            {

                pstDVI_CLKPULLLOW pXCArgs = (pstDVI_CLKPULLLOW) pArgs;
                psXCInstPri->fpDVI_ClkPullLow(pInstance,
                                              pXCArgs->bPullLow,
                                              pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_SWITCHSRC:
            {

                pstDVI_SWITCHSRC pXCArgs = (pstDVI_SWITCHSRC) pArgs;
                psXCInstPri->fpDVI_SwitchSrc(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ:
            {

                pstHDMI_SETFORCLOCKLESSTHAN70MHZ pXCArgs = (pstHDMI_SETFORCLOCKLESSTHAN70MHZ) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_SetForClockLessThan70Mhz(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DVI_ADJUST:
            {

                pstHDMI_DVI_ADJUST pXCArgs = (pstHDMI_DVI_ADJUST) pArgs;
                psXCInstPri->fpHDMI_dvi_adjust(pInstance, pXCArgs->bClockLessThan70MHz);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT:
            {

                pstHDMI_SETUPDATEPHASELINECOUNT pXCArgs = (pstHDMI_SETUPDATEPHASELINECOUNT) pArgs;
                psXCInstPri->fpHDMI_SetUpdatePhaseLineCount(pInstance, pXCArgs->enInputSourceType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_ENABLE:
            {

                pstHDCP_ENABLE pXCArgs = (pstHDCP_ENABLE) pArgs;
                psXCInstPri->fpHDCP_Enable(pInstance, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE:
            {
                pstHDMI_SET_HDCPENABLE pXCArgs = (pstHDMI_SET_HDCPENABLE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_SetHdcpEnable(pInstance, pXCArgs->enInputPortType, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_CLEARSTATUS:
            {

                psXCInstPri->fpHDCP_ClearStatus(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY:
            {

                pstHDCP_INITPRODUCTIONKEY pXCArgs = (pstHDCP_INITPRODUCTIONKEY) pArgs;
                psXCInstPri->fpHDCP_initproductionkey(pInstance, pXCArgs->pu8HdcpKeyData);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_GETSTATUS:
            {

                pstHDCP_GETSTATUS pXCArgs = (pstHDCP_GETSTATUS) pArgs;
                pXCArgs->stReturnValue = psXCInstPri->fpHDCP_GetStatus(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_VSYNC_END_EN:
            {

                pstHDCP_VSYNC_END_EN pXCArgs = (pstHDCP_VSYNC_END_EN) pArgs;
                psXCInstPri->fpHDCP_Vsync_end_en(pInstance, pXCArgs->bStable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_OUTPUT:
            {

                pstHDMI_AUDIO_OUTPUT pXCArgs = (pstHDMI_AUDIO_OUTPUT) pArgs;
                psXCInstPri->fpHDMI_audio_output(pInstance, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO:
            {

                pstHDMI_AUDIO_CP_HDR_INFO pXCArgs = (pstHDMI_AUDIO_CP_HDR_INFO) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_audio_cp_hdr_info(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS:
            {

                pstHDMI_AUDIO_CHANNEL_STATUS pXCArgs = (pstHDMI_AUDIO_CHANNEL_STATUS) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_audio_channel_status(pInstance, pXCArgs->u8byte);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETLIBVER:
            {

                pstHDMI_GETLIBVER pXCArgs = (pstHDMI_GETLIBVER) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetLibVer(pInstance, pXCArgs->ppVersion);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETINFO:
            {

                pstHDMI_GETINFO pXCArgs = (pstHDMI_GETINFO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetInfo(pInstance, pXCArgs->pInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETSTATUS:
            {

                pstHDMI_GETSTATUS pXCArgs = (pstHDMI_GETSTATUS) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetStatus(pInstance, pXCArgs->pStatus);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_READ_DDCRAM:
            {
				printf("[%s][%d] skip read \n",__func__,__LINE__);
               // pstHDMI_READ_DDCRAM pXCArgs = (pstHDMI_READ_DDCRAM) pArgs;
                printf("[%s][%d]\n",__func__,__LINE__);
				//psXCInstPri->fpHDMI_READ_DDCRAM(pInstance, pXCArgs->pstDDCRam_Info, pXCArgs->u32SizeOfInfo);
                   printf("[%s][%d]\n",__func__,__LINE__);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PROG_DDCRAM:
            {
                pstHDMI_PROG_DDCRAM pXCArgs = (pstHDMI_PROG_DDCRAM) pArgs;
                psXCInstPri->fpHDMI_PROG_DDCRAM(pInstance, pXCArgs->pstDDCRam_Info, pXCArgs->u32SizeOfInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE:
            {
                pstHDMI_GET_CONTENT_TYPE pXCArgs = (pstHDMI_GET_CONTENT_TYPE) pArgs;
                pXCArgs->enReturnValue = psXCInstPri->fpHDMI_Get_Content_Type(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY:
            {
                pstHDMI_GET_EXT_COLORIMETRY pXCArgs = (pstHDMI_GET_EXT_COLORIMETRY) pArgs;
                pXCArgs->enReturnValue = psXCInstPri->fpHDMI_Get_Ext_Colorimetry(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION:
            {
                pstHDMI_GET_PIXEL_REPETITION pXCArgs = (pstHDMI_GET_PIXEL_REPETITION) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_Pixel_Repetition(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CHECK_4K2K:
            {
                pstHDMI_CHECK_4K2K pXCArgs = (pstHDMI_CHECK_4K2K) pArgs;

                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Check4K2K(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS:
            {

                psXCInstPri->fpHDMI_3D_4Kx2K_Process(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_SetHPD_HL:
            {
                pstHDMI_SetHPD_HL pXCArgs = (pstHDMI_SetHPD_HL) pArgs;
                psXCInstPri->fpHDMI_SetHPD_HL(pInstance, pXCArgs->bTrue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN:
            {

                pstHDMI_AVG_SCALERINGDOWN pXCArgs = (pstHDMI_AVG_SCALERINGDOWN) pArgs;
                psXCInstPri->fpHDMI_AVG_ScaleringDown(pInstance, pXCArgs->bTrue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT:
            {

                pstHDMI_CHECK_ADDITIONAL_FORMAT pXCArgs = (pstHDMI_CHECK_ADDITIONAL_FORMAT) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Check_Additional_Format(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE:
            {

                pstHDMI_GET_3D_STRUCTURE pXCArgs = (pstHDMI_GET_3D_STRUCTURE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_3D_Structure(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_CMD_GET_3D_EXT_DATA:
            {

                pstHDMI_GET_3D_EXT_DATA pXCArgs = (pstHDMI_GET_3D_EXT_DATA) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_3D_Ext_Data(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_3D_META_FIELD:
            {

                pstHDMI_GET_3D_META_FIELD pXCArgs = (pstHDMI_GET_3D_META_FIELD) pArgs;
                psXCInstPri->fpHDMI_Get_3D_Meta_Field(pInstance, pXCArgs->pdata);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_VIC_CODE:
            {

                pstHDMI_GET_VIC_CODE pXCArgs = (pstHDMI_GET_VIC_CODE) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_VIC_Code(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE:
            {

                pstHDMI_GET_4KX2K_VIC_CODE pXCArgs = (pstHDMI_GET_4KX2K_VIC_CODE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_4Kx2K_VIC_Code(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ARC_PINCONTROL:
            {
                pstHDMI_ARC_PINCONTROL pXCArgs = (pstHDMI_ARC_PINCONTROL) pArgs;
                psXCInstPri->fpHDMI_ARC_PINControl(pInstance,
                                                   pXCArgs->enInputPortType,
                                                   pXCArgs->bEnable,
                                                   pXCArgs->bDrivingHigh);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET:
            {
                pstDVI_SOFTWARE_RESET pXCArgs = (pstDVI_SOFTWARE_RESET) pArgs;
                psXCInstPri->fpDVI_Software_Reset(pInstance,
                                                  pXCArgs->enInputPortType,
                                                  pXCArgs->u16Reset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_POWERSTATE:
            {
                pstHDMI_SET_POWERSTATE pXCArgs = (pstHDMI_SET_POWERSTATE) pArgs;
                pXCArgs->u32ReturnValue = psXCInstPri->fpHDMI_Set_PowerState(pInstance, pXCArgs->enPowerState);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_DVI_HF_ADJUST:
            {
                psXCInstPri->fpHDMI_dvi_hf_adjust(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING:
            {
                pstHDMI_CheckHDMI20_Setting pXCArgs = (pstHDMI_CheckHDMI20_Setting) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_checkhdmi20_setting(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_STABLE_POLLING:
            {
                psXCInstPri->fpHDMI_StablePolling(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_HDE:
            {

                pstHDMI_GETHVSTATUS pXCArgs = (pstHDMI_GETHVSTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDCP_GetHde(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_GET_VDE:
            {

                pstHDMI_GETHVSTATUS pXCArgs = (pstHDMI_GETHVSTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDCP_GetVde(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_GET_HTT:
            {

                pstHDMI_GETHVSTATUS pXCArgs = (pstHDMI_GETHVSTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDCP_GetHtt(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_GET_VTT:
            {

                pstHDMI_GETHVSTATUS pXCArgs = (pstHDMI_GETHVSTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDCP_GetVtt(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_SOURCE_VERSION:
            {
                pstHDMI_Get_Source_Version pXCArgs = (pstHDMI_Get_Source_Version) pArgs;
                pXCArgs->ucSourceVersion = psXCInstPri->fpHDMI_GetSourceVersion(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT:
            {
                pstHDMI_Set5VDetectGPIOSelect pXCArgs = (pstHDMI_Set5VDetectGPIOSelect) pArgs;
                psXCInstPri->fpHDMI_Set5VDetectGPIOSelect(pInstance, pXCArgs->ul5VDetectGPIOSelect);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS:
            {
                pstHDMI_Get_De_Stable_Status pXCArgs = (pstHDMI_Get_De_Stable_Status) pArgs;
                pXCArgs->bStatus = psXCInstPri->fpHDMI_GetDEStableStatus(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DDC_CHANNEL_ENABLE:
            {
                pstHDMI_DDC_Channel_Enable pXCArgs = (pstHDMI_DDC_Channel_Enable) pArgs;
                psXCInstPri->fpHDMI_DDCChannelEnable(pInstance, pXCArgs->enInputPortType, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

		/*************************** HDCP22 ***************************/
		case E_XC_HDMI_IF_CMD_HDCP22_INIT_CBFUNC:
			{
				pstHDCP22_InitCBFunc pXCArgs = (pstHDCP22_InitCBFunc) pArgs;

				psXCInstPri->fpHDCP22_InitCBFunc(pInstance, pXCArgs->pCBFunc, pXCArgs->pContext);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
		case E_XC_HDMI_IF_CMD_HDCP22_PORT_INIT:
			{
				pstHDCP22_PortInit pXCArgs = (pstHDCP22_PortInit) pArgs;

				psXCInstPri->fpHDCP22_PortInit(pInstance, pXCArgs->ucPortIdx);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
		case E_XC_HDMI_IF_CMD_HDCP22_POLLING_READ_DONE:
			{
				pstHDCP22_PollingReadDone pXCArgs = (pstHDCP22_PollingReadDone) pArgs;

				pXCArgs->bReturnValue = psXCInstPri->fpHDCP22_PollingReadDone(pInstance, pXCArgs->ucPortIdx);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
		case E_XC_HDMI_IF_CMD_HDCP22_ENABLE_CIPHER:
			{
				pstHDCP22_EnableCipher pXCArgs = (pstHDCP22_EnableCipher) pArgs;

				psXCInstPri->fpHDCP22_EnableCipher(pInstance, pXCArgs->ucPortType, pXCArgs->ucPortIdx, pXCArgs->bIsEnable);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
		case E_XC_HDMI_IF_CMD_HDCP22_SEND_MSG:
			{
				pstHDCP22_SendMsg pXCArgs = (pstHDCP22_SendMsg) pArgs;

				psXCInstPri->fpHDCP22_SendMsg(pInstance, pXCArgs->ucPortType, pXCArgs->ucPortIdx,
											pXCArgs->pucData, pXCArgs->dwDataLen, pXCArgs->pDummy);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
		case E_XC_HDMI_IF_CMD_HDCP22_HANDLER:
			{
				pstHDCP22_Handler pXCArgs = (pstHDCP22_Handler) pArgs;

				psXCInstPri->fpHDCP22_Handler(pInstance, pXCArgs->ucPortIdx);
				u32Return = UTOPIA_STATUS_SUCCESS;

				break;
			}
	/*************************** HDCP22 ***************************/
        case E_XC_HDMI_IF_CMD_NONE:
        case E_XC_HDMI_IF_CMD_MAX:
        default:
            {
                printf("Command %ld is not existed\n",u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}
