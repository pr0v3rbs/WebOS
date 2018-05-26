/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  VCP register details. ( used only within kdriver )
 *
 *  author     Taewoong Kim (taewoong.kim@lge.com)
 *  version    1.0
 *  date       2013.11.05
 *
 */

#ifndef _VCP_REG_H_
#define _VCP_REG_H_
//#define D14
#include "vdec_type_defs.h"



extern volatile UINT32 stpVcpReg;
extern volatile UINT32 stpHdmiReg;
/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#define VDEC_VCP_BASE		(VDEC_BASE + 0x62000) //D14 Fix
#define VDEC_HDMI_BASE		(VDEC_BASE + 0x80000) //D14 Fix

// D14 vcp fix
#define VCP_CH_PIC_INIT(i)		(*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x000))
#define VCP_CH_PIC_START(i)		(*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x004))
#define VCP_CH_VCP_INTR_REG(i)      (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x040))
#define VCP_CH_VCP_INTR_MASK(i)      (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x044))
#define VCP_CH_C_SCALER_CTRL1(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x054))
#define VCP_CH_C_SCALER_CTRL2(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x058))
#define VCP_CH_CHAMP_CTRL0(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x068))
#define VCP_CH_CHAMP_CTRL2(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x070))
#define VCP_CH_CCV_CTRL1(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x07C))
#define VCP_CH_FVS_CTRL5(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x098))
#define VCP_CH_DESHP_CTRL2(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x0C8))
#define VCP_CH_FVS_CTRL0(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x084))
#define VCP_CH_FVS_CTRL1(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x088))
#define VCP_CH_FVS_CTRL3(i)	 (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x090))
#define VCP_CH_FVS_CTRL4(i)  (*(volatile UINT32 *)(stpVcpReg + 0x200*i + 0x094))

// VSI HDMI VIC info
#define HDMI_FC_VSDPAYLOAD1(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40cc))
// VSI Channel info
#define HDMI_FC_VSDPAYLOAD4(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40d8))
// VSI PicHW
#define HDMI_FC_VSDPAYLOAD5(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40dc))
#define HDMI_FC_VSDPAYLOAD6(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40e0))
#define HDMI_FC_VSDPAYLOAD7(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40e4))
#define HDMI_FC_VSDPAYLOAD8(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40e8))
// AVInfo
#define HDMI_FC_AVIETB0(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x4074))
#define HDMI_FC_AVIETB1(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x4078))
#define HDMI_FC_AVISBB0(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x407C))
#define HDMI_FC_AVISBB1(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x4080))
// VSI PicAspect Ratio
#define HDMI_FC_VSDPAYLOAD9(i)  (*(volatile UINT32 *)(stpHdmiReg + 0x20000*i + 0x40ec))

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
void VCP_Init(void);

#ifdef __cplusplus
extern "C" {
#endif
/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _VCP_REG_H_ */

/* from 'vdo-REG.csv' 20120326 19:08:42   대한민국 표준시 by getregs v2.7 */
