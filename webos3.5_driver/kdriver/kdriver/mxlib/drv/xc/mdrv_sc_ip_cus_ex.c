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
/// file    Mdrv_sc_ip_ex.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  _MDRV_SC_IP_EX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
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
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_Internal.h"
#include "drv_xc_calibration.h"
#include "mhal_menuload.h"
#include "drv_sc_scaling.h"

#include "mhal_ip.h"
#include "mhal_ip_ex.h"
#include "mhal_mux.h"
#include "halCHIP.h"



#define PCMSG_TIMING(x)             //x

void MApi_XC_SetIPMUX_TestPattern( MS_BOOL bEnable, MS_U16 u16R_Cr, MS_U16 u16G_Y, MS_U16 u16B_Cb)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
    		return;
        }
    }

    stXC_Set_IPMUX_TestPattern XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u16R_Cr = u16R_Cr;
    XCArgs.u16G_Y = u16G_Y;
    XCArgs.u16B_Cb = u16B_Cb;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_Set_IPMUX_TestPattern, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
}


void MApi_XC_SetIPMUX_TestPattern_U2(void *pInstance, MS_BOOL bEnable, MS_U16 u16R_Cr, MS_U16 u16G_Y, MS_U16 u16B_Cb)
{

#if 1

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MDrv_XC_wait_input_vsync(pInstance,1,50,MAIN_WINDOW);
    Hal_SC_SetIPMUX_TestPattern(bEnable, u16R_Cr, u16G_Y, u16B_Cb);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
    MDrv_XC_wait_input_vsync(pInstance,1,50,MAIN_WINDOW);
    Hal_SC_SetIPMUX_TestPattern(bEnable, u16R_Cr, u16G_Y, u16B_Cb);
    _XC_RETURN();
#endif //#if SC2_ENABLE




 #endif
}

MS_BOOL MDrv_XC_PCMonitor_InvalidTimingDetect_YPbPr(void *pInstance,MS_BOOL *bSyncChanged, MS_BOOL bPollingOnly, SCALER_WIN eWindow)
{


#if 1
	MS_BOOL bResult = FALSE;
	XC_IP_SYNC_STATUS sXC_Sync_Status;
	memset(&sXC_Sync_Status, 0, sizeof(XC_IP_SYNC_STATUS));
	XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
	UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
	MDrv_XC_GetSyncStatus(pInstance,pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc, &sXC_Sync_Status, eWindow);

	if ( ( sXC_Sync_Status.u8SyncStatus & XC_MD_SYNC_LOSS ) ||	// no sync
		( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_VSYNC_POR_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_VSYNC_POR_BIT) ) ||  // sync polarity changed
		( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_HSYNC_POR_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_HSYNC_POR_BIT) ) ||
		( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_INTERLACE_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_INTERLACE_BIT) ) )
	{
		bResult = TRUE;
	 *bSyncChanged = TRUE;
		PCMSG_TIMING(printf("oldSyncStatus=0x%x, new=0x%x\n",
						pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus, sXC_Sync_Status.u8SyncStatus));
	}
	else // have signal
	{
		if ( abs( sXC_Sync_Status.u16Hperiod - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod ) > MD_HPERIOD_TORLANCE )
		{
			PCMSG_TIMING(printf("Hperiod=%x->%x, Tolerance %x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod, sXC_Sync_Status.u16Hperiod, (MS_U16)MD_HPERIOD_TORLANCE));
			bResult = TRUE;
		}
		else
		{
			//PCMSG(printf("HPD=%x\n", sXC_Sync_Status.u16Hperiod));
		}

		// HSync
		if ( bPollingOnly != TRUE )
			pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod = sXC_Sync_Status.u16Hperiod;

		if ( abs( sXC_Sync_Status.u16Vtotal - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal ) > MD_VTOTAL_TORLANCE )
		{
			PCMSG_TIMING(printf("VTT=%x->%x, Tolerance %x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal, sXC_Sync_Status.u16Vtotal,  (MS_U16)MD_VTOTAL_TORLANCE));
			bResult = TRUE;
		}
		else
		{
			//printf("Vtt %x\n", sXC_Sync_Status.u16Vtotal);
		}

		// VSync
		if ( bPollingOnly != TRUE )
			pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal = sXC_Sync_Status.u16Vtotal;

		if( IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc) || IsSrcTypeDVI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc) )
		{
			MS_WINDOW_TYPE sWin;
			memset(&sWin, 0, sizeof(MS_WINDOW_TYPE));

			MDrv_XC_GetDEWindow(pInstance,&sWin, eWindow);

			if (abs( sWin.width - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width) > MD_HDE_TORLANCE)
			{
				PCMSG_TIMING(printf("HDE %x->%x, Tolerance=%x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width, sWin.width, (MS_U16)MD_HDE_TORLANCE));
				bResult = TRUE;
			}

			if (abs(sWin.height - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height) > MD_VDE_TORLANCE)
			{
				PCMSG_TIMING(printf("VDE %x->%x, Tolerance=%x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height, sWin.height, (MS_U16)MD_VDE_TORLANCE));
				bResult = TRUE;
			}

			if ( bPollingOnly != TRUE )
				memcpy(&(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info), &sWin, sizeof(MS_WINDOW_TYPE));

		}
	}

	if(bPollingOnly != TRUE )
	{
		pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus = sXC_Sync_Status.u8SyncStatus;
	}

	return bResult; // stable sync
     #endif
}


void _XC_Restore_FD_Mask(void)
{
#if 0
    if ( MApi_XC_Get_FD_Mask()==ENABLE )
    {
        SC_W2BYTEMSK(REG_SC_BK01_26_L, (0), BIT(8));
        MDrv_XC_wait_output_vsync(6, 240, MAIN_WINDOW);
        SC_W2BYTEMSK(REG_SC_BK01_26_L, (MApi_XC_Get_FD_Mask()?BIT(8):0), BIT(8));
//        printf(" _XC_Restore_FD_Mask [private] : FD_MASK %s \r\n", _EXT_FD_MASK?"ON":"OFF");
    }
 #endif
}




#undef  _MDRV_SC_IP_EX_C_


