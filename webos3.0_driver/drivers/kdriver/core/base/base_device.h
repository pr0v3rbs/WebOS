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
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author     raxis.lim
 *  @version    1.0
 *  @date       2010-11-18
 *  @note       Additional information.
 */

#ifndef	_BASE_DEV_CFG_H_
#define	_BASE_DEV_CFG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Some default values
----------------------------------------------------------------------------------------*/
// define the minimum memory size for mali 400 (GPU)
//
// @note this value is not used for L8
#define	KDRV_GPU_MIN_SIZE	(80*1024*1024)

/*----------------------------------------------------------------------------------------
	Driver Major Number
----------------------------------------------------------------------------------------*/
#define	KDRV_MAJOR_BASE		140		/* 260 */
#define	KDRV_MAJOR_END		179		/* 289 */

//
//	device major number list.
//	if major is zero, linux will assign empty major number for your module.
//	kdriver will reserive total 30 major number. its range should be determined after test in the real system.
//
#define AAD_MAJOR			(KDRV_MAJOR_BASE+0)
#define AAD_MINOR			0

#define ADEC_MAJOR			(KDRV_MAJOR_BASE+1)
#define ADEC_MINOR			0

#define AFE_MAJOR			(KDRV_MAJOR_BASE+2)
#define AFE_MINOR			0

#define AUDIO_MAJOR			(KDRV_MAJOR_BASE+3)
#define AUDIO_MINOR			0

#define CI_MAJOR			(KDRV_MAJOR_BASE+4)
#define CI_MINOR			0

#define DE_MAJOR			(KDRV_MAJOR_BASE+5)
#define DE_MINOR			0

#define DEMOD_MAJOR			(KDRV_MAJOR_BASE+6)
#define DEMOD_MINOR			0

#define DENC_MAJOR			(KDRV_MAJOR_BASE+7)
#define DENC_MINOR			0

#define FBDEV_MAJOR			29	// 29 is fixed value for linux fb model
#define FBDEV_MINOR			0

#define GFX_MAJOR			(KDRV_MAJOR_BASE+11)
#define	GFX_MINOR			0

#define GPIO_MAJOR			(KDRV_MAJOR_BASE+12)
#define	GPIO_MINOR			0

#define SPI_MAJOR			(KDRV_MAJOR_BASE+13)
#define	SPI_MINOR			0


#define I2C_MAJOR			(KDRV_MAJOR_BASE+14)
#define	I2C_MINOR			0

#define PNG_MAJOR			(KDRV_MAJOR_BASE+15)
#define	PNG_MINOR			0

#define PE_MAJOR			(KDRV_MAJOR_BASE+16)
#define	PE_MINOR			0

#define PVR_MAJOR			(KDRV_MAJOR_BASE+17)
#define	PVR_MINOR			0

#define SDEC_MAJOR			(KDRV_MAJOR_BASE+18)
#define	SDEC_MINOR			0

#define SE_MAJOR			(KDRV_MAJOR_BASE+19)
#define	SE_MINOR			0

#define VBI_MAJOR			(KDRV_MAJOR_BASE+20)
#define	VBI_MINOR			0

#define VDEC_MAJOR			(KDRV_MAJOR_BASE+21)
#define	VDEC_MINOR			0

#define VENC_MAJOR			(KDRV_MAJOR_BASE+22)
#define	VENC_MINOR			0

#define SYS_MAJOR			(KDRV_MAJOR_BASE+23)
#define	SYS_MINOR			0

#define CLK_MAJOR			(KDRV_MAJOR_BASE+24)
#define	CLK_MINOR			0

#define MONITOR_MAJOR		(KDRV_MAJOR_BASE+25)
#define	MONITOR_MINOR		0

/* major 166 (KDRV_MAJOR_BASE+26) is used by access USB in Linux */

#define SCI_MAJOR			(KDRV_MAJOR_BASE+27)
#define	SCI_MINOR			0

#define MJPEG_MAJOR			(KDRV_MAJOR_BASE+28)
#define	MJPEG_MINOR			0

#define APR_MAJOR			(KDRV_MAJOR_BASE+29)
#define	APR_MINOR			0

#define BE_MAJOR			(KDRV_MAJOR_BASE+30)
#define BE_MINOR			0

#define GPU_MAJOR			(KDRV_MAJOR_BASE+31)
#define GPU_MINOR			0

#define OVI_MAJOR			(KDRV_MAJOR_BASE+33)
#define OVI_MINOR			0

#ifdef INCLUDE_KDRV_MEMPROT
#define MEMPROT_MAJOR			(KDRV_MAJOR_BASE+34)
#define MEMPROT_MINOR			0
#endif

#define HDMI_MAJOR			(KDRV_MAJOR_BASE+36) //175
#define	HDMI_MINOR			0

#define HDMI20_MAJOR		HDMI_MAJOR
#define HDMI20_MINOR		1

#define HDMI20_HDCP22RX0_MAJOR	HDMI_MAJOR	
#define HDMI20_HDCP22RX0_MINOR	2

#define HDMI20_HDCP22RX1_MAJOR	HDMI_MAJOR	
#define HDMI20_HDCP22RX1_MINOR	3

#define HDMI20_HDCP22TX0_MAJOR	HDMI_MAJOR	
#define HDMI20_HDCP22TX0_MINOR	4


#define PM_MAJOR			(KDRV_MAJOR_BASE+37)
#define PM_MINOR			0


#define SAMPLE_MAJOR		(KDRV_MAJOR_END-1)
#define	SAMPLE_MINOR		0
#define TEMPLATE_MAJOR		(KDRV_MAJOR_END-0)
#define	TEMPLATE_MINOR		0

#define LGBUS_MAJOR			270
#define LGBUS_MINOR			0


#ifdef INCLUDE_KDRV_SLT
#define SLT_MAJOR			290
#define SLT_MINOR			0
#endif

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PROTOS_H_ */

