#ifndef _API_XC_PANEL_V2_C_
#define _API_XC_PANEL_V2_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "PNL_private.h"
#include "utopia.h"


// ========== Global definition ==========
void* pModulePNL = NULL;

void PNLRegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 u32ResourceStatusCheck[E_PNL_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    UtopiaModuleCreate(MODULE_PNL, 0, &pModulePNL);
    UtopiaModuleRegister(pModulePNL);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModulePNL, (FUtopiaOpen)PNLOpen, (FUtopiaClose)PNLClose, (FUtopiaIOctl)PNLIoctl);

    // 2. deal with resource  , PNL may need 2 resources, one is readonly, the other is XC resource
    void* psResource = NULL;
    void* psResource1 = NULL;

    // 3. resource can allocate private for internal use
    MS_U32 u32MaxDeviceNum = _XC_PNL_GET_DEVICE_NUM();
    MS_U32 i = 0;
    for(i = 0 ; i < u32MaxDeviceNum ;i++)
    {
        // need refine
        if(i == 0)
        {
            UtopiaModuleAddResourceStart(pModulePNL, E_PNL_POOL_ID_INTERNAL);
            u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL] = UtopiaResourceCreate("PNL_VARIABLE", sizeof(PNL_RESOURCE_PRIVATE), &psResource);
            UtopiaResourceRegister(pModulePNL, psResource, E_PNL_POOL_ID_INTERNAL);
            UtopiaModuleAddResourceEnd(pModulePNL, E_PNL_POOL_ID_INTERNAL);
        }
        else
        {
            UtopiaModuleAddResourceStart(pModulePNL, E_PNL_POOL_ID_INTERNAL1);
            u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL1] = UtopiaResourceCreate("PNL_VARIABLE1", sizeof(PNL_RESOURCE_PRIVATE), &psResource1);
            UtopiaResourceRegister(pModulePNL, psResource1, E_PNL_POOL_ID_INTERNAL1);
            UtopiaModuleAddResourceEnd(pModulePNL, E_PNL_POOL_ID_INTERNAL1);
        }
    }

    //4. init resource private members here   (aka, global variable)
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pPNLResourcePrivate));

    if (u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pPNLResourcePrivate->bResourceRegistered = TRUE;

        // apiPNL.c
        pPNLResourcePrivate->stapiPNL._bSkipTimingChange = FALSE;
        pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_ALL;
        pPNLResourcePrivate->stapiPNL._pu8GammaTab[0]=NULL;
        pPNLResourcePrivate->stapiPNL._pu8GammaTab[1]=NULL;
        pPNLResourcePrivate->stapiPNL._pu8GammaTab[2]=NULL;
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized = FALSE;
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled = FALSE;
        pPNLResourcePrivate->stapiPNL._u16PanelDCLK = 0;
        pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = DISABLE;
        pPNLResourcePrivate->stapiPNL.ePrevPowerState = E_POWER_MECHANICAL;
        pPNLResourcePrivate->stapiPNL._bGstreamerProcess = FALSE;
        pPNLResourcePrivate->stapiPNL._bCaptureProcess = FALSE;
        pPNLResourcePrivate->stapiPNL._bEPIPanel = FALSE;
        pPNLResourcePrivate->stapiPNL._bSTREnable = FALSE;

        // halPNL.c
        pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = 250;
        pPNLResourcePrivate->sthalPNL._u16PnlDefault_PECurrent = 0;
        pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = E_PNL_OUTPUT_CLK_DATA;
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 1; // 00:250mv, 01:350mv, 10:300mv, 11:200mv
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01;
        pPNLResourcePrivate->sthalPNL._usMOD_CALI_OFFSET = 0;
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE  = 0x1F; // 250mv:2'h15,    350mv:2'h1F,    300mv:2'h1A,   200mv:2'h10
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = 0;
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 0;
        pPNLResourcePrivate->sthalPNL._u8PnlDiffSwingLevel = 0x63;
        pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = FALSE;
    }

    if (psResource1 != NULL)
    {
        UtopiaResourceGetPrivate(psResource1,(void**)(&pPNLResourcePrivate));

        if (u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL1] == UTOPIA_STATUS_SHM_EXIST)
        {
            // do nothing, since it is already inited
        }
        else
        {
            // Init flow control related variables here. Other global variable should be
            // inited in each of its init function relatively.
            pPNLResourcePrivate->bResourceRegistered = TRUE;

            // apiPNL.c
            pPNLResourcePrivate->stapiPNL._bSkipTimingChange = FALSE;
            pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_ALL;
            pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized = FALSE;
            pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled = FALSE;
            pPNLResourcePrivate->stapiPNL._u16PanelDCLK = 0;
            pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = DISABLE;
            pPNLResourcePrivate->stapiPNL.ePrevPowerState = E_POWER_MECHANICAL;

            // halPNL.c
            pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = 250;
            pPNLResourcePrivate->sthalPNL._u16PnlDefault_PECurrent = 0;
            pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = E_PNL_OUTPUT_CLK_DATA;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01;
            pPNLResourcePrivate->sthalPNL._usMOD_CALI_OFFSET = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE  = 0x15;
            pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 0;
            pPNLResourcePrivate->sthalPNL._u8PnlDiffSwingLevel = 0x63;
            pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = FALSE;
        }
    }
}

MS_U32 PNLOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    PNL_INSTANCE_PRIVATE *psPNLInstPri = NULL;
    UtopiaInstanceCreate(sizeof(PNL_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psPNLInstPri);

    psPNLInstPri->u32DeviceID = 0;
    psPNLInstPri->fpPNL_GetLibVer = MApi_PNL_GetLibVer_U2;
    psPNLInstPri->fpPNL_GetInfo = MApi_PNL_GetInfo_U2;
    psPNLInstPri->fpPNL_GetStatus = MApi_PNL_GetStatus_U2;
    psPNLInstPri->fpPNL_GetStatusEx = MApi_PNL_GetStatusEx_U2;
    psPNLInstPri->fpPNL_SetDbgLevel = MApi_PNL_SetDbgLevel_U2;
    psPNLInstPri->fpPNL_IOMapBaseInit = MApi_PNL_IOMapBaseInit_U2;
    psPNLInstPri->fpPNL_PreInit = MApi_PNL_PreInit_U2;
    psPNLInstPri->fpPNL_Init = MApi_PNL_Init_U2;
    psPNLInstPri->fpPNL_Init_Ex = MApi_PNL_Init_Ex_U2;
    psPNLInstPri->fpPNL_SetOutput = MApi_PNL_SetOutput_U2;
    psPNLInstPri->fpPNL_ChangePanelType = MApi_PNL_ChangePanelType_U2;
    psPNLInstPri->fpPNL_TCONMAP_DumpTable = MApi_PNL_TCONMAP_DumpTable_U2;
    psPNLInstPri->fpPNL_TCONMAP_Power_Sequence = MApi_PNL_TCONMAP_Power_Sequence_U2;
    psPNLInstPri->fpPNL_TCON_Count_Reset = MApi_PNL_TCON_Count_Reset_U2;
    psPNLInstPri->fpPNL_TCON_Init = MApi_PNL_TCON_Init_U2;
    psPNLInstPri->fpPNL_GetDstInfo = MApi_PNL_GetDstInfo_U2;
    psPNLInstPri->fpPNL_Control_Out_Swing = MApi_PNL_Control_Out_Swing_U2;
    psPNLInstPri->fpPNL_Control_Preemphasis_Level = MApi_PNL_Control_Preemphasis_Level_U2;
    psPNLInstPri->fpPNL_Control_VREG_Level = MApi_PNL_Control_VREG_Level_U2;
    psPNLInstPri->fpPNL_Control_Out_PE_Current = MApi_PNL_Control_Out_PE_Current_U2;
    psPNLInstPri->fpPNL_Control_Out_PE_Current_Partial = MApi_PNL_Control_Out_PE_Current_Partial_U2;
    psPNLInstPri->fpPNL_SetOutputPattern = MApi_PNL_SetOutputPattern_U2;
    psPNLInstPri->fpPNL_Mod_Calibration_Setting = MApi_Mod_Calibration_Setting_U2;
    psPNLInstPri->fpPNL_Mod_Do_Calibration = MApi_Mod_Do_Calibration_U2;
    psPNLInstPri->fpPNL_BD_LVDS_Output_Type = MApi_BD_LVDS_Output_Type_U2;
    psPNLInstPri->fpPNL_SetLPLLTypeExt = MApi_PNL_SetLPLLTypeExt_U2;
    psPNLInstPri->fpPNL_Init_MISC = MApi_PNL_Init_MISC_U2;
    psPNLInstPri->fpPNL_Get_MISC_Status = MApi_PNL_GetMiscStatus_U2;
    psPNLInstPri->fpPNL_MOD_OutputConfig_User = MApi_PNL_MOD_OutputConfig_User_U2;
    psPNLInstPri->fpPNL_HWLVDSReservedtoLRFlag = MApi_PNL_HWLVDSReservedtoLRFlag_U2;
    psPNLInstPri->fpPNL_MOD_PVDD_Power_Setting = MApi_MOD_PVDD_Power_Setting_U2;
    psPNLInstPri->fpPNL_SetSSC_En = MApi_PNL_SetSSC_En_U2;
    psPNLInstPri->fpPNL_SetSSC_Fmodulation = MApi_PNL_SetSSC_Fmodulation_U2;
    psPNLInstPri->fpPNL_SetSSC_Rdeviation = MApi_PNL_SetSSC_Rdeviation_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_En = MApi_PNL_SetOSDSSC_En_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_Fmodulation = MApi_PNL_SetOSDSSC_Fmodulation_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_Rdeviation = MApi_PNL_SetOSDSSC_Rdeviation_U2;
    psPNLInstPri->fpPNL_SkipTimingChange = MApi_PNL_SkipTimingChange_U2;
    psPNLInstPri->fpPNL_PreSetModeOn = MApi_PNL_PreSetModeOn_U2;
    psPNLInstPri->fpPNL_OverDriver_Init = MApi_PNL_OverDriver_Init_U2;
    psPNLInstPri->fpPNL_OverDriver_Enable = MApi_PNL_OverDriver_Enable_U2;
    psPNLInstPri->fpPNL_Get_LPLL_Type = MApi_Pnl_Get_LPLL_Type_U2;
    psPNLInstPri->fpPNL_Get_TCON_Capability = MApi_PNL_Get_TCON_Capability_U2;
    psPNLInstPri->fpPNL_SetOSDCOutputType = MApi_PNL_SetOSDCOutputType_U2;
    psPNLInstPri->fpPNL_Video_Protect = MApi_PNL_VB1_Video_Protect_U2;
    psPNLInstPri->fpPNL_Packer_Check = MApi_PNL_Packer_Check_U2;
    psPNLInstPri->fpPNL_SetPairSwap = MApi_PNL_SetPairSwap_U2;
    psPNLInstPri->fpPNL_SetExt_LPLL_Type = MApi_PNL_SetExt_LPLL_Type_U2;
    psPNLInstPri->fpPNL_CalExtLPLLSETbyDClk = MApi_PNL_CalExtLPLLSETbyDClk_U2;
    psPNLInstPri->fpPNL_SetDiffSwingLevel = MApi_PNL_SetDiffSwingLevel_U2;
    psPNLInstPri->fpPNL_SetPowerState = MApi_PNL_SetPowerState_U2;
    psPNLInstPri->fpPNL_ForceSetPanelDCLK = MApi_PNL_ForceSetPanelDCLK_U2;
    psPNLInstPri->fpPNL_EnableInternalTermination = MApi_PNL_EnableInternalTermination_U2;
    psPNLInstPri->fpPNL_OutputDeviceHandshake = MApi_PNL_OutputDeviceHandshake_U2;
    psPNLInstPri->fpPNL_OutputDeviceOCHandshake = MApi_PNL_OutputDeviceOCHandshake_U2;
    psPNLInstPri->fpPNL_SetOutputInterlaceTiming = MApi_PNL_SetOutputInterlaceTiming_U2;
    psPNLInstPri->fpPNL_GetPanelData = MApi_PNL_GetPanelData_U2;
    psPNLInstPri->fpPNL_DumpPanelData = MApi_PNL_DumpPanelData_U2;
    psPNLInstPri->fpPNL_SetSSC = MApi_PNL_SetSSC_U2;
    psPNLInstPri->fpPNL_GetPanelOnTiming = MApi_PNL_GetPanelOnTiming_U2;
    psPNLInstPri->fpPNL_GetPanelOffTiming = MApi_PNL_GetPanelOffTiming_U2;
    psPNLInstPri->fpPNL_GetPanelDimCtrl = MApi_PNL_GetPanelDimCtrl_U2;
    psPNLInstPri->fpPNL_GetAllGammaTbl = MApi_PNL_GetAllGammaTbl_U2;
    psPNLInstPri->fpPNL_EnablePanel = MApi_PNL_EnablePanel_U2;
    psPNLInstPri->fpPNL_SetGammaTbl = MApi_PNL_SetGammaTbl_U2;
    psPNLInstPri->fpPNL_GetGammaTbl = MApi_PNL_GetGammaTbl_U2;
    psPNLInstPri->fpPNL_SetGammaValue = MApi_PNL_SetGammaValue_U2;
    psPNLInstPri->fpPNL_GetLPLLMode = MApi_PNL_GetLPLLMode_U2;
    psPNLInstPri->fpPNL_GetDefVFreq = MApi_PNL_GetDefVFreq_U2;
    psPNLInstPri->fpPNL_OSDC_Init_Status   = MApi_PNL_OSDC_Init_Status_U2;
    psPNLInstPri->fpPNL_VideoOSDConfig   = MApi_PNL_VideoOSDConfig_U2;
    psPNLInstPri->fpPNL_DivisionModeConfig   = MApi_PNL_DivisionModeConfig_U2;
	psPNLInstPri->fpPNL_Set_GstreamerProcess = MApi_PNL_Set_GstreamerProcess_U2;
	psPNLInstPri->fpPNL_Get_GstreamerProcess = MApi_PNL_Get_IsGstreamerProcess_U2;
    psPNLInstPri->fpPNL_Set_CaptureProcess = MApi_PNL_Set_CaptureProcess_U2;
	psPNLInstPri->fpPNL_Get_CaptureProcess = MApi_PNL_Get_IsCaptureProcess_U2;
	psPNLInstPri->fpPNL_SET_MOD_PATTERN = MApi_PNL_SetModPattern_U2;
	psPNLInstPri->fpPNL_Set_STREnable = MApi_PNL_Set_STREnable_U2;
    psPNLInstPri->fpPNL_Set_PanelInch = MApi_PNL_Set_PanelInch_U2;
    psPNLInstPri->fpPNL_Set_FlagCus = MApi_PNL_Set_FlagCus_U2;
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 PNLIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    PNL_INSTANCE_PRIVATE* psPNLInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psPNLInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);

    pu32PNLInst_private = pInstance;

    switch(u32Cmd)
    {
        case E_PNL_CMD_GETLIBVER:
        {
            pstPNL_GetLibVer pPNLArgs = (pstPNL_GetLibVer)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_GetLibVer(pInstance, pPNLArgs->ppVersion);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETINFO:
        {
            pstPNL_GetInfo pPNLArgs = (pstPNL_GetInfo)pArgs;
            pPNLArgs->pApiInfo = psPNLInstPri->fpPNL_GetInfo(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETSTATUS:
        {
            pstPNL_GetStatus pPNLArgs = (pstPNL_GetStatus)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetStatus(pInstance, pPNLArgs->pPnlStatus);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETSTATUSEX:
        {
            pstPNL_GetStatusEx pPNLArgs = (pstPNL_GetStatusEx)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetStatusEx(pInstance, pPNLArgs->pPnlExtStatus);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETDBGLEVEL:
        {
            pstPNL_SetDbgLevel pPNLArgs = (pstPNL_SetDbgLevel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetDbgLevel(pInstance, pPNLArgs->u16DbgSwitch);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_IOMAPBASEINIT:
        {
            pstPNL_IOMapBaseInit pPNLArgs = (pstPNL_IOMapBaseInit)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_IOMapBaseInit(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_PREINIT:
        {
            pstPNL_PreInit pPNLArgs = (pstPNL_PreInit)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_PreInit(pInstance, pPNLArgs->eInitParam);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT:
        {
            pstPNL_Init pPNLArgs = (pstPNL_Init)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Init(pInstance, pPNLArgs->pSelPanelType, pPNLArgs->bEPIType);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT_EX:
        {
            pstPNL_Init_Ex pPNLArgs = (pstPNL_Init_Ex)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Init_Ex(pInstance, pPNLArgs->pSelPanelType, pPNLArgs->bEPIType, pPNLArgs->LIBVER);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOUTPUT:
        {
            pstPNL_SetOutput pPNLArgs = (pstPNL_SetOutput)pArgs;
            psPNLInstPri->fpPNL_SetOutput(pInstance, pPNLArgs->eOutputMode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CHANGEPANELTYPE:
        {
            pstPNL_ChangePanelType pPNLArgs = (pstPNL_ChangePanelType)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_ChangePanelType(pInstance, pPNLArgs->pSelPanelType, pPNLArgs->bEPIType);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCONMAP_DUMPTABLE:
        {
            pstPNL_TCONMAP_DumpTable pPNLArgs = (pstPNL_TCONMAP_DumpTable)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_TCONMAP_DumpTable(pInstance, pPNLArgs->pTCONTable, pPNLArgs->u8Tcontype);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCONMAP_POWER_SEQUENCE:
        {
            pstPNL_TCONMAP_Power_Sequence pPNLArgs = (pstPNL_TCONMAP_Power_Sequence)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_TCONMAP_Power_Sequence(pInstance, pPNLArgs->pTCONTable, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCON_COUNT_RESET:
        {
            pstPNL_TCON_Count_Reset pPNLArgs = (pstPNL_TCON_Count_Reset)pArgs;
            psPNLInstPri->fpPNL_TCON_Count_Reset(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCON_INIT:
        {
            psPNLInstPri->fpPNL_TCON_Init(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETDSTINFO:
        {
            pstPNL_GetDstInfo pPNLArgs = (pstPNL_GetDstInfo)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetDstInfo(pInstance, pPNLArgs->pDstInfo, pPNLArgs->u32SizeofDstInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_CONTROL_VREG_LEVEL:
        {
            pstPNL_Control_Vreg_Level pPNLArgs = (pstPNL_Control_Vreg_Level)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_VREG_Level(pInstance, pPNLArgs->bEnable, pPNLArgs->u16Vreg_Level);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_CONTROL_PREEMPHASIS_LEVEL:
        {
            pstPNL_Control_Preemphasis_Level pPNLArgs = (pstPNL_Control_Preemphasis_Level)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_Preemphasis_Level(pInstance, pPNLArgs->bEnable, pPNLArgs->u16Preemphasis_Level);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_CONTROL_OUT_SWING:
        {
            pstPNL_Control_Out_Swing pPNLArgs = (pstPNL_Control_Out_Swing)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_Out_Swing(pInstance, pPNLArgs->u16Swing_Level);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CONTROL_OUT_PE_CURRENT:
        {
            pstPNL_Control_Out_PE_Current pPNLArgs = (pstPNL_Control_Out_PE_Current)pArgs;
          //  MS_ASSERT(NULL!=pPNLArgs);
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_Out_PE_Current(pInstance, pPNLArgs->u16Current_Level);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_CONTROL_OUT_PE_CURRENT_PARTIAL:
        {
            pstPNL_Control_Out_PE_Current_Partial pPNLArgs = (pstPNL_Control_Out_PE_Current_Partial)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_Out_PE_Current_Partial(pInstance, pPNLArgs->u8Current_Level_array);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_OUTPUT_PATTERN:
        {
            pstPNL_SetOutputPattern pPNLArgs = (pstPNL_SetOutputPattern)pArgs;
            psPNLInstPri->fpPNL_SetOutputPattern(pInstance, pPNLArgs->bEnable, pPNLArgs->u16Red, pPNLArgs->u16Green, pPNLArgs->u16Blue);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_CALIBRATION_SETTING:
        {
            pstPNL_Mod_Calibration_Setting pPNLArgs = (pstPNL_Mod_Calibration_Setting)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Mod_Calibration_Setting(pInstance, pPNLArgs->pstModCaliInfo);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_DO_CALIBRATION:
        {
            pstPNL_Mod_Do_Calibration pPNLArgs = (pstPNL_Mod_Do_Calibration)pArgs;
            pPNLArgs->bReturn= psPNLInstPri->fpPNL_Mod_Do_Calibration(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_BD_LVDS_OUTPUT_TYPE:
        {
            pstPNL_BD_LVDS_Output_Type pPNLArgs = (pstPNL_BD_LVDS_Output_Type)pArgs;
            psPNLInstPri->fpPNL_BD_LVDS_Output_Type(pInstance, pPNLArgs->Type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETLPLLTYPEEXT:
        {
            pstPNL_SetLPLLTypeExt pPNLArgs = (pstPNL_SetLPLLTypeExt)pArgs;
            psPNLInstPri->fpPNL_SetLPLLTypeExt(pInstance, pPNLArgs->eLPLL_TypeExt);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT_MISC:
        {
            pstPNL_Init_MISC pPNLArgs = (pstPNL_Init_MISC)pArgs;
            psPNLInstPri->fpPNL_Init_MISC(pInstance, pPNLArgs->ePNL_MISC);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_MISC_STATUS:
        {
            pstPNL_Get_MISC_Status pPNLArgs = (pstPNL_Get_MISC_Status)pArgs;
            pPNLArgs->u32Status = psPNLInstPri->fpPNL_Get_MISC_Status(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_OUTPUTCONFIG_USER:
        {
            pstPNL_MOD_OutputConfig_User pPNLArgs = (pstPNL_MOD_OutputConfig_User)pArgs;
            psPNLInstPri->fpPNL_MOD_OutputConfig_User(pInstance, pPNLArgs->u32OutputCFG0_7, pPNLArgs->u32OutputCFG8_15, pPNLArgs->u32OutputCFG16_21);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG:
        {
            pstPNL_HWLVDSReservedtoLRFlag pPNLArgs = (pstPNL_HWLVDSReservedtoLRFlag)pArgs;
            psPNLInstPri->fpPNL_HWLVDSReservedtoLRFlag(pInstance, pPNLArgs->lvdsresinfo);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_PVDD_POWER_SETTING:
        {
            pstPNL_MOD_PVDD_Power_Setting pPNLArgs = (pstPNL_MOD_PVDD_Power_Setting)pArgs;
            psPNLInstPri->fpPNL_MOD_PVDD_Power_Setting(pInstance, pPNLArgs->bIs2p5);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_EN:
        {
            pstPNL_SetSSC_En pPNLArgs = (pstPNL_SetSSC_En)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_En(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_FMODULATION:
        {
            pstPNL_SetSSC_Fmodulation pPNLArgs = (pstPNL_SetSSC_Fmodulation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_Fmodulation(pInstance, pPNLArgs->u16Fmodulation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_RDEVIATION:
        {
            pstPNL_SetSSC_Rdeviation pPNLArgs = (pstPNL_SetSSC_Rdeviation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_Rdeviation(pInstance, pPNLArgs->u16Rdeviation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_EN:
        {
            pstPNL_SetOSDSSC_En pPNLArgs = (pstPNL_SetOSDSSC_En)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_En(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_FMODULATION:
        {
            pstPNL_SetOSDSSC_Fmodulation pPNLArgs = (pstPNL_SetOSDSSC_Fmodulation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_Fmodulation(pInstance, pPNLArgs->u16Fmodulation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_RDEVIATION:
        {
            pstPNL_SetOSDSSC_Rdeviation pPNLArgs = (pstPNL_SetOSDSSC_Rdeviation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_Rdeviation(pInstance, pPNLArgs->u16Rdeviation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SKIPTIMINGCHANGE:
        {
            pstPNL_SkipTimingChange pPNLArgs = (pstPNL_SkipTimingChange)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_SkipTimingChange(pInstance, pPNLArgs->bFlag);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_PRE_SET_MODE_ON:
        {
            pstPNL_PreSetModeOn pPNLArgs = (pstPNL_PreSetModeOn)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_PreSetModeOn(pInstance, pPNLArgs->bSetMode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OVERDRIVER_INIT:
        {
            pstPNL_OverDriver_Init pPNLArgs = (pstPNL_OverDriver_Init)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_OverDriver_Init(pInstance, pPNLArgs->pPNL_ODInitData, pPNLArgs->u32ODInitDataLen);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OVERDRIVER_ENABLE:
        {
            pstPNL_OverDriver_Enable pPNLArgs = (pstPNL_OverDriver_Enable)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_OverDriver_Enable(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_LPLL_TYPE:
        {
            pstPNL_Get_LPLL_Type pPNLArgs = (pstPNL_Get_LPLL_Type)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_Get_LPLL_Type(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_TCON_CAPABILITY:
        {
            pstPNL_Get_TCON_Capability pPNLArgs = (pstPNL_Get_TCON_Capability)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Get_TCON_Capability(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETPAIRSWAP:
        {
            pstPNL_SetPairSwap pPNLArgs = (pstPNL_SetPairSwap)pArgs;
            psPNLInstPri->fpPNL_SetPairSwap(pInstance, pPNLArgs->u32Polarity);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDCOUTPUTTYPE:
        {
            pstPNL_SetOSDCOutputType pPNLArgs = (pstPNL_SetOSDCOutputType)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
            psPNLInstPri->fpPNL_SetOSDCOutputType(pInstance,pPNLArgs->eLPLL_Type, pPNLArgs->eOC_OutputFormat);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_VIDEO_PROTECT:
        {
            pstPNL_SetVB1VideoProtect pPNLArgs = (pstPNL_SetVB1VideoProtect)pArgs;

            psPNLInstPri->fpPNL_Video_Protect(pInstance,pPNLArgs->enable);

            return UTOPIA_STATUS_SUCCESS;
        }




        case E_PNL_CMD_SETPACKER_CHECK:
        {
            pstPNL_SetOSDCOutputType pPNLArgs = (pstPNL_SetOSDCOutputType)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
            psPNLInstPri->fpPNL_Packer_Check(pInstance,pPNLArgs->eLPLL_Type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_EXT_LPLL_TYPE:
        {
            pstPNL_SetExt_LPLL_Type pPNLArgs = (pstPNL_SetExt_LPLL_Type)pArgs;
            psPNLInstPri->fpPNL_SetExt_LPLL_Type(pInstance, pPNLArgs->u16Ext_lpll_type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CALEXTLPLLSETBYDCLK:
        {
            pstPNL_CalExtLPLLSETbyDClk pPNLArgs = (pstPNL_CalExtLPLLSETbyDClk)pArgs;
            psPNLInstPri->fpPNL_CalExtLPLLSETbyDClk(pInstance, pPNLArgs->ldHz);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETDIFFSWINGLEVEL:
        {
            pstPNL_SetDiffSwingLevel pPNLArgs = (pstPNL_SetDiffSwingLevel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetDiffSwingLevel(pInstance, pPNLArgs->u8Swing_Level);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETPOWERSTATE:
        {
            pstPNL_SetPowerState pPNLArgs = (pstPNL_SetPowerState)pArgs;
            pPNLArgs->u32Return = psPNLInstPri->fpPNL_SetPowerState(pInstance, pPNLArgs->ePowerState);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_FORCESETPANELDCLK:
        {
            pstPNL_ForceSetPanelDCLK pPNLArgs = (pstPNL_ForceSetPanelDCLK)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_ForceSetPanelDCLK(pInstance, pPNLArgs->u16PanelDCLK, pPNLArgs->bSetDCLKEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_ENABLEINTERNALTERMINATION:
        {
            pstPNL_EnableInternalTermination pPNLArgs = (pstPNL_EnableInternalTermination)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_EnableInternalTermination(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OUTPUTDEVICEHANDSHAKE:
        {
            pstPNL_OutputDeviceHandshake pPNLArgs = (pstPNL_OutputDeviceHandshake)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_OutputDeviceHandshake(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OUTPUTDEVICEOCHANDSHAKE:
        {
            pstPNL_OutputDeviceOCHandshake pPNLArgs = (pstPNL_OutputDeviceOCHandshake)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_OutputDeviceOCHandshake(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOUTPUTINTERLACETIMING:
        {
            pstPNL_SetOutputInterlaceTiming pPNLArgs = (pstPNL_SetOutputInterlaceTiming)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOutputInterlaceTiming(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELDATA:
        {
            pstPNL_GetPanelData pPNLArgs = (pstPNL_GetPanelData)pArgs;
            psPNLInstPri->fpPNL_GetPanelData(pInstance, pPNLArgs->pstPNLData);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_DUMPPANELDATA:
        {
            psPNLInstPri->fpPNL_DumpPanelData(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC:
        {
            pstPNL_SetSSC pPNLArgs = (pstPNL_SetSSC)pArgs;
            psPNLInstPri->fpPNL_SetSSC(pInstance, pPNLArgs->u16Fmodulation, pPNLArgs->u16Rdeviation, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELONTIMING:
        {
            pstPNL_GetPanelOnTiming pPNLArgs = (pstPNL_GetPanelOnTiming)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetPanelOnTiming(pInstance, pPNLArgs->seq);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELOFFTIMING:
        {
            pstPNL_GetPanelOffTiming pPNLArgs = (pstPNL_GetPanelOffTiming)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetPanelOffTiming(pInstance, pPNLArgs->seq);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELDIMCTRL:
        {
            pstPNL_GetPanelDimCtrl pPNLArgs = (pstPNL_GetPanelDimCtrl)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetPanelDimCtrl(pInstance, pPNLArgs->dim_type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETALLGAMMATBL:
        {
            pstPNL_GetAllGammaTbl pPNLArgs = (pstPNL_GetAllGammaTbl)pArgs;
            pPNLArgs->ppu8Return = psPNLInstPri->fpPNL_GetAllGammaTbl(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_ENABLEPANEL:
        {
            pstPNL_EnablePanel pPNLArgs = (pstPNL_EnablePanel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_EnablePanel(pInstance, pPNLArgs->bPanelOn);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETGAMMATBL:
        {
            pstPNL_SetGammaTbl pPNLArgs = (pstPNL_SetGammaTbl)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetGammaTbl(pInstance, pPNLArgs->eGammaType, pPNLArgs->pu8GammaTab, pPNLArgs->Gamma_Map_Mode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETGAMMATBL:
        {
            pstPNL_GetGammaTbl pPNLArgs = (pstPNL_GetGammaTbl)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetGammaTbl(pInstance, pPNLArgs->eGammaType, pPNLArgs->pu8GammaTab, pPNLArgs->Gamma_Map_Mode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETGAMMAVALUE:
        {
            pstPNL_SetGammaValue pPNLArgs = (pstPNL_SetGammaValue)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetGammaValue(pInstance, pPNLArgs->u8Channel, pPNLArgs->u16Offset, pPNLArgs->u16GammaValue);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETLPLLMODE:
        {
            pstPNL_GetLPLLMode pPNLArgs = (pstPNL_GetLPLLMode)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetLPLLMode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETDEFVFREQ:
        {
            pstPNL_GetDefVFreq pPNLArgs = (pstPNL_GetDefVFreq)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetDefVFreq(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_PNL_OSDC_INIT_STATUS:
		{
			pstPNL_VideoOSDConfig pPNLArgs = (pstPNL_VideoOSDConfig)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);

			pPNLArgs->eReturn = psPNLInstPri->fpPNL_OSDC_Init_Status(pInstance, pPNLArgs->econfig);

			return UTOPIA_STATUS_SUCCESS;
		}

        case E_PNL_CMD_VIDEOOSD_CONFIG:
		{
			pstPNL_VideoOSDConfig pPNLArgs = (pstPNL_VideoOSDConfig)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			pPNLArgs->eReturn = psPNLInstPri->fpPNL_VideoOSDConfig(pInstance, pPNLArgs->econfig);

			return UTOPIA_STATUS_SUCCESS;
		}

        case E_PNL_CMD_DIVISIONMODE_CONFIG:
		{
			pstPNL_DivisionModeConfig pPNLArgs = (pstPNL_DivisionModeConfig)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			pPNLArgs->eReturn = psPNLInstPri->fpPNL_DivisionModeConfig(pInstance, pPNLArgs->econfig);

			return UTOPIA_STATUS_SUCCESS;
		}

		case E_PNL_CMD_SET_GSTREAMER_PROCESS:
		{
			pstPNL_Set_GstreamerProcess pPNLArgs = (pstPNL_Set_GstreamerProcess)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			psPNLInstPri->fpPNL_Set_GstreamerProcess(pInstance, pPNLArgs->bReturn);

			return UTOPIA_STATUS_SUCCESS;
		}

		case E_PNL_CMD_GET_GSTREAMER_PROCESS:
		{
			pstPNL_Get_GstreamerProcess pPNLArgs = (pstPNL_Get_GstreamerProcess)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			pPNLArgs->bReturn = psPNLInstPri->fpPNL_Get_GstreamerProcess(pInstance);

			return UTOPIA_STATUS_SUCCESS;
		}

        case E_PNL_CMD_SET_CAPTURE_PROCESS:
		{
			pstPNL_Set_CaptureProcess pPNLArgs = (pstPNL_Set_CaptureProcess)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			psPNLInstPri->fpPNL_Set_CaptureProcess(pInstance, pPNLArgs->bReturn);

			return UTOPIA_STATUS_SUCCESS;
		}

		case E_PNL_CMD_GET_CAPTURE_PROCESS:
		{
			pstPNL_Get_CaptureProcess pPNLArgs = (pstPNL_Get_CaptureProcess)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			pPNLArgs->bReturn = psPNLInstPri->fpPNL_Get_CaptureProcess(pInstance);

			return UTOPIA_STATUS_SUCCESS;
		}

        case E_PNL_CMD_SET_MOD_PATTERN:
        {
			pstPNL_SetModPattern pPNLArgs = (pstPNL_SetModPattern)pArgs;
            //MS_ASSERT(NULL!=pPNLArgs);
			psPNLInstPri->fpPNL_SET_MOD_PATTERN(pInstance, pPNLArgs->bEnable);
            return UTOPIA_STATUS_SUCCESS;
        }
        
        case E_PNL_CMD_SET_STRENABLE:
        {
            pstPNL_Set_STREnable pPNLArgs = (pstPNL_Set_STREnable)pArgs;
            psPNLInstPri->fpPNL_Set_STREnable(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_PANELINCH:
        {
            pstPNL_Set_PanelInch pPNLArgs = (pstPNL_Set_PanelInch)pArgs;
            psPNLInstPri->fpPNL_Set_PanelInch(pInstance, pPNLArgs->u16SwingLevel, pPNLArgs->u16PECurrent);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_FLAGCUS:
        {
            pstPNL_Set_FlagCus pPNLArgs = (pstPNL_Set_FlagCus)pArgs;
            psPNLInstPri->fpPNL_Set_FlagCus(pInstance, pPNLArgs->bPNL_InitByCus);

            return UTOPIA_STATUS_SUCCESS;
        }
        default:
            printf("Error Command = %ld\n", u32Cmd);
            break;
    }

    return UTOPIA_STATUS_FAIL;
}

MS_U32 PNLClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

#endif
