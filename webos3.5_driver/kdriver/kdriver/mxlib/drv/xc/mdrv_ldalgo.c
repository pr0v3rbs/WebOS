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
/// file    Mdrv_ldalgo.c
/// @brief  local dimming algorithm
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_LDALGO_C_

#include "MsCommon.h"
#include "MsOS.h"
#include "utopia_dapi.h"
#include "mhal_xc_chip_config.h"
#include "hwreg_sc.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
// Internal Definition
#include "mhal_ld.h"
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#include "mdrv_ld_gamma.h"
#include "mdrv_ld_led.h"

#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"



#if  (LD_ENABLE == 1)
//--------------------------------------------------------
//--------Local dimmming macro define---------------------
//--------------------------------------------------------
#define MAX_AVG_BLENDING                1
#define DIRECTTYPE_PATCH_ENABLE         0
#define ADD_OPPOSITE_LUMA               1

#define LUMA_AVG_DIFF_THRESHOLD1        0x38
#define LUMA_AVG_DIFF_THRESHOLD2        0x28
#define BIN_WIDTH                       16

#define LD_MEM_MODE
//#define LD_OVERFLOW_FEEDBACK
//#define LD_HISTOGRAM
//#define LD_LOWPASS_SPATIAL_FILTER
//#define LD_DC_BUG_PATCH_ON

#ifndef MIN
#define MIN(a,b)                 (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)                 (((a) > (b)) ? (a) : (b))
#endif

#define ABS(x)                    (((x) > 0) ? (x) : (-(x)))
#define DIFF(a,b)                (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define MINMAX(v,a,b)             (((v) < (a)) ? (a) : ((v) > (b)) ? (b) : (v))

#ifndef UNUSED
#define UNUSED(x)                 ((x) = (x))
#endif

#ifndef min
#define min(x,y) 		MIN(x,y) 
#endif

#ifndef max
#define max(x,y) 		MAX(x,y) 
#endif

//----------------------------------------------------------
//--------Local dimmming Local variables--------------------
//----------------------------------------------------------
#if 0 //
static Enu_LD_BLOCK_NUM s_a_LDBlockTBL[]={
 LD_BLOCK_10,   //LD_PANEL_LG32inch_LR10
 LD_BLOCK_10,   //LD_PANEL_LG37inch_LR10
 LD_BLOCK_16,   //LD_PANEL_LG42inch_LR16
 LD_BLOCK_16,   //LD_PANEL_LG47inch_LR16
 LD_BLOCK_16,   //LD_PANEL_LG55inch_LR16
 LD_BLOCK_TSB,   //LD_PANEL_TOSHIBA_LR16
 LD_BLOCK_TSB,   //LD_PANEL_TOSHIBA_TB16
 LD_BLOCK_DEFALUT, //===Empty===
 LD_BLOCK_32,   //LD_PANEL_LG50inchCinema_TB32
 LD_BLOCK_32,   //LD_PANEL_LG60inchCinema_TB32
 LD_BLOCK_72,   //LD_PANEL_LG47inch_Direct72
 LD_BLOCK_96,   //LD_PANEL_LG55inch_Direct96
 LD_BLOCK_480,  //LD_PANEL_LG72inch_Direct480
};
#endif

#ifdef LD_HISTOGRAM
static MS_U16 HISTOGRAM_bins[32];
#endif

//-------------------------------------------------------------
//--------Local dimmming internal interface--------------------
//-------------------------------------------------------------
void _MDrv_LDAlgo_AdjustSPIData(void *pInstance, MS_U16* BL_blocks)
{
	MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
	MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
	MS_U16 n_leds = led_width * led_height;
	MS_U16 block_idx = 0;
	//MS_U16 tmp;		
	pInstance = pInstance;

	for (block_idx = 0; block_idx < n_leds/2; block_idx++)
	{
		//tmp = BL_blocks[block_idx];
		BL_blocks[block_idx] = BL_blocks[n_leds - block_idx - 1];
		BL_blocks[n_leds - block_idx - 1] = BL_blocks[block_idx];
	}
}


void MDrv_appLDModeHandler_R2(void)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._stLDInfo = MDrv_LD_GetLDInfo();
    //MFC_3D_FORMAT e3DMode = MDrv_SysInfo_GetInternal3DMode();
    //Eun_LD_PANEL_TYPE eLDPanelType =  MDrv_SysInfo_GetPanelLDType();
    pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num = LD_BLOCK_16;///s_a_LDBlockTBL[eLDPanelType];

    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumV * pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightH * pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D = 0;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D_tmax = 0;
}

MS_U32 MDrv_Temporal_Max(MS_BOOL bLR_Flag)
{
    MS_U8 u8MaxTemp1 = 0;
    MS_U8 u8MaxTemp2 = 0;
    MS_U8 u8MaxTemp3 = 0;
    MS_U8 i = 0;
    MS_U8 j = 0;
    MS_U32 u32Luma_Sum = 0;
    MS_U32 u32Luma_Avg = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    for (i = 0; i < BIN_WIDTH; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] = 0;
    }

    if (bLR_Flag)
    {
        for (j = 0 ; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8MaxTemp1 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
                pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block_Pre[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
            }
        }
    }
    else
    {
        pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero = 0;
        for (j = 0 ; j <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0 ; i <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8MaxTemp1 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
                u8MaxTemp2 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block_Pre[j][i];
                u8MaxTemp3 = MAX(u8MaxTemp1, u8MaxTemp2);
                pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] = u8MaxTemp3;

                pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/BIN_WIDTH)]++;
                if(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] == 0)
                {
                    pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero++;
                }
            }
        }
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize == 0)
        {
            return 0;
        }
        else
        {
            return u32Luma_Avg = u32Luma_Sum/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize;
        }
    }

    return 0;
}



MS_U32 MDrv_FramLockBffdepack_CSep_8B_Full(void)      // depack per 4 pixel    = 128bit = 16 bytes
{
#if 0
    MS_U32 u32Ioffset = 0;           // Byte index of packed data
    MS_U8 u8Bpixel = 0, u8Rpixel = 0;
    MS_U32 u32Luma_Avg = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U8 u8LDWidth_Input = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth;
    MS_U8 u8LDHeight_Input = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight;
    MS_U8 u8MaxTemp1 = 0;

#if (MAX_AVG_BLENDING == 0)
    MS_U32 u32Block_Avg = 0;
    MS_U32 u32Block_Max = 0;
    //int block_alpha=0;
    MS_U32 u32Block_Alpha_Tmp = 0;
    //long u32Frame_Avg_Sum,u32Frame_Avg;
    MS_U8 u8Block_Avg_Thrd_High = 50;
    MS_U8 u8Block_Avg_Thrd_Low  = 34;
#endif
    pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum = 0;

    for (u8Rpixel = 0; u8Rpixel < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight; u8Rpixel++)
    {
        for (u8Bpixel = 0; u8Bpixel < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth; u8Bpixel++)
        {
            #if (MAX_AVG_BLENDING)
            if (pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0)
            {
                u8MaxTemp1 = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD*u8Rpixel);
            }
            else
            {
                u8MaxTemp1 = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD*u8Rpixel);
            }
            pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[u8Rpixel][u8Bpixel] = u8MaxTemp1;
            #else
            {
                //u32Block_Avg   = readb(BANK_MIU+Ipack+8*(u8Bpixel+_stLDPanelPara.u8LDWidth*u8Rpixel)+FramLockMbase);
                //u32Block_Max   = readb(BANK_MIU+Ipack+8*(u8Bpixel+_stLDPanelPara.u8LDWidth*u8Rpixel)+4+FramLockMbase);
                //block_alpha = readb(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x1F)) & 0xF;
                if (pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0)
                {
                    u32Block_Avg = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel));
                    u32Block_Max = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel) + 4);
                }
                else
                {
                    u32Block_Avg = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel));
                    u32Block_Max = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel) + 4);
                }

                if ( ABS(u32Block_Max-u32Block_Avg) >= u8Block_Avg_Thrd_High )
                {
                    u8MaxTemp1 = (u32Block_Max * 16 + u32Block_Avg * 0)/16;
                }
                else if ( (ABS(u32Block_Max-u32Block_Avg) < u8Block_Avg_Thrd_High) && (ABS(u32Block_Max-u32Block_Avg) >= u8Block_Avg_Thrd_Low) )
                {
                    u32Block_Alpha_Tmp = MAX(ABS(u32Block_Max - u32Block_Avg) - 34, 0);
                    u8MaxTemp1 = (u32Block_Max * u32Block_Alpha_Tmp + u32Block_Avg * (16 - u32Block_Alpha_Tmp))/16;
                }
                else
                {
                    u8MaxTemp1 = (u32Block_Max * 0 + u32Block_Avg * 16)/16;
                }

                pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[u8Rpixel][u8Bpixel] = u8MaxTemp1;
            }
            #endif
            pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum += u8MaxTemp1;
            u32Ioffset++;
        }
    }

    if ( (u8LDHeight_Input == 0) || (u8LDWidth_Input == 0) )
        return pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum;
    else
        return u32Luma_Avg = pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum/(u8LDHeight_Input * u8LDWidth_Input);
#else
	return 0;
#endif
}

MS_BOOL  MDrv_LD_Get_Value(void *pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize )
{
    MS_U8 u8Bpixel = 0, u8Rpixel = 0;
    MS_U8 i=0;
    MS_U32 u32HScalingDownRatio=0, u32VScalingDownRatio=0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    // boundary of weight / height
    if((u8WSize > 32 || u8WSize < 1) || (u8HSize > 18 || u8HSize < 1))
    {
        return FALSE;
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, 0 ,0xFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, 0 ,0xFF);

    //Calculate [9:5]Height [4:0]Width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, ((MS_U16)(u8HSize-1) << 5 |(u8WSize-1))   , 0x3FF);

    //input horizontal scaling down ratio = (W_ledx1024x1024/W_lcd)+1
    u32HScalingDownRatio = ((MS_U32)u8WSize * 1024 *1024 /pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width) +1 ;

    //input vertical scaling down ratio = (H_ledx1024x1024/H_lcd)+1
    u32VScalingDownRatio = ((MS_U32)u8HSize * 1024 *1024 /pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height) +1 ;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L, (MS_U16)(u32HScalingDownRatio&0xFFFF) ,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1A_L, (MS_U16)((u32HScalingDownRatio>> 16)&0xF) ,0xF); 
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L, (MS_U16)(u32VScalingDownRatio&0xFFFF)  ,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1C_L, (MS_U16)((u32VScalingDownRatio>> 16)&0xF) ,0xF); 

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0 ,0xF);

    for (u8Rpixel = 0; u8Rpixel < u8HSize; u8Rpixel++)
    {
        for (u8Bpixel = 0; u8Bpixel < u8WSize; u8Bpixel++)
        {
            pu8LDValue[i] = MDrv_LD_MIUReadByte(pInstance, LD_BUF_ReadDBuf_0, u8Bpixel + u8WSize*u8Rpixel);
            i++;
        }
    }
    return TRUE;

}

void MDrv_Output_SPIdata(void)
{
#if 0
    MS_U8 i = 0,j = 0;
    MS_U8 u8Tmp = 0;
    #if 0 //for debug printf
    MS_U8 h,v;
    #endif
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U32 u32WriteBuf = ((pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0) ? LD_BUF_SPIBuf_0 : LD_BUF_SPIBuf_1);
    MS_U8 u8Min_BLight_value = SC_R2BYTEMSK(REG_SC_BK2E_76_L, 0x00FF);//readb(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0xEC/2)) & 0xFF;

    MS_U32 u32Frame_Avg_Sum = 0,u32Frame_Avg = 0;

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            u8Tmp = pXCResourcePrivate->stdrv_LDALGO._u8LED_resort[i + j * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth];
            pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i] = MAX(u8Tmp, u8Min_BLight_value);
            u32Frame_Avg_Sum += pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i];
            pXCResourcePrivate->stdrv_LDALGO._u8LD_AlgoSPIBuffer[j * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth + i] = pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i];
        }
    }

    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh % 2) // odd patch
        {
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, 0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, 0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//8
        }
        else
        {
            if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 6)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//3
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//9
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//7
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//10
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//8
            }
            else
            {
                if (Mdrv_LD_GetPanelType() == E_LD_PANEL_CMO42inch_LR16)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);
                }
                else
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);
                }
            }
        }
    }
    else
    {
        for (j = 0 ; j <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, i + j*pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);
            }
        }
    }

    u32Frame_Avg = (u32Frame_Avg_Sum + (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize/2) )/(pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize);

    if (REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_PWM_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt + 1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20 + pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);//1
            }
        }
    }
    #if  0
    for( v = 0; v < 6; ++ v )
    {
        for( h = 0; h < 2; ++ h )
        {
            printf("0x%02X,", _u8LD_AlgoSPIBuffer[i] );
            i += 1;
        }
        printf("\n" );
    }
    #endif
    MDrv_LD_SetLocalDimmingData(pXCResourcePrivate->stdrv_LDALGO._u8LD_AlgoSPIBuffer, (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth));
#endif
}


void MDrv_Output_DigitalBLdata(void)
{
#if 0
    MS_U8 i = 0,j = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U32 u32WriteBuf = ((pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0) ? LD_BUF_WriteBuf_0 : LD_BUF_WriteBuf_1);

    if ( pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh % 2) // odd patch
        {
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//8
        }
        else
        {
            #if 0
            for(i = 0;i<_stLDPanelPara.u8PWMWidth;i++)
            {
                for(j=0 ; j< _stLDPanelPara.u8PWMHigh; j++)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, j+i*_u8pack_length_PWM ,_u8PWM_Block[j][i]);
                }
            }
            #endif
            if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 6)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//10
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//8
            }
            else
            {
                if (Mdrv_LD_GetPanelType() == E_LD_PANEL_CMO42inch_LR16)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]); //1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]); //2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]); //2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]); //4

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//10

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);//10

                }
                else
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);
                }
            }
        }
    }
    else
    {
        for (j = 0 ; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth;i++)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, i + j * pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);
            }
        }
    }
#endif
}


void MDrv_LD_3Ddownsample(MS_U8 u8ModeSelection_3D)
{
    MS_U8 i = 0,j = 0;
    MS_U8 u8MaxL = 0,u8MaxR = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    switch (u8ModeSelection_3D)
    {
        case 0:
        default:
            return;

        case 1:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2); i++)
                {
                    u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                    u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2];
                    pXCResourcePrivate->stdrv_LDALGO._u8SBSDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                }
            }
            break;

        case 2:
            if ( pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num != LD_BLOCK_480)
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                    }
                }
            }
            else
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight+1)/2; j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                    }
                }
            }
            break;

        case 3:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2); i++)
                {
                    u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                    u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2];
                    pXCResourcePrivate->stdrv_LDALGO._u8SBSDown_Block[j][i] = MAX(u8MaxL, 0);
                }
            }
            break;

        case 4:
            if (pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num != LD_BLOCK_480)
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, 0);
                    }
                }
            }
            else
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight+1)/2; j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, 0);
                    }
                }
            }
            break;
    }
}

void MDrv_Get_Histogram(void)
{
    MS_U8 i = 0,j = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero = 0;

    for (i = 0; i < BIN_WIDTH; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] = 0;
    }


    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight; j++)
    {
        for (i = 0 ; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[(pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i]/BIN_WIDTH)]++;
            if (pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i] < 3)
            {
                pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero++;
            }
        }
    }
}

void MDrv_BLMxNto2xN_White_8B(void)
{
    MS_U8 i = 0, j = 0;
    MS_U8 u8Max = 0, u8Itemp = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MDrv_Get_Histogram();

    MDrv_LD_3Ddownsample(pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D);

    switch (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D)
    {
        case 0:
        default:
            for (j = 0 ; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0 ; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i=i+pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio )
                {
                    u8Max = 0;
                    for (u8Itemp = 0 ; u8Itemp < pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio; u8Itemp++)
                    {
                        u8Max = MAX(u8Max,pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i+u8Itemp]);
                    }

                    pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i/pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio] = u8Max;
                }
            }
            break;

#if 0   //A5 no need 3D scaling
        case 1:
        case 3:
            for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
            {
                for (i = 0 ; i < (_stLDPanelPara.u8LDWidth/2); i=i+_u8H_Down_Ratio_3D)
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_Down_Ratio_3D ; u8Itemp++)
                        u8Max = MAX(u8Max,_u8SBSdown_Block[j][i+u8Itemp]);
                    _u8Hdown_Block[j][i/_u8H_Down_Ratio_3D] = u8Max;
                }
            }
            break;

        case 2:
        case 4:
            for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh/2); j++)
            {
                for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio)
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                        u8Max = MAX(u8Max,_u8TBdown_Block[j][i+u8Itemp]);

                    _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
                }
            }
            break;

        default:
            break;
#endif
    }

}


void MDrv_BL2xNto2xNd3_White_8B(void)
{
#if 0
    MS_U8 i =0,j = 0;
    MS_U8 u8Max = 0, u8Itemp = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    switch (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D)
    {
        case 0:
        case 1:
        case 3:
        default:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j = j + pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio )
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                {
                    u8Max = 0;
                    for (u8Itemp = 0; u8Itemp < pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio ; u8Itemp++)
                    {
                        u8Max = MAX(u8Max, pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j+u8Itemp][i]);
                    }

                    pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j/pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio][i] = u8Max;
                }
            }
            break;

        case 2:
        case 4:
            if (pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D == 0)
            {
                for (j = 0; j <  (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                    {
                        u8Max = pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j*2][i] = u8Max;
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j*2+1][i] = u8Max;
                    }
                }
            }
            else if (pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D == 1)
            {
                for (j = 0; j <  (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i];
                    }
                }
            }
            break;
    }

    if (REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_VDown_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt+1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20 + pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i , pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]);//1
            }
        }
    }
#endif
}

#if 0
void MDrv_BLMxNto2xN_White_8B_480block(void)
{
    int i, j, u8Itemp;
    int u8Max;

    MDrv_Get_histogram();

    MDrv_LD_3Ddownsample(_stLDPanelPara.u8LD_Mode2D3D);

    if(_stLDPanelPara.u8LD_Mode2D3D==0 )
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio )
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                    u8Max = MAX(u8Max,_u8Input_Block[j][i+u8Itemp]);
                    _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
                }
        }
    }
    else if(_stLDPanelPara.u8LD_Mode2D3D==1 || _stLDPanelPara.u8LD_Mode2D3D==3)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth/2); i++)
            {
                u8Max = _u8SBSdown_Block[j][i];
                _u8Hdown_Block[j][i*2] = u8Max;
                _u8Hdown_Block[j][i*2+1] = u8Max;
            }
        }
    }
    else if(_stLDPanelPara.u8LD_Mode2D3D==2 || _stLDPanelPara.u8LD_Mode2D3D==4)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh+1)/2 ; j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio)
            {
                u8Max = 0 ;
                for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                u8Max = MAX(u8Max,_u8TBdown_Block[j][i+u8Itemp]);
                _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
            }
        }
    }
}


void MDrv_BL2xNto2xNd3_White_8B_480block(void)
{
    int j, i;
    int u8Max;
    int u8Itemp;
    if( _stLDPanelPara.u8LD_Mode2D3D==0 || _stLDPanelPara.u8LD_Mode2D3D==1  || _stLDPanelPara.u8LD_Mode2D3D==3)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j=j+_u8V_down_ratio )
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth); i++)
            {
                u8Max = 0;
                for (u8Itemp = 0 ; u8Itemp < _u8V_down_ratio ; u8Itemp++)
                {
                    u8Max = MAX(u8Max,_u8Hdown_Block[j+u8Itemp][i]);
                }
                _u8Vdown_Block[j/_u8V_down_ratio][i] = u8Max;

            }
        }
    }
    else if( _stLDPanelPara.u8LD_Mode2D3D==2 ||  _stLDPanelPara.u8LD_Mode2D3D==4)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth); i++)
            {
                u8Max = MAX ( _u8Hdown_Block[j/2][i],_u8Hdown_Block[MAX(j-(j/2),0)][i] );
                _u8Vdown_Block[j][i] = u8Max;
            }
        }
    }
}
#endif

MS_U8 MDrv_Spatial_Filtering1D_Large(MS_U8 x_index, MS_U8 y_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 u8Tmp_Value_Adj[5];
    MS_S8  x = 0,y = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        for(y = -2; y < 3; y++)
        {
            if( ( (y+y_index >= 0)  && (y+y_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) )	 )
            {
                u8Tmp_Value_Adj[y+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y+y_index][x_index];
            }
            else
            {
                u8Tmp_Value_Adj[y+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x_index];
            }
        }
    }
    else
    {
        for(x = -2; x < 3; x++)
        {
            if( ( (x+x_index >= 0)  && (x+x_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh ) )  )
            {
                u8Tmp_Value_Adj[x+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x+x_index];
            }
            else
            {
                u8Tmp_Value_Adj[x+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x_index];
            }
        }
    }
    for(x = -2; x < 3; x++)
    {
        if(u8Tmp_Value_Adj[x+2] >= u8Max_Value)
        {
            u8Max_Value = u8Tmp_Value_Adj[x+2];
        }
    }
    if(u8Tmp_Value_Adj[2] == u8Max_Value )
    {
        return u8Max_Value;
    }
    else if(u8Tmp_Value_Adj[1] == u8Max_Value || u8Tmp_Value_Adj[3] == u8Max_Value)
    {
        if(u8Tmp_Value_Adj[2]>= MAX ( u8Max_Value -u8Spatial_Strength,0) )
        {
            return u8Tmp_Value_Adj[2];
        }
        else
        {
            return MAX( u8Max_Value - u8Spatial_Strength, 0);
        }
    }
    else if(u8Tmp_Value_Adj[0] == u8Max_Value || u8Tmp_Value_Adj[4] == u8Max_Value)
    {
        if(u8Tmp_Value_Adj[2] >= MAX ( u8Max_Value - 2*u8Spatial_Strength, 0) )
        {
            return u8Tmp_Value_Adj[2];
        }
        else
        {
            return MAX ( u8Max_Value - 2* u8Spatial_Strength,0);
        }
    }
	return 0;
}

MS_U8 MDrv_Spatial_Filtering2D(MS_U8 i_index, MS_U8 j_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 au8Tmp_Value_Adj[5][5];
    MS_U8 u8Max_Value_Array = 0, u8Max_Value_Array1 = 0, u8Max_Value_Array2 = 0;
    MS_U8 u8Loop_Circle = 2;
    MS_S8 i = 0,j = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    for (j = -2; j < 3; j++)
    {
        for (i = -2; i < 3; i++)
        {
            if (( (i+i_index >= 0)  && (i + i_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth ) )
                && ( (j+j_index >= 0)  && (j+j_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) ))
            {
                au8Tmp_Value_Adj[j+2][i+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j+j_index][i+i_index];
            }
            else
            {
                au8Tmp_Value_Adj[j+2][i+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }

    for (j = -2;j < 3; j++)
    {
        for ( i =-2; i < 3; i++)
        {
            if ( (au8Tmp_Value_Adj[j+2][i+2] > u8Max_Value_Array2) && (MAX( ABS(i),ABS(j)) == 2) )
            {
                u8Max_Value_Array2 = au8Tmp_Value_Adj[j+2][i+2];
                //u8Loop_Circle = 2;
            }
            if ( (au8Tmp_Value_Adj[j+2][i+2] > u8Max_Value_Array1) && (MAX( ABS(i),ABS(j)) == 1) )
            {
                u8Max_Value_Array1 = au8Tmp_Value_Adj[j+2][i+2];
                // u8Loop_Circle = 2;
            }
        }
    }

    if (u8Max_Value_Array1 >= u8Max_Value_Array2)
    {
        u8Loop_Circle = 1;
        u8Max_Value_Array = u8Max_Value_Array1;
    }
    else
    {
        u8Loop_Circle = 2;
        u8Max_Value_Array = u8Max_Value_Array2;
    }

    if ( au8Tmp_Value_Adj[2][2] >= u8Max_Value_Array)
    {
        u8Max_Value = au8Tmp_Value_Adj[2][2];
    }
    else
    {
        if (u8Loop_Circle == 2)
        {
            if (( au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = au8Tmp_Value_Adj[2][2];
            }
            else if ( ( au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 );
            }
            else if ( ( au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 );
            }
            else if ( ( au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array2-u8Spatial_Strength*2, 0 );
            }
        }
        else if (u8Loop_Circle == 1)
        {
            if (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1 - u8Spatial_Strength, 0 ) )
            {
                u8Max_Value = au8Tmp_Value_Adj[2][2];
            }
            else
            {
                u8Max_Value = MAX( u8Max_Value_Array1 -u8Spatial_Strength, 0 );
            }
        }
    }
    return u8Max_Value;
}

MS_U8 MDrv_Spatial_Filtering1D(MS_U8 i_index, MS_U8 j_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 au8Tmp_Value_Adj[3];
    MS_S8 i = 0,j = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        for (j = -1; j < 2; j++)
        {
            if ( ( (j+j_index>=0)  && (j+j_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) ))
            {
                au8Tmp_Value_Adj[j+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j+j_index][i_index];
            }
            else
            {
                au8Tmp_Value_Adj[j+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }
    else
    {
        for (i = -1; i < 2; i++)
        {
            if ( (i+i_index >= 0) && (i+i_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth ) )
            {
                au8Tmp_Value_Adj[i+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i+i_index];
            }
            else
            {
                au8Tmp_Value_Adj[i+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }

    if ( (au8Tmp_Value_Adj[1] >= au8Tmp_Value_Adj[0]) && (au8Tmp_Value_Adj[1] >= au8Tmp_Value_Adj[2]) )
    {
        u8Max_Value = au8Tmp_Value_Adj[1];
    }
    else if ( ( DIFF(au8Tmp_Value_Adj[1],au8Tmp_Value_Adj[0]) <= u8Spatial_Strength) && (DIFF(au8Tmp_Value_Adj[1],au8Tmp_Value_Adj[2])<=u8Spatial_Strength) )
    {
        u8Max_Value = au8Tmp_Value_Adj[1];
    }
    else
    {
        u8Max_Value = MAX (MAX(au8Tmp_Value_Adj[0],au8Tmp_Value_Adj[2]) - u8Spatial_Strength, 0);
    }
#if (ADD_OPPOSITE_LUMA)
    return MIN(u8Max_Value+pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][(i_index == 1 ? 0 : 1)]/2,0xFF);
#else
    return u8Max_Value;
#endif
}


void  MDrv_Maxnxn_FrameData_CSep_8B(MS_U32 u32Luma_Avg_Diff, MS_U8 u8Idx, MS_U32 u32Luma_Avg_OSD_diff, MS_U8 u8Spatial_Strength)
{
#if 0
    MS_U8 i = 0,j = 0;//, ChannelIdx;
    MS_U32 u32Frame_Factor =0;
    MS_U32 u32Frame_Factor_dec = 0;
    MS_U16 u16Pindex = 0,u16Pindex2 = 0;
    MS_U16 u16Tmp = 0;
    MS_U16 u16Tmp1 = 0;
    MS_U32 u32P_Tmp_Cur = 0, u32P_Tmp_Pre = 0, u32P_Tmp_IIR = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    u32Frame_Factor = SC_R2BYTEMSK(REG_SC_BK2E_24_L,0xFF00) >> 8;
    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            u16Pindex = ( j*pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth + i);
            u16Pindex2 = u16Pindex * 2;

            if ( (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2) || (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 2) )
            {
                u32P_Tmp_Cur = MDrv_Spatial_Filtering1D_Large(i, j, u8Spatial_Strength)*256;
            }
            else
            {
                u32P_Tmp_Cur = MDrv_Spatial_Filtering2D(i, j, u8Spatial_Strength)*256;
            }

            u16Tmp = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[u16Pindex2+1];
            u16Tmp1 = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[u16Pindex2];

            u32P_Tmp_Pre = u16Tmp*256 + u16Tmp1;

            if ( (u32Luma_Avg_Diff >= LUMA_AVG_DIFF_THRESHOLD1) /*|| (u32Luma_Avg_OSD_diff > 200*/)//To fix konka flicker bug. Mantis 0265653 )
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-0) + u32P_Tmp_Pre*0 + 0 )>>8;//div 256;
            }
            else if ( (u32Luma_Avg_Diff<LUMA_AVG_DIFF_THRESHOLD1) && (u32Luma_Avg_Diff>=LUMA_AVG_DIFF_THRESHOLD2))
            {
                u32Frame_Factor_dec = u32Frame_Factor - (u32Frame_Factor/16) * (u32Luma_Avg_Diff - LUMA_AVG_DIFF_THRESHOLD2);
                u32P_Tmp_IIR = (u32P_Tmp_Cur * (256-u32Frame_Factor_dec) + u32P_Tmp_Pre * u32Frame_Factor_dec + 0 )>>8;//div 256;
            }
            else  if ( u32Luma_Avg_Diff < LUMA_AVG_DIFF_THRESHOLD2)
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-u32Frame_Factor) + u32P_Tmp_Pre*u32Frame_Factor )>>8;//div 256;
            }

            MS_U8 u8Gamma_Min = SC_R2BYTEMSK(REG_SC_BK2E_78_L, 0x00FF);//MFC_ReadByte(REG_2FF0);
            if (u8Gamma_Min == 1)
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-u32Frame_Factor) + u32P_Tmp_Pre*u32Frame_Factor )>>8;//div 256;
            }

            if (u8Idx == 1)
            {
                u32P_Tmp_IIR = (255*256/1);
            }

            pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[u16Pindex2] = u32P_Tmp_IIR & 0x0FF;
            pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[u16Pindex2+1] = (u32P_Tmp_IIR & 0x0000FF00)>>8;
        }
    }

    //prepare for next frame
    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize*2; j++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[j] = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[j];
    }
#else
	UNUSED(u32Luma_Avg_Diff);
	UNUSED(u8Idx);
	UNUSED(u32Luma_Avg_OSD_diff);
	UNUSED(u8Spatial_Strength);
#endif
}


void MDrv_BLgamma(MS_BOOL bDark_Info)
{
#if 0	
    MS_U8 i = 0,j = 0;
    MS_U8 u8LUT_Index_L = 0,u8LUT_Index_H = 0;
    MS_U8 u8Blending = 0;
    MS_U8 u8Gamma_Min = 0;//SC_R2BYTEMSK(REG_SC_BK2E_78_L,0xFF);
    const MS_U8* pu8GammaTbl = NULL;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

#if (DIRECTTYPE_PATCH_ENABLE)
    if (bDark_Info)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = 1;
    }

#else
        pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = (SC_R2BYTEMSK(REG_SC_BK2E_23_L,0xFF00))>>8;
#endif

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
        }
    }
    switch (pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control)
    {
        default:
        pu8GammaTbl = NULL; //No gamma adaptation
        break;

        case 1:
        pu8GammaTbl = gamma_0dot4;
        break;

        case 2:
        pu8GammaTbl = gamma_0dot6;
        break;

        case 3:
        pu8GammaTbl = gamma_1dot2;
        break;

        case 4:
        pu8GammaTbl = gamma_1dot4;
        break;

        case 5:
        pu8GammaTbl = gamma_0dot9;
        break;

        case 6:
        pu8GammaTbl = gamma_0dot8;
        break;

        case 7:
        pu8GammaTbl = gamma_0dot7;
        break;

        case 8:
        pu8GammaTbl = gamma_0dot75;
        break;

        case 9:
        pu8GammaTbl = gamma_0dot78;
        break;

        case 10:
        pu8GammaTbl = LGE_sample3;
        break;

        case 11:
        pu8GammaTbl = LGE_sampleTHX;
        break;

        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        pu8GammaTbl = LGE_sample1_adjust;
        break;

        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        pu8GammaTbl = LGE_sample2_adjust;
        break;

        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        pu8GammaTbl = LGE_sample7_adjust;
        break;

        case 39:
        pu8GammaTbl = LGE_sample1_0dot81;
        break;
    }

    if (pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control)
    {
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8LUT_Index_L =  MINMAX(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/4, 0, 63);
                u8LUT_Index_H =  MINMAX(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/4+1, 0, 63);
                u8Blending = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] % 4;
                pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = ( (4-u8Blending)*pu8GammaTbl[u8LUT_Index_L] + (u8Blending)*pu8GammaTbl[u8LUT_Index_H] +2 )/4;
            }
        }
    }

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = MAX(u8Gamma_Min, pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i]);
        }
    }

    if(REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_Gamma_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt+1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20+pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i, pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i]);//1
            }
        }
    }
#else
	UNUSED(bDark_Info);
#endif
}



void MDrv_EdgeModeImpulse(void)
{
#if 0	
    MS_U32 u32Rounding = 0,u32Value = 0;
    MS_U32 i = 0;
    MS_U16 u16BIN_Pulse = 0;
    MS_U8 u8BLStrength = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    u16BIN_Pulse  = SC_R2BYTE(REG_SC_BK2E_22_L);// readw(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x44/2) );
    u8BLStrength = SC_R2BYTEMSK(REG_SC_BK2E_23_L, 0xFF);  //readw(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x46/2) );

    for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize*2;i = i+2)
    {
        u32Rounding = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[i];
        u32Value =  pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[i+1];
        if (u32Value == 0xFF)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i/2] = 0xFF;
        }
        else
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i/2] = MIN(u32Value + (u32Rounding/128), 0xFF);
        }
    }

    if ( (u16BIN_Pulse > 0) && (u16BIN_Pulse != 0xFFFF) )//For testing control, only one lamp enabled, others off.
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2) //LR edge type
        {
            for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
            {
                if (i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)
                {
                    if (i == u16BIN_Pulse)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[(i*2-1)-1] = u8BLStrength;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[(i*2-1)-1] = 0;
                    }
                }
                else
                {
                    if (i == u16BIN_Pulse)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[((i - pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)*2)-1] = u8BLStrength;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[((i - pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)*2)-1] = 0;
                    }
                }
            }
        }
        else
        {
            for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
            {
                if (i == u16BIN_Pulse)
                {
                    pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = u8BLStrength;
                }
                else
                {
                    pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = 0;
                }
            }
        }

    }
    else if (u16BIN_Pulse == 0xFFFF)
    {
        for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = u8BLStrength;
        }
    }

    for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8LED_resort[i] = pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i];
    }
#endif
}


MS_U8 MDrv_volumn_OSD_patch(void)
{
    return 0;
}


void MDrv_GetFrameIndex(void)
{
#if 0
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx = SC_R2BYTEMSK(REG_SC_BK2E_40_L, 0xFF) & 0x10 ;
#endif
}

void MDrv_LDimmingFunction(void)
{
#if 0
    MS_U32 u32Luma_Cur = 0;
    MS_U32 u32Luma_Diff = 0;
    MS_U32 u32Luma_OSD_Diff = 0;
    MS_U8 u8Spatial_Strength = 0,u8Spatial_Strength_adj = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
#if (DIRECTTYPE_PATCH_ENABLE)
    MS_U8 u8Histogram_Max1 = 0;
    MS_U8 u8Histogram_Max2 = 0;
    MS_U8 u8Histogram_Max1_bin = 0;
    MS_U8 u8Histogram_Max2_bin = 0xF;
    MS_U8 u8Histogram_Bin_Diff = 0;
    MS_BOOL bDark_Info = TRUE;
    MS_U8 i = 0;
#endif
    MDrv_GetFrameIndex();

    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Test = pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx;
    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final = pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx;
    //MDrv_Write2Byte(0x2620,0xA5A5);
    pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth;
    pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh;
    pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio_3D = pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio/2;
    pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D = pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio/2;
    pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth>16) ? 32 : 16 ;

    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth != 2 && pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh != 2 ) //direct type
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth > 16) ? 32 : 16 ;
    }
    else   //edge type
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)   //LR edge
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh > 16) ? 32 : 16 ;
        }
        else                          //TB edge
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth > 16) ? 32 : 16 ;
        }
    }

    u32Luma_Cur =  MDrv_FramLockBffdepack_CSep_8B_Full();
    MDrv_BLMxNto2xN_White_8B();
    MDrv_BL2xNto2xNd3_White_8B();
    //=======================================
    u8Spatial_Strength = SC_R2BYTEMSK(REG_SC_BK2E_24_L, 0xFF);
    u8Spatial_Strength_adj = u8Spatial_Strength;

#if (DIRECTTYPE_PATCH_ENABLE)
    for (i = 0; i < 16; i++)
    {
        if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max1 ) && ( pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max2) )
        {
            u8Histogram_Max2_bin = u8Histogram_Max1_bin;
            u8Histogram_Max2 = u8Histogram_Max1;
            u8Histogram_Max1_bin = i;
            u8Histogram_Max1 = pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i];
        }
        else if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] < u8Histogram_Max1 ) && ( pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max2) )
        {
            u8Histogram_Max2_bin = i;
            u8Histogram_Max2 = pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i];
        }
    }

    u8Histogram_Bin_Diff = ABS(u8Histogram_Max1_bin - u8Histogram_Max2_bin);

    if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[0] >= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize*9/10 )
        && (pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero >= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize*9/10 ) )
    {
        u8Spatial_Strength_adj = MINMAX( u8Spatial_Strength+0x80, 0, 0xFF );
        //MDrv_Write2Byte(0x3346,0x00FF);
    }
    else if ( u8Histogram_Max1_bin <= 8 &&  u8Histogram_Bin_Diff >= 5 )
    {
        u8Spatial_Strength_adj = MINMAX( u8Spatial_Strength , 0, 0xFF );
        bDark_Info = TRUE;
    }
    else if ( u8Histogram_Max1_bin <= 8 && MDrv_volumn_OSD_patch() == 1)
    {
        u8Spatial_Strength_adj = MINMAX(u8Spatial_Strength, 0, 0xFF);
        bDark_Info = TRUE;
    }

    //===============================================================
    MDrv_BLgamma(bDark_Info);
#else
    MDrv_BLgamma(FALSE);
#endif
    u32Luma_Diff = DIFF(u32Luma_Cur, pXCResourcePrivate->stdrv_LDALGO._u8Iluma_Pre);
    pXCResourcePrivate->stdrv_LDALGO._u8Iluma_Pre = u32Luma_Cur;

    if(pXCResourcePrivate->stdrv_LDALGO._u8OSDEnable && (u32Luma_Cur < 30))
    {
        u32Luma_OSD_Diff = 255;
    }

    MDrv_Maxnxn_FrameData_CSep_8B(u32Luma_Diff, pXCResourcePrivate->stdrv_LDALGO._bLDoff, u32Luma_OSD_Diff, u8Spatial_Strength_adj);          //T2b -> T2
    // MDrv_Maxnxn_FrameData_CSep_8B(u32Luma_Diff,0,0,u8Spatial_Strength_adj);// T2b -> T2
    MDrv_EdgeModeImpulse();                  // upperline / lower line             T2
    MDrv_Output_SPIdata();             //for SPI data
    MDrv_Output_DigitalBLdata();               // 0xAA
#endif
}

// ================== URSA New LD ====================
extern MS_BOOL s_bMDrvLD_InitFinished;
extern MS_BOOL s_bMDrvLD_SignalHalved;
#ifdef LD_SUPPORT_1XN_NX1
extern MS_BOOL s_bMDrvLD_Led_1xN;
extern MS_BOOL s_bMDrvLD_Led_Nx1;
#endif
extern t_sDrvLdCusPtrMap sCusPtrMap;
extern const MS_U8* Tbl_LD_Gamma[16];
extern const MS_U8* Tbl_LD_Remap[16];

/*
MS_U32 max(MS_U32 u32A, MS_U32 u32B)
{
   return (u32A <= u32B) ? u32B : u32A;
}

MS_U32 min(MS_U32 u32A, MS_U32 u32B)
{
   return (u32A <= u32B) ? u32A : u32B;
}
*/

void MDrv_LD_Load_LDF_FrameStatistics(void *pInstance, MS_U8* LDF_blocks)
{
    MS_U32 addr_base_L = MHal_LD_Get_LDF_FrameBufBaseAddr_L0(pInstance);
    //MS_U32 addr_base_R = MHal_LD_Get_LDF_FrameBufBaseAddr_R0();
    if (MHal_LD_Get_CurFrameIdx(pInstance) == 1)
    {
        addr_base_L = MHal_LD_Get_LDF_FrameBufBaseAddr_L1(pInstance);
        //addr_base_R = MHal_LD_Get_LDF_FrameBufBaseAddr_R1();
    }

    MS_U16 ldf_width = MHal_LD_Get_LDF_BacklightWidth(pInstance);
    MS_U16 ldf_height = MHal_LD_Get_LDF_BacklightHeight(pInstance);
    //ldf_width /= 2; // left/right 2 engine
    if (MHal_LD_Get_LDF_WriteDCMaxOFEn(pInstance)) // 4 bytes : OF[15:8] OF[7:0] Max DC
        ldf_width *= 4;
    else if (MHal_LD_Get_LDF_WriteDCMaxEn(pInstance)) // 2 bytes : Max DC
        ldf_width *= 2;
    //else // 1 bytes : BLEND
    //    ldf_width *= 1;

    MS_U16 pack_length = 0;
    if (ldf_width > 0)
        pack_length = (((ldf_width - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;

    //    1 2 3 4 5 6            1 2 3      4 5 6
    //    7 8 9 a b c   -->   L: 7 8 9   R: a b c
    //    d e f g h i            d e f      g h i
    //    j k l m n o            j k l      m n o
    //MS_U32 addr_offset_L = 0, addr_offset_R = 0;
    MS_U32 addr_offset_L = 0;
    MS_U16 i = 0, j = 0;
    for (j = 0; j < ldf_height; j++)
    {
        //addr_offset_R = addr_offset_L = j * pack_length;
        addr_offset_L = j * pack_length;
        for (i = 0; i < ldf_width; i++)
            *LDF_blocks++ = MDrv_LD_MIUReadByte(pInstance, addr_base_L, addr_offset_L++);
        //for (i = 0; i < ldf_width; i++)
        //    *LDF_blocks++ = MDrv_LD_MIUReadByte(pInstance, addr_base_R, addr_offset_R++);
    }
}

// Assertion : 0 <= LDF_blocks[block_idx] <= 0xFF ----> 0 <= SD_blocks[block_idx] <= 0xFF00
void MDrv_LDAlgo_ScalingDown(void *pInstance, const MS_U8* LDF_blocks, MS_U16* SD_blocks)
{
#if SUPPORT_LD 
    MS_U16 ldf_width = MHal_LD_Get_LDF_BacklightWidth(pInstance);
    MS_U16 ldf_height = MHal_LD_Get_LDF_BacklightHeight(pInstance);
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 hsd_factor = ldf_width / led_width;
    MS_U16 vsd_factor = ldf_height / led_height;
    //MS_U16 sd_factor = hsd_factor * vsd_factor;
    MS_U16 LDF_idx = 0, SD_idx = 0, i = 0, j = 0, m=0, n=0, k=0, insert_idx=0;
    MS_U16 u16Max_nth = MHal_LD_GetDummyReg(pInstance, 0x334E);
    MS_U32 SortMax_blocks[LD_MAX_BLOCK];

#ifdef LD_OVERFLOW_FEEDBACK
    static int OVF_blocks[LD_MAX_BW*LD_MAX_BH];
    static MS_BOOL OVF_init = FALSE;
    MS_U16 overflow_th = MHal_LD_GetDummyReg(pInstance, 0x3384);
    MS_U16 overflow_cur = 0;
    int tuned_bl = 0;

    if (!OVF_init)
    {
        for (LDF_idx = 0; LDF_idx < led_width*led_height; LDF_idx++)
            OVF_blocks[LDF_idx] = 0;
        OVF_init = TRUE;
    }
#endif

    MS_U32 alpha = MHal_LD_Get_LDF_DCMaxAlpha(pInstance);
    MS_U32 block_cur = 0, block_mean = 0, block_max = 0; // 0 ~ 0xFF00
    for (SD_idx = 0; SD_idx < n_leds; SD_idx++)
        SD_blocks[SD_idx] = 0;
    if (MHal_LD_Get_LDF_WriteDCMaxOFEn(pInstance)) // 4 bytes : OF[15:8] OF[7:0] Max DC
    {
        for (n = 0; n < led_height; n++)
        {
            for (m = 0; m < led_width; m++)
            {
                SD_idx = m + n*led_width;

                for (k = 0; k <= u16Max_nth; k++)
                    SortMax_blocks[k] = 0;

                block_cur = 0;
                for (j = 0; j < vsd_factor; j++)
                {
                    for (i = 0; i < hsd_factor; i++)
                    {
                        LDF_idx = (i + m*hsd_factor) + (j + n*vsd_factor)*ldf_width;

                        block_mean = LDF_blocks[LDF_idx*4];
                        block_max = LDF_blocks[LDF_idx*4 + 1];
                        // DC bug may cause mean > max
                        if (block_mean > block_max) block_mean = block_max;
/*
#ifdef LD_OVERFLOW_FEEDBACK
    // overflow control speed = 0x10 (16-bit)
    overflow_cur = LDF_blocks[LDF_idx*4 + 3];
    overflow_cur = (overflow_cur << 8) | LDF_blocks[LDF_idx*4 + 2];
    if (overflow_cur > overflow_th)
        OVF_blocks[LDF_idx] += 0x10;
    else if (overflow_cur < overflow_th)
        OVF_blocks[LDF_idx] -= 0x10;

    // block_mean <= tuned_bl <= block_max
    tuned_bl = OVF_blocks[LDF_idx] + block_cur;
    if (tuned_bl > block_max)
        tuned_bl = block_max;
    else if (tuned_bl < block_mean)
        tuned_bl = block_mean;
    block_cur = (U16)tuned_bl;

    OVF_blocks[LDF_idx] = tuned_bl - block_cur;
#endif
*/

#ifdef LD_DC_BUG_PATCH_ON
                        block_cur += block_max;
#else
                        //block_cur = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;
                        block_cur += block_mean;
#endif

                        // MAX_THRESHOLD
                        for (k = 0; k <= u16Max_nth; k++)
                        {
                            if (block_max > SortMax_blocks[k])
                            {
                                if(u16Max_nth==0)
                                {
                                    SortMax_blocks[0] = block_max;
                                }
                                else
                                {
                                    for (insert_idx = u16Max_nth; insert_idx > k; insert_idx--)
                                        SortMax_blocks[insert_idx] = SortMax_blocks[insert_idx-1];
                                    SortMax_blocks[k] = block_max;
                                }
                                break;
                            }
                        }
                    }
                }
                block_mean = (block_cur << 8) / (hsd_factor * vsd_factor);
                block_max = (MS_U16)SortMax_blocks[u16Max_nth] << 8;
                SD_blocks[SD_idx] = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;
            }
        }
    }
    else if (MHal_LD_Get_LDF_WriteDCMaxEn(pInstance)) // 2 bytes : Max DC
    {
        for (n = 0; n < led_height; n++)
        {
            for (m = 0; m < led_width; m++)
            {
                SD_idx = m + n*led_width;

                for (k = 0; k <= u16Max_nth; k++)
                    SortMax_blocks[k] = 0;

                block_cur = 0;
                for (j = 0; j < vsd_factor; j++)
                {
                    for (i = 0; i < hsd_factor; i++)
                    {
                        LDF_idx = (i + m*hsd_factor) + (j + n*vsd_factor)*ldf_width;

                        block_mean = LDF_blocks[LDF_idx*2];
                        block_max = LDF_blocks[LDF_idx*2 + 1];
                        // DC bug may cause mean > max
                        if (block_mean > block_max) block_mean = block_max;

#ifdef LD_DC_BUG_PATCH_ON
                        block_cur += block_max;
#else
                        //block_cur = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;
                        block_cur += block_mean;
#endif

                         // MAX_THRESHOLD
                        for (k = 0; k <= u16Max_nth; k++)
                        {
                            if (block_max > SortMax_blocks[k])
                            {
                                if(u16Max_nth==0)
                                {
                                    SortMax_blocks[0] = block_max;
                                }
                                else
                                {
                                    for (insert_idx = u16Max_nth; insert_idx > k; insert_idx--)
                                        SortMax_blocks[insert_idx] = SortMax_blocks[insert_idx-1];
                                    SortMax_blocks[k] = block_max;
                                }
                                break;
                            }
                        }
                    }
                }
                block_mean = (block_cur << 8) / (hsd_factor * vsd_factor);
                block_max = (MS_U16)SortMax_blocks[u16Max_nth] << 8;

                SD_blocks[SD_idx] = (alpha * block_max + (8-alpha) * block_mean + (0x1 << 2)) >> 3;
            }
        }
    }
    else // 1 bytes : BLEND
    {
        for (j = 0; j < ldf_height; j++)
        {
            for (i = 0; i < ldf_width; i++)
            {
                LDF_idx = i + j*ldf_width;
                block_cur = LDF_blocks[LDF_idx];
                block_cur <<= 8;

                SD_idx = (i / hsd_factor) + (j / vsd_factor)*led_width;
                if (block_cur > SD_blocks[SD_idx])
                    SD_blocks[SD_idx] = block_cur;
            }
        }
    }

#ifdef LD_HISTOGRAM
	MS_U16 block_idx = 0;
    MS_U16 bin_idx = 0;
    for (bin_idx = 0; bin_idx < 32; bin_idx++)
        HISTOGRAM_bins[bin_idx] = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
        HISTOGRAM_bins[SD_blocks[block_idx] >> 12] ++;
#endif

#endif
}


void MDrv_LDAlgo_BacklightGamma(void *pInstance, MS_U16* BL_blocks, const MS_U8* Table)
{
    if (!Table)
        return;

    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0;
    MS_U16 bl_msb = 0, bl_lsb = 0; // 0 ~ 0xFF
    MS_U32 bl_low = 0, bl_high = 0; // 0 ~ 0xFF00
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        bl_msb = BL_blocks[block_idx] >> 8;
        bl_lsb = BL_blocks[block_idx] & 0xFF;
        bl_low = Table[bl_msb];
        bl_low <<= 8;
        bl_high = Table[(bl_msb == 0xFF) ? bl_msb : bl_msb + 1];
        bl_high <<= 8;
        BL_blocks[block_idx] = ((256-bl_lsb)*bl_low + bl_lsb*bl_high + 0x80) >> 8;
    }
}


// Assertion : 0 <= IN_blocks[block_idx] <= 0xFF00 ----> 0 <= OUT_blocks[block_idx] <= 0xFF00
void MDrv_LDAlgo_TemporalFilter(void *pInstance, const MS_U16* IN_blocks, MS_U16* OUT_blocks)
{
    static MS_U16 luma_pre = 0;
    static MS_U16 PRE_blocks[LD_MAX_BLOCK];

    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0;

    // get frame average luma
    MS_U32 luma_cur = 0, luma_diff = 0;
    MS_U32 luma_sum = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
        luma_sum += IN_blocks[block_idx];
    luma_cur = luma_sum / n_leds; // 0 ~ 0xFF00
    luma_diff = (luma_cur > luma_pre) ? luma_cur - luma_pre : luma_pre - luma_cur;
    luma_pre = luma_cur; // 0 ~ 0xFF00

    // get blending alpha based on luma diff
    MS_U32 tf_strength_dn = MHal_LD_Get_SW_TemporalFilterStrengthDn(pInstance); // 0 ~ 0xFF
    MS_U32 tf_strength_up = MHal_LD_Get_SW_TemporalFilterStrengthUp(pInstance); // 0 ~ 0xFF
    MS_U32 luma_low = MHal_LD_Get_SW_TemporalFilterLowTh(pInstance); // 0 ~ 0xFF
    MS_U32 luma_high = MHal_LD_Get_SW_TemporalFilterHighTh(pInstance); // 0 ~ 0xFF
    MS_U32 alpha = 0, alpha_up = 0, alpha_dn = 0; // 0 ~ 0xFF
    luma_low <<= 8;
    luma_high <<= 8;
    if (luma_diff >= luma_high)
    {
        alpha_up = 0;
        alpha_dn = 0;
    }
    else if (luma_diff <= luma_low)
    {
        alpha_up = tf_strength_up;
        alpha_dn = tf_strength_dn;
    }
    else
    {
        alpha_up = tf_strength_up * (luma_high - luma_diff) / (luma_high - luma_low);
        alpha_dn = tf_strength_dn * (luma_high - luma_diff) / (luma_high - luma_low);
    }

    MS_U16 max_speed = MHal_LD_Get_MaxSpeedLimit(pInstance);
    max_speed <<= 8;
    MS_U16 block_cur = 0, block_pre = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        block_cur = IN_blocks[block_idx]; // 0 ~ 0xFF00
        block_pre = PRE_blocks[block_idx]; // 0 ~ 0xFF00

        // 16-bit IIR
        alpha =  (block_cur > block_pre) ? alpha_up : alpha_dn;
        block_cur = (alpha*block_pre + (256-alpha)*block_cur + 0x80) >> 8;

#if 0 //useless code
        if (block_cur == block_pre)
        {
            if (block_cur < block_cur)
                block_cur ++;
            else if (block_cur > block_cur)
                block_cur --;
        }
#endif

        // limit the max speed
        if (max_speed > 0)
        {
            if (block_cur > block_pre && block_cur - block_pre > max_speed)
                block_cur = block_pre + max_speed;
            else if (block_cur < block_pre && block_pre - block_cur > max_speed)
                block_cur = block_pre - max_speed;
        }

        OUT_blocks[block_idx] = block_cur;
        PRE_blocks[block_idx] = block_cur;
    }
}


// Assertion : 0 <= IN_blocks[block_idx] <= 0xFF00 ----> 0 <= OUT_blocks[block_idx] <= 0xFF00
void MDrv_LDAlgo_SpatialFilter(void *pInstance, const MS_U16* IN_blocks, MS_U16* OUT_blocks)
{
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0, neighbor_idx = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
        OUT_blocks[block_idx] = IN_blocks[block_idx];

    int i = 0, j = 0, k = 0, p = 0, q = 0;
    MS_U16 sf_str[5];
    sf_str[0] = MHal_LD_Get_SW_SpatialFilterStrength(pInstance);
    sf_str[1] = MHal_LD_Get_SW_SpatialFilterStrength2(pInstance);
    sf_str[2] = MHal_LD_Get_SW_SpatialFilterStrength3(pInstance);
    sf_str[3] = MHal_LD_Get_SW_SpatialFilterStrength4(pInstance);
    sf_str[4] = MHal_LD_Get_SW_SpatialFilterStrength5(pInstance);

    EnuLD_LED_TYPE led_type = (EnuLD_LED_TYPE)MHal_LD_Get_LEDType(pInstance);
    if (MHal_LD_Get_SW_SpatialFilterModeEn(pInstance)) // linear style
    {
        if (led_type != HAL_LD_DIRECT_TYPE)
        {
            // Make sure (sf_str[0] + 2*sf_str[1] + 2*sf_str[2] + 3*sf_str[3] + 4*sf_str[4] == 256)
            MS_U16 str_left = 256;
            str_left -= sf_str[0];
            if ((str_left >> 1) < sf_str[1])
                sf_str[1] = str_left >> 1;
            str_left -= (sf_str[1] << 1);
            if ((str_left >> 1) < sf_str[2])
                sf_str[2] = str_left >> 1;
            str_left -= (sf_str[2] << 1);
            if ((str_left >> 1) < sf_str[3])
                sf_str[3] = str_left;
            str_left -= (sf_str[3] << 1);
            if ((str_left >> 1) < sf_str[4])
                sf_str[4] = str_left >> 1;
            str_left -= (sf_str[4] << 1);
            sf_str[0] += str_left;
        }

        MS_U32 block_sum = 0;
        if (led_type == HAL_LD_EDGE_TB_TYPE)
        {
            MS_U8 nk = (sf_str[4] > 0) ? 5 : (sf_str[3] > 0) ? 4 : (sf_str[2] > 0) ? 3 : (sf_str[1] > 0) ? 2 : 1;
            for (j = 0; j < 2; j++) // led_height == 2
            {
                for (i = 0; i < led_width; i++)
                {
                    block_idx = i + led_width*j;
                    block_sum = IN_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        p = max(i - k, 0); // left
                        neighbor_idx = p + led_width*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];

                        p = min(i + k, led_width-1); // right
                        neighbor_idx = p + led_width*j;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];
                    }
                    OUT_blocks[block_idx] = max(IN_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }
        }
        else if (led_type == HAL_LD_EDGE_LR_TYPE)
        {
            MS_U8 nk = (sf_str[4] > 0) ? 5 : (sf_str[3] > 0) ? 4 : (sf_str[2] > 0) ? 3 : (sf_str[1] > 0) ? 2 : 1;
            for (i = 0; i < 2; i++) // led_width == 2
            {
                for (j = 0; j < led_height; j++)
                {
                    block_idx = i + led_width*j;
                    block_sum = IN_blocks[block_idx] * sf_str[0];
                    for (k = 1; k < nk; k++)
                    {
                        q = max(j - k, 0); // up
                        neighbor_idx = i + led_width*q;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];

                        q = min(j + k, led_height-1); // down
                        neighbor_idx = i + led_width*q;
                        block_sum += (MS_U32)IN_blocks[neighbor_idx] * sf_str[k];
                    }
                    OUT_blocks[block_idx] = max(IN_blocks[block_idx], (block_sum + 0x80) >> 8);
                }
            }
        }
        else// if (led_type == HAL_LD_DIRECT_TYPE)
        {
            // TODO : up to 7x7 with 5 registers
        }
    }
    else // nonlinear style
    {
        // direct type
        const int N1_direct = 8; // dist = 1.0
        const int xoffset1_direct[8]  = { -1,  0,  1,  0, -1,  1,  1, -1 };
        const int yoffset1_direct[8]  = {  0, -1,  0,  1, -1, -1,  1,  1 };
        const int N2_direct = 16; // dist = 2.0
        const int xoffset2_direct[16] = { -2,  0,  2,  0, -2, -1,  1,  2,  2,  1, -1, -2, -2,  2,  2, -2 };
        const int yoffset2_direct[16] = {  0, -2,  0,  2, -1, -2, -2, -1,  1,  2,  2,  1, -2, -2,  2,  2 };
        const int N3_direct = 24; // dist = 3.0
        const int xoffset3_direct[24] = { -3,  0,  3,  0, -3, -1,  1,  3,  3,  1, -1, -3, -3, -2,  2,  3,  3,  2, -2, -3, -3,  3,  3, -3 };
        const int yoffset3_direct[24] = {  0, -3,  0,  3, -1, -3, -3, -1,  1,  3,  3,  1, -2, -3, -3, -2,  2,  3,  3,  2, -3, -3,  3,  3 };
        const int N4_direct = 0; // dist = 1.0
        //const int xoffset4_direct[0]  = { }; // Not supported
        //const int yoffset4_direct[0]  = { }; // Not supported
        const int N5_direct = 0; // dist = 1.0
        //const int xoffset5_direct[0]  = { }; // Not supported
        //const int yoffset5_direct[0]  = { }; // Not supported

        // left-right type
        const int N1_LR = 2; // dist = 1.0
        const int xoffset1_LR[2] = {  0,  0 };
        const int yoffset1_LR[2] = {  1, -1 };
        const int N2_LR = 2; // dist = 2.0
        const int xoffset2_LR[2] = {  0,  0 };
        const int yoffset2_LR[2] = {  2, -2 };
        const int N3_LR = 2; // dist = 3.0
        const int xoffset3_LR[2] = {  0,  0 };
        const int yoffset3_LR[2] = {  3, -3 };
        const int N4_LR = 2; // dist = 4.0
        const int xoffset4_LR[2] = {  0,  0 };
        const int yoffset4_LR[2] = {  4, -4 };
        const int N5_LR = 2; // dist = 5.0
        const int xoffset5_LR[2] = {  0,  0 };
        const int yoffset5_LR[2] = {  5, -5 };

        // top-bottom type
        const int N1_TB = 2; // dist = 1.0
        const int xoffset1_TB[2] = {  1, -1 };
        const int yoffset1_TB[2] = {  0,  0 };
        const int N2_TB = 2; // dist = 2.0
        const int xoffset2_TB[2] = {  2, -2 };
        const int yoffset2_TB[2] = {  0,  0 };
        const int N3_TB = 2; // dist = 3.0
        const int xoffset3_TB[2] = {  3, -3 };
        const int yoffset3_TB[2] = {  0,  0 };
        const int N4_TB = 2; // dist = 4.0
        const int xoffset4_TB[2] = {  4, -4 };
        const int yoffset4_TB[2] = {  0,  0 };
        const int N5_TB = 2; // dist = 5.0
        const int xoffset5_TB[2] = {  5, -5 };
        const int yoffset5_TB[2] = {  0,  0 };

        int N1 = 0, N2 = 0, N3 = 0, N4 = 0, N5 = 0;
        const int *xoffset1 = NULL, *xoffset2 = NULL, *xoffset3 = NULL, *xoffset4 = NULL, *xoffset5 = NULL;
        const int *yoffset1 = NULL, *yoffset2 = NULL, *yoffset3 = NULL, *yoffset4 = NULL, *yoffset5 = NULL;
        if (led_type == HAL_LD_EDGE_TB_TYPE)
        {
           N1 = N1_TB; N2 = N2_TB; N3 = N3_TB;
            xoffset1 = xoffset1_TB; xoffset2 = xoffset2_TB; xoffset3 = xoffset3_TB;
            yoffset1 = yoffset1_TB; yoffset2 = yoffset2_TB; yoffset3 = yoffset3_TB;
            N4 = N4_TB; N5 = N5_TB;
            xoffset4 = xoffset4_TB; xoffset5 = xoffset5_TB;
            yoffset4 = yoffset4_TB; yoffset5 = yoffset5_TB;
        }
        else if (led_type == HAL_LD_EDGE_LR_TYPE)
        {
            N1 = N1_LR; N2 = N2_LR; N3 = N3_LR;
            xoffset1 = xoffset1_LR; xoffset2 = xoffset2_LR; xoffset3 = xoffset3_LR;
            yoffset1 = yoffset1_LR; yoffset2 = yoffset2_LR; yoffset3 = yoffset3_LR;
            N4 = N4_LR; N5 = N5_LR;
            xoffset4 = xoffset4_LR; xoffset5 = xoffset5_LR;
            yoffset4 = yoffset4_LR; yoffset5 = yoffset5_LR;
        }
        else// if (led_type == HAL_LD_DIRECT_TYPE)
        {
            N1 = N1_direct; N2 = N2_direct; N3 = N3_direct;
            xoffset1 = xoffset1_direct; xoffset2 = xoffset2_direct; xoffset3 = xoffset3_direct;
            yoffset1 = yoffset1_direct; yoffset2 = yoffset2_direct; yoffset3 = yoffset3_direct;
            N4 = N4_direct; N5 = N5_direct;
            xoffset4 = NULL; xoffset5 = NULL;
            yoffset4 = NULL; yoffset5 = NULL;
        }

        for (k = 0; k < 5; k++)
            sf_str[k] <<= 8; // 0 ~ 0xFF00

        for (j = 0; j < led_height; j++)
        {
            for (i = 0; i < led_width; i++)
            {
                block_idx = i + led_width*j;

                // 3x3 non-linear filter
                if (sf_str[0] < 0xFF00)
                {
                    for (k = 0; k < N1; k++)
                    {
                        q = j + yoffset1[k];
                        if (q < 0 || q >= led_height) continue;
                        p = i + xoffset1[k];
                        if (p < 0 || p >= led_width) continue;

                        neighbor_idx = p + led_width*q;
                        if (sf_str[0] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[0];
                    }
                }

                // 5x5 non-linear filter
                if (sf_str[1] < 0xFF00)
                {
                    for (k = 0; k < N2; k++)
                    {
                        q = j + yoffset2[k];
                        if (q < 0 || q >= led_height) continue;
                        p = i + xoffset2[k];
                        if (p < 0 || p >= led_width) continue;

                        neighbor_idx = p + led_width*q;
                        if (sf_str[1] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[1];
                    }
                }

                // 7x7 non-linear filter
                if (sf_str[2] < 0xFF00)
                {
                    for (k = 0; k < N3; k++)
                    {
                        q = j + yoffset3[k];
                        if (q < 0 || q >= led_height) continue;
                        p = i + xoffset3[k];
                        if (p < 0 || p >= led_width) continue;

                        neighbor_idx = p + led_width*q;
                        if (sf_str[2] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[2];
                    }
                }

                // 9x9 non-linear filter
                if (sf_str[3] < 0xFF00)
                {
                    for (k = 0; k < N4; k++)
                    {
                        q = j + yoffset4[k];
                        if (q < 0 || q >= led_height) continue;
                        p = i + xoffset4[k];
                        if (p < 0 || p >= led_width) continue;

                        neighbor_idx = p + led_width*q;
                        if (sf_str[3] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[3];
                    }
                }

                // 11x11 non-linear filter
                if (sf_str[4] < 0xFF00)
                {
                    for (k = 0; k < N5; k++)
                    {
                        q = j + yoffset5[k];
                        if (q < 0 || q >= led_height) continue;
                        p = i + xoffset5[k];
                        if (p < 0 || p >= led_width) continue;

                        neighbor_idx = p + led_width*q;
                        if (sf_str[4] + OUT_blocks[block_idx] < IN_blocks[neighbor_idx])
                            OUT_blocks[block_idx] = IN_blocks[neighbor_idx] - sf_str[4];
                    }
                }
           }
        }
    }
}

void MDrv_LDAlgo_LocalDimmingStrength(void *pInstance, MS_U16* BL_blocks)
{
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0;

    MS_U32 ld_str = MHal_LD_Get_SW_LocalDimmingStrength(pInstance); // 0 ~ 0xFF
    ld_str = (ld_str > 0x80) ? ld_str+1 : ld_str;

    MS_U16 min_bl_val = MHal_LD_Get_SW_MinClampValue(pInstance);
    min_bl_val <<= 8; // 0 ~ 0xFF00

    MS_U16 bl_val = 0;
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        bl_val = (BL_blocks[block_idx] * ld_str + 0xff00 * (256 - ld_str) + 0x80) >> 8; // 0 ~ 0xFF00
        if (bl_val < min_bl_val)
            bl_val = min_bl_val;
        BL_blocks[block_idx] = bl_val;
    }
}

// Assertion : 0 <= IN_blocks[block_idx] < 0xFF00 ----> 0 <= SPI_blocks[block_idx] < 0xFFFF, 0 <= LDB_blocks[block_idx] < 0xFF
void MDrv_LDAlgo_PostProcess(void *pInstance, const MS_U16* IN_blocks, MS_U16* SPI_blocks, MS_U8* LDB_blocks)
{
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0;

    // Form SPI signal & LDB data
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        MS_U16 backlight = IN_blocks[block_idx];
        if (s_bMDrvLD_SignalHalved)
            backlight = (MS_U16)MIN(MAX(backlight, 0), 0xFF00 >> 1) << 1;
        // transform (0, ..., 0xFF00) to (0, ..., 0xFFFF)
        SPI_blocks[block_idx] = backlight + (backlight >> 8);
        // rounding to 8-bit
        LDB_blocks[block_idx] = (backlight + 0x80) >> 8;
    }

#ifdef LD_MEM_MODE
    MS_BOOL MEM_init = FALSE;
    static MS_U16 MEM_blocks[LD_MAX_BLOCK]; // for measurement
    if (!MEM_init)
    {
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            MEM_blocks[block_idx] = 0;
        MEM_init = TRUE;
    }
#endif

    MS_U8 led_mod = MHal_LD_Get_SW_PulseMode(pInstance);
    MS_U16 led_id = MHal_LD_Get_SW_PulseId(pInstance);
    MS_U16 led_str = MHal_LD_Get_SW_PulseLEDIntensity(pInstance);
    MS_U16 ldb_str = MHal_LD_Get_SW_PulseLDBIntensity(pInstance);
    if (led_mod == 0x0)
    {
        if (led_id == 0x1) // Left: right LSF, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % led_width >= led_width/2)
                {
                    SPI_blocks[block_idx] = (led_str << 8) | led_str;
                    LDB_blocks[block_idx] = led_str;
                }
            }
        }
        else if (led_id == 0x2) // Left: LD off, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % led_width < led_width/2)
                {
                    SPI_blocks[block_idx] = (led_str << 8) | led_str;
                    LDB_blocks[block_idx] = led_str;
                }
            }
        }
        else if (led_id == 0x3) // Left: LD on, Right: left LSF
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % led_width >= led_width/2)
                {
                    SPI_blocks[block_idx] = SPI_blocks[block_idx - led_width/2];
                    LDB_blocks[block_idx] = LDB_blocks[block_idx - led_width/2];
                }
            }
        }
        else if (led_id == 0x4) // Left: right LSF, Right: LD on
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                if (block_idx % led_width < led_width/2)
                {
                    SPI_blocks[block_idx] = SPI_blocks[block_idx + led_width/2];
                    LDB_blocks[block_idx] = LDB_blocks[block_idx + led_width/2];
                }
            }
        }
    }
    else if (led_mod == 0xf)     // 22[15:12]=4'b1111  real backlight
    {
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                SPI_blocks[block_idx] = 0xffff;
            }
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                SPI_blocks[block_idx] = 0;
            }
            SPI_blocks[led_id] = 0xffff;
        }
        MHal_LD_Set_LSFOutMode(pInstance, 0x1);
    }
    else if (led_mod == 0xe)      // 22[15:12]=4'b1110  digital backlight
    {
        for (block_idx = 0; block_idx < n_leds; block_idx++)
        {
            SPI_blocks[block_idx] = 0xffff;
        }
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                LDB_blocks[block_idx] = 0xff;
            }
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
            {
                LDB_blocks[block_idx] = 0;
            }
            LDB_blocks[led_id] = 0xff;
        }
        MHal_LD_Set_LSFOutMode(pInstance, 0x5);
    }
    else if (led_mod == 0xd)      // 22[15:12]=4'b1101 tunable real backlight
    {
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = (led_str << 8) | ldb_str;;
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = 0;
            SPI_blocks[led_id] = (led_str << 8) | ldb_str;;
        }
    }
    else if (led_mod == 0xc)      // 22[15:12]=4'b1100 tunable digital backlight
    {
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            SPI_blocks[block_idx] = (led_str << 8) | led_str;
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = ldb_str;
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = 0;
            LDB_blocks[led_id] = ldb_str;
        }
    }
#ifdef LD_MEM_MODE
    else if (led_mod == 0xb)      // 22[15:12]=4'b1011 set SPI
    {
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = MEM_blocks[block_idx] = (led_str << 8) | ldb_str;
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                SPI_blocks[block_idx] = MEM_blocks[block_idx];
            SPI_blocks[led_id] = MEM_blocks[led_id] = (led_str << 8) | ldb_str;;
        }
    }
    else if (led_mod == 0xa)      // 22[15:12]=4'b1010 set LDB
    {
        for (block_idx = 0; block_idx < n_leds; block_idx++)
            SPI_blocks[block_idx] = (led_str << 8) | led_str;
        if (led_id == 0xfff)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = MEM_blocks[block_idx] = ldb_str;
        }
        else if (led_id < n_leds)
        {
            for (block_idx = 0; block_idx < n_leds; block_idx++)
                LDB_blocks[block_idx] = MEM_blocks[block_idx];
            LDB_blocks[led_id] = MEM_blocks[led_id] = ldb_str;
        }
    }
#endif

#ifdef LD_SUPPORT_1XN_NX1
    // Map 2xN back to 1xN LED
    if (s_bMDrvLD_Led_1xN)
    {
        for (block_idx = 0; block_idx < n_leds; block_idx += 2)
        {
            if (SPI_blocks[block_idx] < SPI_blocks[block_idx+1])
                SPI_blocks[block_idx] = SPI_blocks[block_idx+1];
            SPI_blocks[block_idx+1] = 0;
            if (LDB_blocks[block_idx] < LDB_blocks[block_idx+1])
                LDB_blocks[block_idx] = LDB_blocks[block_idx+1];
            LDB_blocks[block_idx+1] = 0;
        }
    }
    // Map Nx2 back to Nx1 LED
    if (s_bMDrvLD_Led_Nx1)
    {
        for (block_idx = 0; block_idx < led_width; block_idx ++)
        {
            if (SPI_blocks[block_idx] < SPI_blocks[block_idx+led_width])
                SPI_blocks[block_idx] = SPI_blocks[block_idx+led_width];
            SPI_blocks[block_idx+led_width] = 0;
            if (LDB_blocks[block_idx] < LDB_blocks[block_idx+led_width])
                LDB_blocks[block_idx] = LDB_blocks[block_idx+led_width];
            LDB_blocks[block_idx+led_width] = 0;
        }
    }
#endif
}

// Assertion : 0 <= BL_blocks[block_idx] <= 0xFFFF
void MDrv_LDAlgo_GlobalDimmingStrength(void *pInstance, MS_U16* BL_blocks)
{
    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 n_leds = led_width * led_height;
    MS_U16 block_idx = 0;

    MS_U32 gd_str = MHal_LD_Get_SW_GlobalDimmingStrength(pInstance); // 0 ~ 0xFF
    for (block_idx = 0; block_idx < n_leds; block_idx++)
    {
        BL_blocks[block_idx] = (BL_blocks[block_idx] * gd_str) / 255;
    }
}

void MDrv_LD_Output_SPI_Signal(void *pInstance, const MS_U16* SPI_blocks)
{
    sCusPtrMap.LED_DEVICE_SET_OutputSPI(pInstance, (const MS_U16*)SPI_blocks );
}

void MDrv_LD_Output_LDB_FrameBacklight(void *pInstance, const MS_U8* LDB_blocks)
{    
	MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);    
	MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);    
	MS_U16 led_half_width = (led_width + 1) >> 1;    
	MS_U16 led_half_height = (led_height + 1) >> 1;    
	MS_U16 block_idx = 0;    
	MS_U16 ldb_width = MHal_LD_Get_LDB_BacklightWidth(pInstance);    
	//MS_U16 ldb_height = MHal_LD_Get_LDB_BacklightHeight();    
	MS_U32 addr_base_L = MHal_LD_Get_LDB_FrameBufBaseAddr_L0(pInstance);    
	//MS_U32 addr_base_R = MHal_LD_Get_LDB_FrameBufBaseAddr_R0(pInstance);    
	MS_U32 addr_offset = 0;    
	MS_U16 pack_length = 0;    
	MS_U16 i = 0, j = 0;    

	if (led_width > 0)        
		pack_length = (((ldb_width - 1) / MDrv_LD_PackLength) + 1) * MDrv_LD_PackLength;    

	if (MHal_LD_Get_Edge2DEn(pInstance)) // ldb_width = led_width    
	{        //    1 2 3 4 5            1 2 3 7 8      1 2 3 7 8       
		//    7 8 9 a b   -->   L: 9 4 5 a b   R: 9 4 5 a b        
		//    d e f g h            d e f g h      d e f g h        
		block_idx = 0;        
		addr_offset = 0;      		

		// top-left quater        
		for (j = 0; j < led_half_height; j++)        
		{            
			for (i = 0; i < led_half_width; i++)            
			{                
				MDrv_LD_MIUWriteByte(pInstance, addr_base_L, addr_offset, LDB_blocks[i + j*led_width]);                
				//MDrv_LD_MIUWriteByte(pInstance, addr_base_R, addr_offset, LDB_blocks[i + j*led_width]);                
				addr_offset ++;                				
				if (block_idx % ldb_width == ldb_width - 1)                    
					addr_offset += pack_length - ldb_width;                
				block_idx ++;            
			}        
		}        

		// top-left quater        
		for (j = 0; j < led_half_height; j++)        
		{            
			for (i = led_half_width; i < led_width; i++)            
			{                
				MDrv_LD_MIUWriteByte(pInstance, addr_base_L, addr_offset, LDB_blocks[i + j*led_width]);                
				//MDrv_LD_MIUWriteByte(pInstance, addr_base_R, addr_offset, LDB_blocks[i + j*led_width]);                
				addr_offset ++;                
				if (block_idx % ldb_width == ldb_width - 1)                    
					addr_offset += pack_length - ldb_width;                
				block_idx ++;            
			}        
		}        

		// bottom-left quater        
		for (j = led_half_height; j < led_height; j++)        
		{            
			for (i = 0; i < led_half_width; i++)            
			{                
				MDrv_LD_MIUWriteByte(pInstance, addr_base_L, addr_offset, LDB_blocks[i + j*led_width]);                
				//MDrv_LD_MIUWriteByte(pInstance, addr_base_R, addr_offset, LDB_blocks[i + j*led_width]);                
				addr_offset ++;                
				if (block_idx % ldb_width == ldb_width - 1)                    
					addr_offset += pack_length - ldb_width;                
				block_idx ++;            
			}        
		}        

		// bottom-left quater        
		for (j = led_half_height; j < led_height; j++)        
		{            
			for (i = led_half_width; i < led_width; i++)            
			{                
				MDrv_LD_MIUWriteByte(pInstance, addr_base_L, addr_offset, LDB_blocks[i + j*led_width]);                
				//MDrv_LD_MIUWriteByte(pInstance, addr_base_R, addr_offset, LDB_blocks[i + j*led_width]);                
				addr_offset ++;                
				if (block_idx % ldb_width == ldb_width - 1)                    
					addr_offset += pack_length - ldb_width;                
				block_idx ++;            
			}        
		}    
	}    
	else  // ldb_width = led_width/2 + 1    
	{        //    1 2 3 4 5 6            1 2 3 4      3 4 5 6        
		//    7 8 9 a b c   -->   L: 7 8 9 a   R: 9 a b c        
		//    d e f g h i            d e f g      f g h i        
		//    j k l m n o            j k l m      l m n o        

		for (j = 0; j < led_height; j++)        
		{            
			addr_offset = j*pack_length;            
			for (i = 0; i < ldb_width; i++)                
				MDrv_LD_MIUWriteByte(pInstance, addr_base_L, addr_offset++, *LDB_blocks++);            
			LDB_blocks -= 2;            
			addr_offset = j*pack_length;            

			//for (i = led_width-ldb_width; i < led_width; i++)                
				//MDrv_LD_MIUWriteByte(pInstance, addr_base_R, addr_offset++, *LDB_blocks++);        
		}    
	}

#if 0
printf("==========================\n");
for(i = 0; i < 64; i++)
{
	if(i % 16 == 0) printf("%.2X     ", i);
	printf("%.2X(%.2X), ", MDrv_LD_MIUReadByte(pInstance, addr_base_L, i), LDB_blocks[i]);
	if(i % 16 == 15) printf("=\n");
}
printf("=\n");
#endif
}

// ================== URSA New LD ====================
extern MS_BOOL gbLEDDemoModeOn;
#define MAX_LED_DEMO_COUNT			60

void MDrv_LDAlgo_Main(SC_INT_SRC enIntNum, void *pParam)
{
#if SUPPORT_LD
    UNUSED(enIntNum);
    //UNUSED(pParam);
    void *pInstance = pParam;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
	//static MS_U32 u32LEDDemoCount = 0;

    MS_U8 LDF_blocks[4*LD_MAX_BW*LD_MAX_BH]={0};
    MS_U16 SD_blocks[LD_MAX_BLOCK]={0};
    MS_U16 TF_blocks[LD_MAX_BLOCK]={0};
    MS_U16 SF_blocks[LD_MAX_BLOCK]={0};
    MS_U8 LDB_blocks[LD_MAX_BLOCK]={0};
    MS_U16 SPI_blocks[LD_MAX_BLOCK]={0};

    // Runtime estimator
//    static U32 u32StartTime = msAPI_Timer_GetSystemTime(), u32EndTime = 0;

    // Frequency counter
//    static BOOL bLockTime = FALSE;
//    static U32 u32LdFreqPerSec = 0;
//    if (!bLockTime)
//    {
//        bLockTIme = TRUE;
//        u32LdFreqPerSec = 0;
//    }

    if (MHal_LD_Get_SoftReset(pInstance))
    {
        // Modify psDrvLdInfo & psDrvLdPQInfo HERE!!

        // Reset algorithm parameters & Edge2D table & compensation table by current register content.
        s_bMDrvLD_InitFinished = FALSE;
        MDrv_LD_Init(pInstance);
    }

    MS_U8 in_gamma = MHal_LD_Get_SW_BacklightInGamma(pInstance);
    MS_U8 out_gamma = MHal_LD_Get_SW_BacklightOutGamma(pInstance);

    // SW input
    if (MHal_LD_Get_SW_ReadLDFEn(pInstance))
        MDrv_LD_Load_LDF_FrameStatistics(pInstance, LDF_blocks);

    // SW algorithm begin
    if (MHal_LD_Get_SW_AlgorithmEn(pInstance))
    {
#if 0
MS_U8 i;
printf("\n=============== LDF ================\n");
for(i = 0; i < 6*2*4; i++)
{
	if(i % 4 == 0) printf("\n%.2d => ", i);
	printf("%.4x, ", LDF_blocks[i]);
}
printf("\n");
#endif
        MDrv_LDAlgo_ScalingDown(pInstance, LDF_blocks, SD_blocks);       // LDF -> SD
#if 0
MS_U8 j;
printf("\n=============== SD ================\n");
for(j = 0; j < 12; j++)
{
	if(i % 6 == 0) printf("\n%.2d => ", j);
	printf("%.4x, ", SD_blocks[j]);
}
printf("\n");
#endif
        MDrv_LDAlgo_BacklightGamma(pInstance, SD_blocks, Tbl_LD_Gamma[in_gamma]);
        MDrv_LDAlgo_SpatialFilter(pInstance, SD_blocks, TF_blocks);     // SD -> TF
        MDrv_LDAlgo_TemporalFilter(pInstance, TF_blocks, SF_blocks);      // TF -> SF
        MDrv_LDAlgo_LocalDimmingStrength(pInstance, SF_blocks);
        MDrv_LDAlgo_BacklightGamma(pInstance, SF_blocks, Tbl_LD_Remap[out_gamma]);
        MDrv_LDAlgo_PostProcess(pInstance, SF_blocks, SPI_blocks, LDB_blocks);      // process SPI & LDB
        MDrv_LDAlgo_GlobalDimmingStrength(pInstance, SPI_blocks);
    }
    // SW algorithm end

    // SW output
    if (MHal_LD_Get_SW_WriteSPIEn(pInstance))
        MDrv_LD_Output_SPI_Signal(pInstance, SPI_blocks); // real backlight

    if (MHal_LD_Get_SW_WriteLDBEn(pInstance))
        MDrv_LD_Output_LDB_FrameBacklight(pInstance, LDB_blocks); // digital backlight
	
#endif
}

void MDrv_XC_LD_ISR(void *pInstance)
{
    MDrv_XC_InterruptAttach(pInstance, SC_INT_VSINT, MDrv_LDAlgo_Main, (void *) pInstance);
}

void MDrv_LD_EnableLD(MS_BOOL bLDEnable)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stdrv_LDALGO._bLDoff == bLDEnable)
    {
        pXCResourcePrivate->stdrv_LDALGO._bLDoff = !bLDEnable;
        MDrv_LD_Set_LocalDimmingEn(bLDEnable);
    }
}

void MDrv_LD_SetGammaControl(MS_U8 u8GammaControl)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = u8GammaControl;
}

void MDrv_LD_SetOSDEnable(MS_U8 u8OSDEnable)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8OSDEnable = u8OSDEnable;
}

#endif

