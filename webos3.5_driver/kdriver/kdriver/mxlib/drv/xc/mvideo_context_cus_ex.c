//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mvideo_context_ex.c
/// @brief  TEMP Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  MVIDEO_CONTEXT_EX_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif

#ifndef MSOS_TYPE_NOS
#define _GNU_SOURCE // need this _GNU_SOURCE because we need to check tid by "syscall"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/unistd.h>
#include <linux/types.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif
#endif

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"

#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "mhal_frc.h"
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "apiXC_DWIN.h"
#include "drv_sc_scaling.h"
#include "drv_sc_DIP_scaling.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "mhal_dip.h"
#include "drvXC_HDMI_Internal.h"
#include "drvMMIO.h"
#include "drvWBLE.h"
#include "apiXC_v2.h"
//#include "mhal_wble.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define MSXC_WR_DBG(x)  //x
#define SC_3D_DBG(x)    //x
#define SeamlessZapping_DBG 0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// panel related
//-----
typedef struct
{
    MS_BOOL bFRC_En;
    MS_U8   u8FRC_LPLL_Mode;

    MS_U16  u16MOD_SwingLevel;
    MS_U16  u16MOD_SwingCLK;

    MS_BOOL bdither6Bits;
    MS_BOOL blvdsShiftPair;
    MS_BOOL blvdsSwapPair;
    MS_U8   u8lvdsSwapCh;

    // EPI
    MS_BOOL bepiLRSwap;
    MS_BOOL bepiLMirror;
    MS_BOOL bepiRMirror;
} PNL_InitData_ADV_st, *p_PNL_InitData_ADV_st;

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//***************************************************
//             SCALER RELATED
//***************************************************


// *************************************************
//          ONLY FOR GSTREAMER SCALER START
// *************************************************
E_APIXC_ReturnValue MDrv_XC_Init_MISC_EXT(XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{

    MST_PANEL_INFO_t stFRCPanelInfo;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    if(u32InitMiscDataLen != sizeof(XC_INITMISC))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!SUPPORT_IMMESWITCH) && (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH ))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!SUPPORT_DVI_AUTO_EQ) && (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_DVI_AUTO_EQ) )
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return E_APIXC_RET_FAIL;
    }

    if ( (!FRC_INSIDE) &&
        ((pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE) || (pXC_Init_Misc->u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return E_APIXC_RET_FAIL;
    }

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    // Start only when all setting ready.
    memset(&stFRCPanelInfo, 0, sizeof(MST_PANEL_INFO_t));

    memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc, pXC_Init_Misc, u32InitMiscDataLen);
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitMiscDataLen_Suspend = u32InitMiscDataLen;
	printf("##Sara [DEBUG][%s][%d] =============\n", __FUNCTION__, __LINE__);

    #if (FRC_INSIDE)
    static MS_BOOL bIsMistInit = FALSE;
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
        || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)
        || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
        || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K))
    {

	    printf("##Sara [DEBUG][%s][%d] =============\n", __FUNCTION__, __LINE__);
        if (FALSE == bIsMistInit)
        {

		    printf("##Sara [DEBUG][%s][%d] =============\n", __FUNCTION__, __LINE__);
            MHal_CLKGEN_FRC_Init(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode);
            MDrv_FRC_PNLInfo_Transform(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo,
                                   &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc,
                                   &stFRCPanelInfo);
		    // when OUTPUT is 4k2k@60Hz then we turn on FRC
		    // in other case , we stil do FRC init but we choose to bypass FRC
            if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq < 600)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 3840)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 2160))
            {
                MDrv_FRC_ByPass_Enable(TRUE);
            }
            else
            {
                MDrv_FRC_ByPass_Enable(FALSE);
            }

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC) // init ready?! if yes, continue
            {
                printf("##Sara [DEBUG][%s][%d] =============\n", __FUNCTION__, __LINE__);
				MDrv_FRC_Init(&stFRCPanelInfo, &(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo));
                gSrcInfo[MAIN_WINDOW].Status2.bInFRCMode = TRUE; //Store FRC enable state in share memory for multi-process
                gSrcInfo[SUB_WINDOW].Status2.bInFRCMode = gSrcInfo[MAIN_WINDOW].Status2.bInFRCMode;
            }
            bIsMistInit = TRUE;
        }

		if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
		{
			// SW patch here, if AP set keep OP as 4K2K, need to adjust DE and Htt before FRC setting is 4K2K
			if (SC_R2BYTE(REG_SC_BK10_0C_L) < 3000)
			{
				if(MDrv_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
				{
#if SC2_ENABLE
                       //     _MLOAD_ENTRY(pInstance);
#else
                            _MLOAD_ENTRY();
#endif //#if SC2_ENABLE

					MDrv_XC_MLoad_WriteCmd(REG_SC_BK10_0C_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal, 0x1FFF);
					MDrv_XC_MLoad_WriteCmd(REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), 0x0FFF);

					MDrv_XC_MLoad_Fire(TRUE);
/*#if SC2_ENABLE
                            _MLOAD_RETURN(pInstance);
#else
                            _MLOAD_RETURN();
#endif //#if SC2_ENABLE*/
				}
				else
				{
					SC_W2BYTE(REG_SC_BK10_0C_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal);
					SC_W2BYTE(REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
				}
			}
		}
    }
    else
    {
        MDrv_FRC_ByPass_Enable(TRUE);
        printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    }
    #endif

    //printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_Init_MISC_EXT(XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
#if SC2_ENABLE
       // _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
#else
        _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    eReturn = MDrv_XC_Init_MISC_EXT(pXC_Init_Misc, u32InitMiscDataLen);
#if SC2_ENABLE
         //   _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR)
#else
            _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
#endif //#if SC2_ENABLE
    return eReturn;
}

// *************************************************
//          ONLY FOR GSTREAMER SCALER END
// *************************************************

//-------------------------------------------------------------------------------------------------
/// Set Line buffer mode of sub window
/// @return void
/// @param ENABLE for ring buffer mode, DISABLE for line buffer mode
//-------------------------------------------------------------------------------------------------
void MApi_SC_SetDisplay_LineBuffer_Mode(MS_BOOL bEnable)
{
    //no use
    //MDrv_SC_SetDisplay_LineBuffer_Mode(bEnable);
}

//-------------------------------------------------------------------------------------------------
/// Get Constant Alpha Value of Video
/// @return void
/// @param ENABLE to start at faling edge of Vsync_init or\n
///        "Disable" for start at advance 1 display line
//-------------------------------------------------------------------------------------------------
void MApi_SC_SetDisplay_Start_Mode(MS_BOOL bEnable)
{
    //no use
#if 0

    MDrv_SC_SetDisplay_Start_Mode(bEnable);
#endif
}


MS_U32 MApi_XC_GetFBSize_U2( void *pInstance,SCALER_WIN eWindow )
{

#if 1
    SC_FRAMEBUF_INFO_t FBInfo;
    Hal_SC_get_framebuf_Info(pInstance,&FBInfo, eWindow);

    if( eWindow == MAIN_WINDOW )
        return (MS_U32)FBInfo.u8FBNum;
    else
        return 0;//FIXME

#endif
}

MS_U32 MApi_XC_GetFBSize( SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_WINDOW XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FBSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#if 1
MS_U32 MApi_XC2_GetFBSize( SCALER_WIN eWindow )
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_WINDOW XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_FBSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
#endif


MS_U16   MApi_XC_Read2Byte(MS_U32 u32Reg)
{
    return (MS_U16)MDrv_Read2Byte( u32Reg );
}

MS_U32 MApi_XC_Read4Byte(MS_U32 u32Reg)
{
    return (MS_U32)MDrv_Read4Byte( u32Reg);
}

E_APIXC_ReturnValue MApi_SC_ForceInterlaceInPMode_U2(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    MDrv_SC_ForceInterlaceInPMode( pInstance,bEnable,  eWindow);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_SC_ForceInterlaceInPMode(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_FORCEINTERLACEINPMODE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FORCEINTERLACEINPMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


MS_BOOL MApi_XC_FPiIn_FPOut(SCALER_WIN eWindow)
{
//no use
#if 0
      XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
      UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
	if((E_XC_3D_INPUT_FRAME_PACKING == MApi_XC_Get_3D_Input_Mode(eWindow))
		&& MApi_XC_3D_Get_SpecialOutputTiming(E_XC_3D_EX_OUTPUT_1920X2160)
		&&MDrv_SC_GetInterlaceInPModeStatus(eWindow)
		&&(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)))
	{
		return TRUE;
	}
	else
	{
		//printf(" (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)) %d \n",(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)));
		//printf("  MApi_XC_Get_3D_Input_Mode(eWindow) %d \n",MApi_XC_Get_3D_Input_Mode(eWindow));
		//printf("  MApi_XC_3D_Get_SpecialOutputTiming(E_XC_3D_SEC_OUTPUT_1920X2160) %d \n",	MApi_XC_3D_Get_SpecialOutputTiming(E_XC_3D_SEC_OUTPUT_1920X2160));
		//printf("  MDrv_SC_GetInterlaceInPModeStatus(eWindow) %d \n",	MDrv_SC_GetFPIForce_PMode());
		return FALSE;
	}
#endif
	return FALSE;

}


//*****************************************
// WBLE
//****************************************

void MDrv_WBLE_init_riu_base(MS_U32 u32riu_base)
{
    u32riu_base=u32riu_base;
    //Hal_WBLE_init_riu_base(u32riu_base);
}

MS_BOOL MApi_WBLE_Init(void)
{
    MS_U32 u32NonPMBankSize, u32DLCRiuBaseAddr=0;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &u32DLCRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_XC ) != TRUE)
    {
        printf("WBLE GetBase failed\n");
        return FALSE;
    }

    //MDrv_WBLE_init_riu_base(u32DLCRiuBaseAddr);
    //MDrv_WBLE_Init();

    return TRUE;
}


void MApi_WBLE_EnableBLE(MS_BOOL bEnable)
{
    //MDrv_WBLE_EnableBLE(bEnable);
}


void MApi_WBLE_Set_SlopeValue(MS_U8 u8Slope)
{
    //MDrv_WBLE_Set_SlopeValue(u8Slope);
}

//*****************************************
// Panel related
//****************************************
#if 0
void MApi_XC_SetLVDS2ndChannelOnOff(MS_BOOL bOnOff)
{
	MHal_PNL_SetLVDSDualMode(bOnOff);
}
#endif


