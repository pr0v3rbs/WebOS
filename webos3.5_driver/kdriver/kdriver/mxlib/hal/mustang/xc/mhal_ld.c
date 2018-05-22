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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    Mdrv_ld.c
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//#include "Board.h"
#if 1//( LD_ENABLE )
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"

#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"

#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"

#include "apiXC_cus_ex.h"
#include "XC_private.h"

#include "mhal_ld.h"

#define LD_DMA_PORT  (sCusPtrMap.p_sDrvLdInfo->u8LdDmaSpiPort)
#if ENABLE_TWO_SPI
#define LD_DMA_PORT2  MSPI_PORT_NUM2
#endif

#define LD_BYTE_PER_WORD           16

t_sDrvLdCusPtrMap sCusPtrMap = 
{
NULL ,NULL ,NULL , NULL ,NULL ,NULL,
};

// h01[0] reg_local_dimming_en
void MHal_LD_Set_LocalDimmingEn(void *pInstance, MS_BOOL bEn)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, (bEn? BIT(0): 0), BIT(0));
}

MS_BOOL MHal_LD_Get_LocalDimmingEn(void *pInstance)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, BIT(0)) == BIT(0));
	return bRet; 
}

// h01[15] reg_ld_rst
void MHal_LD_Set_SoftReset(void *pInstance, MS_BOOL bEn)                  
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, (bEn? BIT(15): 0), BIT(15));
}

MS_BOOL MHal_LD_Get_SoftReset(void *pInstance)                      
{ 	
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, BIT(15)) == BIT(15));
	return bRet; 
}

// h19[19:0] reg_ratio_h_in
void MHal_LD_Set_InputHScalingDownRatio(void *pInstance, MS_U32 u32Ratio) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L, u32Ratio);
}

MS_U32  MHal_LD_Get_InputHScalingDownRatio(void *pInstance)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L) & 0x7FFFF);
	return u32Ret; 
}

// h1B[19:0] reg_ratio_v_in
void MHal_LD_Set_InputVScalingDownRatio(void *pInstance, MS_U32 u32Ratio) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L, u32Ratio); 
}

MS_U32  MHal_LD_Get_InputVScalingDownRatio(void *pInstance)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L) & 0x7FFFF); 
	return u32Ret; 
}

// h03[4:0] reg_bl_width
void MHal_LD_Set_LDF_BacklightWidth(void *pInstance, MS_U8 u8Width)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, u8Width-1, 0x1F); 
}

MS_U8 MHal_LD_Get_LDF_BacklightWidth(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, 0x1F) + 1); 
	return u8Ret; 
}

// h03[9:5] reg_bl_height
void MHal_LD_Set_LDF_BacklightHeight(void *pInstance, MS_U8 u8Height)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, ((MS_U16)(u8Height-1)) << 5, 0x3E0); 
}

MS_U8 MHal_LD_Get_LDF_BacklightHeight(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, 0x3E0) >> 5) + 1); 
	return u8Ret; 
}

// h04[25:0] reg_baseaddr0_l
void MHal_LD_Set_LDF_FrameBufBaseAddr_L0(void *pInstance, MS_U32 u32Addr) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_04_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_L0(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_04_L) & 0x03FFFFFF) * LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h06[25:0] reg_baseaddr1_l
void MHal_LD_Set_LDF_FrameBufBaseAddr_L1(void *pInstance, MS_U32 u32Addr) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_06_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_L1(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_06_L) & 0x03FFFFFF) * LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h08[25:0] reg_baseaddr0_r
void MHal_LD_Set_LDF_FrameBufBaseAddr_R0(void *pInstance, MS_U32 u32Addr) 
{ 
	UNUSED(pInstance);	
	UNUSED(u32Addr);	
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_08_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_R0(void *pInstance)        
{ 
	UNUSED(pInstance);	
	//MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_08_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD; 
	//return u32Ret; 
	return 0;
}

// h0a[25:0] reg_baseaddr1_r
void MHal_LD_Set_LDF_FrameBufBaseAddr_R1(void *pInstance, MS_U32 u32Addr) 
{ 
	UNUSED(pInstance);	
	UNUSED(u32Addr);	
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_0A_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_R1(void *pInstance)        
{ 
	UNUSED(pInstance);	
	//MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_0A_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD; 
	//return u32Ret; 
	return 0;
}

// h18[0] 
MS_BOOL gbAddrLimitEn = FALSE;
void MHal_LD_Set_LDF_AddrLimitEn(void *pInstance, MS_BOOL bEn)            
{ 
	UNUSED(pInstance);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_18_L, (bEn? BIT(0): 0), BIT(0));
	gbAddrLimitEn = bEn;
}

MS_BOOL MHal_LD_Get_LDF_AddrLimitEn(void *pInstance)                
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_18_L, BIT(0)) == BIT(0)); 
	//return bRet; 
	return gbAddrLimitEn;
}

// h1d[7:0] reg_pel_hlpf_thrd
void MHal_LD_Set_LDF_HLPFThreshold(void *pInstance, MS_U8 u8Th)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, u8Th, 0xFF); 
}

MS_U8   MHal_LD_Get_LDF_HLPFThreshold(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, 0xFF); 
	return u8Ret; 
}

// h1d[12] reg_pel_hlpf_en
void MHal_LD_Set_LDF_HLPFEn(void *pInstance, MS_BOOL bEn)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, (bEn? BIT(12): 0), BIT(12));
}

MS_BOOL MHal_LD_Get_LDF_HLPFEn(void *pInstance)                     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, BIT(12)) == BIT(12)); 
	return bRet; 
}

// h1d[13] reg_pel_hlpf_step
void MHal_LD_Set_LDF_HLPFStep(void *pInstance, MS_BOOL bEn)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, (bEn? BIT(13): 0), BIT(13));
}

MS_BOOL MHal_LD_Get_LDF_HLPFStep(void *pInstance)                   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, BIT(13)) == BIT(13)); 
	return bRet; 
}

// h1f[3:0] reg_alpha
void MHal_LD_Set_LDF_DCMaxAlpha(void *pInstance, MS_U8 u8Alpha)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, u8Alpha, 0x0F); 
}

MS_U8   MHal_LD_Get_LDF_DCMaxAlpha(void *pInstance)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x0F)); 
	return u8Ret; 
}

// h1f[7] reg_pel_white_mod_en
void MHal_LD_Set_LDF_PixelWhiteModeEn(void *pInstance, MS_BOOL bEn)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, (bEn? BIT(7): 0), BIT(7));
}

MS_BOOL MHal_LD_Get_LDF_PixelWhiteModeEn(void *pInstance)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, BIT(7)) == BIT(7)); 
	return bRet; 
}

// h1f[8] reg_write_dc_max_en
void MHal_LD_Set_LDF_WriteDCMaxEn(void *pInstance, MS_BOOL bEn)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, (bEn? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_LDF_WriteDCMaxEn(void *pInstance)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, BIT(8)) == BIT(8)); 
	return bRet; 
}

// h1f[9] reg_write_dc_max_of_en
void MHal_LD_Set_LDF_WriteDCMaxOFEn(void *pInstance, MS_BOOL bEn)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, (bEn? BIT(9): 0), BIT(9));
}

MS_BOOL MHal_LD_Get_LDF_WriteDCMaxOFEn(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, BIT(9)) == BIT(9)); 
	return bRet; 
}

// h22[11:0] reg_dummy0
void MHal_LD_Set_SW_PulseId(void *pInstance, MS_U16 u16Id)                
{ 	
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_22_L, u16Id, 0x0FFF); 
}

MS_U16  MHal_LD_Get_SW_PulseId(void *pInstance)                     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U16 u16Ret = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_22_L, 0x0FFF); 
	return u16Ret; 
}

// h22[15:12] reg_dummy0
void MHal_LD_Set_SW_PulseMode(void *pInstance, MS_U8 u8Mode)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_22_L, (MS_U16)u8Mode << 12, 0xF000); 
}

MS_U8   MHal_LD_Get_SW_PulseMode(void *pInstance)                   
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_22_L, 0xF000) >> 12); 
	return u8Ret; 
}

// h23[7:0] reg_dummy1
void MHal_LD_Set_SW_PulseLEDIntensity(void *pInstance, MS_U8 u8Intensity) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L, u8Intensity, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_PulseLEDIntensity(void *pInstance)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L, 0x00FF); 
	return u8Ret; 
}

// h23[15:8] reg_dummy1
void MHal_LD_Set_SW_PulseLDBIntensity(void *pInstance, MS_U8 u8Intensity) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L, (MS_U16)u8Intensity << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_PulseLDBIntensity(void *pInstance)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L, 0xFF00) >> 8); 
	return u8Ret; 
}

// h24[7:0] reg_dummy2
void MHal_LD_Set_SW_SpatialFilterStrength(void *pInstance, MS_U8 u8Str)   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, u8Str, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_SpatialFilterStrength(void *pInstance)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, 0x00FF); 
	return u8Ret; 
}

// h24[15:8] reg_dummy2
void MHal_LD_Set_SW_TemporalFilterStrengthDn(void *pInstance, MS_U8 u8Str)
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, (MS_U16)u8Str << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_TemporalFilterStrengthDn(void *pInstance)    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, 0xFF00) >> 8); 
	return u8Ret; 
}

// h25[7:0] reg_dummy3
void MHal_LD_Set_SW_LocalDimmingStrength(void *pInstance, MS_U8 u8Str)    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_25_L, u8Str, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_LocalDimmingStrength(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_25_L, 0x00FF); 
	return u8Ret; 
}

// h25[15:8] reg_dummy3
void MHal_LD_Set_SW_GlobalDimmingStrength(void *pInstance, MS_U8 u8Str)    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_25_L, (MS_U16)u8Str << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_GlobalDimmingStrength(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_25_L, 0xFF00) >> 8); 
	return u8Ret; 
}

// h30[19:0] reg_ratio_h_out
void MHal_LD_Set_OutputHScalingUpRatio(void *pInstance, MS_U32 u32Ratio)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_30_L, u32Ratio); 
}

MS_U32  MHal_LD_Get_OutputHScalingUpRatio(void *pInstance)          
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_30_L) & 0x0007FFFF; 
	return u32Ret; 
}

// h32[19:0] reg_ratio_v_out
void MHal_LD_Set_OutputVScalingUpRatio(void *pInstance, MS_U32 u32Ratio)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_32_L, u32Ratio); 
}

MS_U32  MHal_LD_Get_OutputVScalingUpRatio(void *pInstance)          
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_32_L) & 0x0007FFFF; 
	return u32Ret; 
}

// h37[0] reg_edge2d_gain_en
void MHal_LD_Set_Edge2DGainEn(void *pInstance, MS_BOOL bEn)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, (bEn? BIT(0): 0), BIT(0));
}

MS_BOOL MHal_LD_Get_Edge2DGainEn(void *pInstance)                   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, BIT(0)) == BIT(0)); 
	return bRet; 
}

// h37[1] reg_edge2d_en
void MHal_LD_Set_Edge2DEn(void *pInstance, MS_BOOL bEn)                   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, (bEn? BIT(1): 0), BIT(1));
}

MS_BOOL MHal_LD_Get_Edge2DEn(void *pInstance)                       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, BIT(1)) == BIT(1)); 
	return bRet; 
}

// h37[2] reg_edge2d_direct_type_en
void MHal_LD_Set_Edge2DDirectTypeEn(void *pInstance, MS_BOOL bEn)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, (bEn? BIT(2): 0), BIT(2));
}

MS_BOOL MHal_LD_Get_Edge2DDirectTypeEn(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, BIT(2)) == BIT(2)); 
	return bRet; 
}

// h37[4] reg_edge2d_pack16_en
MS_BOOL gbEdge2DPack16En = FALSE;
void MHal_LD_Set_Edge2DPack16En(void *pInstance, MS_BOOL bEn)             
{ 
	UNUSED(pInstance);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, (bEn? BIT(4): 0), BIT(4));
	gbEdge2DPack16En = bEn;
}

MS_BOOL MHal_LD_Get_Edge2DPack16En(void *pInstance)                 
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, BIT(4)) == BIT(4)); 
	//return bRet; 
	return gbEdge2DPack16En;
}

// h37[5] reg_bl_depack16b_mod
MS_BOOL gbSpiDepack16En = FALSE;
void MHal_LD_Set_SpiDepack16En(void *pInstance, MS_BOOL bEn)              
{ 
	UNUSED(pInstance);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, (bEn? BIT(5): 0), BIT(5));
	gbSpiDepack16En = bEn;
}

MS_BOOL MHal_LD_Get_SpiDepack16En(void *pInstance)                  
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_37_L, BIT(5)) == BIT(5)); 
	//return bRet; 
	return gbSpiDepack16En;
}

// h38[19:0] reg_lsf_h_init
void MHal_LD_Set_LSF_H_InitialPhase_L(void *pInstance, MS_U32 u32Phase)   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_38_L, u32Phase); 
}

MS_U32 MHal_LD_Get_LSF_H_InitialPhase_L(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_38_L) & 0x0007FFFF; 
	return u32Ret; 
}

// h55[19:0] reg_lsf_h_init_r
void MHal_LD_Set_LSF_H_InitialPhase_R(void *pInstance, MS_U32 u32Phase)   
{ 
	UNUSED(pInstance);	
	UNUSED(u32Phase);	
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_55_L, u32Phase); 
}

MS_U32 MHal_LD_Get_LSF_H_InitialPhase_R(void *pInstance)            
{ 
	UNUSED(pInstance);	
	//MS_U32 u32Ret = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_55_L) & 0x0007FFFF; 
	//return u32Ret; 
	return 0;
}

// h3a[19:0] reg_lsf_v_init
void MHal_LD_Set_LSF_V_InitialPhase(void *pInstance, MS_U32 u32Phase)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, u32Phase); 
}

MS_U32 MHal_LD_Get_LSF_V_InitialPhase(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L) & 0x0007FFFF; 
	return u32Ret; 
}

// h3c[0] reg_lsf_h_shift
void MHal_LD_Set_LSF_H_Boundary_L(void *pInstance, MS_BOOL bEn)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (bEn? BIT(0): 0), BIT(0));
}

MS_BOOL MHal_LD_Get_LSF_H_Boundary_L(void *pInstance)
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, BIT(0)) == BIT(0)); 
	return bRet; 
}

// h3c[15] reg_lsf_h_shift_r
void MHal_LD_Set_LSF_H_Boundary_R(void *pInstance, MS_BOOL bEn)           
{ 
	UNUSED(pInstance);	
	UNUSED(bEn);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (bEn? BIT(15): 0), BIT(15));
}

MS_BOOL MHal_LD_Get_LSF_H_Boundary_R(void *pInstance)               
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, BIT(15)) == BIT(15)); 
	//return bRet; 
	return 0;
}

// h3c[1] reg_lsf_v_shift
void MHal_LD_Set_LSF_V_Boundary(void *pInstance, MS_BOOL bEn)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (bEn? BIT(1): 0), BIT(1));
}

MS_BOOL MHal_LD_Get_LSF_V_Boundary(void *pInstance)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, BIT(1)) == BIT(1)); 
	return bRet; 
}

// h3c[3:2] reg_edge_level_shift
void MHal_LD_Set_EdgeLevelShift(void *pInstance, MS_U8 u8Mode)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, u8Mode << 2, 0x0C); 
}

MS_U8   MHal_LD_Get_EdgeLevelShift(void *pInstance)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, 0x0C) >> 2); 
	return u8Ret; 
}

// h3c[8:4] reg_cmp_blend
void MHal_LD_Set_CompensationBlend(void *pInstance, MS_U8 u8Alpha)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, ((MS_U16)u8Alpha) << 4, 0x01F0); 
}

MS_U8   MHal_LD_Get_CompensationBlend(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, 0x01F0) >> 4); 
	return u8Ret; 
}

// h3c[10] reg_bld_dither_en
void MHal_LD_Set_BlendingDitherEn(void *pInstance, MS_BOOL bEn)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (bEn? BIT(10): 0), BIT(10));
}

MS_BOOL MHal_LD_Get_BlendingDitherEn(void *pInstance)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, BIT(10)) == BIT(10)); 
	return bRet; 
}

// h3c[13] reg_cmp_dither_en
void MHal_LD_Set_CompensationDitherEn(void *pInstance, MS_BOOL bEn)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (bEn? BIT(13): 0), BIT(13));
}

MS_BOOL MHal_LD_Get_CompensationDitherEn(void *pInstance)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, BIT(13)) == BIT(13)); 
	return bRet; 
}

// h3d[11:0] reg_comp_lut
void MHal_LD_Write_CompensationLUT(void *pInstance, MS_U16 u16Lut)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3D_L, u16Lut, 0x0FFF); 
}

// h3e[11:0] reg_r_comp_lut (readonly)
MS_U16  MHal_LD_Read_CompensationLUT(void *pInstance)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U16 u16Ret = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3D_L, 0x0FFF); 
	return u16Ret; 
}

// h3f[7:0] reg_comp_addr
void MHal_LD_Set_CompensationLUTAddress(void *pInstance, MS_U8 u8Addr)    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, u8Addr, 0x00FF); 
}

MS_U8   MHal_LD_Get_CompensationLUTAddress(void *pInstance)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, 0x00FF); 
	return u8Ret; 
}

// h3f[10] reg_comp_wd
//MS_BOOL gbCompensationWriteEn = FALSE;
void MHal_LD_Set_CompensationWriteEn(void *pInstance, MS_BOOL bEn)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, (bEn? BIT(10): 0), BIT(10));
	//gbCompensationWriteEn = bEn;
}

MS_BOOL MHal_LD_Get_CompensationWriteEn(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, BIT(10)) == BIT(10)); 
	return bRet; 
	//return gbCompensationWriteEn;
}

// h3f[11] reg_comp_rd
//MS_BOOL gbCompensationReadEn = FALSE;
void MHal_LD_Set_CompensationReadEn(void *pInstance, MS_BOOL bEn)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, (bEn? BIT(11): 0), BIT(11));
	//gbCompensationReadEn = bEn;	
}

MS_BOOL MHal_LD_Get_CompensationReadEn(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, BIT(11)) == BIT(11)); 
	return bRet; 
	//return gbCompensationReadEn;
}

// h3f[12] reg_comp_mod, 1:incremental
void MHal_LD_Set_CompensationMode(void *pInstance, MS_BOOL bMode)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, (bMode? BIT(12): 0), BIT(12));
}

MS_BOOL MHal_LD_Get_CompensationMode(void *pInstance)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, BIT(12)) == BIT(12)); 
	return bRet; 
}

// h40[4] reg_read_addr_idx (readonly)
MS_BOOL MHal_LD_Get_CurFrameIdx(void *pInstance)                    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_40_L, BIT(4)) == BIT(4)); 
	return bRet; 
}

// h40[14] reg_hw_trig_mod
// h40[15] reg_vsync_select
// h41[7:0] reg_bl_width_led
MS_U8 gu8BackLightWidth = 0;
void MHal_LD_Set_LED_BacklightWidth(void *pInstance, MS_U8 u8Width)       
{ 
	UNUSED(pInstance);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, u8Width-1, 0xFF); 
	gu8BackLightWidth = u8Width;
}

MS_U8   MHal_LD_Get_LED_BacklightWidth(void *pInstance)             
{ 
	UNUSED(pInstance);	
	//MS_U8 u8Ret = (void *pInstance, MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, 0xFF) + 1); 
	//return u8Ret; 
	return gu8BackLightWidth;
}

// h41[15:8] reg_bl_height_led
MS_U8 gu8BackLightHeight = 0;
void MHal_LD_Set_LED_BacklightHeight(void *pInstance, MS_U8 u8Height)     
{ 
	UNUSED(pInstance);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, (void *pInstance, MS_U16)(u8Height - 1) << 8, 0xFF00); 
	gu8BackLightHeight = u8Height;
}

MS_U8   MHal_LD_Get_LED_BacklightHeight(void *pInstance)            
{ 
	UNUSED(pInstance);	
	//MS_U8 u8Ret = (void *pInstance, MS_U8)((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, 0xFF00) >> 8) + 1); 
	//return u8Ret; 
	return gu8BackLightHeight;
}

// h43[15:8] reg_dummy
void MHal_LD_Set_SW_TemporalFilterStrengthUp(void *pInstance, MS_U8 u8Str)
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_43_L, (MS_U16)u8Str << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_TemporalFilterStrengthUp(void *pInstance)    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_43_L, 0xFF00) >> 8); 
	return u8Ret; 
}

void MHal_LD_Set_SW_SpatialFilterModeEn(void *pInstance, MS_BOOL bEn)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, (bEn? BIT(2): 0), BIT(2));
}

MS_BOOL MHal_LD_Get_SW_SpatialFilterModeEn(void *pInstance)                  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(2)) == BIT(2)); 
	return bRet; 
}

// h44[3] reg_dummy10
void MHal_LD_Set_SW_WriteSPIEn(void *pInstance, MS_BOOL bEn)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, (bEn? BIT(3): 0), BIT(3));
}

MS_BOOL MHal_LD_Get_SW_WriteSPIEn(void *pInstance)                  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(3)) == BIT(3)); 
	return bRet; 
}

// h44[7] reg_dummy10
void MHal_LD_Set_SW_WriteLDBEn(void *pInstance, MS_BOOL bEn)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, (bEn? BIT(7): 0), BIT(7));
}

MS_BOOL MHal_LD_Get_SW_WriteLDBEn(void *pInstance)                  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(7)) == BIT(7)); 
	return bRet; 
}

// h44[12] reg_dummy10
void MHal_LD_Set_SW_AlgorithmEn(void *pInstance, MS_BOOL bEn)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, (bEn? BIT(12): 0), BIT(12));
}

MS_BOOL MHal_LD_Get_SW_AlgorithmEn(void *pInstance)                 
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(12)) == BIT(12)); 
	return bRet; 
}

// h44[13] reg_dummy10
void MHal_LD_Set_SW_ReadLDFEn(void *pInstance, MS_BOOL bEn)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, (bEn? BIT(13): 0), BIT(13));
}

MS_BOOL MHal_LD_Get_SW_ReadLDFEn(void *pInstance)                   
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(13)) == BIT(13)); 
	return bRet; 
}

// h44[15:14] reg_dummy10
void MHal_LD_Set_LEDType(void *pInstance, MS_U8 eType)                    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, eType << 14, BIT(14) | BIT(15)); 
}

MS_U8   MHal_LD_Get_LEDType(void *pInstance)                        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, BIT(14) | BIT(15)) >> 14); 
	return u8Ret; 
}

// h46[1] reg_background_w
// h46[5:2] reg_rpt_time
// h46[6] reg_comp_en
void MHal_LD_Set_CompensationEn(void *pInstance, MS_BOOL bEn)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (bEn? BIT(6): 0), BIT(6));
}

MS_BOOL MHal_LD_Get_CompensationEn(void *pInstance)                 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, BIT(6)) == BIT(6)); 
	return bRet; 
}

// h46[15] reg_2chip_mode_en
// h4b[7:0] reg_bl_globe
void MHal_LD_Set_BacklightGlobeGain(void *pInstance, MS_U8 u8Gain)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, u8Gain, 0xFF); 
}

MS_U8   MHal_LD_Get_BacklightGlobeGain(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, 0xFF); 
	return u8Ret; 
}

// h4b[8] reg_bl_globe_mod
void MHal_LD_Set_BacklightGlobeMode(void *pInstance, MS_BOOL bMode)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, (bMode? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_BacklightGlobeMode(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, BIT(8)) == BIT(8)); 
	return bRet; 
}

// h4b[10:9] reg_bl_data_alig
void MHal_LD_Set_BacklightDataAlign(void *pInstance, MS_U8 u8Align)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, (MS_U16)u8Align << 9, 0x0600); 
}

MS_U8   MHal_LD_Get_BacklightDataAlign(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, 0x0600) >> 9); 
	return u8Ret; 
}

// h4c[6:0] reg_bl_width
void MHal_LD_Set_LSF_BacklightWidth(void *pInstance, MS_U8 u8Width)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, u8Width-1, 0x1F); 
}

MS_U8   MHal_LD_Get_LSF_BacklightWidth(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, 0x1F) + 1); 
	return u8Ret; 
}

// h4c[14:8] reg_bl_height
void MHal_LD_Set_LSF_BacklightHeight(void *pInstance, MS_U8 u8Height)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, (MS_U16)(u8Height-1) << 5, 0x03E0); 
}

MS_U8   MHal_LD_Get_LSF_BacklightHeight(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, 0x03E0) >> 5) + 1); 
	return u8Ret; 
}

// h4D[25:0] reg_baseaddr0_l
void MHal_LD_Set_LDB_FrameBufBaseAddr_L0(void *pInstance, MS_U32 u32Addr) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4D_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_L0(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4D_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h4F[25:0] reg_baseaddr1_l
void MHal_LD_Set_LDB_FrameBufBaseAddr_L1(void *pInstance, MS_U32 u32Addr) 
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4F_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_L1(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4F_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h51[25:0] reg_baseaddr0_r
void MHal_LD_Set_LDB_FrameBufBaseAddr_R0(void *pInstance, MS_U32 u32Addr) 
{ 
	UNUSED(pInstance);	
	UNUSED(u32Addr);	
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_51_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_R0(void *pInstance)        
{
	UNUSED(pInstance);	
	//MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_51_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD;
	//return u32Ret; 
	return 0;
}

// h53[25:0] reg_baseaddr1_r
void MHal_LD_Set_LDB_FrameBufBaseAddr_R1(void *pInstance, MS_U32 u32Addr) 
{ 
	UNUSED(pInstance);	
	UNUSED(u32Addr);	
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_53_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_R1(void *pInstance)        
{ 
	UNUSED(pInstance);	
	//MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_53_L) & 0x03FFFFFF)*LD_BYTE_PER_WORD; 
	//return u32Ret; 
	return 0;
}

// h57[7:0] reg_dummy5
void MHal_LD_Set_SW_TemporalFilterLowTh(void *pInstance, MS_U8 u8Th)      
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_57_L, u8Th, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_TemporalFilterLowTh(void *pInstance)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_57_L, 0xFF)); 
	return u8Ret; 
}

// h57[15:8] reg_dummy5
void MHal_LD_Set_SW_TemporalFilterHighTh(void *pInstance, MS_U8 u8Th)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_57_L, (MS_U16)u8Th << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_TemporalFilterHighTh(void *pInstance)        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_57_L, 0xFF00) >> 8); 
	return u8Ret; 
}

// h58[7:0] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength2(void *pInstance, MS_U8 u8Dist)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_58_L, u8Dist, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_SpatialFilterStrength2(void *pInstance)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_58_L, 0xFF)); 
	return u8Ret; 
}

// h58[15:8] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength3(void *pInstance, MS_U8 u8Dist)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_58_L, (MS_U16)u8Dist << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_SpatialFilterStrength3(void *pInstance)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_58_L, 0xFF00) >> 8); 
	return u8Ret; 
}

// h58[15:8] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength4(void *pInstance, MS_U8 u8Dist)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_42_L, u8Dist, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_SpatialFilterStrength4(void *pInstance)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_42_L, 0xFF)); 
	return u8Ret; 
}

// h58[15:8] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength5(void *pInstance, MS_U8 u8Dist)  
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_42_L, (MS_U16)u8Dist << 8, 0xFF00); 
}

MS_U8   MHal_LD_Get_SW_SpatialFilterStrength5(void *pInstance)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_42_L, 0xFF00) >> 8); 
	return u8Ret; 
}
// h60[5:0] reg_bl_width_dma
void MHal_LD_Set_DMA_BacklightWidth(void *pInstance, MS_U8 u8Width)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, u8Width-1, 0x1F); 
}

MS_U8   MHal_LD_Get_DMA_BacklightWidth(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, 0x1F) + 1); 
	return u8Ret; 
}

// h60[9:5] reg_bl_height_dma
void MHal_LD_Set_DMA_BacklightHeight(void *pInstance, MS_U8 u8Height)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, (MS_U16)(u8Height-1) << 5, 0x3E0); 
}

MS_U8   MHal_LD_Get_DMA_BacklightHeight(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, 0x3E0) >> 5) + 1); 
	return u8Ret; 
}

// h63[4:0] reg_bl_width_led
void MHal_LD_Set_LDB_BacklightWidth(void *pInstance, MS_U8 u8Width)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, u8Width-1, 0x1F); 
}

MS_U8   MHal_LD_Get_LDB_BacklightWidth(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, 0x1F) + 1); 
	return u8Ret; 
}

// h63[9:5] reg_bl_height_led
void MHal_LD_Set_LDB_BacklightHeight(void *pInstance, MS_U8 u8Height)     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, (MS_U16)(u8Height-1) << 5, 0x3E0); 
}

MS_U8   MHal_LD_Get_LDB_BacklightHeight(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, 0x3E0) >> 5) + 1); 
	return u8Ret; 
}

// h64[3:0] reg_miu_pack_offset_0
void MHal_LD_Set_DMA0_PackOffset(void *pInstance, MS_U8 u8Offset)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_64_L, u8Offset, 0x0F); 
}

// h65[3:0] reg_miu_pack_length_0
void MHal_LD_Set_DMA0_PackLength(void *pInstance, MS_U8 u8Len)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L, u8Len, 0x0F); 
}

// h65[8] reg_dma0_en
void MHal_LD_Set_DMA0_En(void *pInstance, MS_BOOL bEn)                    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L, (bEn? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_DMA0_En(void *pInstance)                        
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L, BIT(8)) == BIT(8)); 
	return bRet; 
}

// h66[3:0] reg_miu_pack_offset_1
void MHal_LD_Set_DMA1_PackOffset(void *pInstance, MS_U8 u8Offset)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_66_L, u8Offset, 0x0F); 
}

// h67[3:0] reg_miu_pack_length_1
void MHal_LD_Set_DMA1_PackLength(void *pInstance, MS_U8 u8Len)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_67_L, u8Len, 0x0F); 
}

// h67[8] reg_dma1_en
void MHal_LD_Set_DMA1_En(void *pInstance, MS_BOOL bEn)                    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_67_L, (bEn? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_DMA1_En(void *pInstance)                        
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_67_L, BIT(8)) == BIT(8)); 
	return bRet; 
}

// h68[3:0] reg_miu_pack_offset_2
void MHal_LD_Set_DMA2_PackOffset(void *pInstance, MS_U8 u8Offset)         
{ 
	UNUSED(pInstance);	
	UNUSED(u8Offset);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_68_L, u8Offset, 0x1F); 
}

// h69[3:0] reg_miu_pack_length_2
void MHal_LD_Set_DMA2_PackLength(void *pInstance, MS_U8 u8Len)            
{ 
	UNUSED(pInstance);	
	UNUSED(u8Len);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_69_L, u8Len, 0x1F); 
}

// h69[8] reg_dma2_en
void MHal_LD_Set_DMA2_En(void *pInstance, MS_BOOL bEn)                    
{ 
	UNUSED(pInstance);	
	UNUSED(bEn);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_68_L, (bEn? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_DMA2_En(void *pInstance)                        
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_68_L, BIT(8)) == BIT(8)); 
	//return bRet; 
	return FALSE;
}

// h6a[3:0] reg_miu_pack_offset_3
void MHal_LD_Set_DMA3_PackOffset(void *pInstance, MS_U8 u8Offset)         
{ 
	UNUSED(pInstance);	
	UNUSED(u8Offset);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6A_L, u8Offset, 0x1F); 
}

// h6b[3:0] reg_miu_pack_length_3
void MHal_LD_Set_DMA3_PackLength(void *pInstance, MS_U8 u8Len)            
{ 
	UNUSED(pInstance);	
	UNUSED(u8Len);	
	//MDrv_WriteByteMask(REG_SC_BK2E_6B_L, u8Len, 0x1F); 
}

// h6b[8] reg_dma3_en
void MHal_LD_Set_DMA3_En(void *pInstance, MS_BOOL bEn)                    
{ 
	UNUSED(pInstance);	
	UNUSED(bEn);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6B_L, (bEn? BIT(8): 0), BIT(8));
}

MS_BOOL MHal_LD_Get_DMA3_En(void *pInstance)                        
{ 
	UNUSED(pInstance);	
	//MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6B_L, BIT(8)) == BIT(8)); 
	//return bRet; 
	return FALSE;
}

// h6c[5:0] reg_dma_yoff_end_0
void MHal_LD_Set_DMA0_YOffEnd(void *pInstance, MS_U8 u8End)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6C_L, u8End, 0x1F); 
}

// h6d[5:0] reg_dma_yoff_end_1
void MHal_LD_Set_DMA1_YOffEnd(void *pInstance, MS_U8 u8End)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6D_L, u8End, 0x1F); 
}

// h6e[5:0] reg_dma_yoff_end_2
void MHal_LD_Set_DMA2_YOffEnd(void *pInstance, MS_U8 u8End)               
{ 
	UNUSED(pInstance);	
	UNUSED(u8End);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6E_L, u8End, 0x3F); 
}

// h6f[5:0] reg_dma_yoff_end_3
void MHal_LD_Set_DMA3_YOffEnd(void *pInstance, MS_U8 u8End)               
{ 
	UNUSED(pInstance);	
	UNUSED(u8End);	
	//SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6F_L, u8End, 0x3F); 
}

// h70[12:0] reg_frm_width
void MHal_LD_Set_FrameWidth(void *pInstance, MS_U16 u16Width)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_70_L, u16Width-1, 0x1FFF); 
}

MS_U16  MHal_LD_Get_FrameWidth(void *pInstance)                     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U16 u16Ret = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_70_L, 0x1FFF) + 1); 
	return u16Ret; 
}

// h71[11:0] reg_frm_height
void MHal_LD_Set_FrameHeight(void *pInstance, MS_U16 u16Height)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_71_L, u16Height-1, 0x0FFF); 
}

MS_U16  MHal_LD_Get_FrameHeight(void *pInstance)                    
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U16 u16Ret = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_71_L, 0x0FFF) + 1); 
	return u16Ret; 
}

// h72[0] reg_debug_mod
void MHal_LD_Set_DebugMode(void *pInstance, MS_BOOL bMode)                
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, (bMode? BIT(0): 0), BIT(0));
}

MS_BOOL MHal_LD_Get_DebugMode(void *pInstance)                      
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, BIT(0)) == BIT(0)); 
	return bRet; 
}

// h72[3:1] reg_lsf_out_mod
void MHal_LD_Set_LSFOutMode(void *pInstance, MS_U8 u8Mode)                
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, u8Mode << 1, 0x0E); 
}

MS_U8   MHal_LD_Get_LSFOutMode(void *pInstance)                     
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, 0x0E) >> 1); 
	return u8Ret; 
}

// h72[5:4] reg_comp_out_mod
void MHal_LD_Set_CompensationOutMode(void *pInstance, MS_U8 u8Mode)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, u8Mode << 4, 0x30); 
}

MS_U8   MHal_LD_Get_CompensationOutMode(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, 0x30) >> 4); 
	return u8Ret; 
}

// h76[7:0] reg_dummy0
void MHal_LD_Set_SW_MinClampValue(void *pInstance, MS_U8 u8Val)           
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_43_L, u8Val, 0xFF); 
}

MS_U8   MHal_LD_Get_SW_MinClampValue(void *pInstance)               
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
		
	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_43_L, 0xFF)); 
	return u8Ret; 
}

// h76[11:8] reg_dummy0
void MHal_LD_Set_SW_BacklightInGamma(void *pInstance, MS_U8 u8Type)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, (MS_U16)u8Type << 8, 0x0F00); 
}

MS_U8   MHal_LD_Get_SW_BacklightInGamma(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, 0x0F00) >> 8); 
	return u8Ret; 
}

// h76[15:12] reg_dummy0
void MHal_LD_Set_SW_BacklightOutGamma(void *pInstance, MS_U8 u8Type)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, (MS_U16)u8Type << 12, 0xF000); 
}

MS_U8   MHal_LD_Get_SW_BacklightOutGamma(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, 0xF000) >> 12); 
	return u8Ret; 
}

// h77[25:0] reg_baseaddr_offset_spi_2
void MHal_LD_Set_SPI_BufBaseAddr_2(void *pInstance, MS_U32 u32Addr)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_77_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_SPI_BufBaseAddr_2(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_77_L) & 0x03FFFFFF) * LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h78[15] reg_nxt_spi_access_en
void MHal_LD_Set_NextSpiAccessEn(void *pInstance, MS_BOOL bEn)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_78_L, (bEn? BIT(15): 0), BIT(15));
}

MS_BOOL MHal_LD_Get_NextSpiAccessEn(void *pInstance)                
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_BOOL bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_78_L, BIT(15)) == BIT(15)); 
	return bRet; 
}

// h79[25:0] reg_edge_2d_baseaddr
void MHal_LD_Set_Edge2D_BufBaseAddr(void *pInstance, MS_U32 u32Addr)      
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_79_L, u32Addr/LD_BYTE_PER_WORD); 
}

MS_U32  MHal_LD_Get_Edge2D_BufBaseAddr(void *pInstance)             
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_79_L) & 0x03FFFFFF) * LD_BYTE_PER_WORD; 
	return u32Ret; 
}

// h7B[15] reg_spi_vsync_sel
void MHal_LD_Set_SPI_VsyncSel(void *pInstance, MS_BOOL bsrc)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_7B_L, (bsrc? BIT(15): 0), BIT(15));
}

void MHal_LD_Set_SPI_AvgData(void *pInstance, MS_U8 u8Value)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, u8Value, 0xFF);
}

MS_U8 MHal_LD_Get_SPI_AvgData(void *pInstance)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_41_L, 0xFF)); 
	return u8Ret; 
}

// h7E[25:0] reg_baseaddr_offset
void MHal_LD_Set_SPI_BufBaseAddr(void *pInstance, MS_U32 u32Addr)         
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32BaseAddr = MHal_LD_Get_LDB_FrameBufBaseAddr_L0(pInstance);
	if(u32Addr > u32BaseAddr)
	{
		u32Addr -= u32BaseAddr;
	}
	SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L, u32Addr/LD_BYTE_PER_WORD);
	//SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L, u32Addr/LD_BYTE_PER_WORD - SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4D_L) - LD_DMA_PORT * SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_77_L)); 
}

MS_U32  MHal_LD_Get_SPI_BufBaseAddr(void *pInstance)                
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U32 u32BaseAddr = MHal_LD_Get_LDB_FrameBufBaseAddr_L0(pInstance);

	MS_U32 u32Ret = (SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L) * LD_BYTE_PER_WORD);
	//MS_U32 u32Ret = ((SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4D_L) + SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L) + LD_DMA_PORT*SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_77_L)) & 0x03FFFFFF)*LD_BYTE_PER_WORD ;
	return u32Ret + u32BaseAddr; 
}

#if ENABLE_TWO_SPI
void MHal_LD_Set_DmaMiuPack_Offset_Length(void *pInstance, MS_U8 offset1, MS_U8 offset2, MS_U8 len)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_64_L+4*LD_DMA_PORT, offset1, 0x0F) ; //reg_miu_pack_offset_x
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L+4*LD_DMA_PORT, len, 0x0F) ; //reg_miu_pack_length_x

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_64_L+4*LD_DMA_PORT2, offset2, 0x0F) ; //reg_miu_pack_offset_x
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L+4*LD_DMA_PORT2, len, 0x0F) ; //reg_miu_pack_length_x
    
}
#else
void MHal_LD_Set_DmaMiuPack_Offset_Length(void *pInstance, MS_U8 offset, MS_U8 len)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_64_L+4*LD_DMA_PORT, offset, 0x0F) ; //reg_miu_pack_offset_x
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L+4*LD_DMA_PORT, len, 0x0F) ; //reg_miu_pack_length_x
}
#endif
void MHal_LD_Set_DmaEnable(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L+4*LD_DMA_PORT, (bEn? BIT(8): 0), BIT(8));
    #if ENABLE_TWO_SPI
	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L+4*LD_DMA_PORT2, (bEn? BIT(8): 0), BIT(8));
    #endif
}

void MHal_LD_Set_DmaAccessYEnd(void *pInstance, MS_U8 ylen)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6C_L+2*LD_DMA_PORT, ylen, 0x0F) ; //DMA access y end
    #if ENABLE_TWO_SPI
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6C_L+2*LD_DMA_PORT2, ylen, 0x0F) ; //DMA access y end
    #endif
}

void MHal_LD_InitU02(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_44_L, 0x11, 0xFF);    // ECO U02 for LD
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, BIT(15), BIT(15)); // Unicorn fix (partially) DC bug	
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, BIT(0), BIT(0)); 
}

void MHal_LD_SetMiuSelect(void *pInstance, MS_U8 u8Select)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	if(u8Select > 2)
	{
		return;
	}
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_0D_L, (u8Select << 8), (BIT(8) | BIT(9)));    // MIU select
}

MS_U8 MHal_LD_GetMiuSelect(void *pInstance)
{
	MS_U8 u8Select = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u8Select = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_77_L, (BIT(8) | BIT(9))) >> 8);    // MIU select

	if(u8Select > 2)
	{
		u8Select = 0;
	}

	return u8Select;
}

MS_BOOL gbSPIMirror = FALSE;
void MHal_LD_SetSPIMirror(void *pInstance, MS_BOOL bEnable)              
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	gbSPIMirror = bEnable;
}

MS_BOOL MHal_LD_GetSPIMirror(void *pInstance)                   
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	return gbSPIMirror; 
}


typedef enum
{
    E_LD_DUMMY_REG_334C = 0,
    E_LD_DUMMY_REG_334D,
    E_LD_DUMMY_REG_334E,
    E_LD_DUMMY_REG_3352,
    E_LD_DUMMY_REG_33EC,
    E_LD_DUMMY_REG_MAX,			
} E_LD_DUMMY_REG;

MS_U16 gu16DummyReg[E_LD_DUMMY_REG_MAX] = {0};
void MHal_LD_SetDummyReg(void *pInstance, MS_U16 u16Addr, MS_U16 u16Val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	switch(u16Addr)
	{
		case 0x334C:
			gu16DummyReg[E_LD_DUMMY_REG_334C] = u16Val;
			break;

		case 0x334D:
			gu16DummyReg[E_LD_DUMMY_REG_334D] = u16Val;
			break;

		case 0x334E:
			gu16DummyReg[E_LD_DUMMY_REG_334E] = u16Val;
			break;

		case 0x3352:
			gu16DummyReg[E_LD_DUMMY_REG_3352] = u16Val;
			break;

		case 0x33EC:
			gu16DummyReg[E_LD_DUMMY_REG_33EC] = u16Val;
			break;

		default:
			break;
	}
}

MS_U16 MHal_LD_GetDummyReg(void *pInstance, MS_U16 u16Addr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U16 ret = 0;
	switch(u16Addr)
	{
		case 0x334C:
			ret = gu16DummyReg[E_LD_DUMMY_REG_334C];
			break;

		case 0x334D:
			ret = gu16DummyReg[E_LD_DUMMY_REG_334D];
			break;

		case 0x334E:
			ret = gu16DummyReg[E_LD_DUMMY_REG_334E];
			break;

		case 0x3352:
			ret = gu16DummyReg[E_LD_DUMMY_REG_3352];
			break;

		case 0x33EC:
			ret = gu16DummyReg[E_LD_DUMMY_REG_33EC];
			break;

		default:
			break;
	}

	return ret;
}

// h72[5:4] reg_comp_out_mod
void MHal_LD_Set_MaxSpeedLimit(void *pInstance, MS_U8 u8Speed)       
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, u8Speed, 0xFF); 
}

MS_U8   MHal_LD_Get_MaxSpeedLimit(void *pInstance)            
{ 
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	MS_U8 u8Ret = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, 0xFF)); 
	return u8Ret; 
}

MS_BOOL gbEnableLedOutput = TRUE;
void MHal_LD_EnableLedOutput(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	gbEnableLedOutput	 = bEnable;
}

MS_BOOL MHal_LD_IsEnableLedOutput(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

	return gbEnableLedOutput;		
}

#endif

